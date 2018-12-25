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
#include "RenderHandle.hpp"
#include "NodeProxy.hpp"
#include "../renderer/Effect.h"
#include "base/CCVector.h"

RENDERER_BEGIN

/**
 * @addtogroup scene
 * @{
 */

/**
 *  @brief Custom render handle base class
 *  Render components that manages render buffer directly like spine, dragonBones should extend from this handle type.
 */
class CustomRenderHandle : public SystemHandle
{
public:
    CustomRenderHandle();
    virtual ~CustomRenderHandle();
    
    /**
     *  @brief Submit the InputAssembler for the given index to ModelBatcher
     *  @param[in] index Render data index.
     *  @param[in] batcher The ModelBatcher which will transform render data to Model
     */
    virtual void renderIA(std::size_t index, ModelBatcher* batcher) {};
    
    /**
     *  @brief Gets the material for the given index.
     *  @param[in] index Render data index.
     *  @return Effect pointer.
     */
    inline Effect* getEffect(uint32_t index) const
    {
        if (index >= _effects.size())
        {
            return nullptr;
        }
        return _effects.at(index);
    }
    
    /**
     *  @brief Gets Effect count.
     *  @return Count.
     */
    inline std::size_t getEffectCount() const
    {
        return _effects.size();
    }
    
    /**
     *  @brief Gets input assembler count.
     *  @return Count.
     */
    virtual inline std::size_t getIACount() const
    {
        return 0;
    }
    
    /**
     *  @brief Update the material for the given index
     *  @param[in] index Render data index.
     *  @param[in] effect Effect pointer.
     */
    virtual void updateNativeEffect(std::size_t index, Effect* effect);
    
    /**
     *  @brief Gets whether the current handle should use model matrix uniform during rendering
     *  @return useModel
     */
    bool getUseModel() const { return _useModel; };
    
    /**
     *  @brief Sets whether the current handle should use model matrix uniform during rendering
     *  @param[in] useModel
     */
    void setUseModel(bool useModel) { _useModel = useModel; };
    /**
     *  @brief Commit the current render handle to ModelBatcher
     */
    virtual void handle(NodeProxy *node, ModelBatcher* batcher, Scene* scene) override;
    /**
     *  @brief Do nothing
     */
    virtual void postHandle(NodeProxy *node, ModelBatcher* batcher, Scene* scene) override {}
    
    void enable() { _enabled = true; }
    void disable() { _enabled = false; }
protected:
    cocos2d::Vector<Effect*> _effects;//weak container
    bool _useModel = false;
    bool _enabled = false;
};

// end of scene group
/// @}

RENDERER_END
