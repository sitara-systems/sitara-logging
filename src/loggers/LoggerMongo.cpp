#include "LoggerMongo.h"
#include <bsoncxx/types.hpp>
#include <bsoncxx/view_or_value.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/exception/bulk_write_exception.hpp>

using namespace sitara;

LoggerMongo::LoggerMongo(std::string applicationName, mongocxx::client& client, std::string database, std::string collection) : mApplicationName(applicationName),
	mClient(client),
	mDatabaseName(database),
	mCollectionName(collection)
{
	mDatabase = mClient.database(mDatabaseName);
	mCollection = mDatabase[mCollectionName];
}

LoggerMongo::~LoggerMongo() {

}

void LoggerMongo::write(const ci::log::Metadata& meta, const std::string& text) {
	std::string locationString = meta.mLocation.getFunctionName();
	std::string logLevel = "";

	switch (meta.mLevel) {
	case ci::log::LEVEL_VERBOSE:
		logLevel = "verbose";
		break;
	case ci::log::LEVEL_DEBUG:
		logLevel = "debug";
		break;
	case ci::log::LEVEL_INFO:
		logLevel = "info";
		break;
	case ci::log::LEVEL_WARNING:
		logLevel = "warning";
		break;
	case ci::log::LEVEL_ERROR:
		logLevel = "error";
		break;
	case ci::log::LEVEL_FATAL:
		logLevel = "fatal";
		break;
	default:
		return;
	}

	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto millisSinceEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

	std::cout << "\tlogLevel : " << logLevel << std::endl;
	std::cout << "\tfunctionName : " << locationString << std::endl;
	std::cout << "\ttimestamp : " << millisSinceEpoch.count() << std::endl;
	std::cout << "\tmessage : " << text << std::endl;

	bsoncxx::document::value document = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("applicationName", bsoncxx::stdx::string_view(mApplicationName)),
		bsoncxx::builder::basic::kvp("logLevel", bsoncxx::stdx::string_view(logLevel)),
		bsoncxx::builder::basic::kvp("functionName", bsoncxx::stdx::string_view(locationString)),
		bsoncxx::builder::basic::kvp("timestamp", bsoncxx::types::b_date(millisSinceEpoch)),
		bsoncxx::builder::basic::kvp("message", bsoncxx::stdx::string_view(text)));

	try {
		auto res = mCollection.insert_one(document.view());
	}
	catch (mongocxx::bulk_write_exception e) {
		std::cout << "Error in inserting document : " << e.what() << std::endl;
	}
}