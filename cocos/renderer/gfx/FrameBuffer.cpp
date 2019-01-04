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

#include "FrameBuffer.h"
#include "RenderTarget.h"
#include "base/CCGLUtils.h"

RENDERER_BEGIN

FrameBuffer::FrameBuffer()
: _device(nullptr)
, _depthBuffer(nullptr)
, _stencilBuffer(nullptr)
, _depthStencilBuffer(nullptr)
{

}

FrameBuffer::~FrameBuffer()
{
    destroy();
}

void FrameBuffer::destroy()
{
    for (auto colorBufffer : _colorBuffers)
        RENDERER_SAFE_RELEASE(colorBufffer);
    
    RENDERER_SAFE_RELEASE(_depthBuffer);
    RENDERER_SAFE_RELEASE(_stencilBuffer);
    RENDERER_SAFE_RELEASE(_depthStencilBuffer);
    
    if (_glID == 0)
    {
        RENDERER_LOGE("The frame-buffer is invalid!");
        return;
    }
    
    glDeleteFramebuffers(1, &_glID);
    _glID = 0;
}

bool FrameBuffer::init(DeviceGraphics* device, uint16_t width, uint16_t height)
{
    _device = device;
    _width = width;
    _height = height;

    glGenFramebuffers(1, &_glID);

    return true;
}

void FrameBuffer::setColorBuffer(RenderTarget* rt, int index)
{
    if (index >= _colorBuffers.size())
        _colorBuffers.resize(index + 1);
    
    RENDERER_SAFE_RETAIN(rt);
    RENDERER_SAFE_RELEASE(_colorBuffers[index]);
    _colorBuffers[index] = rt;
}

void FrameBuffer::setColorBuffers(const std::vector<RenderTarget*>& renderTargets)
{
    for (auto& colorBufffer : _colorBuffers)
        RENDERER_SAFE_RELEASE(colorBufffer);

    _colorBuffers = renderTargets;
    for (auto& colorBufffer : _colorBuffers)
        RENDERER_SAFE_RETAIN(colorBufffer);
}

void FrameBuffer::setDepthBuffer(RenderTarget* rt)
{
    RENDERER_SAFE_RETAIN(rt);
    RENDERER_SAFE_RELEASE(_depthBuffer);
    _depthBuffer = rt;
}

void FrameBuffer::setStencilBuffer(RenderTarget* rt)
{
    RENDERER_SAFE_RETAIN(rt);
    RENDERER_SAFE_RELEASE(_stencilBuffer);
    _stencilBuffer = rt;
}

void FrameBuffer::setDepthStencilBuffer(RenderTarget* rt)
{
    RENDERER_SAFE_RETAIN(rt);
    RENDERER_SAFE_RELEASE(_depthStencilBuffer);
    _depthStencilBuffer = rt;
}

const std::vector<RenderTarget*>& FrameBuffer::getColorBuffers() const
{
    return _colorBuffers;
}

const RenderTarget* FrameBuffer::getDepthBuffer() const
{
    return _depthBuffer;
}

const RenderTarget* FrameBuffer::getStencilBuffer() const
{
    return _stencilBuffer;
}

const RenderTarget* FrameBuffer::getDepthStencilBuffer() const
{
    return _depthStencilBuffer;
}

RENDERER_END


