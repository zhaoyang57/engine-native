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

#include "ModelBatcher.hpp"

#include <algorithm>
#include "RenderFlow.hpp"

RENDERER_BEGIN

ModelBatcher::ModelBatcher(RenderFlow* flow)
: _flow(flow)
, _iaOffset(0)
, _modelOffset(0)
, _cullingMask(0)
, _walking(false)
, _currEffect(nullptr)
, _buffer(nullptr)
{
    _iaPool.resize(16);
    _modelPool.resize(16);
}

ModelBatcher::~ModelBatcher()
{
    _iaPool.clear();
    _modelPool.clear();
    
    for (auto iter = _buffers.begin(); iter != _buffers.end(); ++iter)
    {
        MeshBuffer *buffer = iter->second;
        buffer->destroy();
        delete buffer;
    }
    _buffers.clear();
}

//void RenderSystem::addRenderHandle(RenderHandle* handle)
//{
//    if (std::find(_handles.begin(), _handles.end(), handle) == _handles.end())
//    {
//        _handles.push_back(handle);
//    }
//}
//
//void RenderSystem::removeRenderHandle(RenderHandle* handle)
//{
//    auto it = std::find(_handles.begin(), _handles.end(), handle);
//    if (it != _handles.end())
//    {
//        _handles.erase(it);
//    }
//}

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
}

void ModelBatcher::commit(NodeProxy* node, RenderHandle* handle)
{
    for (uint32_t i = 0, l = handle->getMeshCount(); i < l; ++i)
    {
        // pre check
        VertexFormat* vfmt = handle->getVertexFormat();
        if (vfmt == nullptr)
        {
            continue;
        }
        
        Effect* effect = handle->getEffect((uint32_t)i);
        int cullingMask = 1 << node->getGroupID();
        const Mat4& worldMat = handle->getUseModel() ? Mat4::IDENTITY : node->getWorldMatrix();
        if (_currEffect == nullptr ||
            _currEffect->getHash() != effect->getHash() ||
            _cullingMask != cullingMask)
        {
            // Break auto batch
            flush();
            
            _modelMat.set(worldMat);
            _currEffect = effect;
            _cullingMask = cullingMask;
        }
        
        MeshBuffer* buffer = getBuffer(vfmt);
        handle->fillBuffers(buffer, i, worldMat);
    }
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
    InputAssembler* ia = _iaPool[_iaOffset];
    if (ia == nullptr)
    {
        ia = new InputAssembler();
        _iaPool[_iaOffset] = ia;
    }
    _iaOffset++;
    ia->setVertexBuffer(_buffer->getVertexBuffer());
    ia->setIndexBuffer(_buffer->getIndexBuffer());
    ia->setStart(indexStart);
    ia->setCount(indexCount);
    
    // Stencil manager process
    
    // Generate model
    Model* model = _modelPool[_modelOffset];
    if (model == nullptr)
    {
        model = new Model();
        _modelPool[_modelOffset] = model;
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
