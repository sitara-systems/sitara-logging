#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "Logging.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class GoogleAnalyticsEventsExampleApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

	std::shared_ptr<sitara::logging::GoogleAnalyticsTracker> mTracker;
};

void GoogleAnalyticsEventsExampleApp::setup() {
	Json::Reader reader;
	Json::Value googleSettings;

	reader.parse(ci::loadString(ci::app::loadAsset("settings.json")), googleSettings);
	std::string googleId = googleSettings["tracking_id"].asString();

	std::string appName = "GoogleAnalyticsEventsExample";
	std::string uuid = "c0670cb4-fad0-11ea-adc1-0242ac120002";

	mTracker = sitara::logging::GoogleAnalyticsTracker::make(googleId);
	mTracker->setup(uuid, appName);
}

void GoogleAnalyticsEventsExampleApp::mouseDown( MouseEvent event ) {
	if (event.isLeft()) {
		mTracker->trackEvent("mouse", "click", "left", -1);
	}
	else if (event.isRight()) {
		mTracker->trackEvent("mouse", "click", "right", -1);
	}
}

void GoogleAnalyticsEventsExampleApp::update() {
}

void GoogleAnalyticsEventsExampleApp::draw() {
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( GoogleAnalyticsEventsExampleApp, RendererGl, [=](cinder::app::App::Settings* settings) {
	settings->setConsoleWindowEnabled();
	})