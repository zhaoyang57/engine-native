#include "scripting/js-bindings/auto/jsb_facebook_games_sdk_auto.hpp"
#include "scripting/js-bindings/manual/jsb_conversions.hpp"
#include "scripting/js-bindings/manual/jsb_global.h"
#include "FacebookPCGamesSDK.h"

se::Object* __jsb_cocos2d_FacebookPCGamesSDK_proto = nullptr;
se::Class* __jsb_cocos2d_FacebookPCGamesSDK_class = nullptr;

static bool js_facebook_pc_games_sdk_FacebookPCGamesSDK_init(se::State& s)
{
    cocos2d::FacebookPCGamesSDK* cobj = (cocos2d::FacebookPCGamesSDK*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_facebook_pc_games_sdk_FacebookPCGamesSDK_init : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= seval_to_std_string(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_facebook_pc_games_sdk_FacebookPCGamesSDK_init : Error processing arguments");
        cobj->init(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_facebook_pc_games_sdk_FacebookPCGamesSDK_init)

static bool js_facebook_pc_games_sdk_FacebookPCGamesSDK_pushEvent(se::State& s)
{
    cocos2d::FacebookPCGamesSDK* cobj = (cocos2d::FacebookPCGamesSDK*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_facebook_pc_games_sdk_FacebookPCGamesSDK_pushEvent : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        int arg0 = 0;
        do { int32_t tmp = 0; ok &= seval_to_int32(args[0], &tmp); arg0 = (int)tmp; } while(false);
        SE_PRECONDITION2(ok, false, "js_facebook_pc_games_sdk_FacebookPCGamesSDK_pushEvent : Error processing arguments");
        cobj->pushEvent(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_facebook_pc_games_sdk_FacebookPCGamesSDK_pushEvent)

static bool js_facebook_pc_games_sdk_FacebookPCGamesSDK_login(se::State& s)
{
    cocos2d::FacebookPCGamesSDK* cobj = (cocos2d::FacebookPCGamesSDK*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_facebook_pc_games_sdk_FacebookPCGamesSDK_login : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    if (argc == 0) {
        cobj->login();
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_facebook_pc_games_sdk_FacebookPCGamesSDK_login)

SE_DECLARE_FINALIZE_FUNC(js_cocos2d_FacebookPCGamesSDK_finalize)

static bool js_facebook_pc_games_sdk_FacebookPCGamesSDK_constructor(se::State& s)
{
    cocos2d::FacebookPCGamesSDK* cobj = new (std::nothrow) cocos2d::FacebookPCGamesSDK();
    s.thisObject()->setPrivateData(cobj);
    return true;
}
SE_BIND_CTOR(js_facebook_pc_games_sdk_FacebookPCGamesSDK_constructor, __jsb_cocos2d_FacebookPCGamesSDK_class, js_cocos2d_FacebookPCGamesSDK_finalize)



static bool js_cocos2d_FacebookPCGamesSDK_finalize(se::State& s)
{
    CCLOGINFO("jsbindings: finalizing JS object %p (cocos2d::FacebookPCGamesSDK)", s.nativeThisObject());
    cocos2d::FacebookPCGamesSDK* cobj = (cocos2d::FacebookPCGamesSDK*)s.nativeThisObject();
    cobj->release();
    return true;
}
SE_BIND_FINALIZE_FUNC(js_cocos2d_FacebookPCGamesSDK_finalize)

bool js_register_facebook_pc_games_sdk_FacebookPCGamesSDK(se::Object* obj)
{
    auto cls = se::Class::create("FacebookPCGamesSDK", obj, nullptr, _SE(js_facebook_pc_games_sdk_FacebookPCGamesSDK_constructor));

    cls->defineFunction("init", _SE(js_facebook_pc_games_sdk_FacebookPCGamesSDK_init));
    cls->defineFunction("pushEvent", _SE(js_facebook_pc_games_sdk_FacebookPCGamesSDK_pushEvent));
    cls->defineFunction("login", _SE(js_facebook_pc_games_sdk_FacebookPCGamesSDK_login));
    cls->defineFinalizeFunction(_SE(js_cocos2d_FacebookPCGamesSDK_finalize));
    cls->install();
    JSBClassType::registerClass<cocos2d::FacebookPCGamesSDK>(cls);

    __jsb_cocos2d_FacebookPCGamesSDK_proto = cls->getProto();
    __jsb_cocos2d_FacebookPCGamesSDK_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

bool register_all_facebook_pc_games_sdk(se::Object* obj)
{
    // Get the ns
    se::Value nsVal;
    if (!obj->getProperty("jsb", &nsVal))
    {
        se::HandleObject jsobj(se::Object::createPlainObject());
        nsVal.setObject(jsobj);
        obj->setProperty("jsb", nsVal);
    }
    se::Object* ns = nsVal.toObject();

    js_register_facebook_pc_games_sdk_FacebookPCGamesSDK(ns);
    return true;
}

