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

#include "MaskRenderHandle.hpp"
#include "./ModelBatcher.hpp"
#include "./StencilManager.hpp"
#include "./GraphicsRenderHandle.hpp"

RENDERER_BEGIN

MaskRenderHandle::MaskRenderHandle()
: _batcher(nullptr)
{
}

void MaskRenderHandle::handle(NodeProxy *node, ModelBatcher* batcher, Scene* scene)
{
    _batcher = batcher;
    batcher->commit(node, this);
    StencilManager* instance = StencilManager::getInstance();
    if(!_imageStencil) {
        auto* render = getSubHandle("render");
        render->handle(node, batcher, scene);
    }
    instance->enableMask();
};

void MaskRenderHandle::postHandle(NodeProxy *node, ModelBatcher *batcher, Scene *scene)
{
    _batcher = batcher;
    batcher->flush();
    StencilManager::getInstance()->exitMask();
}

void MaskRenderHandle::addSubHandle(const std::string& sysid, cocos2d::renderer::SystemHandle* handle)
{
    _subHandles[sysid] = handle;
}

void MaskRenderHandle::removeSubHandle(const std::string& sysid)
{
    _subHandles.erase(sysid);
}

SystemHandle* MaskRenderHandle::getSubHandle(const std::string& sysid)
{
    auto it = _subHandles.find(sysid);
    if (it != _subHandles.end())
    {
        return it->second;
    }
    return nullptr;
}

void MaskRenderHandle::fillBuffers(MeshBuffer *buffer, int index, const Mat4 &worldMat)
{
    StencilManager* instance = StencilManager::getInstance();
    instance->pushMask(_inverted);
    instance->clear();

    GraphicsRenderHandle* clear = (GraphicsRenderHandle*)getSubHandle("clear");
    _batcher->setCurrentEffect(clear->getEffect(0));
    clear->renderIA(0, _batcher);
    StencilManager::getInstance()->enterLevel();

    if(_imageStencil)
    {
        _batcher->setCurrentEffect(getEffect(index));
        RenderHandle::fillBuffers(buffer, index, worldMat);
        _batcher->flush();
    }
}

RENDERER_END
