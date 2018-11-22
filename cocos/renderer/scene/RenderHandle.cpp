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

RenderHandle::RenderData::~RenderData()
{
    jsVertices->unroot();
    jsVertices->decRef();
    jsIndices->unroot();
    jsIndices->decRef();
}

RenderHandle::RenderHandle()
: _enabled(false)
, _vertsDirty(true)
, _useModel(false)
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

void RenderHandle::updateNativeMesh(uint32_t index, const se::HandleObject& vertices, const se::HandleObject& indices)
{
    if (index >= _datas.size())
    {
        return;
    }
    RenderData* data = &_datas[index];
    if (data->jsVertices)
    {
        data->jsVertices->unroot();
        data->jsVertices->decRef();
    }
    if (data->jsIndices)
    {
        data->jsIndices->unroot();
        data->jsIndices->decRef();
    }
    
    data->jsVertices = vertices.get();
    data->jsVertices->root();
    data->jsVertices->incRef();
    data->jsIndices = indices.get();
    data->jsIndices->root();
    data->jsIndices->incRef();
    data->vertices = nullptr;
    data->indices = nullptr;
    data->jsVertices->getTypedArrayData(&data->vertices, &data->vBytes);
    data->jsIndices->getTypedArrayData(&data->indices, &data->iBytes);
}

void RenderHandle::updateNativeEffect(uint32_t index, Effect* effect)
{
    if (index >= _datas.size())
    {
        return;
    }
    _datas[index].effect = effect;
}

void RenderHandle::setMeshCount(uint32_t count)
{
    _datas.resize(count);
}

Effect* RenderHandle::getEffect(uint32_t index)
{
    if (index < _datas.size())
    {
        return _datas[index].effect;
    }
    else
    {
        return nullptr;
    }
}

void RenderHandle::handle(NodeProxy *node, RenderFlow* flow)
{
    flow->getModelBatcher()->commit(node, this);
}

void RenderHandle::postHandle(NodeProxy *node, RenderFlow* flow)
{
}

void RenderHandle::fillBuffers(MeshBuffer* buffer, int index, const Mat4& worldMat)
{
    if (index >= _datas.size())
    {
        return;
    }
    const RenderData& data = _datas[index];
    
    uint32_t bytesPerVertex = _vfmt->getBytes();
    CCASSERT(data.vBytes % bytesPerVertex == 0, "RenderHandle::fillBuffers vertices data doesn't follow vertex format");
    CCASSERT(data.iBytes % 2 == 0, "RenderHandle::fillBuffers indices data is not saved in 16bit");
    uint32_t vertexCount = (uint32_t)data.vBytes / bytesPerVertex;
    uint32_t indexCount = (uint32_t)data.iBytes / 2;

    buffer->request(vertexCount, indexCount);
    
    // Calculate vertices world positions
    if (!_useModel)
    {
        // Assume position is stored in floats
        float* vertices = (float*)data.vertices;
        const VertexFormat::Element& posDesc = _vfmt->getElement(ATTRIB_NAME_POSITION);
        uint32_t num = posDesc.num;
        size_t posOffset = posDesc.offset / 4;
        size_t offset;
        cocos2d::Vec3 pos(0, 0, 0);
        for (uint32_t i = 0; i < vertexCount; ++i)
        {
            offset = i * bytesPerVertex / 4;
            pos.x = vertices[offset + posOffset];
            pos.y = vertices[offset + posOffset + 1];
            if (num == 3)
                pos.z = vertices[offset + posOffset + 2];
            worldMat.transformPoint(&pos);
            vertices[offset + posOffset] = pos.x;
            vertices[offset + posOffset + 1] = pos.y;
            if (num == 3)
                vertices[offset + posOffset + 2] = pos.z;
        }
    }
    // Copy vertex buffer memory
    uint32_t vDataId = buffer->getByteOffset() / MeshBuffer::VDATA_BYTE;
    memcpy(&buffer->vData[vDataId], data.vertices, data.vBytes);
    
    // Copy index buffer with vertex offset
    uint32_t iDataId = buffer->getIndexOffset();
    uint32_t vertexId = buffer->getVertexOffset();
    uint16_t* indices = (uint16_t*)data.indices;
    for (int i = 0; i < indexCount; ++i)
    {
        buffer->iData[iDataId++] = vertexId + indices[i];
    }
}

RENDERER_END
