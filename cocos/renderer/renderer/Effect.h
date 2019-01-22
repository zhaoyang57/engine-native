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

#include <vector>
#include <unordered_map>
#include <map>
#include "base/CCRef.h"
#include "base/CCValue.h"
#include "../Macro.h"
#include "Technique.h"

RENDERER_BEGIN

/**
 * @addtogroup renderer
 * @{
 */

/**
 * @brief Fundamental class of material system, contains techniques, shader template define settings and uniform properties.\n
 * JS API: renderer.Effect
 * @code
 * let pass = new renderer.Pass('sprite');
 * pass.setDepth(false, false);
 * pass.setCullMode(gfx.CULL_NONE);
 * let mainTech = new renderer.Technique(
 *     ['transparent'],
 *     [
 *         { name: 'texture', type: renderer.PARAM_TEXTURE_2D },
 *         { name: 'color', type: renderer.PARAM_COLOR4 }
 *     ],
 *     [
 *         pass
 *     ]
 * );
 * let effect = new renderer.Effect(
 *     [
 *         mainTech
 *     ],
 *     {
 *         'color': {r: 1, g: 1, b: 1, a: 1}
 *     },
 *     [
 *         { name: 'useTexture', value: true },
 *         { name: 'useModel', value: false },
 *         { name: 'alphaTest', value: false },
 *         { name: 'useColor', value: true }
 *     ]
 * );
 * @endcode
 */
class Effect : public Ref
{
public:
    
    using Property = Technique::Parameter;
    
    /*
     * @brief The default constructor.
     */
    Effect();
    /*
     *  @brief The default destructor.
     */
    ~Effect();
    
    /*
     * @brief Initialize with techniques, properties and define settings.
     * @param[in] techniques All techniques in an array
     * @param[in] properties All properties in a map
     * @param[in] defineTemplates All defines and their value in a map
     */
    void init(const Vector<Technique*>& techniques,
              const std::unordered_map<std::string, Property>& properties,
              const std::vector<ValueMap>& defineTemplates);
    /**
     *  @brief Clears techniques and define list.
     */
    void clear();
    
    /**
     *  @brief Gets technique by stage.
     */
    Technique* getTechnique(const std::string& stage) const;
    /*
     *  @brief Gets all techniques.
     */
    const Vector<Technique*>& getTechniques() const { return _techniques; }
    /**
     *  @brief Gets define property value by name.
     */
    Value getDefine(const std::string& name) const;
    /**
     *  @brief Sets a define's value.
     */
    void define(const std::string& name, const Value& value);
    /*
     *  @brief Gets all define values.
     */
    const std::vector<ValueMap>& getDefines() const { return _defineTemplates; }
    /*
     *  @brief Extracts all defines.
     */
    ValueMap* extractDefines();
    
    /**
     *  @brief Gets uniform property value by name.
     */
    const Property& getProperty(const std::string& name) const;
    /**
     *  @brief Sets uniform property value by name.
     */
    void setProperty(const std::string& name, const Property& property);
    /*
     *  @brief Gets all uniform properties.
     */
    const std::unordered_map<std::string, Property>& getProperties() const { return _properties; }
    /**
     *  @brief Updates hash.
     */
    void updateHash(double hash) { _hash = hash; };
    /**
     *  @brief Gets hash.
     */
    double getHash() const { return _hash; };

    /*
     *  @brief Gets the define key for the current define settings.
     */
    const int32_t& getDefinesKey() { return _definesKey; };
    /**
     *  @brief Deep copy from other effect.
     */
    void copy(Effect& effect);

private:
    double _hash;
    int32_t _definesKey;
    Vector<Technique*> _techniques;
    std::vector<ValueMap> _defineTemplates;
    ValueMap _cachedNameValues;
    std::unordered_map<std::string, Property> _properties;
    
    // Global define order.
    static std::map<std::string,std::size_t> _defineBitOrder;
    static std::vector<std::string> _sharedDefineList;
    static void _updateDefineBitOrder(const ValueMap& nameValues);
    
    void generateKey();
};

// end of renderer group
/// @}

RENDERER_END
