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

#include "GraphicsRenderHandle.hpp"
#include "math/CCMath.h"
#include "cocos/scripting/js-bindings/jswrapper/SeApi.h"
#include "cocos/scripting/js-bindings/manual/jsb_conversions.hpp"
#include "cocos/scripting/js-bindings/auto/jsb_renderer_auto.hpp"
#include "renderer/gfx/DeviceGraphics.h"

RENDERER_BEGIN

GraphicsRenderHandle::IARenderData::~IARenderData()
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
    vb->release();
    ib->release();
}

GraphicsRenderHandle::GraphicsRenderHandle()
{
    _bytesPerVertex = VertexFormat::XY_Color->getBytes();
}

GraphicsRenderHandle::~GraphicsRenderHandle()
{
    for (std::size_t i = 0, n = _iaPool.size(); i < n; i++)
    {
        auto ia = _iaPool[i];
        delete ia;
    }
    _iaPool.clear();
}

void GraphicsRenderHandle::updateIA(std::size_t index, int start, int count)
{
    auto size = _iaPool.size();
    InputAssembler* ia = nullptr;
    if (index == size) {
        ia = new InputAssembler();
        _iaPool.push_back(ia);
    }
    else if (index < size)
    {
        ia = _iaPool[index];
    }
    else
    {
        cocos2d::log("GraphicsRenderHandle:updateIA index:%zu is out of range", index);
        return;
    }
    
    auto newIACount = index + 1;
    if (_iaCount < newIACount)
    {
        _iaCount = newIACount;
    }
    
    IARenderData* data = &_datas[index];
    ia->setVertexBuffer(data->vb);
    ia->setIndexBuffer(data->ib);
    ia->setCount(count);
    ia->setStart(start);
}

void GraphicsRenderHandle::renderIA(std::size_t index, ModelBatcher* batcher, NodeProxy* node)
{
    if (index >= _iaCount)
    {
        cocos2d::log("GraphicsRenderHandle:renderIA index:%zu out of range", index);
        return;
    }
    
    batcher->flushIA(_iaPool[index]);
    
    IARenderData* data = &_datas[index];
    uint8_t* verts = data->vertices;
    uint32_t vertexCount = (uint32_t)data->vBytes / _bytesPerVertex;
    uint8_t alpha = node->getRealOpacity();
    for (uint32_t i = 0; i < vertexCount; ++i)
    {
        *(verts + 11) = alpha;
        verts += _bytesPerVertex;
    }
    data->vb->update(0, (float*)data->vertices, data->vBytes);
    data->ib->update(0, (float*)data->indices, data->iBytes);
}

void GraphicsRenderHandle::setMeshCount(uint32_t count)
{
    _datas.resize(count);
}

void GraphicsRenderHandle::updateNativeMesh(uint32_t index, se::Object* vertices, se::Object* indices)
{
    if (index >= _datas.size())
    {
        return;
    }
    se::ScriptEngine::getInstance()->clearException();
    se::AutoHandleScope hs;
    
    IARenderData* data = &_datas[index];
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
    
    if(!data->vb)
    {
        data->vb = new VertexBuffer();
        data->vb->init(DeviceGraphics::getInstance(), VertexFormat::XY_Color, Usage::DYNAMIC, nullptr, 0, (uint32_t)data->vBytes);
    }
    else
    {
        data->vb->setBytes((uint32_t)data->vBytes);
    }
    
    if(!data->ib)
    {
        data->ib = new IndexBuffer();
        data->ib->init(DeviceGraphics::getInstance(), IndexFormat::UINT16, Usage::STATIC, nullptr, 0, (uint32_t)data->iBytes);
    }
    else
    {
        data->ib->setBytes((uint32_t)data->iBytes);
    }
}

void GraphicsRenderHandle::updateNativeEffect(size_t index, Effect* effect)
{
    if (index >= _datas.size())
    {
        return;
    }
    _datas[index].effect = effect;
    CustomRenderHandle::updateNativeEffect(index, effect);
}

void GraphicsRenderHandle::reset()
{
    _iaCount = 0;
    //_datas.resize(0);
}

RENDERER_END
