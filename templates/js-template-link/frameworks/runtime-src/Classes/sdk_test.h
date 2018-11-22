#pragma once
// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#include <memory>
#include "Event.h"
#include "FacebookGameSDK.h"
// #include "SteamBrowser.h"
#include "SystemDefaultBrowser.h"
#include "User.h"

using namespace facebook_games_sdk;

static const FBID_t kAppID(578401515840132);

// Extern ID and namespace are used for App Events and Analytics.
// Specify a namespace for your app and an ID that is unique to the
// user.
static const utility::string_t kExternId(U("<extern_id_here>"));
static const utility::string_t kExternNamespace(U("<extern_namespace_here>"));

#define BROWSER SystemDefaultBrowser

// Example of how to do Facebook Login and retrieve the granted permissions.
std::shared_ptr<User> loginDemo(FacebookGameSDK& sdk_manager) {
	auto user = sdk_manager.doLogin().then([=](std::shared_ptr<User> usr) {
		if (usr) {
			usr->getPermissions().get();
		}
		return usr;
	});

	return user.get();
}

void appEventDemo() {
	// Event representing Activate App
	Event::logEvent(Strings::kEventActivateApp);

	// Event representing View Content with custom parameters
	std::map<utility::string_t, utility::string_t> details;
	details[U("success")] = U("true");
	details[U("source")] = U("PC_SDK");
	Event::logEvent(Strings::kEventViewedContent, details);
}
