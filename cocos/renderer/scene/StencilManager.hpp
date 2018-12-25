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
#include <vector>
#include "../../base/CCVector.h"
#include "renderer/Effect.h"

RENDERER_BEGIN

/**
 * @addtogroup scene
 * @{
 */

// Stage types
enum class Stage{
    // Stencil disabled
    DISABLED = 0,
    // Clear stencil buffer
    CLEAR = 1,
    // Entering a new level, should handle new stencil
    ENTER_LEVEL = 2,
    // In content
    ENABLED = 3,
    // Exiting a level, should restore old stencil or disable
    EXIT_LEVEL = 4
};

/**
 * The stencil manager post process the Effect in Model to make them apply correct stencil states
 * After activated a stencil mask and before desactivated it, all Models committed in between should apply the stencil's states in the Pass of Effect.
 * This is a singleton class mainly used by ModelBatcher.
 */
class StencilManager
{
public:
    StencilManager();
    ~StencilManager();
    /**
     * Reset all states
     */
    void reset();
    /**
     * Apply correct stencil states to the Effect
     */
    Effect* handleEffect(Effect* effect);
    /**
     * Add a mask to the stack
     */
    void pushMask(bool mask);
    /**
     * Stage for clearing the stencil buffer for the last mask
     */
    void clear();
    /**
     * Enters a new mask level, this stage is for drawing the stencil ref to the stencil buffer.
     */
    void enterLevel();
    /**
     * Enables the current mask, this stage is for applying stencil states defined by the current mask.
     */
    void enableMask();
    /**
     * Exits a mask level
     */
    void exitMask();
    uint8_t getWriteMask();
    uint8_t getExitWriteMask();
    uint32_t getStencilRef();
    uint32_t getInvertedRef();

    static StencilManager* getInstance()
    {
        if (_instance == nullptr)
        {
            _instance = new StencilManager;
        }
        
        return _instance;
    }
private:
    const int _maxLevel = 8;
    std::vector<bool> _maskStack;
    Stage _stage;
    static StencilManager* _instance;
};

// end of scene group
/// @}

RENDERER_END
