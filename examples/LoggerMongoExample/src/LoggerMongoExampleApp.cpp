#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "jsoncpp/json.h"
#include "LoggerMongo.h"

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

	ci::log::makeLogger<sitara::LoggerMongo>("Sitara Systems Test Logger", uri, database, collection);

	std::cout << "Setup Complete" << std::endl;
}

void LoggerMongoExampleApp::mouseDown( MouseEvent event ) {
}

void LoggerMongoExampleApp::update() {
	if (ci::app::getElapsedFrames() % 120 == 0) {
		CI_LOG_I("Test Message");
	}
}

void LoggerMongoExampleApp::draw() {
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP(LoggerMongoExampleApp, RendererGl, [=](cinder::app::App::Settings* settings) {
	settings->setConsoleWindowEnabled();
	})
