# MongoEventsExample
An example of the `MongoTracker` class, which tracks events according to the [Google Measurements Protocol](https://developers.google.com/analytics/devguides/collection/protocol/v1/devguide#event).

To protect security of uris (which may contain passwords), it's recommended that you put your mongo settings in a `settings.json` file and have this file listed in your `.gitignore`.  A sample json file (`sample.json`) is provided.
