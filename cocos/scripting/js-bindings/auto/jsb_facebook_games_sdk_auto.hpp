#pragma once
#include "base/ccConfig.h"

#include "cocos/scripting/js-bindings/jswrapper/SeApi.h"

extern se::Object* __jsb_cocos2d_FacebookPCGamesSDK_proto;
extern se::Class* __jsb_cocos2d_FacebookPCGamesSDK_class;

bool js_register_cocos2d_FacebookPCGamesSDK(se::Object* obj);
bool register_all_facebook_pc_games_sdk(se::Object* obj);
SE_DECLARE_FUNC(js_facebook_pc_games_sdk_FacebookPCGamesSDK_init);
SE_DECLARE_FUNC(js_facebook_pc_games_sdk_FacebookPCGamesSDK_pushEvent);
SE_DECLARE_FUNC(js_facebook_pc_games_sdk_FacebookPCGamesSDK_login);
SE_DECLARE_FUNC(js_facebook_pc_games_sdk_FacebookPCGamesSDK_FacebookPCGamesSDK);

