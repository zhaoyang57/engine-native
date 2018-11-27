// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include "TypeDefs.h"
#if defined(_WIN32)
#include <Windows.h>
#endif
#include <string>

namespace facebook_games_sdk {

class EventUtils {
 public:
  static utility::string_t buildAdvertiserIdStr(
      const utility::string_t& advertiser_id = U("0000-0000-0000-0000")) {
    return Strings::kParamAdvertiserID + U("=") + advertiser_id;
  }

  static utility::string_t buildActivityPathStr(FBID_t app_id) {
    return U("/") + utility::conversions::details::to_string_t(app_id) +
        U("/activities");
  }

  static utility::string_t buildApplicationTrackingStr(
      const bool enabled = true) {
    return Strings::kParamApplicationTracking + U("=") +
        (enabled ? U("1") : U("0"));
  }

  static utility::string_t buildAdvertiserTrackingStr(
      const bool enabled = true) {
    return Strings::kParamAdvertiserTracking + U("=") +
        (enabled ? U("1") : U("0"));
  }

  static utility::string_t buildUserIdentityStr(
      const utility::string_t& extern_id,
      const utility::string_t& extern_namespace) {
    if (extern_id.empty() || extern_namespace.empty()) {
      return U("");
    }

    json::value ud_obj;
    ud_obj[Strings::kParamExternID] = json::value::string(extern_id);
    ud_obj[Strings::kParamExternNamespace] =
        json::value::string(extern_namespace);

    return Strings::kParamUserIdentity + U("=") + ud_obj.serialize();
  }

  static utility::string_t buildDeviceIdentityStr() {
    json::value ext_info_obj;
    ext_info_obj[Strings::kParamExtInfoVersion] =
        json::value::string(Strings::kParamValueInfoVer);
    ext_info_obj[Strings::kParamExtInfoLocale] =
        json::value::string(getLocale());
    return Strings::kParamExtInfo + U("=") + ext_info_obj.serialize();
  }

  static utility::string_t buildEventStr(
      const utility::string_t& event_type,
      const json::value& event_details) {
    return U("event=") + event_type + U("&custom_events=[") +
        event_details.serialize() + U("]");
  }

#if defined(_WIN32)
  static utility::string_t getLocale() {
    TCHAR lang_arr[9];
    TCHAR country_arr[9];
    GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO639LANGNAME, lang_arr, 9);
    GetLocaleInfo(
        LOCALE_SYSTEM_DEFAULT, LOCALE_SISO3166CTRYNAME, country_arr, 9);

    utility::string_t lang_val(&lang_arr[0]);
    const utility::string_t language(lang_val.begin(), lang_val.end());

    utility::string_t country_val(&country_arr[0]);
    const utility::string_t country(country_val.begin(), country_val.end());

    return language + U("_") + country;
  }
#else

#endif
};

} // namespace facebook_games_sdk
