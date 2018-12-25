/****************************************************************************
 LICENSING AGREEMENT
 
 Xiamen Yaji Software Co., Ltd., (the “Licensor”) grants the user (the “Licensee”) non-exclusive and non-transferable rights to use the software according to the following conditions:
 a.  The Licensee shall pay royalties to the Licensor, and the amount of those royalties and the payment method are subject to separate negotiations between the parties.
 b.  The software is licensed for use rather than sold, and the Licensor reserves all rights over the software that are not expressly granted (whether by implication, reservation or prohibition).
 c.  The open source codes contained in the software are subject to the MIT Open Source Licensing Agreement (see the attached for the details);
 d.  The Licensee acknowledges and consents to the possibility that errors may occur during the operation of the software for one or more technical reasons, and the Licensee shall take precautions and prepare remedies for such events. In such circumstance, the Licensor shall provide software patches or updates according to the agreement between the two parties. The Licensor will not assume any liability beyond the explicit wording of this Licensing Agreement.
 e.  Where the Licensor must assume liability for the software according to relevant laws, the Licensor’s entire liability is limited to the annual royalty payable by the Licensee.
 f.  The Licensor owns the portions listed in the root directory and subdirectory (if any) in the software and enjoys the intellectual property rights over those portions. As for the portions owned by the Licensor, the Licensee shall not:
 - i. Bypass or avoid any relevant technical protection measures in the products or services;
 - ii. Release the source codes to any other parties;
 - iii. Disassemble, decompile, decipher, attack, emulate, exploit or reverse-engineer these portion of code;
 - iv. Apply it to any third-party products or services without Licensor’s permission;
 - v. Publish, copy, rent, lease, sell, export, import, distribute or lend any products containing these portions of code;
 - vi. Allow others to use any services relevant to the technology of these codes;
 - vii. Conduct any other act beyond the scope of this Licensing Agreement.
 g.  This Licensing Agreement terminates immediately if the Licensee breaches this Agreement. The Licensor may claim compensation from the Licensee where the Licensee’s breach causes any damage to the Licensor.
 h.  The laws of the People's Republic of China apply to this Licensing Agreement.
 i.  This Agreement is made in both Chinese and English, and the Chinese version shall prevail the event of conflict.
 ****************************************************************************/

#include "StencilManager.hpp"
#include "../Types.h"
#include "../renderer/Technique.h"
#include "../renderer/Pass.h"

RENDERER_BEGIN

static const std::string techStage = "transparent";
StencilManager* StencilManager::_instance = nullptr;

StencilManager::StencilManager ()
: _stage(Stage::DISABLED)
{
};

void StencilManager::reset ()
{
    // reset stack and stage
    _maskStack.clear();
    _stage = Stage::DISABLED;
};
    
Effect* StencilManager::handleEffect (Effect* effect)
{
        Technique* tech = effect->getTechnique(techStage);
        Vector<Pass*>& passes = (Vector<Pass*>&)tech->getPasses();
        if (_stage == Stage::DISABLED)
        {
            for (const auto& pass : passes)
            {
                if (pass->getStencilTest()) {
                    pass->disableStencilTest();
                }
            }
            return effect;
        }
        
        uint32_t ref;
        uint8_t stencilMask, writeMask;
        bool mask;
        StencilFunc func;
        StencilOp failOp = StencilOp::KEEP;
        const StencilOp& zFailOp = StencilOp::KEEP;
        const StencilOp& zPassOp = StencilOp::KEEP;
        
        if (_stage == Stage::ENABLED)
        {
            mask = _maskStack.back();
            func = StencilFunc::EQUAL;
            failOp = StencilOp::KEEP;
            ref = getStencilRef();
            stencilMask = ref;
            writeMask = getWriteMask();
        }
        else {
            if (_stage == Stage::CLEAR) {
                mask = _maskStack.back();
                func = StencilFunc::NEVER;
                failOp = mask ? StencilOp::REPLACE : StencilOp::ZERO;
                ref = getWriteMask();
                stencilMask = ref;
                writeMask = ref;
            }
            else if (_stage == Stage::ENTER_LEVEL) {
                mask = _maskStack.back();
                // Fill stencil mask
                func = StencilFunc::NEVER;
                failOp = mask ? StencilOp::ZERO : StencilOp::REPLACE;
                ref = getWriteMask();
                stencilMask = ref;
                writeMask = ref;
            }
        }
        
        for (const auto& pass : passes) {
            pass->setStencilFront(func, ref, stencilMask, failOp, zFailOp, zPassOp, writeMask);
            pass->setStencilBack(func, ref, stencilMask, failOp, zFailOp, zPassOp, writeMask);
        }
        return effect;
};
    
void StencilManager::pushMask (bool mask)
{
    if (_maskStack.size() + 1 > _maxLevel)
    {
        cocos2d::log("StencilManager:pushMask _maxLevel:%d is out of range", _maxLevel);
    }
    _maskStack.push_back(mask);
};
    
void StencilManager::clear ()
{
    _stage = Stage::CLEAR;
};
    
void StencilManager::enterLevel ()
{
    _stage = Stage::ENTER_LEVEL;
};
    
void StencilManager::enableMask ()
{
    _stage = Stage::ENABLED;
};
    
void StencilManager::exitMask ()
{
    if (_maskStack.size() == 0) {
        cocos2d::log("StencilManager:exitMask _maskStack:%lu size is 0", _maskStack.size());
    }
    _maskStack.pop_back();
    if (_maskStack.size() == 0) {
        _stage = Stage::DISABLED;
    }
    else {
        _stage = Stage::ENABLED;
    }
};

uint8_t StencilManager::getWriteMask ()
{
    return 0x01 << (_maskStack.size() - 1);
};
    
uint8_t StencilManager::getExitWriteMask ()
{
    return 0x01 << _maskStack.size();
};
    
uint32_t StencilManager::getStencilRef ()
{
    int32_t result = 0;
    size_t size = _maskStack.size();
    for (int i = 0; i < size; ++i) {
        result += (0x01 << i);
    }
    return result;
};
    
uint32_t StencilManager::getInvertedRef ()
{
    int32_t result = 0;
    size_t size = _maskStack.size();
    for (int i = 0; i < size - 1; ++i)
    {
        result += (0x01 << i);
    }
    return result;
};

RENDERER_END
