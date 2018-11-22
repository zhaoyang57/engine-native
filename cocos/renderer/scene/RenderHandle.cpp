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

void RenderHandle::updateNativeMesh(uint32_t index, se::Object* vertices, se::Object* indices)
{
    if (index >= _datas.size())
    {
        return;
    }
    se::ScriptEngine::getInstance()->clearException();
    se::AutoHandleScope hs;
    
    RenderData* data = &_datas[index];
    if (data->jsVertices != nullptr)
    {
        data->jsVertices->unroot();
        data->jsVertices->decRef();
    }
    if (data->jsIndices != nullptr)
    {
        data->jsIndices->unroot();
        data->jsIndices->decRef();
    }

    vertices->root();
    vertices->incRef();
    data->jsVertices = vertices;
    indices->root();
    indices->incRef();
    data->jsIndices = indices;
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

template<typename T>
void fillVertexElement(uint8_t* vertices, size_t elOffset, uint32_t num, uint32_t bytesPerVertex, float* buffer, uint32_t byteOffset, uint32_t vertexCount)
{
    size_t offset;
    size_t bytes = sizeof(T);
    size_t dataOffset = byteOffset / bytes;
    size_t dataPerVertex = bytesPerVertex / bytes;
    elOffset = elOffset / bytes;
    
    T* src = (T*)vertices;
    T* dest = (T*)buffer;
    
    for (uint32_t i = 0; i < vertexCount; ++i)
    {
        for (uint32_t j = 0; j < num; ++j)
        {
            offset = i * dataPerVertex + elOffset + j;
            dest[dataOffset + offset] = src[offset];
        }
    }
}

//        const std::vector<std::string>& names = _vfmt->getAttributeNames();
//        for (uint32_t i = 0; i < names.size(); ++i) {
//            if (names[i] == ATTRIB_NAME_POSITION) continue;
//            const VertexFormat::Element& desc = _vfmt->getElement(names[i]);
//            AttribType type = desc.type;
//            num = desc.num;
//            switch (type) {
//                case AttribType::INT8:
//                    fillVertexElement<int8_t>(data.vertices, desc.offset, num, bytesPerVertex, buffer->vData.data(), vBufferOffset, vertexCount);
//                    break;
//                case AttribType::INT16:
//                    fillVertexElement<int16_t>(data.vertices, desc.offset, num, bytesPerVertex, buffer->vData.data(), vBufferOffset, vertexCount);
//                    break;
//                case AttribType::INT32:
//                    fillVertexElement<int32_t>(data.vertices, desc.offset, num, bytesPerVertex, buffer->vData.data(), vBufferOffset, vertexCount);
//                    break;
//                case AttribType::UINT8:
//                    fillVertexElement<uint8_t>(data.vertices, desc.offset, num, bytesPerVertex, buffer->vData.data(), vBufferOffset, vertexCount);
//                    break;
//                case AttribType::UINT16:
//                    fillVertexElement<uint16_t>(data.vertices, desc.offset, num, bytesPerVertex, buffer->vData.data(), vBufferOffset, vertexCount);
//                    break;
//                case AttribType::UINT32:
//                    fillVertexElement<uint32_t>(data.vertices, desc.offset, num, bytesPerVertex, buffer->vData.data(), vBufferOffset, vertexCount);
//                    break;
//                case AttribType::FLOAT32:
//                    fillVertexElement<float>(data.vertices, desc.offset, num, bytesPerVertex, buffer->vData.data(), vBufferOffset / sizeof(float), vertexCount);
//                    break;
//                default:
//                    continue;
//            }
//        }


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

    // must retrieve offset before request
    uint32_t vBufferOffset = buffer->getByteOffset();
    uint32_t iDataId = buffer->getIndexOffset();
    uint32_t vertexId = buffer->getVertexOffset();
    buffer->request(vertexCount, indexCount);
    
    // Calculate vertices world positions
    if (!_useModel)
    {
        // Assume position is stored in floats
        float* vertices = (float*)data.vertices;
        size_t dataOffset = vBufferOffset / sizeof(float);
        const VertexFormat::Element& posDesc = _vfmt->getElement(ATTRIB_NAME_POSITION);
        uint32_t num = posDesc.num;
        size_t dataPerVertex = bytesPerVertex / 4;
        size_t elOffset = posDesc.offset / 4;
        size_t offset;
        cocos2d::Vec3 pos(0, 0, 0);
        for (uint32_t i = 0; i < vertexCount; ++i)
        {
            offset = i * dataPerVertex + elOffset;
            pos.x = vertices[offset];
            pos.y = vertices[offset + 1];
            if (num == 3)
                pos.z = vertices[offset + 2];
            worldMat.transformPoint(&pos);
            offset += dataOffset;
            buffer->vData[offset] = pos.x;
            buffer->vData[offset + 1] = pos.y;
            if (num == 3)
                buffer->vData[elOffset + 2] = pos.z;
        }
        const VertexFormat::Element& uvDesc = _vfmt->getElement(ATTRIB_NAME_UV0);
        elOffset = uvDesc.offset / 4;
        for (uint32_t i = 0; i < vertexCount; ++i)
        {
            offset = i * dataPerVertex + elOffset;
            buffer->vData[dataOffset + offset] = vertices[offset];
            buffer->vData[dataOffset + offset + 1] = vertices[offset + 1];
        }
        uint32_t* colors = (uint32_t*)data.vertices;
        const VertexFormat::Element& colorDesc = _vfmt->getElement(ATTRIB_NAME_COLOR);
        elOffset = colorDesc.offset / 4;
        for (uint32_t i = 0; i < vertexCount; ++i)
        {
            offset = i * dataPerVertex + elOffset;
            buffer->vData[dataOffset + offset] = *reinterpret_cast<float*>(&colors[offset]);
        }
    }
    else
    {
        // Copy vertex buffer memory
        memcpy(&buffer->vData[vBufferOffset / 4], (float*)data.vertices, data.vBytes);
    }
    
    // Copy index buffer with vertex offset
    uint16_t* indices = (uint16_t*)data.indices;
    for (int i = 0; i < indexCount; ++i)
    {
        buffer->iData[iDataId++] = vertexId + indices[i];
    }
}

RENDERER_END
