/**
 * @module cocos2dx_editor_support
 */
var middleware = middleware || {};

/**
 * @class Texture2D
 */
middleware.Texture2D = {

/**
 * @method getRealTextureIndex
 * @return {int}
 */
getRealTextureIndex : function (
)
{
    return 0;
},

/**
 * @method setTexParamCallback
 * @param {function} arg0
 */
setTexParamCallback : function (
func 
)
{
},

/**
 * @method setNativeEffect
 * @param {cc.renderer::Effect} arg0
 */
setNativeEffect : function (
effect 
)
{
},

/**
 * @method setPixelsHigh
 * @param {int} arg0
 */
setPixelsHigh : function (
int 
)
{
},

/**
 * @method getNativeTexture
 * @return {cc.renderer::Texture}
 */
getNativeTexture : function (
)
{
    return cc.renderer::Texture;
},

/**
 * @method setPixelsWide
 * @param {int} arg0
 */
setPixelsWide : function (
int 
)
{
},

/**
 * @method getNativeEffect
 * @return {cc.renderer::Effect}
 */
getNativeEffect : function (
)
{
    return cc.renderer::Effect;
},

/**
 * @method getPixelsHigh
 * @return {int}
 */
getPixelsHigh : function (
)
{
    return 0;
},

/**
 * @method getPixelsWide
 * @return {int}
 */
getPixelsWide : function (
)
{
    return 0;
},

/**
 * @method setRealTextureIndex
 * @param {int} arg0
 */
setRealTextureIndex : function (
int 
)
{
},

/**
 * @method setNativeTexture
 * @param {cc.renderer::Texture} arg0
 */
setNativeTexture : function (
texture 
)
{
},

/**
 * @method setTexParameters
 * @param {cc.middleware::Texture2D::_TexParams} arg0
 */
setTexParameters : function (
_texparams 
)
{
},

/**
 * @method Texture2D
 * @constructor
 */
Texture2D : function (
)
{
},

};

/**
 * @class MiddlewareManager
 */
middleware.MiddlewareManager = {

/**
 * @method removeTimer
 * @param {cc.middleware::IMiddleware} arg0
 */
removeTimer : function (
imiddleware 
)
{
},

/**
 * @method getVB
 * @return {cc.renderer::VertexBuffer}
 */
getVB : function (
)
{
    return cc.renderer::VertexBuffer;
},

/**
 * @method update
 * @param {float} arg0
 */
update : function (
float 
)
{
},

/**
 * @method addTimer
 * @param {cc.middleware::IMiddleware} arg0
 */
addTimer : function (
imiddleware 
)
{
},

/**
 * @method getIB
 * @return {cc.renderer::IndexBuffer}
 */
getIB : function (
)
{
    return cc.renderer::IndexBuffer;
},

/**
 * @method destroyInstance
 */
destroyInstance : function (
)
{
},

/**
 * @method getInstance
 * @return {cc.middleware::MiddlewareManager}
 */
getInstance : function (
)
{
    return cc.middleware::MiddlewareManager;
},

/**
 * @method MiddlewareManager
 * @constructor
 */
MiddlewareManager : function (
)
{
},

};

/**
 * @class MiddlewareRenderHandle
 */
middleware.MiddlewareRenderHandle = {

/**
 * @method reset
 */
reset : function (
)
{
},

/**
 * @method updateIA
 * @param {unsigned int} arg0
 * @param {int} arg1
 * @param {int} arg2
 */
updateIA : function (
int, 
int, 
int 
)
{
},

/**
 * @method MiddlewareRenderHandle
 * @constructor
 */
MiddlewareRenderHandle : function (
)
{
},

};
