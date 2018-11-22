// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include "TypeDefs.h"

namespace facebook_games_sdk {

class IEntity {
 public:
  explicit IEntity(const FBID_t id) : id_(id){};

  FBID_t getId() const {
    return id_;
  }

 protected:
  FBID_t id_;
};

} // namespace facebook_games_sdk
