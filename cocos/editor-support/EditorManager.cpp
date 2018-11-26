/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
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
#include "EditorManager.h"
#include "platform/CCApplication.h"
#include "base/CCScheduler.h"
#include "base/CCGLUtils.h"
#include "EditorDef.h"
#include "scripting/js-bindings/jswrapper/SeApi.h"
#include "renderer/gfx/DeviceGraphics.h"

using namespace cocos2d;
using namespace cocos2d::renderer;

static const std::string scheduleKey = "editorScheduleKey";

namespace editor {
    
    EditorManager* EditorManager::_instance = nullptr;
    
    EditorManager::EditorManager()
    :vb(MAX_VB_BUFFER_SIZE)
    ,ib(MAX_IB_BUFFER_SIZE)
    {
        updateGLVB();
        updateGLIB();
        afterInitHandle();
    }

    EditorManager::~EditorManager()
    {
        auto app = cocos2d::Application::getInstance();
        auto scheduler = app->getScheduler();
        scheduler->unschedule(scheduleKey, this);
        
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
    
    void EditorManager::updateGLVB()
    {
        if (glVB && glVB->getBytes() == vb.getCapacity()) return;
        
        if (glVB)
        {
            delete glVB;
            glVB = nullptr;
        }
        
        glVB = new VertexBuffer();
        glVB->init(DeviceGraphics::getInstance(), VertexFormat::XY_UV_Color, Usage::DYNAMIC, nullptr, 0, (uint32_t)vb.getCapacity()/VertexFormat::XY_UV_Color->getBytes());
    }
    
    void EditorManager::updateGLIB()
    {
        if (glIB && glIB->getBytes() == ib.getCapacity()) return;
       
        if (glIB)
        {
            delete glIB;
            glIB = nullptr;
        }
        
        glIB = new IndexBuffer();
        glIB->init(DeviceGraphics::getInstance(), IndexFormat::UINT16, Usage::STATIC, nullptr, 0, (uint32_t)ib.getCapacity()/sizeof(unsigned short));
    }
    
    void EditorManager::afterCleanupHandle()
    {
        auto app = cocos2d::Application::getInstance();
        auto scheduler = app->getScheduler();
        scheduler->unschedule(scheduleKey, this);
        
        se::ScriptEngine::getInstance()->addAfterInitHook(std::bind(&EditorManager::afterInitHandle,this));
    }
    
    void EditorManager::afterInitHandle()
    {
        auto app = cocos2d::Application::getInstance();
        auto scheduler = app->getScheduler();
        scheduler->schedule(
        [&](float passedTime)
        {
            this->update(passedTime);
        },
        this, 0, false, scheduleKey);
        
        se::ScriptEngine::getInstance()->addAfterCleanupHook(std::bind(&EditorManager::afterCleanupHandle,this));
    }
    
    void EditorManager::update(float dt)
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
        
        if (vb.isOutRange())
        {
            vb.resize(vb.getCapacity() + INCREASE_BUFFER_SIZE, true);
            updateGLVB();
        }
        
        if (ib.isOutRange())
        {
            ib.resize(ib.getCapacity() + INCREASE_BUFFER_SIZE, true);
            updateGLIB();
        }
        
        auto length = vb.length();
        if (length > 0)
        {
            glVB->update(0, vb.getBuffer(), length);
        }
        
        length = ib.length();
        if (length > 0)
        {
            glIB->update(0, ib.getBuffer(), length);
        }
    }
    
    void EditorManager::addTimer(IEditor* editor)
    {
        _updateMap[editor] = true;
    }
    
    void EditorManager::removeTimer(IEditor* editor)
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
}
