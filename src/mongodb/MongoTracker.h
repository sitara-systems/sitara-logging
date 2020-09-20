#pragma once

#include "Measurements.h"
#include <mongocxx/client.hpp>

namespace sitara {
	namespace logging {
		class MongoTracker : public sitara::logging::BaseTracker {
		public:
			static std::shared_ptr<MongoTracker> make(const std::string& uri, const std::string& database, const std::string& collection);
			~MongoTracker();

			void setup(std::string uuid, std::string applicationName, std::string applicationVersion = "") override;

			void trackEvent(const std::string& category, const std::string& action, const std::string& label = "", const int value = -1, const std::string& customMessage = "") override;
			void trackScreenView(const std::string& screenName, const std::string& customMessage = "") override;
			void trackHit(std::shared_ptr<sitara::logging::BaseHit> hit) override;
		protected:
			MongoTracker(mongocxx::client& client, const std::string& database, const std::string& collection);
			mongocxx::client& mClient;
			mongocxx::database mDatabase;
			mongocxx::collection mCollection;
		};
	}
}
