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

#include <stdint.h>

#include "../Macro.h"
#include "../gfx/VertexFormat.h"
#include "../gfx/VertexBuffer.h"
#include "../gfx/IndexBuffer.h"

RENDERER_BEGIN

class ModelBatcher;

class MeshBuffer
{
public:
    struct OffsetInfo
    {
        uint32_t vByte;
        uint32_t index;
        uint32_t vertex;
    };
    
    MeshBuffer(ModelBatcher* batcher, VertexFormat* fmt);
    ~MeshBuffer();
    
    bool request(uint32_t vertexCount, uint32_t indexCount, OffsetInfo* offset);
    bool requestStatic(uint32_t vertexCount, uint32_t indexCount);
    void uploadData();
    void reset();
    void destroy();
    
    uint32_t getByteOffset() const { return _byteOffset; };
    uint32_t getVertexStart() const { return _vertexStart; };
    uint32_t getVertexOffset() const { return _vertexOffset; };
    uint32_t getIndexStart() const { return _indexStart; };
    uint32_t getIndexOffset() const { return _indexOffset; };
    
    void updateOffset()
    {
        _byteStart = _byteOffset;
        _vertexStart = _vertexOffset;
        _indexStart = _indexOffset;
    };
    
    VertexBuffer* getVertexBuffer() const { return _vb; };
    IndexBuffer* getIndexBuffer() const { return _ib; };
    
    std::vector<float> vData;
    std::vector<uint16_t> iData;
    VertexFormat* _vertexFmt;
    static const int INIT_VERTEX_COUNT = 256;
    static const uint8_t VDATA_BYTE = sizeof(float);
    static const uint8_t IDATA_BYTE = sizeof(uint16_t);

protected:
    void reallocBuffers();
    
private:
    uint32_t _byteStart;
    uint32_t _byteOffset;
    uint32_t _indexStart;
    uint32_t _indexOffset;
    uint32_t _vertexStart;
    uint32_t _vertexOffset;
    uint32_t _bytesPerVertex;
    uint32_t _vDataCount;
    uint32_t _iDataCount;
    bool _dirty;
    
    ModelBatcher* _batcher;
    VertexBuffer* _vb;
    IndexBuffer* _ib;
};

RENDERER_END
