#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "Logging.h"

using namespace ci;
using namespace ci::app;
using namespace std;


/*
 * Inherit from sitara::logging::BaseHit to create a new custom event
 * make sure to define getString(), getStream(), and getBson()
 */
struct ClickEvent : public sitara::logging::BaseHit {
	ClickEvent(const std::string& appName, const std::string& uuid, const std::string& version,
		const std::string& button, const ci::vec2& position, const std::string& customMessage = "") :
		BaseHit(appName, uuid, version, "screenview", customMessage),
		mButton(button),
		mPosition(position)
	{};

	~ClickEvent() {};

	virtual std::string getString() const override {
		return getStream().str();
	}

	virtual bsoncxx::builder::basic::document getBson() const override {
		auto bsonBuilder = BaseHit::getBson();
		bsonBuilder.append(bsoncxx::builder::basic::kvp("button", bsoncxx::types::b_utf8(mButton)));
		bsonBuilder.append(bsoncxx::builder::basic::kvp("position", [&](bsoncxx::builder::basic::sub_document sdoc) {
			sdoc.append(bsoncxx::builder::basic::kvp("x", mPosition.x));
			sdoc.append(bsoncxx::builder::basic::kvp("y", mPosition.y));
		}));

		return bsonBuilder;
	}

	std::string mButton;
	ci::vec2 mPosition;

protected:
	virtual std::ostringstream getStream() const {
		std::ostringstream parameterStream = BaseHit::getStream();
		parameterStream << "&b=" << mButton;
		parameterStream << "&x=" << mPosition.x;
		parameterStream << "&y=" << mPosition.y;

		return parameterStream;
	}
};

/*
 * Then create a custom MongoTracker that inherits from sitara::logging::MongoTracker.
 * 
 * This class needs to have a make() method that returns a std::shared_ptr (copy this from the main class).
 * Then add the constructor and a trackCustomEvent() methods that creates your custom event, and sends it by calling trackHit(yourCustomEvent).
 */

class ClickMongoTracker : public sitara::logging::MongoTracker {
public:
	static std::shared_ptr<ClickMongoTracker> make(const std::string& uri, const std::string& database, const std::string& collection) {
		sitara::logging::MongoController::getInstance().createPool(uri);
		mongocxx::pool::entry client = sitara::logging::MongoController::getInstance().getClientFromPool();
		std::shared_ptr<ClickMongoTracker> mongoTrackerPtr = std::shared_ptr<ClickMongoTracker>(new ClickMongoTracker(*client, database, collection));
		return mongoTrackerPtr;
	}

	void trackClickEvent(const std::string& button, const ci::vec2& position, const std::string& customMessage="") {
		std::shared_ptr<ClickEvent> click = std::make_shared<ClickEvent>(mApplicationName, mClientId, mProtocolVersion, button, position, customMessage);
		trackHit(click);
	}
protected:
	ClickMongoTracker(mongocxx::client& client, const std::string& database, const std::string& collection) : sitara::logging::MongoTracker(client, database, collection) {
	}
};


class MongoCustomEventsExampleApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

	std::shared_ptr<ClickMongoTracker> mTracker;
};

void MongoCustomEventsExampleApp::setup() {
	Json::Reader reader;
	Json::Value mongoEndpoint;
	reader.parse(ci::loadString(ci::app::loadAsset("settings.json")), mongoEndpoint);
	std::string uri = mongoEndpoint["uri"].asString();
	std::string database = mongoEndpoint["database"].asString();;
	std::string collection = mongoEndpoint["collection"].asString();;

	std::string appName = "MongoEventsExample";
	std::string uuid = "c0670cb4-fad0-11ea-adc1-0242ac120002";

	mTracker = ClickMongoTracker::make(uri, database, collection);
	mTracker->setup(uuid, appName);
}

void MongoCustomEventsExampleApp::mouseDown( MouseEvent event ) {
	if (event.isLeft()) {
		mTracker->trackClickEvent("left", event.getPos());
	}
	else if (event.isRight()) {
		mTracker->trackClickEvent("right", event.getPos());
	}
}

void MongoCustomEventsExampleApp::update()
{
}

void MongoCustomEventsExampleApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( MongoCustomEventsExampleApp, RendererGl, [=](cinder::app::App::Settings* settings) {
	settings->setConsoleWindowEnabled();
	})