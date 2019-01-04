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

#include "RendererUtils.h"

#include "gfx/VertexFormat.h"
#include "gfx/VertexBuffer.h"
#include "gfx/IndexBuffer.h"

#include "InputAssembler.h"

RENDERER_BEGIN

InputAssembler* createIA(DeviceGraphics* device, const IAData& data)
{
    if (data.positions.empty())
    {
        RENDERER_LOGD("The data must have positions field!");
        return nullptr;
    }

    std::vector<float> verts;
    verts.reserve(data.positions.size() + data.normals.size() + data.uvs.size());
    size_t vcount = data.positions.size() / 3;

    for (size_t i = 0; i < vcount; ++i) {
        verts.push_back(data.positions[3 * i]);
        verts.push_back(data.positions[3 * i + 1]);
        verts.push_back(data.positions[3 * i + 2]);

        if (!data.normals.empty()) {
            verts.push_back(data.normals[3 * i]);
            verts.push_back(data.normals[3 * i + 1]);
            verts.push_back(data.normals[3 * i + 2]);
        }

        if (!data.uvs.empty()) {
            verts.push_back(data.uvs[2 * i]);
            verts.push_back(data.uvs[2 * i + 1]);
        }
    }

    std::vector<VertexFormat::Info> vfmt;
    vfmt.push_back({ ATTRIB_NAME_POSITION, AttribType::FLOAT32, 3});

    if (!data.normals.empty())
        vfmt.push_back({ ATTRIB_NAME_NORMAL, AttribType::FLOAT32, 3});

    if (!data.uvs.empty())
        vfmt.push_back({ ATTRIB_NAME_UV0, AttribType::FLOAT32, 2});

    VertexFormat* fmt = new VertexFormat(vfmt);
    auto vb = new VertexBuffer();
    vb->init(device, fmt, Usage::STATIC, verts.data(), verts.size() * sizeof(float), (uint32_t)vcount);
    fmt->release();

    IndexBuffer* ib = nullptr;

    if (!data.indices.empty())
    {
        ib = new IndexBuffer();
        ib->init(device, IndexFormat::UINT16, Usage::STATIC, data.indices.data(), data.indices.size() * sizeof(uint16_t), (uint32_t)data.indices.size());
    }

    auto ia = new InputAssembler();
    ia->init(vb, ib);
    return ia;
}

RENDERER_END
