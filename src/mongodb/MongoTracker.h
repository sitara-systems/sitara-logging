#pragma once

#include "Measurements.h"
#include <mongocxx/client.hpp>
#include <queue>
#include <atomic>

namespace sitara {
	namespace logging {
		class MongoTracker : public sitara::logging::BaseTracker {
		public:
			MongoTracker(const std::string& uri, const std::string& database, const std::string& collection);
			~MongoTracker();
			void setup(const std::string& uuid, const std::string& applicationName, const std::string& applicationVersion = "") override;
			void trackHit(std::shared_ptr<sitara::logging::BaseHit> hit) override;
			void setBatchSize(int batchSize);
		protected:
			std::string mUri;
			std::string mDatabaseName;
			std::string mCollectionName;
			std::atomic<bool> mIsWriting;
			std::thread mWriteThread;
			std::mutex mDocumentMutex;
			int mBatchSize;
			std::queue<bsoncxx::v_noabi::document::value> mDocumentQueue;
			std::vector<bsoncxx::v_noabi::document::value> mDocumentVector;
		};
	}
}
