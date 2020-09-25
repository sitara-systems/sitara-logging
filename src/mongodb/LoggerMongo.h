#pragma once

#include "cinder/Log.h"
#include "MongoController.h"

namespace sitara {
    namespace logging {
        class LoggerMongo : public ci::log::Logger {
        public:
            LoggerMongo() = delete;
            LoggerMongo(const std::string& applicationName, mongocxx::client& client, const std::string& database, const std::string& collection);
            ~LoggerMongo();
            void write(const ci::log::Metadata& meta, const std::string& text) override;
            void setLoggingLevel(ci::log::Level minLevel);
            void setClientId(const std::string& clientId);
            void setApplicationVersion(const std::string& applicationVersion);
        protected:
            mongocxx::client& mClient;
            mongocxx::database mDatabase;
            mongocxx::collection mCollection;
            std::string mApplicationName;
            std::string mApplicationVersion;
            std::string mClientId;
            ci::log::Level mMinLevel;
        };
    }
}
