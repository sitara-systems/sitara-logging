#include "cinder/Log.h"

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


    class LoggerMongo : public ci::log::Logger {
    public:
        LoggerMongo() = delete;
        LoggerMongo(std::string applicationName, std::string uri, std::string database, std::string collection);
        ~LoggerMongo();
        void write(const ci::log::Metadata& meta, const std::string& text) override;
    protected:
        mongocxx::pool::entry mClient;
        mongocxx::database mDatabase;
        mongocxx::collection mCollection;
        std::string mApplicationName;
        std::string mUri;
        std::string mDatabaseName;
        std::string mCollectionName;
    };
}