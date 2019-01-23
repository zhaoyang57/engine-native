/****************************************************************************
 LICENSING AGREEMENT
 
 Xiamen Yaji Software Co., Ltd., (the “Licensor”) grants the user (the “Licensee”) non-exclusive and non-transferable rights to use the software according to the following conditions:
 a.  The Licensee shall pay royalties to the Licensor, and the amount of those royalties and the payment method are subject to separate negotiations between the parties.
 b.  The software is licensed for use rather than sold, and the Licensor reserves all rights over the software that are not expressly granted (whether by implication, reservation or prohibition).
 c.  The open source codes contained in the software are subject to the MIT Open Source Licensing Agreement (see the attached for the details);
 d.  The Licensee acknowledges and consents to the possibility that errors may occur during the operation of the software for one or more technical reasons, and the Licensee shall take precautions and prepare remedies for such events. In such circumstance, the Licensor shall provide software patches or updates according to the agreement between the two parties. The Licensor will not assume any liability beyond the explicit wording of this Licensing Agreement.
 e.  Where the Licensor must assume liability for the software according to relevant laws, the Licensor’s entire liability is limited to the annual royalty payable by the Licensee.
 f.  The Licensor owns the portions listed in the root directory and subdirectory (if any) in the software and enjoys the intellectual property rights over those portions. As for the portions owned by the Licensor, the Licensee shall not:
 - i. Bypass or avoid any relevant technical protection measures in the products or services;
 - ii. Release the source codes to any other parties;
 - iii. Disassemble, decompile, decipher, attack, emulate, exploit or reverse-engineer these portion of code;
 - iv. Apply it to any third-party products or services without Licensor’s permission;
 - v. Publish, copy, rent, lease, sell, export, import, distribute or lend any products containing these portions of code;
 - vi. Allow others to use any services relevant to the technology of these codes;
 - vii. Conduct any other act beyond the scope of this Licensing Agreement.
 g.  This Licensing Agreement terminates immediately if the Licensee breaches this Agreement. The Licensor may claim compensation from the Licensee where the Licensee’s breach causes any damage to the Licensor.
 h.  The laws of the People's Republic of China apply to this Licensing Agreement.
 i.  This Agreement is made in both Chinese and English, and the Chinese version shall prevail the event of conflict.
 ****************************************************************************/

#include "RenderHandle.hpp"

#include "NodeProxy.hpp"
#include "ModelBatcher.hpp"
#include "MeshBuffer.hpp"
#include "../renderer/Scene.h"
#include "math/CCMath.h"
#include "cocos/scripting/js-bindings/jswrapper/SeApi.h"
#include "cocos/scripting/js-bindings/manual/jsb_conversions.hpp"
#include "cocos/scripting/js-bindings/auto/jsb_renderer_auto.hpp"

RENDERER_BEGIN

static MeshBuffer::OffsetInfo s_offsets;

RenderHandle::RenderData::~RenderData()
{
    if(jsVertices != nullptr)
    {
        jsVertices->unroot();
        jsVertices->decRef();
    }
    if(jsIndices != nullptr)
    {
        jsIndices->unroot();
        jsIndices->decRef();
    }
}

RenderHandle::RenderHandle()
: _enabled(false)
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
    data->jsVertices->getTypedArrayData(&data->vertices, (std::size_t*)&data->vBytes);
    data->jsIndices->getTypedArrayData(&data->indices, (std::size_t*)&data->iBytes);
    
    // Initialize opacity in vertex buffer
    this->updateOpacity(index, 255);
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

void RenderHandle::setVertexFormat(VertexFormat* vfmt)
{
    _vfmt = vfmt;
    if (_vfmt)
    {
        _bytesPerVertex = _vfmt->getBytes();
        _vfPos = _vfmt->getElement(ATTRIB_NAME_POSITION);
        _posOffset = _vfPos->offset / 4;
        _vfColor = _vfmt->getElement(ATTRIB_NAME_COLOR);
        _alphaOffset = _vfColor->offset + 3;
    }
}

void RenderHandle::handle(NodeProxy *node, ModelBatcher* batcher, Scene* scene)
{
    batcher->commit(node, this);
}

void RenderHandle::postHandle(NodeProxy *node, ModelBatcher* batcher, Scene* scene)
{
}

void RenderHandle::fillBuffers(MeshBuffer* buffer, int index, const Mat4& worldMat)
{
    if (index >= _datas.size() || _vfmt == nullptr)
    {
        return;
    }
    RenderData& data = _datas[index];
    
    CCASSERT(data.vBytes % _bytesPerVertex == 0, "RenderHandle::fillBuffers vertices data doesn't follow vertex format");
    CCASSERT(data.iBytes % 2 == 0, "RenderHandle::fillBuffers indices data is not saved in 16bit");
    uint32_t vertexCount = (uint32_t)data.vBytes / _bytesPerVertex;
    uint32_t indexCount = (uint32_t)data.iBytes / 2;

    // must retrieve offset before request
    buffer->request(vertexCount, indexCount, &s_offsets);
    uint32_t vBufferOffset = s_offsets.vByte / sizeof(float);
    uint32_t indexId = s_offsets.index;
    uint32_t vertexId = s_offsets.vertex;
    uint32_t num = _vfPos->num;

    float* worldVerts = &buffer->vData[vBufferOffset];
    memcpy(worldVerts, (float*)data.vertices, data.vBytes);
    
    // Calculate vertices world positions
    if (!_useModel)
    {
        size_t dataPerVertex = _bytesPerVertex / sizeof(float);
        float* ptrPos = worldVerts + _posOffset;
        
        switch (num) {
           // Vertex is X Y Z Format
            case 3:
                for (uint32_t i = 0; i < vertexCount; ++i)
                {
                    worldMat.transformPoint((cocos2d::Vec3*)ptrPos);
                    ptrPos += dataPerVertex;
                }
                break;
            // Vertex is X Y Format
            case 2:
                for (uint32_t i = 0; i < vertexCount; ++i)
                {
                    float z = ptrPos[2];
                    ptrPos[2] = 0;
                    worldMat.transformPoint((cocos2d::Vec3*)ptrPos);
                    ptrPos[2] = z;
                    ptrPos += dataPerVertex;
                }
                break;
        }
    }
    
    // Copy index buffer with vertex offset
    uint16_t* indices = (uint16_t*)data.indices;
    for (int i = 0; i < indexCount; ++i)
    {
        buffer->iData[indexId++] = vertexId + indices[i];
    }
}

void RenderHandle::updateOpacity(int index, uint8_t opacity)
{
    // has no color info in vertex buffer
    if(!_vfColor)
    {
        return;
    }
    
    if (index >= _datas.size() || _vfmt == nullptr)
    {
        return;
    }
    RenderData& data = _datas[index];
    
    CCASSERT(data.vBytes % _bytesPerVertex == 0, "RenderHandle::fillBuffers vertices data doesn't follow vertex format");
    uint32_t vertexCount = (uint32_t)data.vBytes / _bytesPerVertex;
    
    size_t dataPerVertex = _bytesPerVertex / sizeof(uint8_t);
    uint8_t* ptrAlpha = (uint8_t*)data.vertices + _alphaOffset;
    for (uint32_t i = 0; i < vertexCount; ++i)
    {
        *ptrAlpha = opacity;
        ptrAlpha += dataPerVertex;
    }
}

RENDERER_END
