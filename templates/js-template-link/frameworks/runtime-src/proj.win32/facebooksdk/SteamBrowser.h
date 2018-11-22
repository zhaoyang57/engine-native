// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include "IBrowser.h"
// #include "ISteamFriends.h"

namespace facebook_games_sdk {

class SteamBrowser : public IBrowser {
 public:
  explicit SteamBrowser(const FBID_t app_id) : IBrowser(app_id) {}
  void openUrl(const utility::string_t& url) override {
    SteamFriends()->ActivateGameOverlayToWebPage(
        utility::conversions::to_utf8string(url).c_str());
  }
};

} // namespace facebook_games_sdk
