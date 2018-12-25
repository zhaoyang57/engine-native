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

#include "VertexBuffer.h"
#include "DeviceGraphics.h"
#include "base/CCGLUtils.h"

RENDERER_BEGIN

VertexBuffer::VertexBuffer()
: _device(nullptr)
, _usage(Usage::STATIC)
, _numVertices(0)
, _bytes(0)
{

}

VertexBuffer::~VertexBuffer()
{
    destroy();
}

bool VertexBuffer::init(DeviceGraphics* device, VertexFormat* format, Usage usage, const void* data, size_t dataByteLength, uint32_t numVertices)
{
    _device = device;
    _format = format;
    CC_SAFE_RETAIN(_format);
    _usage = usage;
    _numVertices = numVertices;

    // calculate bytes
    _bytes = _format->_bytes * numVertices;

    // update
    glGenBuffers(1, &_glID);
    update(0, data, dataByteLength);

    // stats
    //REFINE:    device._stats.ib += _bytes;

    return true;
}

void VertexBuffer::setFormat(VertexFormat* format)
{
    if (_format == format)
        return;
    
    CC_SAFE_RELEASE(_format);
    _format = format;
    CC_SAFE_RETAIN(_format);
    
}

void VertexBuffer::update(uint32_t offset, const void* data, size_t dataByteLength)
{
    if (_glID == 0)
    {
        RENDERER_LOGE("The buffer is destroyed");
        return;
    }

    if (data && dataByteLength + offset > _bytes) {
        RENDERER_LOGE("Failed to update vertex buffer data, bytes exceed.");
        return;
    }

    GLenum glUsage = (GLenum)_usage;
    ccBindBuffer(GL_ARRAY_BUFFER, _glID);
    if (!data)
    {
        glBufferData(GL_ARRAY_BUFFER, _bytes, nullptr, glUsage);
    }
    else
    {
        if (offset > 0)
        {
            glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)offset, (GLsizeiptr)dataByteLength, (const GLvoid*)data);
        }
        else
        {
            glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)dataByteLength, data, glUsage);
        }
    }
    ccBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::destroy()
{
    if (_glID == 0)
        return;
    
    CC_SAFE_RELEASE_NULL(_format);
    
    ccDeleteBuffers(1, &_glID);
    //REFINE:    _device._stats.ib -= _bytes;
    
    _glID = 0;
}

#if GFX_DEBUG > 0
static void testVertexBuffer()
{
    VertexFormat vertexFmt({
        { ATTRIB_NAME_POSITION, AttribType::FLOAT32, 2 }
    });

    DeviceGraphics* device = nullptr;
    float vertex[] = {
        -1, 0, 0
        -1, 1, 1
    };
    VertexBuffer* buffer = new VertexBuffer();
    buffer->init(device, vertexFmt, Usage::STATIC, vertex, sizeof(vertex), 3);

    buffer->release();
}
#endif

RENDERER_END
