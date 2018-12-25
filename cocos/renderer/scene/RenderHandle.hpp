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

#include "../Macro.h"
#include "SystemHandle.hpp"
#include "MeshBuffer.hpp"
#include "math/CCMath.h"
#include "../renderer/Effect.h"

namespace se {
    class Object;
    class HandleObject;
}

RENDERER_BEGIN

class NodeProxy;
class ModelBatcher;

/**
 * @addtogroup scene
 * @{
 */

/**
 *  @brief The render handle is a system handle which occupies rendering datas.
 *  It's kind of a cpp delegate for js RenderComponent and should be created and updated by js RenderComponent.
 *  It update local vertex data to world vertex data if necessary, commit all render datas to the shared vertex and index buffer.
 */
class RenderHandle : SystemHandle
{
public:
    RenderHandle();
    virtual ~RenderHandle();
    /**
     *  @brief Commit the current render handle to ModelBatcher
     */
    virtual void handle(NodeProxy *node, ModelBatcher* batcher, Scene* scene) override;
    /**
     *  @brief Do nothing
     */
    virtual void postHandle(NodeProxy *node, ModelBatcher* batcher, Scene* scene) override;
    /**
     *  @brief Fills render data in given index to the MeshBuffer
     *  @param[in] buffer The shared mesh buffer
     *  @param[in] index The index of render data to be updated
     *  @param[in] worldMat The world transform matrix
     */
    virtual void fillBuffers(MeshBuffer* buffer, int index, const Mat4& worldMat);

    /**
     *  @brief Gets rendering material for the given index.
     *  @param[in] index The material index.
     *  @return The material pointer.
     */
    Effect* getEffect(uint32_t index);
    
    /**
     *  @brief Gets the count of render datas
     *  @return Count.
     */
    uint32_t getMeshCount() const { return (uint32_t)_datas.size(); };
    /**
     *  @brief Sets the count of render datas
     *  @param[in] count
     */
    void setMeshCount(uint32_t count);
    /**
     *  @brief Update the mesh data for the given index.
     *  @param[in] index Render data index.
     *  @param[in] vertices Vertex data.
     *  @param[in] indices Index data.
     */
    void updateNativeMesh(uint32_t index, se::Object* vertices, se::Object* indices);
    /**
     *  @brief Update the material for the given index.
     *  @param[in] index Render data index.
     *  @param[in] effect Effect pointer.
     */
    void updateNativeEffect(uint32_t index, Effect* effect);
    /**
     *  @brief Gets whether the current handle should use model matrix uniform during rendering
     */
    bool getUseModel() const { return _useModel; };
    /**
     *  @brief Sets whether the current handle should use model matrix uniform during rendering
     */
    void setUseModel(bool useModel) { _useModel = useModel; };
    
    /**
     *  @brief Gets the vertex format.
     */
    VertexFormat* getVertexFormat() const { return _vfmt; };
    /**
     *  @brief Sets the vertex format.
     */
    void setVertexFormat(VertexFormat* vfmt);
    
    void enable();
    void disable();
    bool enabled() const { return _enabled; };
    
protected:
    struct RenderData {
        RenderData ()
        : vBytes(0)
        , iBytes(0)
        , effect(nullptr)
        , jsVertices(nullptr)
        , jsIndices(nullptr)
        {
            
        }
        ~RenderData ();
        unsigned long vBytes;
        unsigned long iBytes;
        Effect* effect;
        uint8_t* vertices;
        uint8_t* indices;
        se::Object* jsVertices;
        se::Object* jsIndices;
    };
    
protected:
    bool _enabled;
    bool _useModel;
    uint32_t _bytesPerVertex;
    size_t _posOffset;

    VertexFormat* _vfmt;
    const VertexFormat::Element* _vfpos;
    std::vector<RenderData> _datas;
};

// end of scene group
/// @}

RENDERER_END
