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
#include "MeshBuffer.hpp"
#include "math/CCMath.h"
#include "../renderer/Effect.h"

namespace se {
    class Object;
    class HandleObject;
}

RENDERER_BEGIN

class NodeProxy;
class RenderFlow;

class RenderHandle : SystemHandle
{
public:
    RenderHandle();
    virtual ~RenderHandle();
    virtual void handle(NodeProxy *node, RenderFlow* flow) override;
    virtual void postHandle(NodeProxy *node, RenderFlow* flow) override;
    
    virtual void fillBuffers(MeshBuffer* buffer, int index, const Mat4& worldMat);
//
//    void getVertices(uint32_t index, uint8_t** buffer, size_t* length);
//    void getIndices(uint32_t index, uint8_t** buffer, size_t* length);
    Effect* getEffect(uint32_t index);
    
    void enable();
    void disable();
    bool enabled() const { return _enabled; };
    
    uint32_t getMeshCount() const { return (uint32_t)_datas.size(); };
    void setMeshCount(uint32_t count);
    void updateNativeMesh(uint32_t index, se::Object* vertices, se::Object* indices);
    void updateNativeEffect(uint32_t index, Effect* effect);
    
    bool getUseModel() const { return _useModel; };
    void setUseModel(bool useModel) { _useModel = useModel; };
    
    VertexFormat* getVertexFormat() const { return _vfmt; };
    void setVertexFormat(VertexFormat* vfmt) { _vfmt = vfmt; };
    
    void setVertsDirty() { _vertsDirty = true; };
    
protected:
    struct RenderData {
        RenderData ()
        : vBytes(0)
        , iBytes(0)
        , effect(nullptr)
        , jsVertices(nullptr)
        , jsIndices(nullptr)
        {
            
        }
        ~RenderData ();
        unsigned long vBytes;
        unsigned long iBytes;
        Effect* effect;
        uint8_t* vertices;
        uint8_t* indices;
        se::Object* jsVertices;
        se::Object* jsIndices;
    };
//    void postUpdateRenderData();
//    void postRender();
    
protected:
    bool _enabled;
    bool _vertsDirty;
    bool _useModel;

    VertexFormat* _vfmt;
    std::vector<RenderData> _datas;
};

RENDERER_END
