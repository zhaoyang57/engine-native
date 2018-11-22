// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <cassert>
#include <memory>
#include "Application.h"
#include "IBrowser.h"
#include "OAuthCodeListener.h"
#include "TypeDefs.h"
#include "User.h"

namespace facebook_games_sdk {

class FacebookGameSDK {
 public:
  static FacebookGameSDK& initialize(
      const FBID_t app_id,
      const std::shared_ptr<IBrowser>& browser) {
    auto& self = getInstance();
    self.application_ = std::make_unique<Application>(app_id);
    self.browser_ = browser;
    return self;
  }

  static FacebookGameSDK& getInstance() {
    static FacebookGameSDK self;
    return self;
  }

  const Application& getApplication() const {
    return *application_;
  }
  const IBrowser& getBrowser() const {
    return *browser_;
  }

  SharedPtrResponse_t<User> doLogin() {
    return doLoginImpl(U(""));
  }

  SharedPtrResponse_t<User> doPermissionRequest() {
    return doLoginImpl(U("&auth_type=rerequest"));
  }

  bool hasAccessToken() const {
    const auto oauth2_config = browser_->getOauth2Config();
    return oauth2_config->token().is_valid_access_token();
  }

  const utility::string_t& getAccessToken() const {
    const auto oauth2_config = browser_->getOauth2Config();
    return oauth2_config->token().access_token();
  }

 private:
  FacebookGameSDK() = default;

  pplx::task<bool> getValidAccessToken(const utility::string_t& extra_params) {
    auto oauth2_config = browser_->getOauth2Config();

    if (oauth2_config->token().is_valid_access_token()) {
      return pplx::task_from_result(true);
    }

    const auto auth_uri = oauth2_config->build_authorization_uri(true)
                              .append(U("&scopes="))
                              .append(IBrowser::getScopes())
                              .append(extra_params);
    listener_ = std::make_unique<OAuthCodeListener>(
        oauth2_config, browser_->getRedirectURL());
    return listener_->openListener().then([=]() {
      browser_->openUrl(auth_uri);
      return listener_->listenForCode().then([=](const bool result) {
        if (result) {
          browser_->loginComplete();
        }
        return result;
      });
    });
  }

  SharedPtrResponse_t<User> getUserFromToken() {
    const auto oauth2_config = browser_->getOauth2Config();
    assert(oauth2_config->token().is_valid_access_token());

    return user_ == nullptr ? User::getFromUserToken(oauth2_config->token())
                                  .then([=](std::shared_ptr<User> usr) {
                                    this->user_ = usr;
                                    return usr;
                                  })
                            : pplx::task_from_result(user_);
  }

  SharedPtrResponse_t<User> doLoginImpl(const utility::string_t& extra_params) {
    const auto oauth2_config = browser_->getOauth2Config();

    return getValidAccessToken(extra_params).then([=](const bool result) {
      assert(result);
      return getUserFromToken();
    });
  }

  std::shared_ptr<IBrowser> browser_;
  std::unique_ptr<Application> application_;
  std::shared_ptr<User> user_;
  std::unique_ptr<OAuthCodeListener> listener_;
};

} // namespace facebook_games_sdk
