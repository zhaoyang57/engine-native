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

#include "MeshBuffer.hpp"
#include "../Types.h"
#include "ModelBatcher.hpp"
#include "RenderFlow.hpp"
#include "../gfx/DeviceGraphics.h"

#define MAX_VB_SIZE 1310700

RENDERER_BEGIN

MeshBuffer::MeshBuffer(ModelBatcher* batcher, VertexFormat* fmt)
: _byteStart(0)
, _byteOffset(0)
, _indexStart(0)
, _indexOffset(0)
, _vertexStart(0)
, _vertexOffset(0)
, _vDataCount(0)
, _iDataCount(0)
, _dirty(false)
, _vertexFmt(fmt)
, _batcher(batcher)
, _vbPos(0)
{
    _bytesPerVertex = _vertexFmt->getBytes();
    
    DeviceGraphics* device = _batcher->getFlow()->getDevice();
    _vb = VertexBuffer::create(device, _vertexFmt, Usage::DYNAMIC, nullptr, 0, 0);
    _vbArr.pushBack(_vb);
    
    _ib = IndexBuffer::create(device, IndexFormat::UINT16, Usage::STATIC, nullptr, 0, 0);
    _ib->retain();
    
    _vDataCount = MeshBuffer::INIT_VERTEX_COUNT * 4 * _bytesPerVertex / 4;
    _iDataCount = MeshBuffer::INIT_VERTEX_COUNT * 6;
    
    reallocBuffers();
}

MeshBuffer::~MeshBuffer()
{
    _ib->release();
}

void MeshBuffer::reallocBuffers()
{
    vData.resize(_vDataCount);
    _vb->setBytes(_vDataCount * VDATA_BYTE);
    iData.resize(_iDataCount);
    _ib->setBytes(_iDataCount * IDATA_BYTE);
}

bool MeshBuffer::request(uint32_t vertexCount, uint32_t indexCount, OffsetInfo* offset)
{
    if (_batcher->getCurrentBuffer() != this)
    {
        _batcher->flush();
        _batcher->setCurrentBuffer(this);
    }
    offset->vByte = _byteOffset;
    offset->index = _indexOffset;
    offset->vertex = _vertexOffset;
    return requestStatic(vertexCount, indexCount, offset);
}

bool MeshBuffer::requestStatic(uint32_t vertexCount, uint32_t indexCount, OffsetInfo* offset)
{
    uint32_t byteOffset = _byteOffset + vertexCount * _bytesPerVertex;
    if (MAX_VB_SIZE < byteOffset) {
        // Finish pre data.
        _batcher->flush();
        _vb->update(0, vData.data(), _byteOffset);
        
        // Prepare next data.
        _vbPos++;
        if (_vbPos >= _vbArr.size())
        {
            DeviceGraphics* device = _batcher->getFlow()->getDevice();
            _vb = VertexBuffer::create(device, _vertexFmt, Usage::DYNAMIC, nullptr, 0, 0);
            _vb->setBytes(_vDataCount * VDATA_BYTE);
            _vbArr.pushBack(_vb);
        }
        else
        {
            _vb = _vbArr.at(_vbPos);
        }
        
        _byteStart = 0;
        _byteOffset = 0;
        _vertexStart = 0;
        _vertexOffset = 0;
        
        offset->vByte = 0;
        offset->vertex = 0;
        
        byteOffset = vertexCount * _bytesPerVertex;
    }
    
    uint32_t indexOffset = _indexOffset + indexCount;
    uint32_t vBytes = _vDataCount * VDATA_BYTE;
    
    if (byteOffset > vBytes)
    {
        while (vBytes < byteOffset)
        {
            _vDataCount *= 2;
            vBytes = _vDataCount * VDATA_BYTE;
        }
        
        vData.resize(_vDataCount);
        _vb->setBytes(vBytes);
    }
    
    if (indexOffset > _iDataCount)
    {
        while (_iDataCount < indexOffset)
        {
            _iDataCount *= 2;
        }
        
        iData.resize(_iDataCount);
        _ib->setBytes(_iDataCount * IDATA_BYTE);
    }
    
    _vertexOffset += vertexCount;
    _indexOffset += indexCount;
    _byteOffset = byteOffset;
    _dirty = true;
    return true;
}

void MeshBuffer::uploadData()
{
    _vb->update(0, vData.data(), _byteOffset);
    _ib->update(0, iData.data(), _indexOffset * IDATA_BYTE);
    _dirty = false;
}

void MeshBuffer::reset()
{
    _vbPos = 0;
    _vb = _vbArr.at(0);
    _byteStart = 0;
    _byteOffset = 0;
    _vertexStart = 0;
    _vertexOffset = 0;
    _indexStart = 0;
    _indexOffset = 0;
    _dirty = false;
}

void MeshBuffer::destroy()
{
    _vb->destroy();
    _ib->destroy();
    iData.clear();
    vData.clear();
}

RENDERER_END
