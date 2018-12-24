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

#include <new>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string>
#include "base/CCValue.h"

#include "Macro.h"
#include "platform/CCGL.h"

RENDERER_BEGIN

/**
 * @addtogroup base
 * @{
 */

/**
 * ClearFlag defines what could be cleared during rendering process
 */
enum ClearFlag : uint8_t
{
    /**
     * No clear flags.
     */
    NONE        = 0x00,
    /**
     * Clear color.
     */
    COLOR       = 0x01,
    /**
     * Clear depth.
     */
    DEPTH       = 0x02,
    /**
     * Clear stencil.
     */
    STENCIL     = 0x04
};

/**
 * Comparison functions for stencil and depth test
 */
enum class ComparisonFunc : uint16_t
{
    /**
     * Never pass.
     */
    NEVER               = GL_NEVER,
    /**
     * Pass if (ref & mask) < (stencil & mask).
     */
    LESS                = GL_LESS,
    /**
     * Pass if (ref & mask) = (stencil & mask).
     */
    EQUAL               = GL_EQUAL,
    /**
     * Pass if (ref & mask) <= (stencil & mask).
     */
    LESS_EQUAL          = GL_LEQUAL,
    /**
     * Pass if (ref & mask) > (stencil & mask).
     */
    GREATER             = GL_GREATER,
    /**
     * Pass if (ref & mask) != (stencil & mask).
     */
    NOT_EQUAL           = GL_NOTEQUAL,
    /**
     * Pass if (ref & mask) >= (stencil & mask).
     */
    GREATOR_EQUAL       = GL_GEQUAL,
    /**
     * Always pass.
     */
    ALWAYS              = GL_ALWAYS
};
typedef ComparisonFunc DepthFunc;
typedef ComparisonFunc StencilFunc;
typedef ComparisonFunc SamplerFunc;

/**
 * Stencil test operations
 */
enum class StencilOp : uint16_t
{
    /**
     * Sets the stencil buffer value to 0.
     */
    ZERO                    = GL_ZERO,
    /**
     * Keeps the current value.
     */
    KEEP                    = GL_KEEP,
    /**
     * Sets the stencil buffer value to the reference value as specified by stencil function
     */
    REPLACE                 = GL_REPLACE,
    /**
     * Increments the current stencil buffer value. Clamps to the maximum representable unsigned value.
     */
    INCR                    = GL_INCR,
    /**
     * Decrements the current stencil buffer value. Clamps to 0.
     */
    DECR                    = GL_DECR,
    /**
     * Inverts the current stencil buffer value bitwise.
     */
    INVERT                  = GL_INVERT,
    
    // Does these two belongs to stencil operation?
    /**
     * Increments the current stencil buffer value. Wraps stencil buffer value to zero when incrementing the maximum representable unsigned value.
     */
    INCR_WRAP                = GL_INCR_WRAP,
    /**
     * Decrements the current stencil buffer value. Wraps stencil buffer value to the maximum representable unsigned value when decrementing a stencil buffer value of 0.
     */
    DECR_WRAP                = GL_DECR_WRAP
};

/**
 * Blending factor for source and destination
 */
enum class BlendFactor : uint16_t
{
    /**
     * Multiplies all colors by 0.
     */
    ZERO                        = GL_ZERO,
    /**
     * Multiplies all colors by 1.
     */
    ONE                         = GL_ONE,
    /**
     * Multiplies all colors by the source colors.
     */
    SRC_COLOR                   = GL_SRC_COLOR,
    /**
     * Multiplies all colors by 1 minus each source color.
     */
    ONE_MINUS_SRC_COLOR         = GL_ONE_MINUS_SRC_COLOR,
    /**
     * Multiplies all colors by the source alpha value.
     */
    SRC_ALPHA                   = GL_SRC_ALPHA,
    /**
     * Multiplies all colors by 1 minus the source alpha value
     */
    ONE_MINUS_SRC_ALPHA         = GL_ONE_MINUS_SRC_ALPHA,
    /**
     * Multiplies all colors by the destination alpha value.
     */
    DST_ALPHA                   = GL_DST_ALPHA,
    /**
     * Multiplies all colors by 1 minus the destination alpha value.
     */
    ONE_MINUS_DST_ALPHA         = GL_ONE_MINUS_DST_ALPHA,
    /**
     *
     */
    DST_COLOR                   = GL_DST_COLOR,
    /**
     * Multiplies all colors by 1 minus each destination color.
     */
    ONE_MINUS_DST_COLOR         = GL_ONE_MINUS_DST_COLOR,
    /**
     * Multiplies the RGB colors by the smaller of either the source alpha value or the value of 1 minus the destination alpha value. The alpha value is multiplied by 1.
     */
    SRC_ALPHA_SATURATE          = GL_SRC_ALPHA_SATURATE,
    
    /**
     * Multiplies all colors by a constant color.
     */
    CONSTANT_COLOR              = GL_CONSTANT_COLOR,
    /**
     * Multiplies all colors by 1 minus a constant color.
     */
    ONE_MINUS_CONSTANT_COLOR    = GL_ONE_MINUS_CONSTANT_COLOR,
    /**
     * Multiplies all colors by a constant alpha value.
     */
    CONSTANT_ALPHA              = GL_CONSTANT_ALPHA,
    /**
     * Multiplies all colors by 1 minus a constant alpha value.
     */
    ONE_MINUS_CONSTANT_ALPHA    = GL_ONE_MINUS_CONSTANT_ALPHA
};
/**
 * Blend operation specifying how the alpha component (transparency) of source and destination colors are combined
 */
enum class BlendOp : uint16_t
{
    /**
     * source + destination (default value)
     */
    ADD                 = GL_FUNC_ADD,
    /**
     * source - destination
     */
    SUBTRACT            = GL_FUNC_SUBTRACT,
    /**
     * destination - source
     */
    REVERSE_SUBTRACT    = GL_FUNC_REVERSE_SUBTRACT
};

/**
 * Cull mode specifying whether or not front-facing and/or back-facing polygons can be culled
 */
enum class CullMode : uint16_t
{
    /**
     * Nothing can be culled
     */
    NONE                    = GL_NONE,
    /**
     * Back face can be culled
     */
    BACK                    = GL_BACK,
    /**
     * Front face can be culled
     */
    FRONT                   = GL_FRONT,
    /**
     * Front and back face can be culled
     */
    FRONT_AND_BACK          = GL_FRONT_AND_BACK
};

/**
 * The primitive type to render
 */
enum class PrimitiveType : uint16_t
{
    /**
     * Draws single dots.
     */
    POINTS                  = GL_POINTS,
    /**
     * Draws lines between pairs of vertices.
     */
    LINES                   = GL_LINES,
    /**
     * Draws straight lines one after another vertex, and connects the last vertex back to the first.
     */
    LINE_LOOP               = GL_LINE_LOOP,
    /**
     * Draws straight lines one after another vertex.
     */
    LINE_STRIP              = GL_LINE_STRIP,
    /**
     * Draws triangles for groups of three vertices.
     */
    TRIANGLES               = GL_TRIANGLES,
    /**
     * Draws triangles without indexing, connects automatically successive vertex point to the last two used vertices to form a triangle.
     */
    TRIANGLE_STRIP          = GL_TRIANGLE_STRIP,
    /**
     * Draws a set of connected triangles that share one central vertex
     */
    TRIANGLE_FAN            = GL_TRIANGLE_FAN
};

/**
 * The usage pattern of the data store
 */
enum class Usage : uint16_t
{
    /**
     * Contents of the buffer are likely to be used often and not change often. Contents are written to the buffer, but not read.
     */
    STATIC = GL_STATIC_DRAW,
    /**
     * Contents of the buffer are likely to be used often and change often. Contents are written to the buffer, but not read.
     */
    DYNAMIC = GL_DYNAMIC_DRAW,
    /**
     * Contents of the buffer are likely to not be used often. Contents are written to the buffer, but not read.
     */
    STREAM = GL_STREAM_DRAW
};

/**
 * The size of the index buffer object's data store.
 */
enum class IndexFormat : uint16_t
{
    /**
     * 8 bit per index
     */
    UINT8 = GL_UNSIGNED_BYTE,
    /**
     * 16 bit per index
     */
    UINT16 = GL_UNSIGNED_SHORT,
    /**
     * 32 bit per index
     */
    UINT32 = GL_UNSIGNED_INT// (OES_element_index_uint)
};

// vertex attribute semantic

extern const char* ATTRIB_NAME_POSITION;
extern const char* ATTRIB_NAME_NORMAL;
extern const char* ATTRIB_NAME_TANGENT;
extern const char* ATTRIB_NAME_BITANGENT;
extern const char* ATTRIB_NAME_WEIGHTS;
extern const char* ATTRIB_NAME_JOINTS;
extern const char* ATTRIB_NAME_COLOR;
extern const char* ATTRIB_NAME_COLOR0;
extern const char* ATTRIB_NAME_COLOR1;
extern const char* ATTRIB_NAME_UV;
extern const char* ATTRIB_NAME_UV0;
extern const char* ATTRIB_NAME_UV1;
extern const char* ATTRIB_NAME_UV2;
extern const char* ATTRIB_NAME_UV3;
extern const char* ATTRIB_NAME_UV4;
extern const char* ATTRIB_NAME_UV5;
extern const char* ATTRIB_NAME_UV6;
extern const char* ATTRIB_NAME_UV7;

/**
 * Data type of each component in vertex attribute
 */
enum class AttribType : uint16_t
{
    /** Invalid type */
    INVALID = 0,
    /** Signed 8-bit integer */
    INT8 = GL_BYTE,
    /** Unsigned 8-bit integer */
    UINT8 = GL_UNSIGNED_BYTE,
    /** Signed 16-bit integer */
    INT16 = GL_SHORT,
    /** Unsigned 16-bit integer */
    UINT16 = GL_UNSIGNED_SHORT,
    /** Signed 32-bit integer */
    INT32 = GL_INT,
    /** Unsigned 32-bit integer */
    UINT32 = GL_UNSIGNED_INT,
    /** 32-bit float point number */
    FLOAT32 = GL_FLOAT
};

/**
 * Data type of each component in uniform
 */
enum class UniformElementType : uint8_t
{
    /** Float point number */
    FLOAT,
    /** Integer */
    INT
};

/**
 * Projection type of camera
 */
enum class ProjectionType : uint8_t
{
    /**
     * Perspective projection
     */
    PERSPECTIVE,
    /**
     * Orthographic projection
     */
    ORTHOGRAPHIC
};

/**
 * Define type in shader template
 */
struct Define
{
    /**
     * The define key
     */
    std::string key;
    /**
     * The define value
     */
    Value value;
};
typedef std::vector<Define> DefineVector;

struct Rect
{
    Rect();
    Rect(float x, float y, float w, float h);
    
    void set(float x, float y, float w, float h);
    
    float x = 0;
    float y = 0;
    float w = 0;
    float h = 0;

    static Rect ZERO;
};

// end of base group
/// @}

RENDERER_END
