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

#include "Pass.h"

RENDERER_BEGIN

Pass::Pass(const std::string& programName)
: _programName(programName)
{
//    RENDERER_LOGD("Pass constructor: %p", this);
}

Pass::~Pass()
{
//    RENDERER_LOGD("Pass destructor: %p", this);
}

void Pass::setCullMode(CullMode cullMode)
{
    _cullMode = cullMode;
}

void Pass::setBlend(BlendOp blendEq,
                    BlendFactor blendSrc,
                    BlendFactor blendDst,
                    BlendOp blendAlphaEq,
                    BlendFactor blendSrcAlpha,
                    BlendFactor blendDstAlpha,
                    uint32_t blendColor)
{
    _blend = true;
    _blendEq = blendEq;
    _blendSrc = blendSrc;
    _blendDst = blendDst;
    _blendAlphaEq = blendAlphaEq;
    _blendSrcAlpha = blendSrcAlpha;
    _blendDstAlpha = blendDstAlpha;
    _blendColor = blendColor;
}

void Pass::setDepth(bool depthTest, bool depthWrite, DepthFunc depthFunc)
{
    _depthTest = depthTest;
    _depthWrite = depthWrite;
    _depthFunc = depthFunc;
}

void Pass::setStencilFront(StencilFunc stencilFunc,
                           uint32_t stencilRef,
                           uint8_t stencilMask,
                           StencilOp stencilFailOp,
                           StencilOp stencilZFailOp,
                           StencilOp stencilZPassOp,
                           uint8_t stencilWriteMask)
{
    _stencilTest = true;
    _stencilFuncFront = stencilFunc;
    _stencilRefFront = stencilRef;
    _stencilMaskFront = stencilMask;
    _stencilFailOpFront = stencilFailOp;
    _stencilZFailOpFront = stencilZFailOp;
    _stencilZPassOpFront = stencilZPassOp;
    _stencilWriteMaskFront = stencilWriteMask;
}

void Pass::setStencilBack(StencilFunc stencilFunc,
                          uint32_t stencilRef,
                          uint8_t stencilMask,
                          StencilOp stencilFailOp,
                          StencilOp stencilZFailOp,
                          StencilOp stencilZPassOp,
                          uint8_t stencilWriteMask)
{
    _stencilTest = true;
    _stencilFuncBack = stencilFunc;
    _stencilRefBack = stencilRef;
    _stencilMaskBack = stencilMask;
    _stencilFailOpBack = stencilFailOp;
    _stencilZFailOpBack = stencilZFailOp;
    _stencilZPassOpBack = stencilZPassOp;
    _stencilWriteMaskBack = stencilWriteMask;
}

void Pass::copy(const Pass& pass)
{
    // blending
    _blend = pass._blend;
    _blendEq = pass._blendEq;
    _blendAlphaEq = pass._blendAlphaEq;
    _blendSrc = pass._blendSrc;
    _blendDst = pass._blendDst;
    _blendSrcAlpha = pass._blendSrcAlpha;
    _blendDstAlpha = pass._blendDstAlpha;
    _blendColor = pass._blendColor;
    
    // depth
    _depthTest = pass._depthTest;
    _depthWrite = pass._depthWrite;
    _depthFunc = pass._depthFunc;
    
    // stencil
    
    _stencilTest = pass._stencilTest;
    // front
    _stencilRefFront = pass._stencilRefFront;
    _stencilFuncFront = pass._stencilFuncFront;
    _stencilFailOpFront = pass._stencilFailOpFront;
    _stencilZFailOpFront = pass._stencilZFailOpFront;
    _stencilZPassOpFront = pass._stencilZPassOpFront;
    _stencilWriteMaskFront = pass._stencilWriteMaskFront;
    _stencilMaskFront = pass._stencilMaskFront;
    // back
    _stencilRefBack = pass._stencilRefBack;
    _stencilFuncBack = pass._stencilFuncBack;
    _stencilFailOpBack = pass._stencilFailOpBack;
    _stencilZFailOpBack = pass._stencilZFailOpBack;
    _stencilZPassOpBack = pass._stencilZPassOpBack;
    _stencilWriteMaskBack = pass._stencilWriteMaskBack;
    _stencilMaskBack = pass._stencilMaskBack;
    
    // cull mode
    _cullMode = pass._cullMode;
    
    _programName = pass._programName;
}

RENDERER_END
