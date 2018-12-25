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

#include <stdint.h>
#include <vector>
#include "../Macro.h"
#include "../Types.h"

RENDERER_BEGIN

class VertexBuffer;
class IndexBuffer;
class Program;
class Texture;

/**
 * @addtogroup gfx
 * @{
 */

/**
 * State class save the GL states used in one draw call.
 * It will be used to compare between the previous states and the current states, so that we know which GL states should be updated.
 * User shouldn't be using this Class directly. Instead, they should set the states in Pass of material system.
 * @see `Pass`
 */
struct State final
{
    /**
     * Constructor
     */
    State();
    State(const State&);
    State(State &&);
    ~State();

    State& operator =(const State& rhs);
    State& operator =(State&& rhs);

    /**
     * Reset all states to default values
     */
    void reset();

    /**
     @name Blend
     @{
     */
    /**
     * Indicates blending enabled or not
     */
    bool blend;
    /**
     * Indicates the RGB blend equation and alpha blend equation settings are separate or not
     */
    bool blendSeparation;
    /**
     * The blending color
     */
    uint32_t blendColor;
    /**
     * The blending equation
     */
    BlendOp blendEq;
    /**
     * The blending equation for alpha channel
     */
    BlendOp blendAlphaEq;
    /**
     * The blending source factor
     */
    BlendFactor blendSrc;
    /**
     * The blending destination factor
     */
    BlendFactor blendDst;
    /**
     * The blending source factor for alpha
     */
    BlendFactor blendSrcAlpha;
    /**
     * The blending destination factor for alpha
     */
    BlendFactor blendDstAlpha;
    /**
     end of Blend
     @}
     */
    
    /**
     @name Depth
     @{
     */
    /**
     * Indicates depth test enabled or not
     */
    bool depthTest;
    /**
     * Indicates depth writing enabled or not
     */
    bool depthWrite;
    /**
     * The depth comparison function
     */
    DepthFunc depthFunc;
    /**
     end of Depth
     @}
     */
    
    /**
     @name Stencil
     @{
     */
    /**
     * Indicates stencil test enabled or not
     */
    bool stencilTest;
    /**
     * Indicates stencil test function for front and back is set separatly or not
     */
    bool stencilSeparation;
    /**
     * The front face function for stencil test
     */
    StencilFunc stencilFuncFront;
    /**
     * The front reference value for stencil test
     */
    int32_t stencilRefFront;
    /**
     * The front mask value to AND with the reference value and store the result into stencil buffer when test is done
     */
    uint32_t stencilMaskFront;
    /**
     * The function to use when the stencil test fails for front face
     */
    StencilOp stencilFailOpFront;
    /**
     * The function to use when the stencil test passes but the depth test fails for front face
     */
    StencilOp stencilZFailOpFront;
    /**
     * The function to use when both the stencil and the depth test pass for front face.
     * Or when the stencil test passes and there is no depth buffer or depth testing is disabled.
     */
    StencilOp stencilZPassOpFront;
    /**
     * The bit mask to enable or disable writing of individual bits in the front stencil planes
     */
    uint32_t stencilWriteMaskFront;
    /**
     * The back face function for stencil test
     */
    StencilFunc stencilFuncBack;
    /**
     * The back reference value for stencil test
     */
    int32_t stencilRefBack;
    /**
     * The back mask value to AND with the reference value and store the result into stencil buffer when test is done
     */
    uint32_t stencilMaskBack;
    /**
     * The function to use when the stencil test fails for back face
     */
    StencilOp stencilFailOpBack;
    /**
     * The function to use when the stencil test passes but the depth test fails for back face
     */
    StencilOp stencilZFailOpBack;
    /**
     * The function to use when both the stencil and the depth test pass for back face.
     * Or when the stencil test passes and there is no depth buffer or depth testing is disabled.
     */
    StencilOp stencilZPassOpBack;
    /**
     * The bit mask to enable or disable writing of individual bits in the back stencil planes
     */
    uint32_t stencilWriteMaskBack;
    /**
     end of Stencil
     @}
     */
    
    /**
     * Specifies whether front-facing or back-facing polygons are candidates for culling.
     */
    CullMode cullMode;
    
    /**
     * Specifies the primitive type for rendering
     */
    PrimitiveType primitiveType;
    
    int32_t maxStream;

    /**
     * Specifies the vertex buffer
     */
    void setVertexBuffer(size_t index, VertexBuffer* vertBuf);
    /**
     * Gets the vertex buffer
     */
    VertexBuffer* getVertexBuffer(size_t index) const;

    /**
     * Specifies the vertex buffer offset
     */
    void setVertexBufferOffset(size_t index, int32_t offset);
    /**
     * Gets the vertex buffer offset
     */
    int32_t getVertexBufferOffset(size_t index) const;

    /**
     * Specifies the index buffer
     */
    void setIndexBuffer(IndexBuffer* indexBuf);
    /**
     * Gets the index buffer
     */
    IndexBuffer* getIndexBuffer() const;

    /**
     * Sets the texture for specific texture unit
     */
    void setTexture(size_t index, Texture* texture);
    /**
     * Gets the specific texture unit
     */
    Texture* getTexture(size_t index) const;
    
    /**
     * Gets all texture units
     */
    const std::vector<Texture*>& getTextureUnits() const { return _textureUnits; }

    /**
     * Sets the program used for rendering
     */
    void setProgram(Program* program);
    /**
     * Gets the program used for rendering
     */
    Program* getProgram() const;

private:
    std::vector<VertexBuffer*> _vertexBuffers;
    std::vector<int32_t> _vertexBufferOffsets;
    IndexBuffer *_indexBuffer;
    std::vector<Texture*> _textureUnits;
    Program *_program;
};

// end of gfx group
/// @}

RENDERER_END
