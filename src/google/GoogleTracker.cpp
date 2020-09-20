#include "GoogleTracker.h"
#include "cinder/Log.h"

using namespace sitara::logging;

std::shared_ptr<GoogleAnalyticsTracker> GoogleAnalyticsTracker::make(const std::string& trackingId) {
    return std::shared_ptr<GoogleAnalyticsTracker>();
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

void GoogleAnalyticsTracker::setup(std::string uuid, std::string applicationName, std::string applicationVersion) {
    BaseTracker::setup(uuid, applicationName, applicationVersion);
}

void GoogleAnalyticsTracker::trackHit(std::shared_ptr<BaseHit> hit) {
    auto bsonDocument = hit->getBson();
    auto bson = bsonDocument.extract();
    
    if (mBatchingEnabled) {
        CI_LOG_W("Batching hits is not enabled yet!  Please turn batching off.");
        return;
    }
    else {
        // send it off!

    }
}

GoogleAnalyticsTracker::GoogleAnalyticsTracker(const std::string& trackingId) : mTrackingId(trackingId),
    mAutoSessionsEnabled(false),
    mCacheBusterEnabled(false),
    mMaxHitsPerSession(20),
    mCurrentHitCount(0)
{
}
