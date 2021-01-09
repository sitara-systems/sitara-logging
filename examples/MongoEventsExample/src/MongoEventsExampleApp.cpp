#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "Logging.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class MongoEventsExampleApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

	std::shared_ptr<sitara::logging::MongoTracker> mTracker;
};

void MongoEventsExampleApp::setup() {
	Json::Reader reader;
	Json::Value mongoEndpoint;
	try {
		reader.parse(ci::loadString(ci::app::loadAsset("settings.json")), mongoEndpoint);
	}
	catch (ci::app::AssetLoadExc &e) {
		std::cout << "ERROR : " << e.what() << std::endl;
		ci::sleep(5000);
		exit(0);
	}
	std::string uri = mongoEndpoint["uri"].asString();
	std::string database = mongoEndpoint["database"].asString();;
	std::string collection = mongoEndpoint["collection"].asString();;

	std::string appName = "MongoEventsExample";
	std::string uuid = "c0670cb4-fad0-11ea-adc1-0242ac120002";

	mTracker = std::make_shared<sitara::logging::MongoTracker>(uri, database, collection);
	mTracker->setup(uuid, appName);
}

void MongoEventsExampleApp::mouseDown( MouseEvent event ) {
	if (event.isLeft()) {
		mTracker->trackEvent("mouse", "click", "left", -1, "x:" + std::to_string(event.getPos().x) + ", y:" + std::to_string(event.getPos().y));
	}
	else if (event.isRight()) {
		mTracker->trackEvent("mouse", "click", "right", -1, "x:" + std::to_string(event.getPos().x) + ", y:" + std::to_string(event.getPos().y));
	}
}

void MongoEventsExampleApp::update() {
}

void MongoEventsExampleApp::draw() {
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP(MongoEventsExampleApp, RendererGl, [=](cinder::app::App::Settings* settings) {
	settings->setConsoleWindowEnabled();
	})