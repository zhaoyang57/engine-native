// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

namespace facebook_games_sdk {

class EventSettings {
 public:
  static bool applicationTrackingEnabled;
  static bool advertiserTrackingEnabled;
};

bool EventSettings::applicationTrackingEnabled = true;
bool EventSettings::advertiserTrackingEnabled = true;

} // namespace facebook_games_sdk
