// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include "TypeDefs.h"

namespace facebook_games_sdk {

class IBrowser {
 public:
  explicit IBrowser(const FBID_t app_id)
      : oauth2Config_(std::make_shared<oauth::oauth2_config>(
            std::to_wstring(app_id),
            U(""),
            getLoginURL(),
            U(""),
            IBrowser::getRedirectURL(),
            U(""))),
        httpConfig_(std::make_unique<client::http_client_config>()) {
    oauth2Config_->set_implicit_grant(true);
    oauth2Config_->set_bearer_auth(false);
    oauth2Config_->set_http_basic_auth(false);
    httpConfig_->set_validate_certificates(false);
    httpConfig_->set_oauth2(*oauth2Config_);
  }

  virtual ~IBrowser() = default;
  IBrowser(IBrowser&&) = default;
  IBrowser(const IBrowser&) = delete;
  IBrowser& operator=(const IBrowser&) = delete;
  IBrowser&& operator=(const IBrowser&&) = delete;

  virtual void openUrl(const utility::string_t& url) = 0;

  static const utility::string_t& getScopes() {
    return Strings::kScopes;
  }

  static const utility::string_t& getLoginURL() {
    return Strings::kLoginURL;
  }

  virtual const utility::string_t& getRedirectURL() {
    return Strings::kRedirectURL;
  }

  std::shared_ptr<oauth::oauth2_config> getOauth2Config() const {
    return oauth2Config_;
  };

  virtual void loginComplete() {
    // Update our configuration with our returned access token.
    httpConfig_->set_oauth2(*oauth2Config_);
  }

 protected:
  std::shared_ptr<oauth::oauth2_config> oauth2Config_;
  std::unique_ptr<client::http_client_config> httpConfig_;
};

} // namespace facebook_games_sdk
