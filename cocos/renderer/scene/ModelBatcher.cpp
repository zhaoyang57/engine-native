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

#include "ModelBatcher.hpp"
#include "RenderFlow.hpp"
#include "StencilManager.hpp"

RENDERER_BEGIN

#define INIT_IA_LENGTH 16
#define INIT_MODEL_LENGTH 16

ModelBatcher::ModelBatcher(RenderFlow* flow)
: _flow(flow)
, _iaOffset(0)
, _modelOffset(0)
, _cullingMask(0)
, _walking(false)
, _currEffect(nullptr)
, _buffer(nullptr)
{
    for (int i = 0; i < INIT_IA_LENGTH; i++)
    {
        _iaPool.push_back(new InputAssembler());
    }
    
    for (int i = 0; i < INIT_MODEL_LENGTH; i++)
    {
        _modelPool.push_back(new Model());
    }

    _stencilMgr = StencilManager::getInstance();
}

ModelBatcher::~ModelBatcher()
{
    for (int i = 0; i < _iaPool.size(); i++)
    {
        auto ia = _iaPool[i];
        delete ia;
    }
    _iaPool.clear();
    
    for (int i = 0; i < _modelPool.size(); i++)
    {
        auto model = _modelPool[i];
        delete model;
    }
    _modelPool.clear();
    
    for (auto iter = _buffers.begin(); iter != _buffers.end(); ++iter)
    {
        MeshBuffer *buffer = iter->second;
        buffer->destroy();
        delete buffer;
    }
    _buffers.clear();
}

void ModelBatcher::reset()
{
    _iaOffset = 0;
    _modelOffset = 0;
    
    for (int i = 0; i < _batchedModel.size(); ++i)
    {
        Model* model = _batchedModel[i];
        model->clearInputAssemblers();
        model->clearEffects();
        _flow->getRenderScene()->removeModel(model);
    }
    _batchedModel.clear();
    
    for (auto iter : _buffers)
    {
        iter.second->reset();
    }
    _buffer = nullptr;
    
    _cullingMask = 0;
    _currEffect = nullptr;
    _walking = false;
    
    _modelMat.set(Mat4::IDENTITY);
    
    _stencilMgr->reset();
}

void ModelBatcher::commit(NodeProxy* node, RenderHandle* handle)
{
    // pre check
    VertexFormat* vfmt = handle->getVertexFormat();
    if (vfmt == nullptr)
    {
        return;
    }
    bool useModel = handle->getUseModel();
    for (uint32_t i = 0, l = handle->getMeshCount(); i < l; ++i)
    {
        Effect* effect = handle->getEffect((uint32_t)i);
        int cullingMask = node->getCullingMask();
        const Mat4& worldMat = useModel ? Mat4::IDENTITY : node->getWorldMatrix();
        if (_currEffect == nullptr ||
            _currEffect->getHash() != effect->getHash() ||
            _cullingMask != cullingMask)
        {
            // Break auto batch
            flush();
            
            if (useModel)
            {
                _modelMat.set(worldMat);
            }
            _currEffect = effect;
            _cullingMask = cullingMask;
        }
        
        MeshBuffer* buffer = _buffer;
        if (!_buffer || vfmt != _buffer->_vertexFmt)
        {
            buffer = getBuffer(vfmt);
        }
        if (handle->isOpacityDirty())
        {
            handle->updateOpacity(i, node->getRealOpacity());
        }
        handle->fillBuffers(buffer, i, worldMat);
    }
}

void ModelBatcher::commitIA(NodeProxy* node, CustomRenderHandle* handle)
{
    flush();
    
    for (std::size_t i = 0, n = handle->getIACount(); i < n; i++ )
    {
        const Mat4& worldMat = handle->getUseModel() ? node->getWorldMatrix() : Mat4::IDENTITY;
        _currEffect = handle->getEffect((uint32_t)i);
        _cullingMask = node->getCullingMask();
        _modelMat.set(worldMat);
        handle->renderIA(i, this, node);
    }
}

void ModelBatcher::flushIA(InputAssembler* customIA)
{
    // Generate IA
    InputAssembler* ia = nullptr;
    if (_iaOffset >= _iaPool.size())
    {
        ia = new InputAssembler();
        _iaPool.push_back(ia);
    }
    else
    {
        ia = _iaPool[_iaOffset];
    }
    _iaOffset++;
    ia->setVertexBuffer(customIA->getVertexBuffer());
    ia->setIndexBuffer(customIA->getIndexBuffer());
    ia->setStart(customIA->getStart());
    ia->setCount(customIA->getCount());

    // Stencil manager process
    _stencilMgr->handleEffect(_currEffect);
    
    // Generate model
    Model* model = nullptr;
    _modelPool[_modelOffset];
    if (_modelOffset >= _modelPool.size())
    {
        model = new Model();
        _modelPool.push_back(model);
    }
    else
    {
        model = _modelPool[_modelOffset];
    }
    _modelOffset++;
    model->setWorldMatix(_modelMat);
    model->setCullingMask(_cullingMask);
    model->addEffect(_currEffect);
    model->addInputAssembler(*ia);
    _batchedModel.push_back(model);
    
    _flow->getRenderScene()->addModel(model);
}

void ModelBatcher::startBatch()
{
    reset();
    _walking = true;
}

void ModelBatcher::flush()
{
    if (_buffer == nullptr)
    {
        return;
    }
    int indexStart = _buffer->getIndexStart();
    int indexOffset = _buffer->getIndexOffset();
    int indexCount = indexOffset - indexStart;
    if (!_walking || _currEffect == nullptr || indexCount <= 0)
    {
        return;
    }
    
    // Generate IA
    InputAssembler* ia = nullptr;
    if (_iaOffset >= _iaPool.size())
    {
        ia = new InputAssembler();
        _iaPool.push_back(ia);
    }
    else
    {
        ia = _iaPool[_iaOffset];
    }
    _iaOffset++;
    ia->setVertexBuffer(_buffer->getVertexBuffer());
    ia->setIndexBuffer(_buffer->getIndexBuffer());
    ia->setStart(indexStart);
    ia->setCount(indexCount);
    
    // Stencil manager process
    _stencilMgr->handleEffect(_currEffect);
    
    // Generate model
    Model* model = nullptr;
    _modelPool[_modelOffset];
    if (_modelOffset >= _modelPool.size())
    {
        model = new Model();
        _modelPool.push_back(model);
    }
    else
    {
        model = _modelPool[_modelOffset];
    }
    _modelOffset++;
    model->setWorldMatix(_modelMat);
    model->setCullingMask(_cullingMask);
    model->addEffect(_currEffect);
    model->addInputAssembler(*ia);
    _batchedModel.push_back(model);
    
    _flow->getRenderScene()->addModel(model);
    
    _buffer->updateOffset();
}

void ModelBatcher::terminateBatch()
{
    flush();
    
    for (auto iter : _buffers)
    {
        iter.second->uploadData();
    }
    
    _walking = false;
}

MeshBuffer* ModelBatcher::getBuffer(VertexFormat* fmt)
{
    MeshBuffer* buffer = nullptr;
    auto iter = _buffers.find(fmt);
    if (iter == _buffers.end())
    {
        buffer = new MeshBuffer(this, fmt);
        _buffers.emplace(fmt, buffer);
    }
    else
    {
        buffer = iter->second;
    }
    return buffer;
}

RENDERER_END
