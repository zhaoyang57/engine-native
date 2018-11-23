/****************************************************************************
Copyright (c) 2018 Xiamen Yaji Software Co., Ltd.

http://www.cocos.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated engine source code (the "Software"), a limited,
worldwide, royalty-free, non-assignable, revocable and non-exclusive license
to use Cocos Creator solely to develop games on your target platforms. You shall
not use Cocos Creator software for developing other software or tools that's
used for developing games. You are not granted to publish, distribute,
sublicense, and/or sell copies of Cocos Creator.

The software or tools in this License Agreement are licensed, not sold.
Xiamen Yaji Software Co., Ltd. reserves all rights not expressly granted to you.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "FacebookPCGamesSDK.h"
#include <memory>
#include "Event.h"
#include "SystemDefaultBrowser.h"
#include "User.h"

NS_CC_BEGIN

using namespace facebook_games_sdk;

// Extern ID and namespace are used for App Events and Analytics.
// Specify a namespace for your app and an ID that is unique to the
// user.
static const utility::string_t kExternId(U("<extern_id_here>"));
static const utility::string_t kExternNamespace(U("<extern_namespace_here>"));

FacebookPCGamesSDK::FacebookPCGamesSDK()
	: appId(0)
	, accessToken(U(""))
	, _isEventPrepared(false)
	, loginUser(nullptr)
{
}

FacebookPCGamesSDK::~FacebookPCGamesSDK()
{
	loginUser.reset();
}

void FacebookPCGamesSDK::init(unsigned long long appId)
{
	const auto browser = std::make_shared<SystemDefaultBrowser>(appId);
	FacebookGameSDK::initialize(appId, browser);
}

void FacebookPCGamesSDK::login()
{
	auto user = FacebookGameSDK::getInstance().doLogin().then([=](std::shared_ptr<User> usr) {
		if (usr) {
			usr->getPermissions().get();
		}
		return usr;
	});
	// Calling future::get on a valid future blocks the thread until the provider makes the shared state ready
	loginUser = user.get();
}

// TODO, consider different event 
void FacebookPCGamesSDK::pushEvent(int eventId)
{
	_isEventPrepared ? nullptr : _prepareEvent();

	// Event representing Activate App
	Event::logEvent(Strings::kEventActivateApp);

	// Event representing View Content with custom parameters
	std::map<utility::string_t, utility::string_t> details;
	details[U("success")] = U("true");
	details[U("source")] = U("PC_SDK");
	Event::logEvent(Strings::kEventViewedContent, details);
}

void FacebookPCGamesSDK::_prepareEvent()
{
	accessToken = loginUser ? loginUser->accessToken() : U("");
	// To send events to Facebook
	// You would be able to see the events in Analytics
	Event::initialize(appId, accessToken);

	// Add additional information about the user
	Event::setExternalInfo(kExternId, kExternNamespace);

	_isEventPrepared = true;
}

NS_CC_END