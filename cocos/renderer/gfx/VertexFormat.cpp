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

#include "VertexFormat.h"

RENDERER_BEGIN

static uint32_t attrTypeBytes(AttribType attrType)
{
    if (attrType == AttribType::INT8) {
        return 1;
    } else if (attrType == AttribType::UINT8) {
        return 1;
    } else if (attrType == AttribType::INT16) {
        return 2;
    } else if (attrType == AttribType::UINT16) {
        return 2;
    } else if (attrType == AttribType::INT32) {
        return 4;
    } else if (attrType == AttribType::UINT32) {
        return 4;
    } else if (attrType == AttribType::FLOAT32) {
        return 4;
    }

    RENDERER_LOGW("Unknown ATTR_TYPE: %u", (uint32_t)attrType);
    return 0;
}

VertexFormat* VertexFormat::XY_UV_Color = new VertexFormat(std::vector<Info>({
    Info(ATTRIB_NAME_POSITION, AttribType::FLOAT32, 2),
    Info(ATTRIB_NAME_UV0, AttribType::FLOAT32, 2),
    Info(ATTRIB_NAME_COLOR, AttribType::UINT8, 4, true)
}));

VertexFormat* VertexFormat::XY_Color = new VertexFormat(std::vector<Info>({
    Info(ATTRIB_NAME_POSITION, AttribType::FLOAT32, 2),
    Info(ATTRIB_NAME_COLOR, AttribType::UINT8, 4, true)
}));

VertexFormat::VertexFormat()
{
}

VertexFormat::VertexFormat(const std::vector<Info>& infos)
{
    _bytes = 0;
#if GFX_DEBUG > 0
    std::vector<Element*> _elements;
    std::vector<Element*>& elements = _elements;
#else
    std::vector<Element*> elements;
#endif

    uint32_t offset = 0;
    for (size_t i = 0, len = infos.size(); i < len; ++i)
    {
        const auto& info = infos[i];
        Element el;
        el.name = info._name;
        el.offset = offset;
        el.stride = 0;
        el.stream = -1;
        el.type = info._type;
        el.num = info._num;
        el.normalize = info._normalize;
        el.bytes = info._num * attrTypeBytes(info._type);

        _names.push_back(el.name);
        _attr2el[el.name] = el;
        elements.push_back(&_attr2el[el.name]);

        _bytes += el.bytes;
        offset += el.bytes;
    }

    for (size_t i = 0, len = elements.size(); i < len; ++i)
    {
        auto& el = elements[i];
        el->stride = _bytes;
    }
}

VertexFormat::VertexFormat(const VertexFormat& o)
{
    if (this != &o)
        *this = o;
}

VertexFormat::VertexFormat(VertexFormat&& o)
{
    if (this != &o)
        *this = std::move(o);
}

VertexFormat& VertexFormat::operator=(const VertexFormat& o)
{
    if (this != &o)
    {
        _names = o._names;
        _attr2el = o._attr2el;
#if GFX_DEBUG > 0
        _elements = o._elements;
        _bytes = o._bytes;
#endif
    }
    return *this;
}

VertexFormat& VertexFormat::operator=(VertexFormat&& o)
{
    if (this != &o)
    {
        _names = std::move(o._names);
        _attr2el = std::move(o._attr2el);
#if GFX_DEBUG > 0
        _elements = std::move(o._elements);
        _bytes = o._bytes;
        o._bytes = 0;
#endif
    }
    return *this;
}

const VertexFormat::Element* VertexFormat::getElement(const std::string& attrName) const
{
    static const Element* INVALID_ELEMENT_VALUE;
    const auto& iter = _attr2el.find(attrName);
    if (iter != _attr2el.end())
    {
        return &iter->second;
    }
    return INVALID_ELEMENT_VALUE;
}

RENDERER_END
