#pragma once
#include "base/ccConfig.h"
#if (USE_GFX_RENDERER > 0) && (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#include "cocos/scripting/js-bindings/jswrapper/SeApi.h"

extern se::Object* __jsb_cocos2d_renderer_ProgramLib_proto;
extern se::Class* __jsb_cocos2d_renderer_ProgramLib_class;

bool js_register_cocos2d_renderer_ProgramLib(se::Object* obj);
bool register_all_renderer(se::Object* obj);
SE_DECLARE_FUNC(js_renderer_ProgramLib_getProgram);
SE_DECLARE_FUNC(js_renderer_ProgramLib_define);
SE_DECLARE_FUNC(js_renderer_ProgramLib_getKey);
SE_DECLARE_FUNC(js_renderer_ProgramLib_ProgramLib);

extern se::Object* __jsb_cocos2d_renderer_BaseRenderer_proto;
extern se::Class* __jsb_cocos2d_renderer_BaseRenderer_class;

bool js_register_cocos2d_renderer_BaseRenderer(se::Object* obj);
bool register_all_renderer(se::Object* obj);
SE_DECLARE_FUNC(js_renderer_BaseRenderer_getProgramLib);
SE_DECLARE_FUNC(js_renderer_BaseRenderer_init);
SE_DECLARE_FUNC(js_renderer_BaseRenderer_BaseRenderer);

extern se::Object* __jsb_cocos2d_renderer_View_proto;
extern se::Class* __jsb_cocos2d_renderer_View_class;

bool js_register_cocos2d_renderer_View(se::Object* obj);
bool register_all_renderer(se::Object* obj);
SE_DECLARE_FUNC(js_renderer_View_View);

extern se::Object* __jsb_cocos2d_renderer_Camera_proto;
extern se::Class* __jsb_cocos2d_renderer_Camera_class;

bool js_register_cocos2d_renderer_Camera(se::Object* obj);
bool register_all_renderer(se::Object* obj);
SE_DECLARE_FUNC(js_renderer_Camera_getDepth);
SE_DECLARE_FUNC(js_renderer_Camera_setFov);
SE_DECLARE_FUNC(js_renderer_Camera_getFrameBuffer);
SE_DECLARE_FUNC(js_renderer_Camera_setStencil);
SE_DECLARE_FUNC(js_renderer_Camera_getOrthoHeight);
SE_DECLARE_FUNC(js_renderer_Camera_setCullingMask);
SE_DECLARE_FUNC(js_renderer_Camera_getStencil);
SE_DECLARE_FUNC(js_renderer_Camera_setType);
SE_DECLARE_FUNC(js_renderer_Camera_setFar);
SE_DECLARE_FUNC(js_renderer_Camera_setFrameBuffer);
SE_DECLARE_FUNC(js_renderer_Camera_setRect);
SE_DECLARE_FUNC(js_renderer_Camera_setClearFlags);
SE_DECLARE_FUNC(js_renderer_Camera_getFar);
SE_DECLARE_FUNC(js_renderer_Camera_getType);
SE_DECLARE_FUNC(js_renderer_Camera_getCullingMask);
SE_DECLARE_FUNC(js_renderer_Camera_setNear);
SE_DECLARE_FUNC(js_renderer_Camera_setStages);
SE_DECLARE_FUNC(js_renderer_Camera_setOrthoHeight);
SE_DECLARE_FUNC(js_renderer_Camera_setDepth);
SE_DECLARE_FUNC(js_renderer_Camera_getStages);
SE_DECLARE_FUNC(js_renderer_Camera_getFov);
SE_DECLARE_FUNC(js_renderer_Camera_setColor);
SE_DECLARE_FUNC(js_renderer_Camera_setWorldMatrix);
SE_DECLARE_FUNC(js_renderer_Camera_getNear);
SE_DECLARE_FUNC(js_renderer_Camera_getClearFlags);
SE_DECLARE_FUNC(js_renderer_Camera_Camera);

extern se::Object* __jsb_cocos2d_renderer_ForwardRenderer_proto;
extern se::Class* __jsb_cocos2d_renderer_ForwardRenderer_class;

bool js_register_cocos2d_renderer_ForwardRenderer(se::Object* obj);
bool register_all_renderer(se::Object* obj);
SE_DECLARE_FUNC(js_renderer_ForwardRenderer_renderCamera);
SE_DECLARE_FUNC(js_renderer_ForwardRenderer_init);
SE_DECLARE_FUNC(js_renderer_ForwardRenderer_render);
SE_DECLARE_FUNC(js_renderer_ForwardRenderer_ForwardRenderer);

extern se::Object* __jsb_cocos2d_renderer_Effect_proto;
extern se::Class* __jsb_cocos2d_renderer_Effect_class;

bool js_register_cocos2d_renderer_Effect(se::Object* obj);
bool register_all_renderer(se::Object* obj);
SE_DECLARE_FUNC(js_renderer_Effect_setDefineValue);
SE_DECLARE_FUNC(js_renderer_Effect_getHash);
SE_DECLARE_FUNC(js_renderer_Effect_clear);
SE_DECLARE_FUNC(js_renderer_Effect_updateHash);
SE_DECLARE_FUNC(js_renderer_Effect_Effect);

extern se::Object* __jsb_cocos2d_renderer_Light_proto;
extern se::Class* __jsb_cocos2d_renderer_Light_class;

bool js_register_cocos2d_renderer_Light(se::Object* obj);
bool register_all_renderer(se::Object* obj);
SE_DECLARE_FUNC(js_renderer_Light_getShadowScale);
SE_DECLARE_FUNC(js_renderer_Light_getRange);
SE_DECLARE_FUNC(js_renderer_Light_setShadowResolution);
SE_DECLARE_FUNC(js_renderer_Light_getFrustumEdgeFalloff);
SE_DECLARE_FUNC(js_renderer_Light_setSpotExp);
SE_DECLARE_FUNC(js_renderer_Light_setShadowType);
SE_DECLARE_FUNC(js_renderer_Light_setType);
SE_DECLARE_FUNC(js_renderer_Light_getViewProjMatrix);
SE_DECLARE_FUNC(js_renderer_Light_getShadowBias);
SE_DECLARE_FUNC(js_renderer_Light_getShadowDarkness);
SE_DECLARE_FUNC(js_renderer_Light_getSpotAngle);
SE_DECLARE_FUNC(js_renderer_Light_getSpotExp);
SE_DECLARE_FUNC(js_renderer_Light_getViewPorjMatrix);
SE_DECLARE_FUNC(js_renderer_Light_getType);
SE_DECLARE_FUNC(js_renderer_Light_getIntensity);
SE_DECLARE_FUNC(js_renderer_Light_getShadowMaxDepth);
SE_DECLARE_FUNC(js_renderer_Light_getWorldMatrix);
SE_DECLARE_FUNC(js_renderer_Light_getShadowMap);
SE_DECLARE_FUNC(js_renderer_Light_getColor);
SE_DECLARE_FUNC(js_renderer_Light_setIntensity);
SE_DECLARE_FUNC(js_renderer_Light_getShadowMinDepth);
SE_DECLARE_FUNC(js_renderer_Light_setShadowMinDepth);
SE_DECLARE_FUNC(js_renderer_Light_update);
SE_DECLARE_FUNC(js_renderer_Light_setShadowDarkness);
SE_DECLARE_FUNC(js_renderer_Light_setWorldMatrix);
SE_DECLARE_FUNC(js_renderer_Light_setSpotAngle);
SE_DECLARE_FUNC(js_renderer_Light_setRange);
SE_DECLARE_FUNC(js_renderer_Light_setShadowScale);
SE_DECLARE_FUNC(js_renderer_Light_setColor);
SE_DECLARE_FUNC(js_renderer_Light_setShadowMaxDepth);
SE_DECLARE_FUNC(js_renderer_Light_setFrustumEdgeFalloff);
SE_DECLARE_FUNC(js_renderer_Light_getShadowType);
SE_DECLARE_FUNC(js_renderer_Light_getShadowResolution);
SE_DECLARE_FUNC(js_renderer_Light_setShadowBias);
SE_DECLARE_FUNC(js_renderer_Light_Light);

extern se::Object* __jsb_cocos2d_renderer_Pass_proto;
extern se::Class* __jsb_cocos2d_renderer_Pass_class;

bool js_register_cocos2d_renderer_Pass(se::Object* obj);
bool register_all_renderer(se::Object* obj);
SE_DECLARE_FUNC(js_renderer_Pass_getStencilTest);
SE_DECLARE_FUNC(js_renderer_Pass_setStencilBack);
SE_DECLARE_FUNC(js_renderer_Pass_setStencilTest);
SE_DECLARE_FUNC(js_renderer_Pass_setCullMode);
SE_DECLARE_FUNC(js_renderer_Pass_setBlend);
SE_DECLARE_FUNC(js_renderer_Pass_setProgramName);
SE_DECLARE_FUNC(js_renderer_Pass_disableStencilTest);
SE_DECLARE_FUNC(js_renderer_Pass_setStencilFront);
SE_DECLARE_FUNC(js_renderer_Pass_setDepth);
SE_DECLARE_FUNC(js_renderer_Pass_Pass);

extern se::Object* __jsb_cocos2d_renderer_Scene_proto;
extern se::Class* __jsb_cocos2d_renderer_Scene_class;

bool js_register_cocos2d_renderer_Scene(se::Object* obj);
bool register_all_renderer(se::Object* obj);
SE_DECLARE_FUNC(js_renderer_Scene_reset);
SE_DECLARE_FUNC(js_renderer_Scene_getCameraCount);
SE_DECLARE_FUNC(js_renderer_Scene_addCamera);
SE_DECLARE_FUNC(js_renderer_Scene_removeCamera);
SE_DECLARE_FUNC(js_renderer_Scene_getLightCount);
SE_DECLARE_FUNC(js_renderer_Scene_getCamera);
SE_DECLARE_FUNC(js_renderer_Scene_getLight);
SE_DECLARE_FUNC(js_renderer_Scene_getCameras);
SE_DECLARE_FUNC(js_renderer_Scene_addView);
SE_DECLARE_FUNC(js_renderer_Scene_setDebugCamera);
SE_DECLARE_FUNC(js_renderer_Scene_removeView);
SE_DECLARE_FUNC(js_renderer_Scene_addLight);
SE_DECLARE_FUNC(js_renderer_Scene_removeLight);
SE_DECLARE_FUNC(js_renderer_Scene_Scene);

extern se::Object* __jsb_cocos2d_renderer_NodeProxy_proto;
extern se::Class* __jsb_cocos2d_renderer_NodeProxy_class;

bool js_register_cocos2d_renderer_NodeProxy(se::Object* obj);
bool register_all_renderer(se::Object* obj);
SE_DECLARE_FUNC(js_renderer_NodeProxy_addChild);
SE_DECLARE_FUNC(js_renderer_NodeProxy_setMatrixDirty);
SE_DECLARE_FUNC(js_renderer_NodeProxy_removeAllChildren);
SE_DECLARE_FUNC(js_renderer_NodeProxy_addHandle);
SE_DECLARE_FUNC(js_renderer_NodeProxy_getChildren);
SE_DECLARE_FUNC(js_renderer_NodeProxy_removeHandle);
SE_DECLARE_FUNC(js_renderer_NodeProxy_generateJSMatrix);
SE_DECLARE_FUNC(js_renderer_NodeProxy_setChildrenOrderDirty);
SE_DECLARE_FUNC(js_renderer_NodeProxy_updateLocalMatrixFromJS);
SE_DECLARE_FUNC(js_renderer_NodeProxy_setParent);
SE_DECLARE_FUNC(js_renderer_NodeProxy_getName);
SE_DECLARE_FUNC(js_renderer_NodeProxy_setName);
SE_DECLARE_FUNC(js_renderer_NodeProxy_setGroupID);
SE_DECLARE_FUNC(js_renderer_NodeProxy_visitAsRoot);
SE_DECLARE_FUNC(js_renderer_NodeProxy_reset);
SE_DECLARE_FUNC(js_renderer_NodeProxy_getParent);
SE_DECLARE_FUNC(js_renderer_NodeProxy_getGroupID);
SE_DECLARE_FUNC(js_renderer_NodeProxy_removeChild);
SE_DECLARE_FUNC(js_renderer_NodeProxy_setLocalZOrder);
SE_DECLARE_FUNC(js_renderer_NodeProxy_getChildrenCount);
SE_DECLARE_FUNC(js_renderer_NodeProxy_NodeProxy);

extern se::Object* __jsb_cocos2d_renderer_MeshBuffer_proto;
extern se::Class* __jsb_cocos2d_renderer_MeshBuffer_class;

bool js_register_cocos2d_renderer_MeshBuffer(se::Object* obj);
bool register_all_renderer(se::Object* obj);
SE_DECLARE_FUNC(js_renderer_MeshBuffer_reset);
SE_DECLARE_FUNC(js_renderer_MeshBuffer_getVertexOffset);
SE_DECLARE_FUNC(js_renderer_MeshBuffer_getIndexBuffer);
SE_DECLARE_FUNC(js_renderer_MeshBuffer_updateOffset);
SE_DECLARE_FUNC(js_renderer_MeshBuffer_getIndexStart);
SE_DECLARE_FUNC(js_renderer_MeshBuffer_getIndexOffset);
SE_DECLARE_FUNC(js_renderer_MeshBuffer_request);
SE_DECLARE_FUNC(js_renderer_MeshBuffer_requestStatic);
SE_DECLARE_FUNC(js_renderer_MeshBuffer_uploadData);
SE_DECLARE_FUNC(js_renderer_MeshBuffer_getVertexBuffer);
SE_DECLARE_FUNC(js_renderer_MeshBuffer_getByteOffset);
SE_DECLARE_FUNC(js_renderer_MeshBuffer_getVertexStart);
SE_DECLARE_FUNC(js_renderer_MeshBuffer_destroy);
SE_DECLARE_FUNC(js_renderer_MeshBuffer_MeshBuffer);

extern se::Object* __jsb_cocos2d_renderer_RenderHandle_proto;
extern se::Class* __jsb_cocos2d_renderer_RenderHandle_class;

bool js_register_cocos2d_renderer_RenderHandle(se::Object* obj);
bool register_all_renderer(se::Object* obj);
SE_DECLARE_FUNC(js_renderer_RenderHandle_setMeshCount);
SE_DECLARE_FUNC(js_renderer_RenderHandle_setVertexFormat);
SE_DECLARE_FUNC(js_renderer_RenderHandle_enable);
SE_DECLARE_FUNC(js_renderer_RenderHandle_handle);
SE_DECLARE_FUNC(js_renderer_RenderHandle_postHandle);
SE_DECLARE_FUNC(js_renderer_RenderHandle_getUseModel);
SE_DECLARE_FUNC(js_renderer_RenderHandle_setUseModel);
SE_DECLARE_FUNC(js_renderer_RenderHandle_enabled);
SE_DECLARE_FUNC(js_renderer_RenderHandle_setVertsDirty);
SE_DECLARE_FUNC(js_renderer_RenderHandle_fillBuffers);
SE_DECLARE_FUNC(js_renderer_RenderHandle_disable);
SE_DECLARE_FUNC(js_renderer_RenderHandle_getMeshCount);
SE_DECLARE_FUNC(js_renderer_RenderHandle_getVertexFormat);
SE_DECLARE_FUNC(js_renderer_RenderHandle_getEffect);
SE_DECLARE_FUNC(js_renderer_RenderHandle_RenderHandle);

extern se::Object* __jsb_cocos2d_renderer_ModelBatcher_proto;
extern se::Class* __jsb_cocos2d_renderer_ModelBatcher_class;

bool js_register_cocos2d_renderer_ModelBatcher(se::Object* obj);
bool register_all_renderer(se::Object* obj);
SE_DECLARE_FUNC(js_renderer_ModelBatcher_reset);
SE_DECLARE_FUNC(js_renderer_ModelBatcher_getFlow);
SE_DECLARE_FUNC(js_renderer_ModelBatcher_startBatch);
SE_DECLARE_FUNC(js_renderer_ModelBatcher_setCurrentBuffer);
SE_DECLARE_FUNC(js_renderer_ModelBatcher_terminateBatch);
SE_DECLARE_FUNC(js_renderer_ModelBatcher_flush);
SE_DECLARE_FUNC(js_renderer_ModelBatcher_commit);
SE_DECLARE_FUNC(js_renderer_ModelBatcher_getBuffer);
SE_DECLARE_FUNC(js_renderer_ModelBatcher_getCurrentBuffer);
SE_DECLARE_FUNC(js_renderer_ModelBatcher_ModelBatcher);

extern se::Object* __jsb_cocos2d_renderer_RenderFlow_proto;
extern se::Class* __jsb_cocos2d_renderer_RenderFlow_class;

bool js_register_cocos2d_renderer_RenderFlow(se::Object* obj);
bool register_all_renderer(se::Object* obj);
SE_DECLARE_FUNC(js_renderer_RenderFlow_getRenderScene);
SE_DECLARE_FUNC(js_renderer_RenderFlow_getModelBatcher);
SE_DECLARE_FUNC(js_renderer_RenderFlow_visit);
SE_DECLARE_FUNC(js_renderer_RenderFlow_getDevice);
SE_DECLARE_FUNC(js_renderer_RenderFlow_RenderFlow);

#endif //#if (USE_GFX_RENDERER > 0) && (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
