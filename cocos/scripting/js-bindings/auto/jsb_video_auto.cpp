#include "scripting/js-bindings/auto/jsb_video_auto.hpp"
#if (USE_VIDEO > 0) && (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS) && !defined(CC_TARGET_OS_TVOS)
#include "scripting/js-bindings/manual/jsb_conversions.hpp"
#include "scripting/js-bindings/manual/jsb_global.h"
#include "ui/videoplayer/VideoPlayer.h"

se::Object* __jsb_cocos2d_VideoPlayer_proto = nullptr;
se::Class* __jsb_cocos2d_VideoPlayer_class = nullptr;

static bool js_video_VideoPlayer_setFullScreenEnabled(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_setFullScreenEnabled : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        bool arg0;
        ok &= seval_to_boolean(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_setFullScreenEnabled : Error processing arguments");
        cobj->setFullScreenEnabled(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_setFullScreenEnabled)

static bool js_video_VideoPlayer_enableProgressGesture(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_enableProgressGesture : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        bool arg0;
        ok &= seval_to_boolean(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_enableProgressGesture : Error processing arguments");
        cobj->enableProgressGesture(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_enableProgressGesture)

static bool js_video_VideoPlayer_setPlaybackRate(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_setPlaybackRate : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        float arg0 = 0;
        ok &= seval_to_float(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_setPlaybackRate : Error processing arguments");
        cobj->setPlaybackRate(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_setPlaybackRate)

static bool js_video_VideoPlayer_setLive(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_setLive : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        bool arg0;
        ok &= seval_to_boolean(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_setLive : Error processing arguments");
        cobj->setLive(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_setLive)

static bool js_video_VideoPlayer_duration(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_duration : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        float result = cobj->duration();
        ok &= float_to_seval(result, &s.rval());
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_duration : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_duration)

static bool js_video_VideoPlayer_enableControls(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_enableControls : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        bool arg0;
        ok &= seval_to_boolean(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_enableControls : Error processing arguments");
        cobj->enableControls(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_enableControls)

static bool js_video_VideoPlayer_pause(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_pause : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    if (argc == 0) {
        cobj->pause();
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_pause)

static bool js_video_VideoPlayer_setPoster(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_setPoster : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= seval_to_std_string(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_setPoster : Error processing arguments");
        cobj->setPoster(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_setPoster)

static bool js_video_VideoPlayer_onPlayEvent(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_onPlayEvent : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 2) {
        int arg0 = 0;
        std::string arg1;
        do { int32_t tmp = 0; ok &= seval_to_int32(args[0], &tmp); arg0 = (int)tmp; } while(false);
        ok &= seval_to_std_string(args[1], &arg1);
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_onPlayEvent : Error processing arguments");
        cobj->onPlayEvent(arg0, arg1);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 2);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_onPlayEvent)

static bool js_video_VideoPlayer_addEventListener(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_addEventListener : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 2) {
        std::string arg0;
        std::function<void (std::string)> arg1;
        ok &= seval_to_std_string(args[0], &arg0);
        do {
            if (args[1].isObject() && args[1].toObject()->isFunction())
            {
                se::Value jsThis(s.thisObject());
                se::Value jsFunc(args[1]);
                jsThis.toObject()->attachObject(jsFunc.toObject());
                auto lambda = [=](std::string larg0) -> void {
                    se::ScriptEngine::getInstance()->clearException();
                    se::AutoHandleScope hs;
        
                    CC_UNUSED bool ok = true;
                    se::ValueArray args;
                    args.resize(1);
                    ok &= std_string_to_seval(larg0, &args[0]);
                    se::Value rval;
                    se::Object* thisObj = jsThis.isObject() ? jsThis.toObject() : nullptr;
                    se::Object* funcObj = jsFunc.toObject();
                    bool succeed = funcObj->call(args, thisObj, &rval);
                    if (!succeed) {
                        se::ScriptEngine::getInstance()->clearException();
                    }
                };
                arg1 = lambda;
            }
            else
            {
                arg1 = nullptr;
            }
        } while(false)
        ;
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_addEventListener : Error processing arguments");
        cobj->addEventListener(arg0, arg1);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 2);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_addEventListener)

static bool js_video_VideoPlayer_showCenterPlayBtn(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_showCenterPlayBtn : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        bool arg0;
        ok &= seval_to_boolean(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_showCenterPlayBtn : Error processing arguments");
        cobj->showCenterPlayBtn(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_showCenterPlayBtn)

static bool js_video_VideoPlayer_seekTo(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_seekTo : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        float arg0 = 0;
        ok &= seval_to_float(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_seekTo : Error processing arguments");
        cobj->seekTo(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_seekTo)

static bool js_video_VideoPlayer_play(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_play : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    if (argc == 0) {
        cobj->play();
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_play)

static bool js_video_VideoPlayer_stop(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_stop : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    if (argc == 0) {
        cobj->stop();
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_stop)

static bool js_video_VideoPlayer_setURL(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_setURL : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= seval_to_std_string(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_setURL : Error processing arguments");
        cobj->setURL(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_setURL)

static bool js_video_VideoPlayer_isKeepAspectRatioEnabled(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_isKeepAspectRatioEnabled : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        bool result = cobj->isKeepAspectRatioEnabled();
        ok &= boolean_to_seval(result, &s.rval());
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_isKeepAspectRatioEnabled : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_isKeepAspectRatioEnabled)

static bool js_video_VideoPlayer_setMute(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_setMute : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    if (argc == 0) {
        cobj->setMute();
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_setMute)

static bool js_video_VideoPlayer_setKeepAspectRatioEnabled(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_setKeepAspectRatioEnabled : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        bool arg0;
        ok &= seval_to_boolean(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_setKeepAspectRatioEnabled : Error processing arguments");
        cobj->setKeepAspectRatioEnabled(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_setKeepAspectRatioEnabled)

static bool js_video_VideoPlayer_setFrame(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_setFrame : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 4) {
        float arg0 = 0;
        float arg1 = 0;
        float arg2 = 0;
        float arg3 = 0;
        ok &= seval_to_float(args[0], &arg0);
        ok &= seval_to_float(args[1], &arg1);
        ok &= seval_to_float(args[2], &arg2);
        ok &= seval_to_float(args[3], &arg3);
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_setFrame : Error processing arguments");
        cobj->setFrame(arg0, arg1, arg2, arg3);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 4);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_setFrame)

static bool js_video_VideoPlayer_setLooping(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_setLooping : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        cobj->setLooping();
        return true;
    }
    if (argc == 1) {
        bool arg0;
        ok &= seval_to_boolean(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_setLooping : Error processing arguments");
        cobj->setLooping(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_setLooping)

static bool js_video_VideoPlayer_currentTime(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_currentTime : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        float result = cobj->currentTime();
        ok &= float_to_seval(result, &s.rval());
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_currentTime : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_currentTime)

static bool js_video_VideoPlayer_setObjectFitStyle(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_setObjectFitStyle : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= seval_to_std_string(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_setObjectFitStyle : Error processing arguments");
        cobj->setObjectFitStyle(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_setObjectFitStyle)

static bool js_video_VideoPlayer_setVisible(se::State& s)
{
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_video_VideoPlayer_setVisible : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        bool arg0;
        ok &= seval_to_boolean(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_video_VideoPlayer_setVisible : Error processing arguments");
        cobj->setVisible(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_video_VideoPlayer_setVisible)

SE_DECLARE_FINALIZE_FUNC(js_cocos2d_VideoPlayer_finalize)

static bool js_video_VideoPlayer_constructor(se::State& s)
{
    cocos2d::VideoPlayer* cobj = new (std::nothrow) cocos2d::VideoPlayer();
    s.thisObject()->setPrivateData(cobj);
    return true;
}
SE_BIND_CTOR(js_video_VideoPlayer_constructor, __jsb_cocos2d_VideoPlayer_class, js_cocos2d_VideoPlayer_finalize)




static bool js_cocos2d_VideoPlayer_finalize(se::State& s)
{
    CCLOGINFO("jsbindings: finalizing JS object %p (cocos2d::VideoPlayer)", s.nativeThisObject());
    cocos2d::VideoPlayer* cobj = (cocos2d::VideoPlayer*)s.nativeThisObject();
    cobj->release();
    return true;
}
SE_BIND_FINALIZE_FUNC(js_cocos2d_VideoPlayer_finalize)

bool js_register_video_VideoPlayer(se::Object* obj)
{
    auto cls = se::Class::create("VideoPlayer", obj, nullptr, _SE(js_video_VideoPlayer_constructor));

    cls->defineFunction("setFullScreenEnabled", _SE(js_video_VideoPlayer_setFullScreenEnabled));
    cls->defineFunction("enableProgressGesture", _SE(js_video_VideoPlayer_enableProgressGesture));
    cls->defineFunction("setPlaybackRate", _SE(js_video_VideoPlayer_setPlaybackRate));
    cls->defineFunction("setLive", _SE(js_video_VideoPlayer_setLive));
    cls->defineFunction("duration", _SE(js_video_VideoPlayer_duration));
    cls->defineFunction("enableControls", _SE(js_video_VideoPlayer_enableControls));
    cls->defineFunction("pause", _SE(js_video_VideoPlayer_pause));
    cls->defineFunction("setPoster", _SE(js_video_VideoPlayer_setPoster));
    cls->defineFunction("onPlayEvent", _SE(js_video_VideoPlayer_onPlayEvent));
    cls->defineFunction("addEventListener", _SE(js_video_VideoPlayer_addEventListener));
    cls->defineFunction("showCenterPlayBtn", _SE(js_video_VideoPlayer_showCenterPlayBtn));
    cls->defineFunction("seekTo", _SE(js_video_VideoPlayer_seekTo));
    cls->defineFunction("play", _SE(js_video_VideoPlayer_play));
    cls->defineFunction("stop", _SE(js_video_VideoPlayer_stop));
    cls->defineFunction("setURL", _SE(js_video_VideoPlayer_setURL));
    cls->defineFunction("isKeepAspectRatioEnabled", _SE(js_video_VideoPlayer_isKeepAspectRatioEnabled));
    cls->defineFunction("setMute", _SE(js_video_VideoPlayer_setMute));
    cls->defineFunction("setKeepAspectRatioEnabled", _SE(js_video_VideoPlayer_setKeepAspectRatioEnabled));
    cls->defineFunction("setFrame", _SE(js_video_VideoPlayer_setFrame));
    cls->defineFunction("setLooping", _SE(js_video_VideoPlayer_setLooping));
    cls->defineFunction("currentTime", _SE(js_video_VideoPlayer_currentTime));
    cls->defineFunction("setObjectFitStyle", _SE(js_video_VideoPlayer_setObjectFitStyle));
    cls->defineFunction("setVisible", _SE(js_video_VideoPlayer_setVisible));
    cls->defineFinalizeFunction(_SE(js_cocos2d_VideoPlayer_finalize));
    cls->install();
    JSBClassType::registerClass<cocos2d::VideoPlayer>(cls);

    __jsb_cocos2d_VideoPlayer_proto = cls->getProto();
    __jsb_cocos2d_VideoPlayer_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

bool register_all_video(se::Object* obj)
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

    js_register_video_VideoPlayer(ns);
    return true;
}

#endif //#if (USE_VIDEO > 0) && (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS) && !defined(CC_TARGET_OS_TVOS)
