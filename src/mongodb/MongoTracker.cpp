#include "MongoTracker.h"
#include "mongodb/MongoController.h"
#include <mongocxx/exception/bulk_write_exception.hpp>

using namespace sitara::logging;

std::shared_ptr<MongoTracker> MongoTracker::make(const std::string& uri, const std::string& database, const std::string& collection) {
	sitara::logging::MongoController::getInstance().createPool(uri);
	mongocxx::pool::entry client = sitara::logging::MongoController::getInstance().getClientFromPool();
	std::shared_ptr<MongoTracker> mongoTrackerPtr = std::shared_ptr<MongoTracker>(new MongoTracker(*client, database, collection));
	return mongoTrackerPtr;
}

MongoTracker::~MongoTracker() {
}

void MongoTracker::setup(const std::string& uuid, const std::string& applicationName, const std::string& applicationVersion) {
	BaseTracker::setup(uuid, applicationName, applicationVersion);
}

void MongoTracker::trackHit(std::shared_ptr<sitara::logging::BaseHit> hit) {
	auto bsonDocument = hit->getBson();
	auto bson = bsonDocument.extract();

	if (mBatchingEnabled) {
			CI_LOG_W("Batching hits is not enabled yet!  Please turn batching off.");
			return;
	}
	else {
			// send it off!
			try {
				auto res = mCollection.insert_one(bson.view());
			}
			catch (mongocxx::bulk_write_exception& e) {
				CI_LOG_E("Error in inserting document : " << e.what());
			}
	}
}

MongoTracker::MongoTracker(mongocxx::client& client, const std::string& database, const std::string& collection) : mClient(client) {
	mDatabase = mClient.database(database);
	mCollection = mDatabase[collection];
}
