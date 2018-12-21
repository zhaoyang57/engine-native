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

#include <map>

#include "../Macro.h"
#include "RenderHandle.hpp"
#include "CustomRenderHandle.hpp"
#include "MeshBuffer.hpp"
#include "../renderer/Renderer.h"
#include "math/CCMath.h"

RENDERER_BEGIN

class RenderFlow;
class StencilManager;

/**
 * @addtogroup scene
 * @{
 */

/**
 *  @brief Collect render data.
 */
class ModelBatcher
{
public:
    /**
     *  @brief The default constructor.
     */
    ModelBatcher(RenderFlow* flow);
    /**
     *  @brief The default destructor.
     */
    ~ModelBatcher();
    /**
     *  @brief Reset all render buffer.
     */
    void reset();
    
    /**
     *  @brief Begin to collect render data.
     *  @param[in] node Be collect node.
     *  @param[in] handle Specific collect performer.
     */
    void commit(NodeProxy* node, RenderHandle* handle);
    /**
     *  @brief Begin to collect custom render data.
     *  @param[in] node Be collect node.
     *  @param[in] handle Specific collect performer.
     */
    void commitIA(NodeProxy* node, CustomRenderHandle* handle);
    
    /**
     *  @brief Invoke before all node visit.
     */
    void startBatch();
    /**
     *  @brief End once render batcher.
     */
    void flush();
    /**
     *  @brief End once custom render batcher.
     */
    void flushIA(InputAssembler* customIA);
    /**
     *  @brief Invoke after all node visit.
     */
    void terminateBatch();
    
    /**
     *  @brief Get corresponding vertex format render buffer.
     *  @param[in] fmt Vertex format value
     */
    MeshBuffer* getBuffer(VertexFormat* fmt);
    /**
     *  @brief Get current render buffer.
     */
    const MeshBuffer* getCurrentBuffer() const { return _buffer; };
    /**
     *  @brief Set current render buffer.
     *  @param[in] buffer
     */
    void setCurrentBuffer(MeshBuffer* buffer) { _buffer = buffer; };
    /**
     *  @brief Get render flow pointer.
     */
    RenderFlow* getFlow() const { return _flow; };
    
    void setCurrentEffect(Effect* effect) { _currEffect = effect; };
private:
    int _iaOffset;
    int _modelOffset;
    int _cullingMask;
    bool _walking;
    cocos2d::Mat4 _modelMat;
    
    MeshBuffer* _buffer;
    Effect* _currEffect;
    RenderFlow* _flow;

    StencilManager* _stencilMgr;
    
    std::vector<InputAssembler*> _iaPool;
    std::vector<Model*> _modelPool;
    std::vector<Model*> _batchedModel;
    std::unordered_map<VertexFormat*, MeshBuffer*> _buffers;
};

// end of scene group
/// @}

RENDERER_END
