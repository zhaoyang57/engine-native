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

#pragma once

#include "../Macro.h"
#include "SystemHandle.hpp"
#include "NodeProxy.hpp"
#include "../renderer/Effect.h"

RENDERER_BEGIN

struct Vertex
{
    float x;
    float y;
    float z;
    float u;
    float v;
    uint32_t color;
};

class RenderHandle : SystemHandle
{
public:
    RenderHandle();
    virtual ~RenderHandle();
    virtual void handle(NodeProxy *node) override;
    virtual void postHandle(NodeProxy *node) override;
    
    virtual void updateRenderData();
    
    void setEnable(bool enabled);
    
protected:
//    void updateColor();
//    void render();
//    void postUpdateRenderData();
//    void postRender();
    
protected:
    std::vector<std::vector<Vertex>> _meshes;
    std::vector<Effect> _effects;
    
    uint32_t _color;
    bool _enabled;
    bool _useModel;
    bool _dirty;
};

RENDERER_END
