#pragma once

#include "Measurements.h"
#include "cinder/app/App.h"
#include "cinder/Utilities.h"
#include "mongodb/MongoController.h"
#include "mongodb/LoggerMongo.h"
#include "mongodb/MongoTracker.h"
#include "google/GoogleTracker.h"

namespace sitara {
	namespace logging {
		static inline std::shared_ptr<sitara::logging::LoggerMongo> createLoggerMongo(std::string applicationName, std::string uri, std::string database, std::string collection) {
			sitara::logging::MongoController::getInstance().createPool(uri);
			mongocxx::pool::entry client = sitara::logging::MongoController::getInstance().getClientFromPool();
			auto ptr = ci::log::makeLogger<sitara::logging::LoggerMongo>(applicationName, *client, database, collection);
			return ptr;
		}

		inline void loadLoggerFromFile(const std::filesystem::path& mongoConfigFile) {
			if (!ci::app::getAssetPath(mongoConfigFile).empty()) {
				Json::Reader jsonReader;
				Json::Value mongoRoot;

				bool result = jsonReader.parse(ci::loadString(ci::app::loadAsset(mongoConfigFile)), mongoRoot);
				if (result) {
					auto errorLogger = createLoggerMongo(mongoRoot["applicationName"].asString(), mongoRoot["uri"].asString(),
						mongoRoot["errors"]["database"].asString(),
						mongoRoot["errors"]["collection"].asString());

					errorLogger->setLoggingLevel(ci::log::Level::LEVEL_INFO);
					errorLogger->setClientId(mongoRoot["clientId"].asString());
					errorLogger->setApplicationVersion(mongoRoot["applicationVersion"].asString());
				}
				else {
					CI_LOG_W("Error reading json file " << mongoConfigFile << "; please check for valid JSON.  Application will not write to MongoDB endpoint.");
				}
			}
			else {
				CI_LOG_I("Could not find JSON file " << mongoConfigFile << "; application will not write to MongoDB endpoint.");
			}
		}
	}
}