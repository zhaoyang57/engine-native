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
#include "MiddlewareManager.h"
#include "base/CCGLUtils.h"
#include "MiddlewareMacro.h"
#include "scripting/js-bindings/jswrapper/SeApi.h"
#include "renderer/gfx/DeviceGraphics.h"

using namespace cocos2d;
using namespace cocos2d::renderer;

MIDDLEWARE_BEGIN

MiddlewareManager* MiddlewareManager::_instance = nullptr;

MiddlewareManager::MiddlewareManager()
:vb(INIT_VB_BUFFER_SIZE_XYUVC)
,ib(INIT_IB_BUFFER_SIZE)
{
    vb.setMaxSize(MAX_VB_BUFFER_SIZE_XYUVC);
    vb.setFullCallback([this]
    {
        uploadVB();
        nextGLVB();
    });
    
    _glIB = new IndexBuffer();
    _glIB->init(DeviceGraphics::getInstance(), IndexFormat::UINT16, Usage::STATIC, nullptr, 0, (uint32_t)ib.getCapacity()/sizeof(unsigned short));
    
    _glVB = new VertexBuffer();
    _glVB->init(DeviceGraphics::getInstance(), VertexFormat::XY_UV_Color, Usage::DYNAMIC, nullptr, 0, (uint32_t)vb.getCapacity()/VertexFormat::XY_UV_Color->getBytes());
    _vbArr.push_back(_glVB);
}

MiddlewareManager::~MiddlewareManager()
{
    for (auto i = 0; i < _vbArr.size(); i++)
    {
        auto glVB = _vbArr[i];
        if (glVB) {
            delete glVB;
        }
    }
    _vbArr.clear();
    
    if (_glIB)
    {
        delete _glIB;
        _glIB = nullptr;
    }
}

void MiddlewareManager::nextGLVB()
{
    _vbPos++;
    if (_vbPos >= _vbArr.size())
    {
        _glVB = new VertexBuffer();
        _glVB->init(DeviceGraphics::getInstance(), VertexFormat::XY_UV_Color, Usage::DYNAMIC, nullptr, 0, (uint32_t)vb.getCapacity()/VertexFormat::XY_UV_Color->getBytes());
        _vbArr.push_back(_glVB);
    }
    else
    {
        _glVB = _vbArr[_vbPos];
    }
}

void MiddlewareManager::update(float dt)
{
    vb.reset();
    ib.reset();
    _vbPos = 0;
    _glVB = _vbArr[0];
    
    isUpdating = true;
    
    for (auto it = _updateMap.begin(); it != _updateMap.end(); it++)
    {
        auto editor = it->first;
        if (_removeList.size() > 0)
        {
            auto removeIt = std::find(_removeList.begin(), _removeList.end(), editor);
            if (removeIt == _removeList.end())
            {
                editor->update(dt);
            }
        }
        else
        {
            editor->update(dt);
        }
    }
    
    isUpdating = false;
    
    for (std::size_t i = 0; i < _removeList.size(); i++)
    {
        auto editor = _removeList[i];
        auto it = _updateMap.find(editor);
        if (it != _updateMap.end())
        {
            _updateMap.erase(it);
        }
    }
    
    _removeList.clear();
    
    uploadIB();
    uploadVB();
}

void MiddlewareManager::uploadVB()
{
    auto length = vb.length();
    if (length > 0)
    {
        _glVB->setBytes((uint32_t)length);
        _glVB->update(0, vb.getBuffer(), length);
    }
}

void MiddlewareManager::uploadIB()
{
    auto length = ib.length();
    if (length > 0)
    {
        _glIB->setBytes((uint32_t)length);
        _glIB->update(0, ib.getBuffer(), length);
    }
}

void MiddlewareManager::addTimer(IMiddleware* editor)
{
    auto it = std::find(_removeList.begin(), _removeList.end(), editor);
    if (it != _removeList.end())
    {
        _removeList.erase(it);
    }
    _updateMap[editor] = true;
}

void MiddlewareManager::removeTimer(IMiddleware* editor)
{
    if (isUpdating)
    {
        _removeList.push_back(editor);
    }
    else
    {
        auto it = _updateMap.find(editor);
        if (it != _updateMap.end())
        {
            _updateMap.erase(it);
        }
    }
}

MIDDLEWARE_END
