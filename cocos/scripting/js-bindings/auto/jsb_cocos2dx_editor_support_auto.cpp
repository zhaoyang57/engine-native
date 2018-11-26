#include "scripting/js-bindings/auto/jsb_cocos2dx_editor_support_auto.hpp"
#if USE_NATIVE_MIDDLEWARE > 0
#include "scripting/js-bindings/manual/jsb_conversions.hpp"
#include "scripting/js-bindings/manual/jsb_global.h"
#include "editor-support/editor-adapter.h"
#include "editor-support/EditorManager.h"
#include "editor-support/EditorRenderHandle.h"

se::Object* __jsb_editor_Texture2D_proto = nullptr;
se::Class* __jsb_editor_Texture2D_class = nullptr;

static bool js_cocos2dx_editor_support_Texture2D_getRealTextureIndex(se::State& s)
{
    editor::Texture2D* cobj = (editor::Texture2D*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_Texture2D_getRealTextureIndex : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        int result = cobj->getRealTextureIndex();
        ok &= int32_to_seval(result, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_Texture2D_getRealTextureIndex : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_Texture2D_getRealTextureIndex)

static bool js_cocos2dx_editor_support_Texture2D_setTexParamCallback(se::State& s)
{
    editor::Texture2D* cobj = (editor::Texture2D*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_Texture2D_setTexParamCallback : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        std::function<void (int, unsigned int, unsigned int, unsigned int, unsigned int)> arg0;
        do {
            if (args[0].isObject() && args[0].toObject()->isFunction())
            {
                se::Value jsThis(s.thisObject());
                se::Value jsFunc(args[0]);
                jsThis.toObject()->attachObject(jsFunc.toObject());
                auto lambda = [=](int larg0, unsigned int larg1, unsigned int larg2, unsigned int larg3, unsigned int larg4) -> void {
                    se::ScriptEngine::getInstance()->clearException();
                    se::AutoHandleScope hs;
        
                    CC_UNUSED bool ok = true;
                    se::ValueArray args;
                    args.resize(5);
                    ok &= int32_to_seval(larg0, &args[0]);
                    ok &= uint32_to_seval(larg1, &args[1]);
                    ok &= uint32_to_seval(larg2, &args[2]);
                    ok &= uint32_to_seval(larg3, &args[3]);
                    ok &= uint32_to_seval(larg4, &args[4]);
                    se::Value rval;
                    se::Object* thisObj = jsThis.isObject() ? jsThis.toObject() : nullptr;
                    se::Object* funcObj = jsFunc.toObject();
                    bool succeed = funcObj->call(args, thisObj, &rval);
                    if (!succeed) {
                        se::ScriptEngine::getInstance()->clearException();
                    }
                };
                arg0 = lambda;
            }
            else
            {
                arg0 = nullptr;
            }
        } while(false)
        ;
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_Texture2D_setTexParamCallback : Error processing arguments");
        cobj->setTexParamCallback(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_Texture2D_setTexParamCallback)

static bool js_cocos2dx_editor_support_Texture2D_setNativeEffect(se::State& s)
{
    editor::Texture2D* cobj = (editor::Texture2D*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_Texture2D_setNativeEffect : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        cocos2d::renderer::Effect* arg0 = nullptr;
        ok &= seval_to_native_ptr(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_Texture2D_setNativeEffect : Error processing arguments");
        cobj->setNativeEffect(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_Texture2D_setNativeEffect)

static bool js_cocos2dx_editor_support_Texture2D_setPixelsHigh(se::State& s)
{
    editor::Texture2D* cobj = (editor::Texture2D*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_Texture2D_setPixelsHigh : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        int arg0 = 0;
        do { int32_t tmp = 0; ok &= seval_to_int32(args[0], &tmp); arg0 = (int)tmp; } while(false);
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_Texture2D_setPixelsHigh : Error processing arguments");
        cobj->setPixelsHigh(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_Texture2D_setPixelsHigh)

static bool js_cocos2dx_editor_support_Texture2D_getNativeTexture(se::State& s)
{
    editor::Texture2D* cobj = (editor::Texture2D*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_Texture2D_getNativeTexture : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        cocos2d::renderer::Texture* result = cobj->getNativeTexture();
        ok &= native_ptr_to_seval<cocos2d::renderer::Texture>((cocos2d::renderer::Texture*)result, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_Texture2D_getNativeTexture : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_Texture2D_getNativeTexture)

static bool js_cocos2dx_editor_support_Texture2D_setPixelsWide(se::State& s)
{
    editor::Texture2D* cobj = (editor::Texture2D*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_Texture2D_setPixelsWide : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        int arg0 = 0;
        do { int32_t tmp = 0; ok &= seval_to_int32(args[0], &tmp); arg0 = (int)tmp; } while(false);
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_Texture2D_setPixelsWide : Error processing arguments");
        cobj->setPixelsWide(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_Texture2D_setPixelsWide)

static bool js_cocos2dx_editor_support_Texture2D_getNativeEffect(se::State& s)
{
    editor::Texture2D* cobj = (editor::Texture2D*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_Texture2D_getNativeEffect : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        cocos2d::renderer::Effect* result = cobj->getNativeEffect();
        ok &= native_ptr_to_seval<cocos2d::renderer::Effect>((cocos2d::renderer::Effect*)result, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_Texture2D_getNativeEffect : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_Texture2D_getNativeEffect)

static bool js_cocos2dx_editor_support_Texture2D_getPixelsHigh(se::State& s)
{
    editor::Texture2D* cobj = (editor::Texture2D*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_Texture2D_getPixelsHigh : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        int result = cobj->getPixelsHigh();
        ok &= int32_to_seval(result, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_Texture2D_getPixelsHigh : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_Texture2D_getPixelsHigh)

static bool js_cocos2dx_editor_support_Texture2D_getPixelsWide(se::State& s)
{
    editor::Texture2D* cobj = (editor::Texture2D*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_Texture2D_getPixelsWide : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        int result = cobj->getPixelsWide();
        ok &= int32_to_seval(result, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_Texture2D_getPixelsWide : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_Texture2D_getPixelsWide)

static bool js_cocos2dx_editor_support_Texture2D_setRealTextureIndex(se::State& s)
{
    editor::Texture2D* cobj = (editor::Texture2D*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_Texture2D_setRealTextureIndex : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        int arg0 = 0;
        do { int32_t tmp = 0; ok &= seval_to_int32(args[0], &tmp); arg0 = (int)tmp; } while(false);
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_Texture2D_setRealTextureIndex : Error processing arguments");
        cobj->setRealTextureIndex(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_Texture2D_setRealTextureIndex)

static bool js_cocos2dx_editor_support_Texture2D_setNativeTexture(se::State& s)
{
    editor::Texture2D* cobj = (editor::Texture2D*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_Texture2D_setNativeTexture : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        cocos2d::renderer::Texture* arg0 = nullptr;
        ok &= seval_to_native_ptr(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_Texture2D_setNativeTexture : Error processing arguments");
        cobj->setNativeTexture(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_Texture2D_setNativeTexture)

static bool js_cocos2dx_editor_support_Texture2D_setTexParameters(se::State& s)
{
    editor::Texture2D* cobj = (editor::Texture2D*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_Texture2D_setTexParameters : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        editor::Texture2D::_TexParams arg0;
        #pragma warning NO CONVERSION TO NATIVE FOR _TexParams
        ok = false;
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_Texture2D_setTexParameters : Error processing arguments");
        cobj->setTexParameters(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_Texture2D_setTexParameters)

SE_DECLARE_FINALIZE_FUNC(js_editor_Texture2D_finalize)

static bool js_cocos2dx_editor_support_Texture2D_constructor(se::State& s)
{
    editor::Texture2D* cobj = new (std::nothrow) editor::Texture2D();
    s.thisObject()->setPrivateData(cobj);
    return true;
}
SE_BIND_CTOR(js_cocos2dx_editor_support_Texture2D_constructor, __jsb_editor_Texture2D_class, js_editor_Texture2D_finalize)




static bool js_editor_Texture2D_finalize(se::State& s)
{
    CCLOGINFO("jsbindings: finalizing JS object %p (editor::Texture2D)", s.nativeThisObject());
    editor::Texture2D* cobj = (editor::Texture2D*)s.nativeThisObject();
    cobj->release();
    return true;
}
SE_BIND_FINALIZE_FUNC(js_editor_Texture2D_finalize)

bool js_register_cocos2dx_editor_support_Texture2D(se::Object* obj)
{
    auto cls = se::Class::create("Texture2D", obj, nullptr, _SE(js_cocos2dx_editor_support_Texture2D_constructor));

    cls->defineFunction("getRealTextureIndex", _SE(js_cocos2dx_editor_support_Texture2D_getRealTextureIndex));
    cls->defineFunction("setTexParamCallback", _SE(js_cocos2dx_editor_support_Texture2D_setTexParamCallback));
    cls->defineFunction("setNativeEffect", _SE(js_cocos2dx_editor_support_Texture2D_setNativeEffect));
    cls->defineFunction("setPixelsHigh", _SE(js_cocos2dx_editor_support_Texture2D_setPixelsHigh));
    cls->defineFunction("getNativeTexture", _SE(js_cocos2dx_editor_support_Texture2D_getNativeTexture));
    cls->defineFunction("setPixelsWide", _SE(js_cocos2dx_editor_support_Texture2D_setPixelsWide));
    cls->defineFunction("getNativeEffect", _SE(js_cocos2dx_editor_support_Texture2D_getNativeEffect));
    cls->defineFunction("getPixelsHigh", _SE(js_cocos2dx_editor_support_Texture2D_getPixelsHigh));
    cls->defineFunction("getPixelsWide", _SE(js_cocos2dx_editor_support_Texture2D_getPixelsWide));
    cls->defineFunction("setRealTextureIndex", _SE(js_cocos2dx_editor_support_Texture2D_setRealTextureIndex));
    cls->defineFunction("setNativeTexture", _SE(js_cocos2dx_editor_support_Texture2D_setNativeTexture));
    cls->defineFunction("setTexParameters", _SE(js_cocos2dx_editor_support_Texture2D_setTexParameters));
    cls->defineFinalizeFunction(_SE(js_editor_Texture2D_finalize));
    cls->install();
    JSBClassType::registerClass<editor::Texture2D>(cls);

    __jsb_editor_Texture2D_proto = cls->getProto();
    __jsb_editor_Texture2D_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

se::Object* __jsb_editor_EditorManager_proto = nullptr;
se::Class* __jsb_editor_EditorManager_class = nullptr;

static bool js_cocos2dx_editor_support_EditorManager_addTimer(se::State& s)
{
    editor::EditorManager* cobj = (editor::EditorManager*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_EditorManager_addTimer : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        editor::IEditor* arg0 = nullptr;
        ok &= seval_to_native_ptr(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_EditorManager_addTimer : Error processing arguments");
        cobj->addTimer(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_EditorManager_addTimer)

static bool js_cocos2dx_editor_support_EditorManager_removeTimer(se::State& s)
{
    editor::EditorManager* cobj = (editor::EditorManager*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_EditorManager_removeTimer : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        editor::IEditor* arg0 = nullptr;
        ok &= seval_to_native_ptr(args[0], &arg0);
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_EditorManager_removeTimer : Error processing arguments");
        cobj->removeTimer(arg0);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_EditorManager_removeTimer)

static bool js_cocos2dx_editor_support_EditorManager_getIB(se::State& s)
{
    editor::EditorManager* cobj = (editor::EditorManager*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_EditorManager_getIB : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        cocos2d::renderer::IndexBuffer* result = cobj->getIB();
        ok &= native_ptr_to_seval<cocos2d::renderer::IndexBuffer>((cocos2d::renderer::IndexBuffer*)result, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_EditorManager_getIB : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_EditorManager_getIB)

static bool js_cocos2dx_editor_support_EditorManager_getVB(se::State& s)
{
    editor::EditorManager* cobj = (editor::EditorManager*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_EditorManager_getVB : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        cocos2d::renderer::VertexBuffer* result = cobj->getVB();
        ok &= native_ptr_to_seval<cocos2d::renderer::VertexBuffer>((cocos2d::renderer::VertexBuffer*)result, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_EditorManager_getVB : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_EditorManager_getVB)

static bool js_cocos2dx_editor_support_EditorManager_destroyInstance(se::State& s)
{
    const auto& args = s.args();
    size_t argc = args.size();
    if (argc == 0) {
        editor::EditorManager::destroyInstance();
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_EditorManager_destroyInstance)

static bool js_cocos2dx_editor_support_EditorManager_getInstance(se::State& s)
{
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        editor::EditorManager* result = editor::EditorManager::getInstance();
        ok &= native_ptr_to_seval<editor::EditorManager>((editor::EditorManager*)result, &s.rval());
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_EditorManager_getInstance : Error processing arguments");
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_EditorManager_getInstance)

SE_DECLARE_FINALIZE_FUNC(js_editor_EditorManager_finalize)

static bool js_cocos2dx_editor_support_EditorManager_constructor(se::State& s)
{
    editor::EditorManager* cobj = new (std::nothrow) editor::EditorManager();
    s.thisObject()->setPrivateData(cobj);
    se::NonRefNativePtrCreatedByCtorMap::emplace(cobj);
    return true;
}
SE_BIND_CTOR(js_cocos2dx_editor_support_EditorManager_constructor, __jsb_editor_EditorManager_class, js_editor_EditorManager_finalize)




static bool js_editor_EditorManager_finalize(se::State& s)
{
    CCLOGINFO("jsbindings: finalizing JS object %p (editor::EditorManager)", s.nativeThisObject());
    auto iter = se::NonRefNativePtrCreatedByCtorMap::find(s.nativeThisObject());
    if (iter != se::NonRefNativePtrCreatedByCtorMap::end())
    {
        se::NonRefNativePtrCreatedByCtorMap::erase(iter);
        editor::EditorManager* cobj = (editor::EditorManager*)s.nativeThisObject();
        delete cobj;
    }
    return true;
}
SE_BIND_FINALIZE_FUNC(js_editor_EditorManager_finalize)

bool js_register_cocos2dx_editor_support_EditorManager(se::Object* obj)
{
    auto cls = se::Class::create("EditorManager", obj, nullptr, _SE(js_cocos2dx_editor_support_EditorManager_constructor));

    cls->defineFunction("addTimer", _SE(js_cocos2dx_editor_support_EditorManager_addTimer));
    cls->defineFunction("removeTimer", _SE(js_cocos2dx_editor_support_EditorManager_removeTimer));
    cls->defineFunction("getIB", _SE(js_cocos2dx_editor_support_EditorManager_getIB));
    cls->defineFunction("getVB", _SE(js_cocos2dx_editor_support_EditorManager_getVB));
    cls->defineStaticFunction("destroyInstance", _SE(js_cocos2dx_editor_support_EditorManager_destroyInstance));
    cls->defineStaticFunction("getInstance", _SE(js_cocos2dx_editor_support_EditorManager_getInstance));
    cls->defineFinalizeFunction(_SE(js_editor_EditorManager_finalize));
    cls->install();
    JSBClassType::registerClass<editor::EditorManager>(cls);

    __jsb_editor_EditorManager_proto = cls->getProto();
    __jsb_editor_EditorManager_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

se::Object* __jsb_editor_EditorRenderHandle_proto = nullptr;
se::Class* __jsb_editor_EditorRenderHandle_class = nullptr;

static bool js_cocos2dx_editor_support_EditorRenderHandle_reset(se::State& s)
{
    editor::EditorRenderHandle* cobj = (editor::EditorRenderHandle*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_EditorRenderHandle_reset : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    if (argc == 0) {
        cobj->reset();
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_EditorRenderHandle_reset)

static bool js_cocos2dx_editor_support_EditorRenderHandle_updateIA(se::State& s)
{
    editor::EditorRenderHandle* cobj = (editor::EditorRenderHandle*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_editor_support_EditorRenderHandle_updateIA : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 3) {
        size_t arg0 = 0;
        int arg1 = 0;
        int arg2 = 0;
        ok &= seval_to_size(args[0], &arg0);
        do { int32_t tmp = 0; ok &= seval_to_int32(args[1], &tmp); arg1 = (int)tmp; } while(false);
        do { int32_t tmp = 0; ok &= seval_to_int32(args[2], &tmp); arg2 = (int)tmp; } while(false);
        SE_PRECONDITION2(ok, false, "js_cocos2dx_editor_support_EditorRenderHandle_updateIA : Error processing arguments");
        cobj->updateIA(arg0, arg1, arg2);
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 3);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_editor_support_EditorRenderHandle_updateIA)

SE_DECLARE_FINALIZE_FUNC(js_editor_EditorRenderHandle_finalize)

static bool js_cocos2dx_editor_support_EditorRenderHandle_constructor(se::State& s)
{
    editor::EditorRenderHandle* cobj = new (std::nothrow) editor::EditorRenderHandle();
    s.thisObject()->setPrivateData(cobj);
    se::NonRefNativePtrCreatedByCtorMap::emplace(cobj);
    return true;
}
SE_BIND_CTOR(js_cocos2dx_editor_support_EditorRenderHandle_constructor, __jsb_editor_EditorRenderHandle_class, js_editor_EditorRenderHandle_finalize)



extern se::Object* __jsb_cocos2d_renderer_CustomRenderHandle_proto;

static bool js_editor_EditorRenderHandle_finalize(se::State& s)
{
    CCLOGINFO("jsbindings: finalizing JS object %p (editor::EditorRenderHandle)", s.nativeThisObject());
    auto iter = se::NonRefNativePtrCreatedByCtorMap::find(s.nativeThisObject());
    if (iter != se::NonRefNativePtrCreatedByCtorMap::end())
    {
        se::NonRefNativePtrCreatedByCtorMap::erase(iter);
        editor::EditorRenderHandle* cobj = (editor::EditorRenderHandle*)s.nativeThisObject();
        delete cobj;
    }
    return true;
}
SE_BIND_FINALIZE_FUNC(js_editor_EditorRenderHandle_finalize)

bool js_register_cocos2dx_editor_support_EditorRenderHandle(se::Object* obj)
{
    auto cls = se::Class::create("EditorRenderHandle", obj, __jsb_cocos2d_renderer_CustomRenderHandle_proto, _SE(js_cocos2dx_editor_support_EditorRenderHandle_constructor));

    cls->defineFunction("reset", _SE(js_cocos2dx_editor_support_EditorRenderHandle_reset));
    cls->defineFunction("updateIA", _SE(js_cocos2dx_editor_support_EditorRenderHandle_updateIA));
    cls->defineFinalizeFunction(_SE(js_editor_EditorRenderHandle_finalize));
    cls->install();
    JSBClassType::registerClass<editor::EditorRenderHandle>(cls);

    __jsb_editor_EditorRenderHandle_proto = cls->getProto();
    __jsb_editor_EditorRenderHandle_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

bool register_all_cocos2dx_editor_support(se::Object* obj)
{
    // Get the ns
    se::Value nsVal;
    if (!obj->getProperty("jsbEditor", &nsVal))
    {
        se::HandleObject jsobj(se::Object::createPlainObject());
        nsVal.setObject(jsobj);
        obj->setProperty("jsbEditor", nsVal);
    }
    se::Object* ns = nsVal.toObject();

    js_register_cocos2dx_editor_support_EditorRenderHandle(ns);
    js_register_cocos2dx_editor_support_EditorManager(ns);
    js_register_cocos2dx_editor_support_Texture2D(ns);
    return true;
}

#endif //#if USE_NATIVE_MIDDLEWARE > 0
