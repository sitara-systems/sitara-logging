#include "MongoTracker.h"
#include "mongodb/MongoController.h"
#include <mongocxx/exception/bulk_write_exception.hpp>

using namespace sitara::logging;

MongoTracker::MongoTracker(const std::string& uri, const std::string& database, const std::string& collection) : 
	mUri(uri), 
	mDatabaseName(database), 
	mCollectionName(collection),
	mBatchSize(10),
	mIsWriting(true) {
	sitara::logging::MongoController::getInstance().createPool(uri);

	mWriteThread = std::thread([&]() {
		mongocxx::pool::entry client = sitara::logging::MongoController::getInstance().getClientFromPool();

		while (mIsWriting) {
			if (mBatchingEnabled) {
				std::lock_guard<std::mutex> lock(mDocumentMutex);
				if (mDocumentVector.size() > mBatchSize) {
					// wait until the batch size is met, then send all documents at once
					try {
						auto res = client->database(mDatabaseName)[mCollectionName].insert_many(mDocumentVector);
						mDocumentVector.clear();
					}
					catch (mongocxx::bulk_write_exception& e) {
						CI_LOG_E("Error in inserting document : " << e.what());
					}
				}
			}
			else {
				while (!mDocumentQueue.empty()) {
					bsoncxx::v_noabi::document::view documentView = mDocumentQueue.front().view();

					try {
						auto res = client->database(mDatabaseName)[mCollectionName].insert_one(documentView);
					}
					catch (mongocxx::bulk_write_exception& e) {
						CI_LOG_E("Error in inserting document : " << e.what());
					}

					mDocumentQueue.pop();
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
	});
}

MongoTracker::~MongoTracker() {
	mIsWriting = false;
	mWriteThread.join();
}

void MongoTracker::setup(const std::string& uuid, const std::string& applicationName, const std::string& applicationVersion) {
	BaseTracker::setup(uuid, applicationName, applicationVersion);
}

void MongoTracker::trackHit(std::shared_ptr<sitara::logging::BaseHit> hit) {
	bsoncxx::builder::basic::document bsonDocument = hit->getBson();
	bsoncxx::v_noabi::document::value bson = bsonDocument.extract();

	std::lock_guard<std::mutex> lock(mDocumentMutex);
	if (mBatchingEnabled) {
		mDocumentVector.push_back(bson);
	}
	else {
		mDocumentQueue.push(bson);
	}
}

void MongoTracker::setBatchSize(int batchSize) {
	mBatchSize = batchSize;
}
