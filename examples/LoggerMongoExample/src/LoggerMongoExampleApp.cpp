#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/Utilities.h"
#include "jsoncpp/json.h"
#include "Logging.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class LoggerMongoExampleApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void LoggerMongoExampleApp::setup() {
	Json::Reader reader;
	Json::Value mongoEndpoint;
	reader.parse(ci::loadString(ci::app::loadAsset("settings.json")), mongoEndpoint);
	std::string uri = mongoEndpoint["uri"].asString();
	std::string database = mongoEndpoint["database"].asString();;
	std::string collection = mongoEndpoint["collection"].asString();;

	sitara::createLoggerMongo("Sitara Systems Test Application", uri, database, collection);

	std::cout << "Setup Complete" << std::endl;
}

void LoggerMongoExampleApp::mouseDown( MouseEvent event ) {
}

void LoggerMongoExampleApp::update() {
	if (ci::app::getElapsedFrames() % 300 == 0) {
		int random = ci::randInt(0, 6);
		switch (random) {
		case 0:
			CI_LOG_V("This is a verbose message");
			break;
		case 1:
			CI_LOG_D("This is a debug message");
			break;
		case 2:
			CI_LOG_I("This is an informational message");
			break;
		case 3:
			CI_LOG_W("This is a warning message");
			break;
		case 4:
			CI_LOG_E("This is an error message");
			break;
		case 5:
			CI_LOG_F("This is a fatal message");
			break;
		}
	}
}

void LoggerMongoExampleApp::draw() {
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP(LoggerMongoExampleApp, RendererGl, [=](cinder::app::App::Settings* settings) {
	settings->setConsoleWindowEnabled();
	})
