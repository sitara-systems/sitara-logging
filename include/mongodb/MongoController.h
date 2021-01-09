#pragma once

#include "cinder/Log.h"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/exception/exception.hpp>

namespace sitara {
    namespace logging {
        class MongoController {
        public:
            static MongoController& getInstance() {
                static MongoController mongoController;
                return mongoController;
            }

            /* Create a pool object only once from MongoDB URI */
            void createPool(const std::string& uri) {
                if (!mClientPool) {
                    try {
                        mClientPool = (std::unique_ptr<mongocxx::pool>)new mongocxx::pool(mongocxx::uri(uri));
                    }
                    catch (mongocxx::exception& e) {
                        CI_LOG_E("Error connecting to MongoDB : " << e.what());
                    }
                }
            }

            /* Acquire a client from the pool */
            mongocxx::pool::entry getClientFromPool() {
                return mClientPool->acquire();
            }

            ~MongoController() {};
        private:
            MongoController() {};
            mongocxx::instance mInstance;
            std::unique_ptr<mongocxx::pool> mClientPool;
        };
    }
}
