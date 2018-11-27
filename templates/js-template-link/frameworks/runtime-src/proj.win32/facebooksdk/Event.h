// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <chrono>
#include <map>
#include <string>
#include "EventSettings.h"
#include "EventUtils.h"
#include "GraphAPI.h"
#include "TypeDefs.h"

namespace facebook_games_sdk {

enum EventsFlushBehavior { AUTO, EXPLICIT_ONLY };

class Event {
 public:
  static void initialize(
      const FBID_t& app_id,
      const utility::string_t& access_token = U(""),
      const utility::string_t& extern_id = U(""),
      const utility::string_t& extern_namespace = U("")) {
    appId_ = app_id;
    accessToken_ = access_token;
    externId_ = extern_id;
    externNamespace_ = extern_namespace;
  }

  static void setExternalInfo(
      const utility::string_t& extern_id,
      const utility::string_t& extern_namespace) {
    externId_ = extern_id;
    externNamespace_ = extern_namespace;
  }

  static void logEvent(
      const utility::string_t& event_name,
      const std::map<utility::string_t, utility::string_t>& parameters = {},
      const double value_to_sum = 0.0) {
    json::value event_details;
    event_details[Strings::kParamEventName] = json::value::string(event_name);
    event_details[Strings::kParamLogTime] = json::value::number(
        std::chrono::system_clock::now().time_since_epoch().count());
    if (value_to_sum != 0.0) {
      event_details[Strings::kParamValueToSum] =
          json::value::number(value_to_sum);
    }

    for (auto const& iter : parameters) {
      event_details[iter.first] = json::value::string(iter.second);
    }

    return GraphAPI::post(
               accessToken_,
               EventUtils::buildActivityPathStr(appId_),
               {EventUtils::buildAdvertiserIdStr(),
                EventUtils::buildAdvertiserTrackingStr(
                    EventSettings::advertiserTrackingEnabled),
                EventUtils::buildApplicationTrackingStr(
                    EventSettings::applicationTrackingEnabled),
                EventUtils::buildEventStr(
                    Strings::kEventTypeCustomAppEvents, event_details),
                EventUtils::buildDeviceIdentityStr(),
                EventUtils::buildUserIdentityStr(externId_, externNamespace_)})
        .then([=](json::value json_response) -> pplx::task<void> {
          // Handle failure cases
          if (json_response.has_boolean_field(U("success")) &&
              !json_response[U("success")].as_bool()) {
            OutputDebugString(json_response[U("message")].as_string().c_str());
          }
          return pplx::task_from_result();
        })
        .get();
  }

 private:
  static FBID_t appId_;
  static utility::string_t accessToken_;
  static utility::string_t externId_;
  static utility::string_t externNamespace_;
};

FBID_t Event::appId_ = 0;
utility::string_t Event::accessToken_;
utility::string_t Event::externId_;
utility::string_t Event::externNamespace_;

} // namespace facebook_games_sdk
