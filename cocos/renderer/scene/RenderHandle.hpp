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
 *  @brief Compute NodeProxy local vertex data to world vertex data,and put vertex,uv,color into render buffer.
 */
class RenderHandle : SystemHandle
{
public:
    /**
     *  @brief The default constructor.
     */
    RenderHandle();
    /**
     *  @brief The default destructor.
     */
    virtual ~RenderHandle();
    /**
     *  @brief Invoke before visit child node.
     *  @param[in] node The node will be handle.
     *  @param[in] batcher The global render data collector.
     *  @param[in] scene Graphics scene.
     */
    virtual void handle(NodeProxy *node, ModelBatcher* batcher, Scene* scene) override;
    /**
     *  @brief Invoke after visit child node.
     *  @param[in] node The node will be handle.
     *  @param[in] batcher The global render data collector.
     *  @param[in] scene Graphics scene.
     */
    virtual void postHandle(NodeProxy *node, ModelBatcher* batcher, Scene* scene) override;
    /**
     *  @brief To fill render data in buffer.
     *  @param[in] buffer The container to put render data in it.
     *  @param[in] index Index of render times.
     *  @param[in] worldMat The world transform matrix.
     */
    virtual void fillBuffers(MeshBuffer* buffer, int index, const Mat4& worldMat);

    /**
     *  @brief Get render material by material index.
     *  @param[in] index Material index.
     *  @return material pointer.
     */
    Effect* getEffect(uint32_t index);
    
    /**
     *  @brief Get the model mesh data count.
     *  @return count.
     */
    uint32_t getMeshCount() const { return (uint32_t)_datas.size(); };
    /**
     *  @brief Set the model mesh data count.
     *  @param[in] count Mesh data count.
     */
    void setMeshCount(uint32_t count);
    /**
     *  @brief Update mesh data by index.
     *  @param[in] index Mesh data index.
     *  @param[in] vertices Vertex data.
     *  @param[in] indices Index data.
     */
    void updateNativeMesh(uint32_t index, se::Object* vertices, se::Object* indices);
    /**
     *  @brief Update materiar.
     *  @param[in] index Effect index.
     *  @param[in] effect Effect pointer.
     */
    void updateNativeEffect(uint32_t index, Effect* effect);
    /**
     *  @brief Get use model switch.
     *  @return useModel.
     */
    bool getUseModel() const { return _useModel; };
    /**
     *  @brief Set use model switch.
     *  @param[in] useModel Switch value.
     */
    void setUseModel(bool useModel) { _useModel = useModel; };
    
    /**
     *  @brief Get vertex format.
     *  @return VertexFormat pointer.
     */
    VertexFormat* getVertexFormat() const { return _vfmt; };
    /**
     *  @brief Get vertex format.
     *  @return VertexFormat pointer.
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
