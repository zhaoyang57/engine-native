// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <set>
#include "GraphAPI.h"
#include "IEntity.h"
#include "TypeDefs.h"

namespace facebook_games_sdk {

struct ProfilePicture {
  ProfilePicture(const int h, const int w, utility::string_t url)
      : height(h), width(h), url(std::move(url)) {}

  int height;
  int width;
  utility::string_t url;
};

struct UserFriend : IEntity {
  explicit UserFriend(const FBID_t id) : IEntity(id){};
};

class User : public IEntity {
 public:
  explicit User(
      utility::string_t access_token,
      const FBID_t user_id = 0,
      utility::string_t email = U(""),
      utility::string_t name = U(""))
      : IEntity(user_id),
        accessToken_(std::move(access_token)),
        email_(std::move(email)),
        name_(std::move(name)) {}

  // Factory methods
  static SharedPtrResponse_t<User> getFromUserToken(
      const oauth::oauth2_token& user_token) {
    return GraphAPI::get(
               user_token.access_token(),
               U("/me"),
               {U("fields=id,name,email,picture")})
        .then([=](json::value json_response) -> std::shared_ptr<User> {
          auto field_id = json_response.has_field(U("id"))
              ? std::stoll(json_response[U("id")].as_string())
              : 0;
          auto field_email = json_response.has_field(U("email"))
              ? json_response[U("email")].as_string()
              : U("");
          auto field_name = json_response.has_field(U("name"))
              ? json_response[U("name")].as_string()
              : U("");

          auto user = std::make_shared<User>(
              user_token.access_token(), field_id, field_email, field_name);

          if (json_response.has_field(U("picture"))) {
            auto pic =
                json_response[U("picture")].as_object()[U("data")].as_object();
            user->picture_ = std::make_unique<ProfilePicture>(
                pic[U("height")].as_integer(),
                pic[U("width")].as_integer(),
                pic[U("url")].as_string());
          }

          return user;
        });
  }

  // GET methods
  pplx::task<void> getPermissions() {
    return GraphAPI::get(accessToken_, U("/me/permissions"))
        .then([=](json::value json_response) -> pplx::task<void> {
          if (json_response.has_field(U("data"))) {
            auto data = json_response[U("data")].as_array();
            grantedPermissions_.clear();
            for (auto permission = data.begin(); permission != data.end();
                 ++permission) {
              auto perm_obj = permission->as_object();
              if (perm_obj[U("status")].as_string() == U("granted")) {
                grantedPermissions_.insert(
                    perm_obj[U("permission")].as_string());
              }
            }
          }
          return pplx::task_from_result();
        });
  }

  pplx::task<void> getFriends() {
    return GraphAPI::get(accessToken_, U("/me/friends"))
        .then([=](json::value json_response) -> pplx::task<void> {
          if (json_response.has_field(U("data"))) {
            auto data = json_response[U("data")].as_array();
            friends_.clear();
            for (auto f = data.begin(); f != data.end(); ++f) {
              const auto field_id = json_response.has_field(U("id"))
                  ? std::stoll(json_response[U("id")].as_string())
                  : 0;
              auto friend_obj = f->as_object();
              friends_.emplace_back(UserFriend(field_id));
            }
          }
          return pplx::task_from_result();
        });
  }

  // Mutators
  pplx::task<void> deauthorizeApp() const {
    return GraphAPI::del(accessToken_, U("/me/permissions"))
        .then([](json::value response) -> pplx::task<void> {
          return pplx::task_from_result();
        });
  }

  void logout() {
    accessToken_.clear();
    picture_.reset();
    name_.clear();
    email_.clear();
    id_ = 0;
  }

  // Accessors

  const utility::string_t& email() const {
    return email_;
  }
  const utility::string_t& name() const {
    return name_;
  }
  const utility::string_t& accessToken() const {
    return accessToken_;
  }
  bool hasPicture() const {
    return picture_ != nullptr;
  }
  const ProfilePicture& picture() const {
    return *picture_;
  }
  const std::vector<UserFriend>& friends() const {
    return friends_;
  }
  const std::set<utility::string_t>& permissions() const {
    return grantedPermissions_;
  }

 private:
  std::unique_ptr<ProfilePicture> picture_{};
  std::set<utility::string_t> grantedPermissions_{};
  std::vector<UserFriend> friends_{};
  utility::string_t accessToken_;
  utility::string_t email_;
  utility::string_t name_;
};

} // namespace facebook_games_sdk
