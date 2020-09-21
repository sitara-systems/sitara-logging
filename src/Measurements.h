#pragma once

#include "jsoncpp/Json.h"
#include "cinder/Url.h"
#include <bsoncxx/builder/basic/document.hpp>

/*
 * Based off of the Google Measurement Protocol v1
 * These measurements can be used to either write to Google Analytics, or can also
 * be used for writing your own event trackers.
 *
 * All measurements are created as structs and can be exported as a std::string formatted
 * for a HTTP Request (Google Analytics) or a JSON string for export elsewhere.
 */

namespace sitara {
	namespace logging {

		enum class SessionControl {
			NONE,
			START,
			END
		};

		struct BaseHit {
			BaseHit(const std::string& appName, const std::string& uuid, const std::string& version, const std::string& type, const std::string& customMessage = "") :
				mTimestamp(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())),
				mApplicationName(appName),
				mClientId(uuid),
				mProtocolVersion(version),
				mMeasurementType(type),
				mCustomMessage(customMessage)
			{
			};

			virtual ~BaseHit() {};

			virtual std::string getString() const {
				return getStream().str();
			}

			virtual bsoncxx::builder::basic::document getBson() const {
				bsoncxx::builder::basic::document bsonBuilder;

				bsonBuilder.append(bsoncxx::builder::basic::kvp("version", bsoncxx::types::b_utf8(mProtocolVersion)));
				bsonBuilder.append(bsoncxx::builder::basic::kvp("applicationName", bsoncxx::types::b_utf8(mApplicationName)));
				bsonBuilder.append(bsoncxx::builder::basic::kvp("clientId", bsoncxx::types::b_utf8(mClientId)));
				bsonBuilder.append(bsoncxx::builder::basic::kvp("type", bsoncxx::types::b_utf8(mMeasurementType)));
				bsonBuilder.append(bsoncxx::builder::basic::kvp("timestamp", bsoncxx::types::b_date(std::chrono::milliseconds(mTimestamp))));

				if (!mAppVersion.empty()) {
					bsonBuilder.append(bsoncxx::builder::basic::kvp("applicationVersion", bsoncxx::types::b_utf8(mAppVersion)));
				}
				if (!mCustomMessage.empty()) {
					bsonBuilder.append(bsoncxx::builder::basic::kvp("message", bsoncxx::types::b_utf8(mCustomMessage)));
				}

				return bsonBuilder;
			}

			// mandatory values
			std::chrono::milliseconds mTimestamp;
			std::string mApplicationName;
			std::string mClientId; // check about using Windows UuidCreate(), UUID, and UuidToStringA()
			std::string mProtocolVersion;
			std::string mMeasurementType;

			// optional values
			std::string mCustomMessage;
			std::string mAppVersion = "";
			bool mAnonymizeIp = false;
			bool mCacheBuster = true;
			SessionControl mSessionControl = SessionControl::NONE;

		protected:
			virtual std::ostringstream getStream() const {
				std::ostringstream parameterStream;

				std::chrono::milliseconds currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
				float queueTime = (currentTime - mTimestamp).count();

				parameterStream << "v=" << ci::Url::encode(mProtocolVersion) << "&an=" << ci::Url::encode(mApplicationName) <<
					"&cid=" << ci::Url::encode(mClientId) << "&t=" << ci::Url::encode(mMeasurementType) << "&qt=" << queueTime;

				if (!mAppVersion.empty()) {
					parameterStream << "&av=" << ci::Url::encode(mAppVersion);
				}
				if (!mCustomMessage.empty()) {
					parameterStream << ci::Url::encode(mCustomMessage);
				}
				if (mAnonymizeIp) {
					parameterStream << "&aip=1";
				}
				if (mSessionControl == SessionControl::START) {
					parameterStream << "&sc=start";
				}
				else if (mSessionControl == SessionControl::END) {
					parameterStream << "&sc=end";
				}
				if (mCacheBuster) {
					parameterStream << "&z=" << ci::Url::encode(std::to_string(rand()));
				}

				return parameterStream;
			}
		};

		struct Event : public BaseHit {
			Event(const std::string& appName, const std::string& uuid, const std::string& version,
				const std::string& category, const std::string& action, const std::string& label, double value = -1, const std::string& customMessage = "") :
				BaseHit(appName, uuid, version, "event", customMessage),
				mCategory(category),
				mAction(action),
				mLabel(label),
				mValue(value)
			{};

			~Event() {};

			virtual std::string getString() const override {
				return getStream().str();
			}

			virtual bsoncxx::builder::basic::document getBson() const override {
				auto bsonBuilder = BaseHit::getBson();
				bsonBuilder.append(bsoncxx::builder::basic::kvp("category", bsoncxx::types::b_utf8(mCategory)));
				bsonBuilder.append(bsoncxx::builder::basic::kvp("action", bsoncxx::types::b_utf8(mAction)));

				if (!mLabel.empty()) {
					bsonBuilder.append(bsoncxx::builder::basic::kvp("label", bsoncxx::types::b_utf8(mLabel)));
				}
				if (mValue > 0) {
					bsonBuilder.append(bsoncxx::builder::basic::kvp("value", mValue));
				}

				return bsonBuilder;
			}

			std::string mCategory;
			std::string mAction;
			std::string mLabel;
			double mValue;

		protected:
			virtual std::ostringstream getStream() const {
				std::ostringstream parameterStream = BaseHit::getStream();
				parameterStream << "&ec=" << ci::Url::encode(mCategory);
				parameterStream << "&ea=" << ci::Url::encode(mAction);
				if (!mLabel.empty()) {
					parameterStream << "&el=" << ci::Url::encode(mLabel);
				}
				if (mValue > 0) {
					parameterStream << "&ev=" << mValue;
				}

				return parameterStream;
			}
		};

		struct ScreenView : public BaseHit {
			ScreenView(const std::string& appName, const std::string& uuid, const std::string& version,
				const std::string& screenName, const std::string& customMessage = "") :
				BaseHit(appName, uuid, version, "screenview", customMessage),
				mScreenName(screenName)
			{};

			~ScreenView() {};

			virtual std::string getString() const override {
				return getStream().str();
			}

			virtual bsoncxx::builder::basic::document getBson() const override {
				auto bsonBuilder = BaseHit::getBson();
				bsonBuilder.append(bsoncxx::builder::basic::kvp("screenName", bsoncxx::types::b_utf8(mScreenName)));

				return bsonBuilder;
			}

			std::string mScreenName;

		protected:
			virtual std::ostringstream getStream() const {
				std::ostringstream parameterStream = BaseHit::getStream();
				parameterStream << "&cd=" << ci::Url::encode(mScreenName);

				return parameterStream;
			}
		};

		class BaseTracker {
		public:
			BaseTracker() : mProtocolVersion("1"), mBatchingEnabled(false) {};
			~BaseTracker() {};

			virtual void setup(const std::string& uuid, const std::string& applicationName, const std::string& applicationVersion = "") {
				mClientId = uuid;
				mApplicationName = applicationName;
				mApplicationVersion = applicationVersion;
			}

			void trackEvent(const std::string& category, const std::string& action, const std::string& label = "", const int value = -1, const std::string& customMessage = "") {
				std::shared_ptr<Event> event = std::make_shared<Event>(mApplicationName, mClientId, mProtocolVersion, category, action, label, value, customMessage);
				trackHit(event);
			};

			void trackScreenView(const std::string& screenName, const std::string& customMessage = "") {
				std::shared_ptr<ScreenView> view = std::make_shared<ScreenView>(mApplicationName, mClientId, mProtocolVersion, screenName, customMessage);
				trackHit(view);
			};
			
			virtual void trackHit(std::shared_ptr<BaseHit> hit) {};

			void setClientId(const std::string& uuid) {
				mClientId = uuid;
			}

			std::string getClientId() const {
				return mClientId;
			}

			void setApplicationName(const std::string appName) {
				mApplicationName = appName;
			}

			std::string getApplicationName() const {
				return mApplicationName;
			}

			void setApplicationVersion(const std::string appVersion) {
				mApplicationVersion = appVersion;
			}

			std::string getApplicationVersion() const {
				return mApplicationVersion;
			}

			void setBatchingEnabled(bool enabled) {
				mBatchingEnabled = enabled;
			}

			bool isBatchingEnabled() {
				return mBatchingEnabled;
			}

		protected:			
			std::string mClientId;
			std::string mApplicationName;
			std::string mApplicationVersion;
			std::string mProtocolVersion;
			bool mBatchingEnabled;
		};
	}
}