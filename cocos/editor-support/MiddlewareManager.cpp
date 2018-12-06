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
:vb(MAX_VB_BUFFER_SIZE)
,ib(MAX_IB_BUFFER_SIZE)
{
    updateGLVB();
    updateGLIB();
}

MiddlewareManager::~MiddlewareManager()
{
    if (glVB)
    {
        delete glVB;
        glVB = nullptr;
    }
    
    if (glVB)
    {
        delete glVB;
        glVB = nullptr;
    }
}

void MiddlewareManager::updateGLVB()
{
    if (glVB)
    {
        delete glVB;
        glVB = nullptr;
    }
    
    glVB = new VertexBuffer();
    glVB->init(DeviceGraphics::getInstance(), VertexFormat::XY_UV_Color, Usage::DYNAMIC, nullptr, 0, (uint32_t)vb.getCapacity()/VertexFormat::XY_UV_Color->getBytes());
}

void MiddlewareManager::updateGLIB()
{
    if (glIB)
    {
        delete glIB;
        glIB = nullptr;
    }
    
    glIB = new IndexBuffer();
    glIB->init(DeviceGraphics::getInstance(), IndexFormat::UINT16, Usage::STATIC, nullptr, 0, (uint32_t)ib.getCapacity()/sizeof(unsigned short));
}

void MiddlewareManager::update(float dt)
{
    vb.reset();
    ib.reset();
    
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
    
    auto length = vb.length();
    if (length > 0)
    {
        glVB->setBytes((uint32_t)length);
        glVB->update(0, vb.getBuffer(), length);
    }
    
    length = ib.length();
    if (length > 0)
    {
        glIB->setBytes((uint32_t)length);
        glIB->update(0, ib.getBuffer(), length);
    }
}

void MiddlewareManager::addTimer(IMiddleware* editor)
{
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
