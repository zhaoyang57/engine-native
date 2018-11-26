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
#pragma once

#include "IOBuffer.h"
#include "map"
#include "vector"
#include "base/CCRef.h"
#include "renderer/gfx/VertexBuffer.h"
#include "renderer/gfx/IndexBuffer.h"

namespace editor {
    
    class IEditor {
    public:
        IEditor(){}
        virtual ~IEditor(){}
        virtual void update(float dt) = 0;
    };
    
    class EditorManager {
    public:
        static EditorManager* getInstance()
        {
            if (_instance == nullptr)
            {
                _instance = new EditorManager;
            }
            
            return _instance;
        }
        
        static void destroyInstance()
        {
            if (_instance)
            {
                delete _instance;
                _instance = nullptr;
            }
        }
        
        /**
         * @brief Third party module add in _updateMap,it will update perframe.
         * @param[in] editor Module must implement IEditor interface.
         */
        void addTimer(IEditor* editor);
        
        /**
         * @brief Third party module remove from _updateMap,it will stop update.
         * @param[in] editor Module must implement IEditor interface.
         */
        void removeTimer(IEditor* editor);
        
        editor::IOBuffer vb;
        editor::IOBuffer ib;
        
        inline cocos2d::renderer::VertexBuffer* getVB()
        {
            return glVB;
        }
        
        inline cocos2d::renderer::IndexBuffer* getIB()
        {
            return glIB;
        }
        
        EditorManager();
        ~EditorManager();
        
        // If manager is traversing _updateMap,will set the flag,untill traverse is finished.
        bool isUpdating = false;
        
    private:
        void update(float dt);
        void uploadVB();
        void uploadIB();
        void afterCleanupHandle();
        void afterInitHandle();
        void updateGLIB();
        void updateGLVB();
    private:
        std::map<IEditor*, bool> _updateMap;
        std::vector<IEditor*> _removeList;
        
        cocos2d::renderer::VertexBuffer* glVB = nullptr;
        cocos2d::renderer::IndexBuffer* glIB = nullptr;
        
        static EditorManager* _instance;
    };
}
