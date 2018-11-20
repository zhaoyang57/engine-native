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

#include "NodeProxy.hpp"
#include "RenderFlow.hpp"
#include "math/CCMath.h"
#include "cocos/scripting/js-bindings/jswrapper/SeApi.h"
#include "cocos/scripting/js-bindings/manual/jsb_conversions.hpp"
#include "cocos/scripting/js-bindings/auto/jsb_renderer_auto.hpp"

RENDERER_BEGIN

RenderHandle::RenderHandle()
: _enabled(false)
, _vertsDirty(true)
, _colorDirty(true)
, _useModel(false)
, _meshCount(0)
, _vfmt(nullptr)
{
}

RenderHandle::~RenderHandle()
{
    disable();
}

void RenderHandle::enable()
{
    if (!_enabled)
    {
        _enabled = true;
    }
}

void RenderHandle::disable()
{
    if (_enabled)
    {
        _enabled = false;
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
    CCASSERT(ok, "RenderHandle_updateRenderData : RenderHandle.prototype._updateRenderData is not implemented");
    
    _meshes.clear();
    _effects.clear();
    
    // Update vertices
    func.toObject()->call(se::EmptyValueArray, jsVal.toObject());
}

void RenderHandle::updateColor()
{
    se::ScriptEngine::getInstance()->clearException();
    se::AutoHandleScope hs;
    
    se::Value jsVal;
    bool ok = native_ptr_to_seval<RenderHandle>((RenderHandle*)this, &jsVal);
    CCASSERT(ok, "RenderHandle_updateColor : JS object missing");
    
    se::Value func;
    ok = __jsb_cocos2d_renderer_RenderHandle_proto->getProperty("_updateColor", &func);
    CCASSERT(ok, "RenderHandle_updateColor : RenderHandle.prototype._updateColor is not implemented");
    
    // Update color
    func.toObject()->call(se::EmptyValueArray, jsVal.toObject());
}

void RenderHandle::addMesh(Mesh* mesh, Effect* effect)
{
    _meshes.push_back(mesh);
    _effects.push_back(effect);
    _meshCount = (uint32_t)_meshes.size();
}

Mesh* RenderHandle::getMesh(uint32_t index)
{
    if (index < _meshCount)
    {
        return _meshes[index];
    }
    else
    {
        return nullptr;
    }
}

Effect* RenderHandle::getEffect(uint32_t index)
{
    if (index < _meshCount)
    {
        return _effects[index];
    }
    else
    {
        return nullptr;
    }
}

void RenderHandle::handle(NodeProxy *node, RenderFlow* flow)
{
    if (_vertsDirty)
    {
        updateRenderData();
    }
    
    flow->getModelBatcher()->commit(node, this);
}

void RenderHandle::postHandle(NodeProxy *node, RenderFlow* flow)
{
}

void RenderHandle::fillBuffers(MeshBuffer* buffer, int index, const Mat4& worldMat)
{
    Mesh* mesh = getMesh(index);
    if (mesh == nullptr)
    {
        return;
    }
    
    uint32_t vDataId = buffer->getByteOffset() / MeshBuffer::VDATA_BYTE;
    uint32_t iDataId = buffer->getIndexOffset();
    uint32_t vertexId = buffer->getVertexOffset();
    
    const std::vector<Mesh::Vertex>& vertices = mesh->getVertices();
    const std::vector<uint16_t>& indices = mesh->getIndices();
    uint32_t vertexCount = (uint32_t)vertices.size();
    uint32_t indexCount = (uint32_t)indices.size();
    
    buffer->request(vertexCount, indexCount);
    
    if (_useModel)
    {
        for (int i = 0; i < vertexCount; ++i)
        {
            Mesh::Vertex vertex = vertices[i];
            buffer->vData[vDataId++] = vertex.x;
            buffer->vData[vDataId++] = vertex.y;
            buffer->vData[vDataId++] = vertex.u;
            buffer->vData[vDataId++] = vertex.v;
            buffer->vData[vDataId++] = *reinterpret_cast<float*>(&vertex.color);
        }
    }
    // Not using model matrix, need to calcuclate world position in CPU
    else
    {
        for (int i = 0; i < vertexCount; ++i)
        {
            Mesh::Vertex vertex = vertices[i];
            cocos2d::Vec3 pos(vertex.x, vertex.y, 0);
            worldMat.transformPoint(&pos);
            buffer->vData[vDataId++] = pos.x;
            buffer->vData[vDataId++] = pos.y;
            buffer->vData[vDataId++] = vertex.u;
            buffer->vData[vDataId++] = vertex.v;
            buffer->vData[vDataId++] = *reinterpret_cast<float*>(&vertex.color);
        }
    }
    for (int i = 0; i < indexCount; ++i)
    {
        buffer->iData[iDataId++] = vertexId + indices[i];
    }
}

RENDERER_END
