// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

namespace facebook_games_sdk {

class Strings {
public:
  // GraphAPI universal
  static const utility::string_t kGraphAPIVersion;
  static const utility::string_t kFacebookDomain;

  // Facebook Login and permissions
  static const utility::string_t kScopes;
  static const utility::string_t kLoginURL;
  static const utility::string_t kRedirectURL;

  // App Event-related
  static const utility::string_t kParamEventName;
  static const utility::string_t kParamLogTime;
  static const utility::string_t kParamImplicitlyLogged;
  static const utility::string_t kParamValueToSum;
  static const utility::string_t kParamCurrency;
  static const utility::string_t kParamRegistrationMethod;
  static const utility::string_t kParamContentType;
  static const utility::string_t kParamContent;
  static const utility::string_t kParamContentId;
  static const utility::string_t kParamSearchString;
  static const utility::string_t kParamSuccess;
  static const utility::string_t kParamMaxRatingValue;
  static const utility::string_t kParamPaymentInfoAvailable;
  static const utility::string_t kParamNumItems;
  static const utility::string_t kParamLevel;
  static const utility::string_t kParamDescription;
  static const utility::string_t kEventActivateApp;
  static const utility::string_t kEventCompletedRegistration;
  static const utility::string_t kEventViewedContent;
  static const utility::string_t kEventSearch;
  static const utility::string_t kEventRate;
  static const utility::string_t kEventCompletedTutorial;
  static const utility::string_t kEventAddedToCart;
  static const utility::string_t kEventAddedToWishList;
  static const utility::string_t kEventInitiatedCheckout;
  static const utility::string_t kEventAddedPaymentInfo;
  static const utility::string_t kEventPurchased;
  static const utility::string_t kEventAchievedLevel;
  static const utility::string_t kEventUnlockedAchievement;
  static const utility::string_t kEventSpentCredits;
  static const utility::string_t kEventTypeCustomAppEvents;
  static const utility::string_t kParamAdvertiserID;
  static const utility::string_t kParamAdvertiserTracking;
  static const utility::string_t kParamApplicationTracking;
  static const utility::string_t kParamValueInfoVer;
  static const utility::string_t kParamExternID;
  static const utility::string_t kParamExternNamespace;
  static const utility::string_t kParamExtInfo;
  static const utility::string_t kParamExtInfoVersion;
  static const utility::string_t kParamExtInfoAppPkgName;
  static const utility::string_t kParamExtInfoPkgVerCode;
  static const utility::string_t kParamExtInfoPkgVerInfoName;
  static const utility::string_t kParamExtInfoOsVersion;
  static const utility::string_t kParamExtInfoDevModel;
  static const utility::string_t kParamExtInfoLocale;
  static const utility::string_t kParamExtInfoCpuCores;
  static const utility::string_t kParamUserIdentity;
};

const utility::string_t Strings::kGraphAPIVersion = U("v3.1");
const utility::string_t Strings::kFacebookDomain = U("facebook.com");

const utility::string_t Strings::kScopes = U("email");
const utility::string_t Strings::kLoginURL =
    U("https://www.facebook.com/dialog/oauth");
const utility::string_t Strings::kRedirectURL = U("http://localhost:8890");

const utility::string_t Strings::kParamEventName = U("_eventName");
const utility::string_t Strings::kParamLogTime = U("_logTime");
const utility::string_t Strings::kParamImplicitlyLogged =
    U("_implicitlyLogged");
const utility::string_t Strings::kParamValueToSum = U("_valueToSum");
const utility::string_t Strings::kParamCurrency = U("fb_currency");
const utility::string_t Strings::kParamRegistrationMethod =
    U("fb_registeration_method");
const utility::string_t Strings::kParamContentType = U("fb_content_type");
const utility::string_t Strings::kParamContent = U("fb_content");
const utility::string_t Strings::kParamContentId = U("fb_content_id");
const utility::string_t Strings::kParamSearchString = U("fb_search_string");
const utility::string_t Strings::kParamSuccess = U("fb_success");
const utility::string_t Strings::kParamMaxRatingValue =
    U("fb_max_rating_value");
const utility::string_t Strings::kParamPaymentInfoAvailable =
    U("fb_payment_info_available");
const utility::string_t Strings::kParamNumItems = U("fb_num_items");
const utility::string_t Strings::kParamLevel = U("fb_level");
const utility::string_t Strings::kParamDescription = U("fb_description");
const utility::string_t Strings::kEventActivateApp =
    U("fb_mobile_activate_app");
const utility::string_t Strings::kEventCompletedRegistration =
    U("fb_mobile_complete_registeration");
const utility::string_t Strings::kEventViewedContent =
    U("fb_mobile_content_view");
const utility::string_t Strings::kEventSearch = U("fb_mobile_search");
const utility::string_t Strings::kEventRate = U("fb_mobile_rate");
const utility::string_t Strings::kEventCompletedTutorial =
    U("fb_mobile_tutorial_completion");
const utility::string_t Strings::kEventAddedToCart = U("fb_mobile_add_to_cart");
const utility::string_t Strings::kEventAddedToWishList =
    U("fb_mobile_add_to_wishlist");
const utility::string_t Strings::kEventInitiatedCheckout =
    U("fb_mobile_initiated_checkout");
const utility::string_t Strings::kEventAddedPaymentInfo =
    U("fb_mobile_add_payment_info");
const utility::string_t Strings::kEventPurchased = U("fb_mobile_purchase");
const utility::string_t Strings::kEventAchievedLevel =
    U("fb_mobile_level_achieved");
const utility::string_t Strings::kEventUnlockedAchievement =
    U("fb_mobile_achievement_unlocked");
const utility::string_t Strings::kEventSpentCredits =
    U("fb_mobile_spent_credits");
const utility::string_t Strings::kEventTypeCustomAppEvents =
    U("CUSTOM_APP_EVENTS");
const utility::string_t Strings::kParamAdvertiserID = U("advertiser_id");
const utility::string_t Strings::kParamAdvertiserTracking =
    U("advertiser_tracking_enabled");
const utility::string_t Strings::kParamApplicationTracking =
    U("application_tracking_enabled");
const utility::string_t Strings::kParamValueInfoVer = U("pcg1");
const utility::string_t Strings::kParamExternID = U("extern_id");
const utility::string_t Strings::kParamExternNamespace = U("extern_namespace");
const utility::string_t Strings::kParamExtInfo = U("ext_info");
const utility::string_t Strings::kParamExtInfoVersion = U("0");
const utility::string_t Strings::kParamExtInfoAppPkgName = U("1");
const utility::string_t Strings::kParamExtInfoPkgVerCode = U("2");
const utility::string_t Strings::kParamExtInfoPkgVerInfoName = U("3");
const utility::string_t Strings::kParamExtInfoOsVersion = U("4");
const utility::string_t Strings::kParamExtInfoDevModel = U("5");
const utility::string_t Strings::kParamExtInfoLocale = U("6");
const utility::string_t Strings::kParamExtInfoCpuCores = U("12");
const utility::string_t Strings::kParamUserIdentity = U("ud");

} // namespace facebook_games_sdk
