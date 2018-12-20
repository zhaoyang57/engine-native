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

#include <stdio.h>
#include "./RenderHandle.hpp"
#include "./SystemHandle.hpp"
#include "./MeshBuffer.hpp"
#include "./GraphicsRenderHandle.hpp"
#include "math/CCMath.h"

class ModelBatcher;

RENDERER_BEGIN

class MaskRenderHandle: public RenderHandle
{
public:
    MaskRenderHandle();
    virtual ~MaskRenderHandle(){};
    virtual void handle(NodeProxy *node, ModelBatcher* batcher, Scene* scene) override;
    virtual void postHandle(NodeProxy *node, ModelBatcher* batcher, Scene* scene) override;
    virtual void fillBuffers(MeshBuffer* buffer, int index, const Mat4& worldMat) override;

    void setMaskInverted(bool inverted) { _inverted = inverted; };
    bool getMaskInverted() { return _inverted; };

    void setRenderSubHandle(GraphicsRenderHandle* renderSubHandle) { _renderSubHandle = renderSubHandle; };
    void setClearSubHandle(GraphicsRenderHandle* clearSubHandle) { _clearSubHandle = clearSubHandle; };

    void setImageStencil(bool isImageStencil) { _imageStencil = isImageStencil; };
protected:
    bool _inverted;
    bool _imageStencil;

private:
    ModelBatcher* _batcher;
    GraphicsRenderHandle* _renderSubHandle;
    GraphicsRenderHandle* _clearSubHandle;
};

RENDERER_END
