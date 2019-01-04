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

#include "State.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture2D.h"
#include "Program.h"

RENDERER_BEGIN

namespace {
    const State __defaultState;
}

State::State()
// blend
: blend(false)
, blendSeparation(false)
, blendColor(0xFFFFFFFF)
, blendEq(BlendOp::ADD)
, blendAlphaEq(BlendOp::ADD)
, blendSrc(BlendFactor::ONE)
, blendDst(BlendFactor::ZERO)
, blendSrcAlpha(BlendFactor::ONE)
, blendDstAlpha(BlendFactor::ZERO)
// depth
, depthTest(false)
, depthWrite(false)
, depthFunc(DepthFunc::LESS)
// stencil
, stencilTest(false)
, stencilSeparation(false)
, stencilFuncFront(StencilFunc::ALWAYS)
, stencilRefFront(0)
, stencilMaskFront(0xFF)
, stencilFailOpFront(StencilOp::KEEP)
, stencilZFailOpFront(StencilOp::KEEP)
, stencilZPassOpFront(StencilOp::KEEP)
, stencilWriteMaskFront(0xFF)
, stencilFuncBack(StencilFunc::ALWAYS)
, stencilRefBack(0)
, stencilMaskBack(0xFF)
, stencilFailOpBack(StencilOp::KEEP)
, stencilZFailOpBack(StencilOp::KEEP)
, stencilZPassOpBack(StencilOp::KEEP)
, stencilWriteMaskBack(0xFF)
// cull-mode
, cullMode(CullMode::BACK)

// primitive-type
, primitiveType(PrimitiveType::TRIANGLES)

// bindings
, maxStream(-1)
, _indexBuffer(nullptr)
, _program(nullptr)
{
}

State::State(const State& o)
{
    *this = o;
    
    _textureUnits.reserve(10);
}

State::State(State&& o)
{
    *this = std::move(o);
}

State::~State()
{
    for (auto vertexBuf : _vertexBuffers)
    {
        RENDERER_SAFE_RELEASE(vertexBuf);
    }

    RENDERER_SAFE_RELEASE(_indexBuffer);

    for (auto texture : _textureUnits)
    {
        RENDERER_SAFE_RELEASE(texture);
    }

    RENDERER_SAFE_RELEASE(_program);
}

State& State::operator=(const State& o)
{
    if (this != &o)
    {
        // blend
        blend = o.blend;
        blendSeparation = o.blendSeparation;
        blendColor = o.blendColor;
        blendEq = o.blendEq;
        blendAlphaEq = o.blendAlphaEq;
        blendSrc = o.blendSrc;
        blendDst = o.blendDst;
        blendSrcAlpha = o.blendSrcAlpha;
        blendDstAlpha = o.blendDstAlpha;
        // depth
        depthTest = o.depthTest;
        depthWrite = o.depthWrite;
        depthFunc = o.depthFunc;
        // stencil
        stencilTest = o.stencilTest;
        stencilSeparation = o.stencilSeparation;
        stencilFuncFront = o.stencilFuncFront;
        stencilRefFront = o.stencilRefFront;
        stencilMaskFront = o.stencilMaskFront;
        stencilFailOpFront = o.stencilFailOpFront;
        stencilZFailOpFront = o.stencilZFailOpFront;
        stencilZPassOpFront = o.stencilZPassOpFront;
        stencilWriteMaskFront = o.stencilWriteMaskFront;
        stencilFuncBack = o.stencilFuncBack;
        stencilRefBack = o.stencilRefBack;
        stencilMaskBack = o.stencilMaskBack;
        stencilFailOpBack = o.stencilFailOpBack;
        stencilZFailOpBack = o.stencilZFailOpBack;
        stencilZPassOpBack = o.stencilZPassOpBack;
        stencilWriteMaskBack = o.stencilWriteMaskBack;
        // cull-mode
        cullMode = o.cullMode;

        // primitive-type
        primitiveType = o.primitiveType;

        // bindings
        maxStream = o.maxStream;

        _vertexBufferOffsets = o._vertexBufferOffsets;

        for (auto vertexBuf : _vertexBuffers)
        {
            RENDERER_SAFE_RELEASE(vertexBuf);
        }

        if (o._vertexBuffers.empty())
        {
            _vertexBuffers.clear();
        }
        else
        {
            _vertexBuffers.resize(o._vertexBuffers.size());

            for (size_t i = 0, len = o._vertexBuffers.size(); i < len; ++i)
            {
                _vertexBuffers[i] = o._vertexBuffers[i];
                RENDERER_SAFE_RETAIN(_vertexBuffers[i]);
            }
        }

        if (_indexBuffer != o._indexBuffer)
        {
            RENDERER_SAFE_RETAIN(o._indexBuffer);
            RENDERER_SAFE_RELEASE(_indexBuffer);
            _indexBuffer = o._indexBuffer;
        }

        for (auto texture : _textureUnits)
        {
            RENDERER_SAFE_RELEASE(texture);
        }

        if (o._textureUnits.empty())
        {
            _textureUnits.clear();
        }
        else
        {
            _textureUnits.resize(o._textureUnits.size());
            for (size_t i = 0, len = o._textureUnits.size(); i < len; ++i)
            {
                _textureUnits[i] = o._textureUnits[i];
                RENDERER_SAFE_RETAIN(_textureUnits[i]);
            }
        }

        if (_program != o._program)
        {
            RENDERER_SAFE_RETAIN(o._program);
            RENDERER_SAFE_RELEASE(_program);
            _program = o._program;
        }
    }

    return *this;
}

State& State::operator=(State&& o)
{
    if (this != &o)
    {
        // blend
        blend = o.blend;
        blendSeparation = o.blendSeparation;
        blendColor = o.blendColor;
        blendEq = o.blendEq;
        blendAlphaEq = o.blendAlphaEq;
        blendSrc = o.blendSrc;
        blendDst = o.blendDst;
        blendSrcAlpha = o.blendSrcAlpha;
        blendDstAlpha = o.blendDstAlpha;
        // depth
        depthTest = o.depthTest;
        depthWrite = o.depthWrite;
        depthFunc = o.depthFunc;
        // stencil
        stencilTest = o.stencilTest;
        stencilSeparation = o.stencilSeparation;
        stencilFuncFront = o.stencilFuncFront;
        stencilRefFront = o.stencilRefFront;
        stencilMaskFront = o.stencilMaskFront;
        stencilFailOpFront = o.stencilFailOpFront;
        stencilZFailOpFront = o.stencilZFailOpFront;
        stencilZPassOpFront = o.stencilZPassOpFront;
        stencilWriteMaskFront = o.stencilWriteMaskFront;
        stencilFuncBack = o.stencilFuncBack;
        stencilRefBack = o.stencilRefBack;
        stencilMaskBack = o.stencilMaskBack;
        stencilFailOpBack = o.stencilFailOpBack;
        stencilZFailOpBack = o.stencilZFailOpBack;
        stencilZPassOpBack = o.stencilZPassOpBack;
        stencilWriteMaskBack = o.stencilWriteMaskBack;
        // cull-mode
        cullMode = o.cullMode;

        // primitive-type
        primitiveType = o.primitiveType;

        // bindings
        maxStream = o.maxStream;

        _vertexBufferOffsets = std::move(o._vertexBufferOffsets);

        for (auto vertexBuf : _vertexBuffers)
        {
            RENDERER_SAFE_RELEASE(vertexBuf);
        }
        _vertexBuffers = std::move(o._vertexBuffers);

        RENDERER_SAFE_RELEASE(_indexBuffer);
        _indexBuffer = o._indexBuffer;
        o._indexBuffer = nullptr;

        for (auto texture : _textureUnits)
        {
            RENDERER_SAFE_RELEASE(texture);
        }
        _textureUnits = std::move(o._textureUnits);

        RENDERER_SAFE_RELEASE(_program);
        _program = o._program;
        o._program = nullptr;

        // reset o
        o.blend = false;
        o.blendSeparation = false;
        o.blendColor = 0xFFFFFFFF;
        o.blendEq = BlendOp::ADD;
        o.blendAlphaEq = BlendOp::ADD;
        o.blendSrc = BlendFactor::ONE;
        o.blendDst = BlendFactor::ZERO;
        o.blendSrcAlpha = BlendFactor::ONE;
        o.blendDstAlpha = BlendFactor::ZERO;
        // depth
        o.depthTest = false;
        o.depthWrite = false;
        o.depthFunc = DepthFunc::LESS;
        // stencil
        o.stencilTest = false;
        o.stencilSeparation = false;
        o.stencilFuncFront = StencilFunc::ALWAYS;
        o.stencilRefFront = 0;
        o.stencilMaskFront = 0xFF;
        o.stencilFailOpFront = StencilOp::KEEP;
        o.stencilZFailOpFront = StencilOp::KEEP;
        o.stencilZPassOpFront = StencilOp::KEEP;
        o.stencilWriteMaskFront = 0xFF;
        o.stencilFuncBack = StencilFunc::ALWAYS;
        o.stencilRefBack = 0;
        o.stencilMaskBack = 0xFF;
        o.stencilFailOpBack = StencilOp::KEEP;
        o.stencilZFailOpBack = StencilOp::KEEP;
        o.stencilZPassOpBack = StencilOp::KEEP;
        o.stencilWriteMaskBack = 0xFF;
        // cull-mode
        o.cullMode = CullMode::BACK;

        // primitive-type
        o.primitiveType = PrimitiveType::TRIANGLES;

        // bindings
        o.maxStream = -1;
    }

    return *this;
}

void State::reset()
{
    *this = __defaultState;
}

void State::setVertexBuffer(size_t index, VertexBuffer* vertBuf)
{
    if (index >= _vertexBuffers.size())
    {
        _vertexBuffers.resize(index + 1);
    }

    VertexBuffer* oldBuf = _vertexBuffers[index];
    if (oldBuf != vertBuf)
    {
        RENDERER_SAFE_RELEASE(oldBuf);
        _vertexBuffers[index] = vertBuf;
        RENDERER_SAFE_RETAIN(vertBuf);
    }
}

VertexBuffer* State::getVertexBuffer(size_t index) const
{
    if (_vertexBuffers.empty())
        return nullptr;
    assert(index < _vertexBuffers.size());
    return _vertexBuffers[index];
}

void State::setVertexBufferOffset(size_t index, int32_t offset)
{
    if (index >= _vertexBufferOffsets.size())
    {
        _vertexBufferOffsets.resize(index + 1);
    }

    _vertexBufferOffsets[index] = offset;
}

int32_t State::getVertexBufferOffset(size_t index) const
{
    assert(index < _vertexBufferOffsets.size());
    return _vertexBufferOffsets[index];
}

void State::setIndexBuffer(IndexBuffer* indexBuf)
{
    if (_indexBuffer != indexBuf)
    {
        RENDERER_SAFE_RELEASE(_indexBuffer);
        _indexBuffer = indexBuf;
        RENDERER_SAFE_RETAIN(_indexBuffer);
    }
}

IndexBuffer* State::getIndexBuffer() const
{
    return _indexBuffer;
}

void State::setTexture(size_t index, Texture* texture)
{
    if (index >= _textureUnits.size())
    {
        _textureUnits.resize(index + 1);
    }

    Texture* oldTexture = _textureUnits[index];
    if (oldTexture != texture)
    {
        RENDERER_SAFE_RELEASE(oldTexture);
        _textureUnits[index] = texture;
        RENDERER_SAFE_RETAIN(texture);
    }
}

Texture* State::getTexture(size_t index) const
{
    if (_textureUnits.empty())
        return nullptr;
    assert(index < _textureUnits.size());
    return _textureUnits[index];
}

void State::setProgram(Program* program)
{
    assert(program != nullptr);
    if (_program != program)
    {
        RENDERER_SAFE_RELEASE(_program);
        _program = program;
        RENDERER_SAFE_RETAIN(_program);
    }
}

Program* State::getProgram() const
{
    return _program;
}

RENDERER_END
