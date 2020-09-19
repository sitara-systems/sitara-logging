#pragma once

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>

namespace sitara {
  class MongoController {
     public:
         static MongoController& getInstance() {
             static MongoController mongoController;
             return mongoController;
         }

         /* Create a pool object only once from MongoDB URI */
         void createPool(std::string uri) {
             if (!mClientPool) {
                 mClientPool = (std::unique_ptr<mongocxx::pool>)new mongocxx::pool(mongocxx::uri(uri));
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
