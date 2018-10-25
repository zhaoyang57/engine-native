/****************************************************************************
 Copyright (c) 2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "RenderHandle.hpp"

#include "RenderFlow.hpp"
#include "RenderSystem.hpp"
#include "cocos/scripting/js-bindings/jswrapper/SeApi.h"
#include "cocos/scripting/js-bindings/manual/jsb_conversions.hpp"
#include "cocos/scripting/js-bindings/auto/jsb_renderer_auto.hpp"

RENDERER_BEGIN

RenderHandle::RenderHandle()
: _useModel(false)
, _enabled(false)
, _dirty(true)
, _color(0)
{
}

RenderHandle::~RenderHandle()
{
    setEnable(false);
}

void RenderHandle::setEnable(bool enabled)
{
    if (_enabled != enabled)
    {
        _enabled = enabled;
        
        RenderSystem* renderSys = RenderFlow::getInstance()->getRenderSystem();
        if (_enabled)
        {
            renderSys->addRenderHandle(this);
        }
        else
        {
            renderSys->removeRenderHandle(this);
        }
    }
}

void RenderHandle::updateRenderData()
{
    se::ScriptEngine::getInstance()->clearException();
    se::AutoHandleScope hs;
    
    se::Value jsVal;
    bool ok = native_ptr_to_seval<RenderHandle>((RenderHandle*)this, &jsVal);
    CCASSERT(ok, "RenderHandle_updateRenderData : JS object missing");
    
    se::Value func;
    ok = __jsb_cocos2d_renderer_RenderHandle_proto->getProperty("_updateRenderData", &func);
    CCASSERT(ok, "RenderHandle_updateRenderData : RenderHandle _updateRenderData is not implemented");
    
    // Update local matrix
    func.toObject()->call(se::EmptyValueArray, jsVal.toObject());
}

void RenderHandle::handle(NodeProxy *node)
{
    
}

void RenderHandle::postHandle(NodeProxy *node)
{
}

RENDERER_END
