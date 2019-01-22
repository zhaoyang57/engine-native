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
#include "platform/CCGL.h"
#include "base/CCVector.h"
#include "base/CCRef.h"
#include "../Macro.h"

RENDERER_BEGIN

class Pass;
class Texture;

/**
 * @addtogroup renderer
 * @{
 */

/**
 * @brief Technique is a important part of Effect, it contains a selective uniform parameters and all passes of effect.\n
 * JS API: renderer.Technique
 * @code
 * let pass = new renderer.Pass('sprite');
 * pass.setDepth(false, false);
 * pass.setCullMode(gfx.CULL_NONE);
 * let technique = new renderer.Technique(
 *     ['transparent'],
 *     [
 *         { name: 'texture', type: renderer.PARAM_TEXTURE_2D },
 *         { name: 'color', type: renderer.PARAM_COLOR4 }
 *     ],
 *     [
 *         pass
 *     ]
 * );
 * @endcode
 */
class Technique : public Ref
{
public:
    
    /*
     *  @brief Uniform parameter of Technique, defines the uniform name and type
     */
    class Parameter final
    {
    public:
        enum class Type : uint8_t
        {
            INT = 0,
            INT2,
            INT3,
            INT4,
            FLOAT,
            FLOAT2,
            FLOAT3,
            FLOAT4,
            COLOR3,
            COLOR4,
            MAT2,
            MAT3,
            MAT4,
            TEXTURE_2D,
            TEXTURE_CUBE,
            UNKNOWN
        };
        
        // How many elements of each type, for example:
        // INT -> 1
        // INT2 -> 2
        // MAT4 -> 16
        static uint8_t getElements(Type type);
        
        /*
         *  @brief The default constructor.
         */
        Parameter();
        /*
         *  @brief Constructor with integer.
         */
        Parameter(const std::string& name, Type type, int* value, uint8_t count = 1);
        /*
         *  @brief Constructor with float.
         */
        Parameter(const std::string& name, Type type, float* value, uint8_t count = 1);
        /*
         *  @brief Constructor with texture.
         */
        Parameter(const std::string& name, Type type, Texture* texture);
        /*
         *  @brief Constructor with texture array.
         */
        Parameter(const std::string& name, Type type, const std::vector<Texture*>& textures);
        Parameter(const std::string& name, Type type);
        Parameter(const Parameter& rh);
        Parameter(Parameter&& rh);
        ~Parameter();
        
        Parameter& operator=(const Parameter& rh);
        
        /*
         *  @brief Gets the uniform type.
         */
        inline Type getType() const { return _type; }
        /*
         *  @brief Gets the uniform name.
         */
        inline const std::string& getName() const { return _name; }
        /*
         *  @brief Gets the counts of uniform components.
         */
        inline uint8_t getCount() const { return _count; }
        /*
         *  @brief Gets parameter value.
         */
        inline void* getValue() const { return _value; }
        /*
         *  @brief Gets bytes occupied by primitive uniform parameter.
         */
        inline uint16_t getBytes() const { return _bytes; };
        
        /*
         *  @brief Gets the texture array.
         */
        std::vector<Texture*> getTextureArray() const;
        /*
         *  @brief Sets the texture pointer.
         */
        void setTexture(Texture* texture);
        /*
         *  @brief Gets the texture pointer.
         */
        Texture* getTexture() const;
        
    private:
        static uint8_t elementsOfType[(int)Type::UNKNOWN + 1];
        
        void freeValue();
        void copyValue(const Parameter& rh);
        
        std::string _name = "";
        // how many elements, for example, how many INT2 or how many MAT2
        uint8_t _count = 0;
        Type _type = Type::UNKNOWN;
        void* _value = nullptr;
        
        // It is meaningful if type is not Texture2D or TEXTURE_CUBE.
        uint16_t _bytes = 0;
    };
    
    /**
     *  @brief Constructor.
     *  @param[in] stages All stages it belongs to.
     *  @param[in] parameters All uniform parameters.
     *  @param[in] passes All passes.
     */
    Technique(const std::vector<std::string>& stages,
              const std::vector<Parameter>& parameters,
              const Vector<Pass*>& passes,
              int layer = 0);
    
    /*
     *  @brief The default constructor.
     */
    Technique();
    /*
     *  @brief The default destructor.
     */
    ~Technique();
    
    /**
     *  @brief Sets the stages it belongs to.
     */
    void setStages(const std::vector<std::string>& stages);
    /*
     *  @brief Sets the pass to the given index.
     */
    void setPass(int index, Pass* pass);

    /*
     *  @brief Gets all uniform parameters.
     */
    const std::vector<Parameter>& getParameters() const { return _parameters; }
    /*
     *  @brief Gets all passes.
     */
    const Vector<Pass*>& getPasses() const { return _passes; }
    /*
     *  @brief Gets all stageID.
     */
    uint32_t getStageIDs() const { return _stageIDs; }
    /**
     *  @brief Deep copy from other techique.
     */
    void copy(const Technique& tech);
    
private:
    static uint32_t _genID;
    
    uint32_t _id = 0;
    uint32_t _stageIDs = 0;
    int _layer = 0;
    std::vector<Parameter> _parameters;
    Vector<Pass*> _passes;
};

// end of renderer group
/// @}

RENDERER_END
