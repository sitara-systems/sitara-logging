#pragma once

#include "Measurements.h"
#include "mongodb/MongoController.h"
#include "mongodb/LoggerMongo.h"
#include "mongodb/MongoTracker.h"

namespace sitara {
    static void createLoggerMongo(std::string applicationName, std::string uri, std::string database, std::string collection) {
        sitara::logging::MongoController::getInstance().createPool(uri);
        mongocxx::pool::entry client = sitara::logging::MongoController::getInstance().getClientFromPool();
        ci::log::makeLogger<sitara::logging::LoggerMongo>(applicationName, *client, database, collection);
    }
}