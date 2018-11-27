// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include "IBrowser.h"
#include "TypeDefs.h"

#ifdef _WIN32
#include "WinUser.h"
#include "shellapi.h"
#endif

namespace facebook_games_sdk {

class SystemDefaultBrowser : public IBrowser {
 public:
  explicit SystemDefaultBrowser(const FBID_t app_id) : IBrowser(app_id) {}
  void openUrl(const utility::string_t& url) override {
#if defined(_WIN32)
    // NOTE: Windows desktop only.
    ShellExecuteA(
        nullptr,
        "open",
        utility::conversions::utf16_to_utf8(url).c_str(),
        nullptr,
        nullptr,
        SW_SHOWNORMAL);
#elif defined(__APPLE__)
    // NOTE: OS X only.
    string_t browser_cmd(U("open \"") + auth_uri + U("\""));
    (void)system(browser_cmd.c_str());
#else
    // NOTE: Linux/X11 only.
    string_t browser_cmd(U("xdg-open \"") + auth_uri + U("\""));
    (void)system(browser_cmd.c_str());
#endif
  }
};

} // namespace facebook_games_sdk
