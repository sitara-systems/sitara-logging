#pragma once

#include "Measurements.h"

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

		  void setup(std::string uuid, std::string applicationName, std::string applicationVersion = "") override;
		  void trackHit(std::shared_ptr<sitara::logging::BaseHit> hit) override;
	  protected:
		  GoogleAnalyticsTracker(const std::string& trackingId);
		  std::string mTrackingId;
		  bool mAutoSessionsEnabled;
		  bool mCacheBusterEnabled;
		  int mMaxHitsPerSession;
		  int mCurrentHitCount;
	  };
  }
}
