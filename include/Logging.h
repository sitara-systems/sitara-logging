#pragma once

#include "Measurements.h"
#include "mongodb/MongoController.h"
#include "mongodb/LoggerMongo.h"
#include "mongodb/MongoTracker.h"
#include "google/GoogleTracker.h"

namespace sitara {
    static std::shared_ptr<sitara::logging::LoggerMongo> createLoggerMongo(std::string applicationName, std::string uri, std::string database, std::string collection) {
        sitara::logging::MongoController::getInstance().createPool(uri);
        mongocxx::pool::entry client = sitara::logging::MongoController::getInstance().getClientFromPool();
        auto ptr = ci::log::makeLogger<sitara::logging::LoggerMongo>(applicationName, *client, database, collection);
        return ptr;
    }
}