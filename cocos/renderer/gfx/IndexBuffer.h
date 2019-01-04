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

#include <functional>
#include "../Macro.h"
#include "../Types.h"
#include "GraphicsHandle.h"

RENDERER_BEGIN

/**
 * @addtogroup gfx
 * @{
 */

class DeviceGraphics;

/**
 * IndexBuffer manages the GL buffer for element indices.
 * It creates, updates and destoies the GL buffer.
 */
class IndexBuffer final : public GraphicsHandle
{
public:
    /**
     * Creates an IndexBuffer object
     * @param[in] device DeviceGraphics pointer.
     * @param[in] format Index element format.
     * @param[in] usage The usage pattern of the index buffer.
     * @param[in] data Data pointer, could be nullptr, then nothing is updated to the buffer.
     * @param[in] dataByteLength Data's byte length.
     * @param[in] numIndices Count of indices.
     */
    RENDERER_DEFINE_CREATE_METHOD_6(IndexBuffer, init, DeviceGraphics*, IndexFormat, Usage, const void*, size_t, uint32_t)

    IndexBuffer();
    virtual ~IndexBuffer();

    /**
     * Initializes the index buffer with necessary options.
     * @param[in] device DeviceGraphics pointer.
     * @param[in] format Index element format.
     * @param[in] usage The usage pattern of the index buffer.
     * @param[in] data Data pointer, could be nullptr, then nothing is updated to the buffer.
     * @param[in] dataByteLength Data's byte length.
     * @param[in] numIndices Count of indices.
     */
    bool init(DeviceGraphics* device, IndexFormat format, Usage usage, const void* data, size_t dataByteLength, uint32_t numIndices);
    /**
     * Update data to the GL index buffer
     * @param[in] offset Destination offset for filling buffer data.
     * @param[in] data Data to be updated.
     * @param[in] dataByteLength Data byte length to be updated.
     */
    void update(uint32_t offset, const void* data, size_t dataByteLength);

    /**
     * Gets the count of indices.
     */
    inline uint32_t getCount() const { return _numIndices; }
    /**
     * Sets the count of indices.
     */
    inline void setCount(uint32_t numIndices) { _numIndices = numIndices; }

    /**
     * Gets the format of indices.
     */
    inline IndexFormat getFormat() const { return _format; }
    /**
     * Sets the format of indices.
     */
    inline void setFormat(IndexFormat format) { _format = format; }

    /**
     * Gets bytes occupied by each index.
     */
    inline uint32_t getBytesPerIndex() const { return _bytesPerIndex; }
    /**
     * Sets bytes occupied by each index.
     */
    inline void setBytesPerIndex(uint32_t bytesPerIndex) {_bytesPerIndex = bytesPerIndex; }

    /**
     * Gets usage pattern of the index buffer
     */
    inline Usage getUsage() const { return _usage; }
    /**
     * Sets usage pattern of the index buffer
     */
    inline void setUsage(Usage usage) { _usage = usage; }

    /**
     * Gets byte size of the index buffer
     */
    inline uint32_t getBytes() const { return _bytes; }
    /**
     * Sets byte size of the index buffer
     */
    inline void setBytes(uint32_t bytes) { _bytes = bytes; }

    using FetchDataCallback = std::function<uint8_t*(size_t*)>;
    void setFetchDataCallback(const FetchDataCallback& cb) { _fetchDataCallback = cb; }
    uint8_t* invokeFetchDataCallback(size_t* bytes) {
        if (_fetchDataCallback == nullptr)
        {
            *bytes = 0;
            return nullptr;
        }
        return _fetchDataCallback(bytes);
    }

    /**
     * Destroies the index buffer
     */
    void destroy();
    
private:
    DeviceGraphics* _device;
    IndexFormat _format;
    Usage _usage;
    uint32_t _numIndices;
    uint32_t _bytesPerIndex;
    uint32_t _bytes;

    FetchDataCallback _fetchDataCallback;

    CC_DISALLOW_COPY_ASSIGN_AND_MOVE(IndexBuffer)
};

// end of gfx group
/// @}

RENDERER_END
