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

#pragma once

#include <string>
#include <base/CCRef.h>
#include "../Macro.h"
#include "../Types.h"

RENDERER_BEGIN

/**
 * @addtogroup renderer
 * @{
 */

/**
 * @brief Pass describes base render pass configurations, including program, cull face, blending, depth testing and stencil testing configs.\n
 * JS API: renderer.Pass
 * @code
 * let pass = new renderer.Pass('sprite');
 * pass.setDepth(false, false);
 * pass.setCullMode(gfx.CULL_NONE);
 * pass.setBlend(
 *     gfx.BLEND_FUNC_ADD,
 *     gfx.BLEND_SRC_ALPHA, gfx.BLEND_ONE_MINUS_SRC_ALPHA,
 *     gfx.BLEND_FUNC_ADD,
 *     gfx.BLEND_SRC_ALPHA, gfx.BLEND_ONE_MINUS_SRC_ALPHA
 * );
 * @endcode
 */
class Pass : public Ref
{
public:
    /**
     * @brief Constructor with linked program name.
     * @param[in] programName Shader program name
     */
    Pass(const std::string& programName);
    Pass() {}
    ~Pass();
    
    /**
     *  @brief Sets cull mode.
     *  @param[in] cullMode Cull front or back or both.
     */
    void setCullMode(CullMode cullMode);
    /**
     *  @brief Sets blend mode.
     *  @param[in] blendEq RGB blend equation.
     *  @param[in] blendSrc Src RGB blend factor.
     *  @param[in] blendDst Dst RGB blend factor.
     *  @param[in] blendAlphaEq Alpha blend equation.
     *  @param[in] blendSrcAlpha Src Alpha blend equation.
     *  @param[in] blendDstAlpha Dst Alpha blend equation.
     *  @param[in] blendColor Blend constant color value.
     */
    void setBlend(BlendOp blendEq = BlendOp::ADD,
                  BlendFactor blendSrc = BlendFactor::ONE,
                  BlendFactor blendDst = BlendFactor::ZERO,
                  BlendOp blendAlphaEq = BlendOp::ADD,
                  BlendFactor blendSrcAlpha = BlendFactor::ONE,
                  BlendFactor blendDstAlpha = BlendFactor::ZERO,
                  uint32_t blendColor = 0xffffffff);
    /**
     *  @brief Switch depth test or write, and sets depth test function.
     *  @param[in] depthTest Enable depth test or not.
     *  @param[in] depthWrite Enable depth write or not.
     *  @param[in] depthFunc Depth test function.
     */
    void setDepth(bool depthTest = false,
                  bool depthWrite = false,
                  DepthFunc depthFunc = DepthFunc::LESS);
    /**
     *  @brief Sets stencil front-facing function, reference, mask, fail operation, write mask.
     */
    void setStencilFront(StencilFunc stencilFunc = StencilFunc::ALWAYS,
                         uint32_t stencilRef = 0,
                         uint8_t stencilMask = 0xff,
                         StencilOp stencilFailOp = StencilOp::KEEP,
                         StencilOp stencilZFailOp = StencilOp::KEEP,
                         StencilOp stencilZPassOp = StencilOp::KEEP,
                         uint8_t stencilWriteMask = 0xff);
    /**
     *  @brief Sets stencil back-facing function, reference, mask, fail operation, write mask.
     */
    void setStencilBack(StencilFunc stencilFunc = StencilFunc::ALWAYS,
                        uint32_t stencilRef = 0,
                        uint8_t stencilMask = 0xff,
                        StencilOp stencilFailOp = StencilOp::KEEP,
                        StencilOp stencilZFailOp = StencilOp::KEEP,
                        StencilOp stencilZPassOp = StencilOp::KEEP,
                        uint8_t stencilWriteMask = 0xff);
    /**
     *  @brief Sets stencil test enabled or not.
     */
    inline void setStencilTest(bool value) { _stencilTest = value; }
    /**
     *  @brief Gets stencil test enabled or not.
     */
    inline bool getStencilTest() const { return _stencilTest; }
    /**
     *  @brief Sets linked program name.
     */
    inline void setProgramName(const std::string& programName) { _programName = programName; }
    /*
     *  @brief Disable stencil test.
     */
    inline void disableStencilTest() { _stencilTest = false; }
    /**
     *  @brief deep copy from other pass.
     */
    void copy(const Pass& pass);
    
private:
    friend class BaseRenderer;
    
    // blending
    bool _blend = false;
    BlendOp _blendEq = BlendOp::ADD;
    BlendOp _blendAlphaEq = BlendOp::ADD;
    BlendFactor _blendSrc = BlendFactor::ONE;
    BlendFactor _blendDst = BlendFactor::ZERO;
    BlendFactor _blendSrcAlpha = BlendFactor::ONE;
    BlendFactor _blendDstAlpha = BlendFactor::ZERO;
    uint32_t _blendColor = 0xffffffff;
    
    // depth
    bool _depthTest = false;
    bool _depthWrite = false;
    DepthFunc _depthFunc = DepthFunc::LESS;
    
    // stencil
    
    bool _stencilTest = false;
    // front
    uint32_t _stencilRefFront = 0;
    StencilFunc _stencilFuncFront = StencilFunc::ALWAYS;
    StencilOp _stencilFailOpFront = StencilOp::KEEP;
    StencilOp _stencilZFailOpFront = StencilOp::KEEP;
    StencilOp _stencilZPassOpFront = StencilOp::KEEP;
    uint8_t _stencilWriteMaskFront = 0xff;
    uint8_t _stencilMaskFront = 0xff;
    // back
    uint32_t _stencilRefBack = 0;
    StencilFunc _stencilFuncBack = StencilFunc::ALWAYS;
    StencilOp _stencilFailOpBack = StencilOp::KEEP;
    StencilOp _stencilZFailOpBack = StencilOp::KEEP;
    StencilOp _stencilZPassOpBack = StencilOp::KEEP;
    uint8_t _stencilWriteMaskBack = 0xff;
    uint8_t _stencilMaskBack = 0xff;
    
    // cull mode
    CullMode _cullMode = CullMode::BACK;
    
    std::string _programName = "";
};

// end of renderer group
/// @}
RENDERER_END
