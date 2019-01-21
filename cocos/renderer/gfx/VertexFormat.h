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
#include <vector>
#include <unordered_map>

#include "base/CCRef.h"
#include "../Types.h"

RENDERER_BEGIN

/**
 * @addtogroup gfx
 * @{
 */

/**
 * The vertex format defines the attributes and their data layout in the VertexBuffer\n
 * JS API: gfx.VertexFormat
 @code
 let vertexFmt = new gfx.VertexFormat([
     { name: gfx.ATTR_POSITION, type: gfx.ATTR_TYPE_FLOAT32, num: 3 },
     { name: gfx.ATTR_UV0, type: gfx.ATTR_TYPE_FLOAT32, num: 2 },
     { name: gfx.ATTR_COLOR, type: gfx.ATTR_TYPE_FLOAT32, num: 4, normalize: true },
 ]);
 @endcode
 */
class VertexFormat : public Ref
{
public:
    /*
     * Informations used to define an attribute in vertex data layout.
     * @struct Info
     */
    struct Info
    {
        /*
         * Constructor
         * @param[in] name Attribute name
         * @param[in] type Data type of each component
         * @param[in] num Number of components per attribute
         * @param[in] normalized Whether integer data values should be normalized into a certain range when being casted to a float
         */
        Info(const std::string& name, AttribType type, uint32_t num, bool normalize = false)
        : _name(name)
        , _num(num)
        , _type(type)
        , _normalize(normalize)
        {
        }
        std::string _name;
        uint32_t _num;
        AttribType _type;
        bool _normalize;
    };

    static Info INFO_END;

    /*
     * Element describes informations of an attribute
     * @struct Info
     */
    struct Element
    {
        Element()
        : offset(0)
        , stride(0)
        , stream(-1)
        , num(0)
        , bytes(0)
        , type(AttribType::INVALID)
        , normalize(false)
        {}

        inline bool isValid() const
        {
            return type != AttribType::INVALID;
        }

        /*
         * Name of the attribute
         */
        std::string name;
        /*
         * Byte offset in each vertex data
         */
        size_t offset;
        /*
         * Specifies the offset in bytes between the beginning of consecutive vertex attributes
         */
        uint32_t stride;
        int32_t stream;
        /*
         * Number of components per attribute unit
         */
        uint32_t num;
        /*
         * Total bytes per attribute unit
         */
        uint32_t bytes;
        /*
         * Data type of each component
         */
        AttribType type;
        /*
         * Specifies whether integer data values should be normalized into a certain range when being casted to a float
         */
        bool normalize;
    };

    /*
     * Default constructor
     */
    VertexFormat();
    /**
     * Constructor with specific attribute informations
     * @param[in] infos Array of all elements informations
     */
    VertexFormat(const std::vector<Info>& infos);
    /*
     * Copy constructor
     */
    VertexFormat(const VertexFormat& o);
    /*
     * Move constructor
     */
    VertexFormat(VertexFormat&& o);

    VertexFormat& operator=(const VertexFormat& o);
    VertexFormat& operator=(VertexFormat&& o);

    /**
     * Gets all attribute names
     */
    const std::vector<std::string>& getAttributeNames() const { return _names; };
    /**
     * Getes an attribute element by name
     */
    const Element* getElement(const std::string& attrName) const;
    
    /**
     * Gets total byte size of a vertex
     */
    uint32_t getBytes() const { return _bytes; };
    
    /*
     * Builtin VertexFormat with 2d position, uv, color attributes
     */
    static VertexFormat* XY_UV_Color;
    /*
     * Builtin VertexFormat with 2d position, color attributes
     */
    static VertexFormat* XY_Color;

private:
    std::vector<std::string> _names;
    std::unordered_map<std::string, Element> _attr2el;
#if GFX_DEBUG > 0
    std::vector<Element> _elements;
#endif
    uint32_t _bytes;

    friend class VertexBuffer;
};

// end of gfx group
/// @}

RENDERER_END
