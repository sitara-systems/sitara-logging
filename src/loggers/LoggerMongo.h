#pragma once

#include "cinder/Log.h"
#include "MongoController.h"

namespace sitara {
    class LoggerMongo : public ci::log::Logger {
    public:
        LoggerMongo() = delete;
        LoggerMongo(std::string applicationName, mongocxx::client& client, std::string database, std::string collection);
        ~LoggerMongo();
        void write(const ci::log::Metadata& meta, const std::string& text) override;
    protected:
        mongocxx::client& mClient;
        mongocxx::database mDatabase;
        mongocxx::collection mCollection;
        std::string mApplicationName;
        std::string mDatabaseName;
        std::string mCollectionName;
    };
}