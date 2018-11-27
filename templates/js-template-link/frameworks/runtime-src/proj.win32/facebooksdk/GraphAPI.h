// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include "TypeDefs.h"

namespace facebook_games_sdk {

class GraphAPI {
 public:
  static GraphAPI& getInstance() {
    static GraphAPI graph_api;
    return graph_api;
  }

  const static utility::string_t& getVersion() {
    return Strings::kGraphAPIVersion;
  }

  const static utility::string_t& getBaseURL() {
    static const auto graph_url =
        U("https://graph.") + Strings::kFacebookDomain;
    return graph_url;
  }

  static JsonResponse_t get(
      const utility::string_t& access_token,
      const utility::string_t& path,
      const std::vector<utility::string_t>& args = {}) {
    return request(access_token, http::methods::GET, path, args);
  }

  static JsonResponse_t del(
      const utility::string_t& access_token,
      const utility::string_t& path,
      const std::vector<utility::string_t>& args = {}) {
    return request(access_token, http::methods::DEL, path, args);
  }

  static JsonResponse_t post(
      const utility::string_t& access_token,
      const utility::string_t& path,
      const std::vector<utility::string_t>& args = {}) {
    return request(access_token, http::methods::POST, path, args);
  }

 private:
  GraphAPI()
      : httpClient_(std::make_unique<client::http_client>(getBaseURL())) {}

  static JsonResponse_t handleJsonResponse(
      const http::http_response& response) {
    if (response.status_code() == http::status_codes::OK) {
      return response.extract_json();
    }
    json::value resp;
    resp[U("success")] = json::value::boolean(false);
    resp[U("error_code")] = json::value::number(response.status_code());
    resp[U("message")] = json::value::string(response.reason_phrase());
    return pplx::task_from_result<json::value>(resp);
  }

  static JsonResponse_t request(
      const utility::string_t& access_token,
      const http::method& method,
      const utility::string_t& path,
      const std::vector<utility::string_t>& args) {
    auto& api = getInstance();
    web::uri_builder builder(U("/") + getVersion() + path);
    if (!access_token.empty())
      builder.append_query(U("access_token"), access_token);
    std::for_each(
        args.begin(), args.end(), [&builder](const utility::string_t a) {
          builder.append_query(a, true);
        });

    return api.httpClient_->request(method, builder.to_string())
        .then(handleJsonResponse)
        .then([](pplx::task<json::value> previous_task) {
          try {
            // Perform actions here to process the JSON value...
            const auto& v = previous_task.get();
            return pplx::task_from_result(v);
          } catch (const http::http_exception& e) {
            return pplx::task_from_exception<json::value>(e);
          }
        });
  }

  std::unique_ptr<client::http_client> httpClient_{};
};

} // namespace facebook_games_sdk
