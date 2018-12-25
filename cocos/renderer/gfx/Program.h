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

#include "../Macro.h"
#include "../Types.h"

#include "GraphicsHandle.h"

#include <string>
#include <vector>

RENDERER_BEGIN

/**
 * @addtogroup gfx
 * @{
 */

class DeviceGraphics;

/**
 * Program class manages the internal GL shader program.
 */
class Program final: public GraphicsHandle
{
public:
    /**
     * Describes vertex attribute informations used in the program
     * @struct Attribute
     */
    struct Attribute
    {
        /**
         * Attribute name
         */
        std::string name;
        /**
         * Number of components per attribute
         */
        GLsizei size;
        /**
         * Attribute location
         */
        GLuint location;
        /**
         * Attribute type
         */
        GLenum type;
    };

    /**
     * Describes uniform informations used in the program
     * @struct Uniform
     */
    struct Uniform
    {
        /**
         * Uniform name
         */
        std::string name;
        /**
         * The length of the array for uniforms declared as arrays, default value is 1
         */
        GLsizei size;
        /**
         * Uniform location
         */
        GLint location;
        /**
         * Uniform type
         */
        GLenum type;
        /**
         * Sets the uniform value
         */
        void setUniform(const void* value, UniformElementType elementType) const;
        /**
         * Sets the callback which will be called when uniform updated
         */
        using SetUniformCallback = void (*)(GLint, GLsizei, const void*, UniformElementType); // location, count, value, elementType
    private:
        SetUniformCallback _callback;
        friend class Program;
    };

    /**
     * Creates a Program with device and shader sources
     */
    RENDERER_DEFINE_CREATE_METHOD_3(Program, init, DeviceGraphics*, const char*, const char*)
    Program();
    virtual ~Program();

    /**
     * Initializes a Program with device and program sources
     * @param[in] device DeviceGraphics pointer
     * @param[in] vertSource Vertex shader program
     * @param[in] fragSource Fragment shader program
     */
    bool init(DeviceGraphics* device, const char* vertSource, const char* fragSource);
    /**
     * Gets the id of program
     */
    inline uint32_t getID() const { return _id; }
    /**
     * Gets the attibutes used in the program
     */
    inline const std::vector<Attribute>& getAttributes() const { return _attributes; }
    /**
     * Gets the uniforms used in the program
     */
    inline const std::vector<Uniform>& getUniforms() const { return _uniforms; }
    /**
     * Indicates whether the program is successfully linked
     */
    inline bool isLinked() const { return _linked; }
    /**
     * Link the program with its shader sources
     */
    void link();
private:
    DeviceGraphics* _device;
    std::vector<Attribute> _attributes;
    std::vector<Uniform> _uniforms;
    std::string _vertSource;
    std::string _fragSource;
    uint32_t _id;
    bool _linked;
};

// end of gfx group
/// @}

RENDERER_END
