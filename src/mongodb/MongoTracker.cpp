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

void MongoTracker::setup(std::string uuid, std::string applicationName, std::string applicationVersion) {
	BaseTracker::setup(uuid, applicationName, applicationVersion);
}

void MongoTracker::trackEvent(const std::string& category, const std::string& action, const std::string& label, const int value, const std::string& customMessage) {
	std::shared_ptr<Event> event = std::make_shared<Event>(mApplicationName, mClientId, mProtocolVersion, category, action, label, value, customMessage);
	trackHit(event);
}

void MongoTracker::trackScreenView(const std::string& screenName, const std::string& customMessage) {
	std::shared_ptr<ScreenView> view = std::make_shared<ScreenView>(mApplicationName, mClientId, mProtocolVersion, screenName, customMessage);
	trackHit(view);
}

void MongoTracker::trackHit(std::shared_ptr<sitara::logging::BaseHit> hit) {
	auto bsonDocument = hit->getBson();
	auto bson = bsonDocument.extract();

	try {
		auto res = mCollection.insert_one(bson.view());
	}
	catch (mongocxx::bulk_write_exception& e) {
		CI_LOG_E("Error in inserting document : " << e.what());
	}
}

MongoTracker::MongoTracker(mongocxx::client& client, const std::string& database, const std::string& collection) : mClient(client) {
	mDatabase = mClient.database(database);
	mCollection = mDatabase[collection];
}