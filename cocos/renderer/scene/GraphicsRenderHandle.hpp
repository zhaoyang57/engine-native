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
#pragma once

#include <stdio.h>
#include "math/CCMath.h"
#include "../Macro.h"
#include "./CustomRenderHandle.hpp"
#include "../renderer/InputAssembler.h"
#include "./ModelBatcher.hpp"
#include "./MeshBuffer.hpp"
#include "../gfx/VertexFormat.h"

namespace se {
    class Object;
    class HandleObject;
}

RENDERER_BEGIN

class GraphicsRenderHandle: public CustomRenderHandle
{
    protected:
        struct IARenderData {
            IARenderData ()
            : vBytes(0)
            , iBytes(0)
            , effect(nullptr)
            , jsVertices(nullptr)
            , jsIndices(nullptr)
            , vb(nullptr)
            , ib(nullptr)
            {
                
            }
            ~IARenderData ();
            unsigned long vBytes;
            unsigned long iBytes;
            Effect* effect;
            uint8_t* vertices;
            uint8_t* indices;
            se::Object* jsVertices;
            se::Object* jsIndices;
            std::vector<uint8_t> worldVerts;
            VertexBuffer* vb;
            IndexBuffer* ib;
        };
    public:
        GraphicsRenderHandle();
        virtual ~GraphicsRenderHandle();
        virtual void renderIA(std::size_t index, ModelBatcher* batcher) override;
        void updateIA(std::size_t index, int start, int count);
        uint32_t getMeshCount() const { return (uint32_t)_datas.size(); };
        void setMeshCount(uint32_t count);
        void updateNativeMesh(uint32_t index, se::Object* vertices, se::Object* indices);
        virtual void updateNativeEffect(std::size_t index, Effect* effect) override;
        void reset();
        virtual inline std::size_t getIACount() const override
        {
            return _iaCount;
        };
    private:
        std::vector<InputAssembler*> _iaPool;
        std::vector<IARenderData> _datas;
        std::size_t _iaCount = 0;
        int _dataOffset = 0;
};

RENDERER_END
