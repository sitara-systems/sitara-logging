#pragma once

#include "mongodb/MongoController.h"
#include "mongodb/LoggerMongo.h"

namespace sitara {
    static void createLoggerMongo(std::string applicationName, std::string uri, std::string database, std::string collection) {
        sitara::MongoController::getInstance().createPool(uri);
        mongocxx::pool::entry client = sitara::MongoController::getInstance().getClientFromPool();
        ci::log::makeLogger<sitara::LoggerMongo>(applicationName, *client, database, collection);
    }
}