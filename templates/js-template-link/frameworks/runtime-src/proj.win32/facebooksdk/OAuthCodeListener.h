// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include "TypeDefs.h"

namespace facebook_games_sdk {

class OAuthCodeListener {
 public:
  OAuthCodeListener(
      std::shared_ptr<oauth::oauth2_config> oauth2_config,
      const utility::string_t& listen_url)
      : listener_(std::make_unique<listener::http_listener>(listen_url)),
        oauth2Config_(std::move(oauth2_config)) {}

  ~OAuthCodeListener() {
    shutdown();
  }

  static const utility::string_t kGetTokenJs;

  pplx::task<void> openListener() {
    listener_->support(
        [this](const http::http_request& request) { handleRequest(request); });

    return listener_->open();
  }

  void handleRequest(const http::http_request& request) {
    if (request.request_uri().path() == U("/") &&
        !request.request_uri().query().empty()) {
      respLock_.lock();
      tokenFromQuery(request.request_uri())
          .then([this, request](pplx::task<void> token_task) {
            try {
              token_task.get();
              request
                  .reply(
                      http::status_codes::OK,
                      U("Success. You may now close this window."))
                  .then([=]() { tce_.set(true); });
            } catch (const oauth::oauth2_exception& e) {
              const auto msg = std::string("Error: ").append(e.what());
              request.reply(http::status_codes::OK, msg).then([=]() {
                tce_.set(false);
              });
            }
          });

      respLock_.unlock();
    } else {
      request.reply(http::status_codes::NotFound, kGetTokenJs, U("text/html"))
          .get();
    }
  }

  pplx::task<bool> listenForCode() const {
    return create_task(tce_);
  }

  pplx::task<void> tokenFromQuery(const http::uri& redirected_uri) const {
    auto query = http::uri::split_query(redirected_uri.query());
    const auto state_param =
        query.find(http::oauth2::details::oauth2_strings::state);
    if (state_param == query.end()) {
      return pplx::task_from_exception<void>(
          http::oauth2::experimental::oauth2_exception(
              U("parameter 'state' missing from redirected URI.")));
    }
    if (oauth2Config_->state() != state_param->second) {
      utility::ostringstream_t err;
      err.imbue(std::locale::classic());
      err << U("redirected URI parameter 'state'='") << state_param->second
          << U("' does not match state='") << oauth2Config_->state() << U("'.");
      return pplx::task_from_exception<void>(
          http::oauth2::experimental::oauth2_exception(err.str()));
    }

    const auto code_param =
        query.find(http::oauth2::details::oauth2_strings::code);
    if (code_param != query.end()) {
      return oauth2Config_->token_from_code(code_param->second);
    }

    // NOTE: The redirected URI contains access token only in the implicit
    // grant. The implicit grant never passes a refresh token.
    const auto token_param =
        query.find(http::oauth2::details::oauth2_strings::access_token);
    if (token_param == query.end()) {
      return pplx::task_from_exception<
          void>(http::oauth2::experimental::oauth2_exception(U(
          "either 'code' or 'access_token' parameter must be in the redirected URI.")));
    }

    http::oauth2::experimental::oauth2_token token(token_param->second);
    const auto expires_in_param =
        query.find(http::oauth2::details::oauth2_strings::expires_in);
    if (expires_in_param != query.end()) {
      token.set_expires_in(std::stoi(expires_in_param->second));
    }

    oauth2Config_->set_token(token);
    return pplx::task_from_result();
  }

  void shutdown() const {
    listener_->close();
  }

 private:
  std::unique_ptr<listener::http_listener> listener_;
  pplx::task_completion_event<bool> tce_;
  std::shared_ptr<oauth::oauth2_config> oauth2Config_;
  std::mutex respLock_;
};

const utility::string_t OAuthCodeListener::kGetTokenJs =
    U("<html><head><title>Auth</title></head><body><script>(function()"
      "{window.location.replace("
      "window.location.href.replace('#', '?').replace('??','?'));})();"
      "</script><div>OK!</div></body></html>");

} // namespace facebook_games_sdk
