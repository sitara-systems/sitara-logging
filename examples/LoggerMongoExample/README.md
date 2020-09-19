# LoggerMongoExample
An example of the `LoggerMongo` class, which extends from `ci::log::Logger`.

To protect security of uris (which may contain passwords), it's recommended that you put your mongo settings in a `settings.json` file and have this file listed in your `.gitignore`.  A sample json file (`sample.json`) is provided.

The Logger writes to a mongo database with the following JSON object:

```
{
  "applicationName" : "Name of Application",
  "logLevel" : "info", // can also be "verbose", "debug", "warning", "error", or "fatal"
  "functionName" : "LoggerMongoExampleApp::update", // tells us where exactly the error originated from in software
  "timestamp" : 2020 - 09 - 19T04:38 : 38.504 + 00 : 00,
  "message" : "Any custom message with extra information"
}
```

`mongocxx` requires the use of a Singleton `mongocxx::instance` class, and due to `Logger`'s multithreaded nature the block needs to use clients from a `mongocxx::pool`.  This logic is all wrapped into `sitara::MongoController`.

For convenience, all necessary logic to create a new `LoggerMongo` has been wrapped in  `sitara::createLoggerMongo()`, found in `Logger.h`.
