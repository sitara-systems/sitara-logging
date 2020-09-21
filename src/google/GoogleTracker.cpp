#include "GoogleTracker.h"
#include "cinder/app/App.h"
#include "cinder/Log.h"

using namespace sitara::logging;

std::shared_ptr<GoogleAnalyticsTracker> GoogleAnalyticsTracker::make(const std::string& trackingId) {
	return std::shared_ptr<GoogleAnalyticsTracker>(new GoogleAnalyticsTracker(trackingId));
}

GoogleAnalyticsTracker::~GoogleAnalyticsTracker() {
}

bool GoogleAnalyticsTracker::getAutoSessionsEnabled() const {
	return mAutoSessionsEnabled;
}

void GoogleAnalyticsTracker::setAutoSessionsEnabled(const bool sessionsEnabled) {
	mAutoSessionsEnabled = sessionsEnabled;
}

int GoogleAnalyticsTracker::getMaxHitsPerSession() const {
	return mMaxHitsPerSession;
}

void GoogleAnalyticsTracker::setMaxHitsPerSession(const int maxHits) {
	mMaxHitsPerSession = maxHits;
}

bool GoogleAnalyticsTracker::getCacheBusterEnabled() const {
	return mCacheBusterEnabled;
}

void GoogleAnalyticsTracker::setCacheBusterEnabled(const bool enabled) {
	mCacheBusterEnabled = enabled;
}

void GoogleAnalyticsTracker::setup(const std::string& uuid, const std::string& applicationName, const std::string& applicationVersion) {
	BaseTracker::setup(uuid, applicationName, applicationVersion);
}

void GoogleAnalyticsTracker::trackHit(std::shared_ptr<BaseHit> hit) {
	std::string trackerString = "&tid=" + ci::Url::encode(mTrackingId);
	std::string httpString = hit->getString() + trackerString;

	if (mBatchingEnabled) {
		CI_LOG_W("Batching hits is not enabled yet!  Please turn batching off.");
		return;
	}
	else {
		// send it off!
		sitara::http::HttpRequest Post;
		Post.mUrl = mBaseUrl + mSoloSuffix;
		Post.mMethod = sitara::http::HTTP_POST;
		Post.mRequestBody = httpString;
		Post.mCallback = [=](sitara::http::HttpResponse* response, sitara::http::HttpClient* curl) {
			std::cout << "POST Request complete with code " << response->mResponseCode << std::endl;
		};

		mHttpClient->addRequest(Post);
	}
}

GoogleAnalyticsTracker::GoogleAnalyticsTracker(const std::string& trackingId) :
	mTrackingId(trackingId),
	mAutoSessionsEnabled(false),
	mCacheBusterEnabled(false),
	mMaxHitsPerSession(20),
	mCurrentHitCount(0),
	mBaseUrl("www.google-analytics.com"),
	mBatchSuffix("/batch"),
	mSoloSuffix("/collect") 
{
	mHttpClient = sitara::http::HttpClient::make();
	mHttpClient->setCaCertPath(ci::app::getAssetPath("ssl\\cacert.pem").string());
}
