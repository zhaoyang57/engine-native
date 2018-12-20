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

#include <map>

#include "../Macro.h"
#include "RenderHandle.hpp"
#include "CustomRenderHandle.hpp"
#include "MeshBuffer.hpp"
#include "math/CCMath.h"
#include "../renderer/Renderer.h"

RENDERER_BEGIN

class RenderFlow;
class StencilManager;

class ModelBatcher
{
public:
    ModelBatcher(RenderFlow* flow);
    ~ModelBatcher();
    
    void reset();
    
//    void addRenderHandle(RenderHandle* handle);
//    void removeRenderHandle(RenderHandle* handle);
    void commit(NodeProxy* node, RenderHandle* handle);
    void commitIA(NodeProxy* node, CustomRenderHandle* handle);
    
    void startBatch();
    void flush();
    void flushIA(InputAssembler* customIA);
    void terminateBatch();
    
    MeshBuffer* getBuffer(VertexFormat* fmt);
    const MeshBuffer* getCurrentBuffer() const { return _buffer; };
    void setCurrentBuffer(MeshBuffer* buffer) { _buffer = buffer; };
    
    RenderFlow* getFlow() const { return _flow; };
    
    void setCurrentEffect(Effect* effect) { _currEffect = effect; };
private:
    int _iaOffset;
    int _modelOffset;
    int _cullingMask;
    bool _walking;
    cocos2d::Mat4 _modelMat;
    
    MeshBuffer* _buffer;
    Effect* _currEffect;
    RenderFlow* _flow;
    StencilManager* _stencilMgr;
    
    std::vector<InputAssembler*> _iaPool;
    std::vector<Model*> _modelPool;
    std::vector<Model*> _batchedModel;
    std::unordered_map<VertexFormat*, MeshBuffer*> _buffers;
//    std::vector<RenderHandle*> _handles;
};

RENDERER_END
