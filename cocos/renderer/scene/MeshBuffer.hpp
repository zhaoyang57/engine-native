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

/**
 * @addtogroup scene
 * @{
 */

/**
 *  @brief Render buffer
 */
class MeshBuffer
{
public:
    /**
     *  @brief render buffer info
     */
    struct OffsetInfo
    {
        // bytes count
        uint32_t vByte;
        // index offset
        uint32_t index;
        // vertex offset
        uint32_t vertex;
    };
    
    /**
     *  @brief Constructor
     *  @param[in] batcher To collect vertex and index data,and then set in render buffer.
     *  @param[in] fmt Buffer format.
     */
    MeshBuffer(ModelBatcher* batcher, VertexFormat* fmt);
    /**
     *  @brief Destructor
     */
    ~MeshBuffer();
    
    /**
     *  @brief Guarantee render buffer has enough capacity.
     *  @param[in] vertexCount.
     *  @param[in] indexCount.
     *  @param[out] offset.
     */
    bool request(uint32_t vertexCount, uint32_t indexCount, OffsetInfo* offset);
    bool requestStatic(uint32_t vertexCount, uint32_t indexCount);
    
    /**
     *  @brief Upload data.
     */
    void uploadData();
    /**
     *  @brief Reset render buffer state.
     */
    void reset();
    /**
     *  @brief Destroy render buffer.
     */
    void destroy();
    
    /**
     *  @brief Get render buffer byte offset.
     *  @return byte offset.
     */
    uint32_t getByteOffset() const { return _byteOffset; };
    /**
     *  @brief Get render buffer vertex start.
     *  @return vertex start.
     */
    uint32_t getVertexStart() const { return _vertexStart; };
    /**
     *  @brief Get render buffer vertex offset.
     *  @return vertex offset.
     */
    uint32_t getVertexOffset() const { return _vertexOffset; };
    /**
     *  @brief Get index buffer index start.
     *  @return index start.
     */
    uint32_t getIndexStart() const { return _indexStart; };
    /**
     *  @brief Get index buffer index offset.
     *  @return index offset.
     */
    uint32_t getIndexOffset() const { return _indexOffset; };
    
    /**
     *  @brief Update offset.
     */
    void updateOffset()
    {
        _byteStart = _byteOffset;
        _vertexStart = _vertexOffset;
        _indexStart = _indexOffset;
    };
    
    /**
     *  @brief Get vertex buffer.
     */
    VertexBuffer* getVertexBuffer() const { return _vb; };
    /**
     *  @brief Get index buffer.
     */
    IndexBuffer* getIndexBuffer() const { return _ib; };
    
    /**
     *  @brief Use to storage vertex data.
     */
    std::vector<float> vData;
    /**
     *  @brief Use to storage index data.
     */
    std::vector<uint16_t> iData;
    /**
     *  @brief Vertex format.
     */
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

// end of scene group
/// @}

RENDERER_END
