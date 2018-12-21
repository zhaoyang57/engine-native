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
#include "NodeProxy.hpp"
#include "ModelBatcher.hpp"
#include "../renderer/Scene.h"
#include "../renderer/ForwardRenderer.h"
#include "../gfx/DeviceGraphics.h"

RENDERER_BEGIN

/**
 * @addtogroup scene
 * @{
 */

/**
 *  @brief Responsible for visit all node.
 */
class RenderFlow
{
public:
    /**
     *  @brief Get model batcher which is responsible for collect render data.
     */
    ModelBatcher* getModelBatcher() const { return _batcher; };
    /**
     *  @brief Get graphics device which is render api's abstract.
     */
    DeviceGraphics* getDevice() const { return _device; };
    /**
     *  @brief Get render scene.
     */
    Scene* getRenderScene() const { return _scene; };
    /**
     *  @brief Begin to render node tree.
     *  @param[in] scene The root node.
     */
    void render(NodeProxy* scene);
    /**
     *  @brief Constructor.
     *  @param[in] device DeviceGraphics.
     *  @param[in] scene Scene.
     *  @param[in] forward ForwardRenderer.
     */
    RenderFlow(DeviceGraphics* device, Scene* scene, ForwardRenderer* forward);
    /**
     *  @brief Destructor.
     */
    ~RenderFlow();
private:
    ModelBatcher* _batcher;
    Scene* _scene;
    DeviceGraphics* _device;
    ForwardRenderer* _forward;
};

// end of scene group
/// @}

RENDERER_END
