#pragma once

#include "Measurements.h"
#include "HttpClient.h"

namespace sitara {
  namespace logging {
	  class GoogleAnalyticsTracker : public sitara::logging::BaseTracker {
	  public:
		  static std::shared_ptr<GoogleAnalyticsTracker> make(const std::string& trackingId);
		  ~GoogleAnalyticsTracker();

		  bool getAutoSessionsEnabled() const;
		  void setAutoSessionsEnabled(const bool sessionsEnabled);

		  int getMaxHitsPerSession() const;
		  void setMaxHitsPerSession(const int maxHits);

		  bool getCacheBusterEnabled() const;
		  void setCacheBusterEnabled(const bool enabled);

		  void setup(const std::string& uuid, const std::string& applicationName, const std::string& applicationVersion = "") override;
		  void trackHit(std::shared_ptr<sitara::logging::BaseHit> hit) override;
	  protected:
		  GoogleAnalyticsTracker(const std::string& trackingId);
		  std::string mTrackingId;
		  std::string mBaseUrl;
		  std::string mBatchSuffix;
		  std::string mSoloSuffix;
		  bool mAutoSessionsEnabled;
		  bool mCacheBusterEnabled;
		  int mMaxHitsPerSession;
		  int mCurrentHitCount;
		  std::shared_ptr<sitara::http::HttpClient> mHttpClient;
	  };
  }
}
