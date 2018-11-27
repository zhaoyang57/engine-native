// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include "IEntity.h"
#include "TypeDefs.h"

namespace facebook_games_sdk {

class Application : public IEntity {
 public:
  // Do not embed your app secret into client side code.
  // Only use app secret for server side calls.
  explicit Application(const FBID_t id, utility::string_t app_secret = U(""))
      : IEntity(id), appSecret_(std::move(app_secret)) {}

  const utility::string_t& getAppAccessToken() const {
    return appSecret_;
  }

 private:
  const utility::string_t appSecret_;
};

} // namespace facebook_games_sdk
