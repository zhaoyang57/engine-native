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

#include "Effect.h"
#include "Config.h"

RENDERER_BEGIN

std::map<std::string,std::size_t> Effect::_defineBitOrder;
std::vector<std::string> Effect::_sharedDefineList;

Effect::Effect()
: _hash(0)
, _definesKey(0)
{}

void Effect::init(const Vector<Technique*>& techniques,
               const std::unordered_map<std::string, Property>& properties,
               const std::vector<ValueMap>& defineTemplates)
{
    _techniques = techniques;
    _properties = properties;
    _defineTemplates = defineTemplates;
    
    for (const auto defineTemplate: _defineTemplates)
        _cachedNameValues.emplace(defineTemplate.at("name").asString(),
                                  defineTemplate.at("value"));
    generateKey();
}

Effect::~Effect()
{
//    RENDERER_LOGD("Effect destruction: %p", this);
    clear();
}

void Effect::clear()
{
    _techniques.clear();
    _defineTemplates.clear();
}

Technique* Effect::getTechnique(const std::string& stage) const
{
    int stageID = Config::getStageID(stage);
    if (-1 == stageID)
        return nullptr;
    
    for (const auto& tech : _techniques)
    {
        if (tech->getStageIDs() & stageID)
            return tech;
    }
    
    return nullptr;
}

Value Effect::getDefineValue(const std::string& name) const
{
    for (const auto& def : _defineTemplates)
    {
        if (name == def.at("name").asString())
            return def.at("value");
    }
    
    RENDERER_LOGW("Failed to set define %s, define not found.", name.c_str());
    return Value::Null;
}

void Effect::setDefineValue(const std::string& name, const Value& value)
{
    for (auto& def : _defineTemplates)
    {
        if (name == def.at("name").asString())
        {
            def["value"] = value;
            if (_cachedNameValues[name] != value)
            {
                _cachedNameValues[name] = value;
                generateKey();
            }
            return;
        }
    };
}

ValueMap* Effect::extractDefines()
{
    return &_cachedNameValues;
}

const Effect::Property& Effect::getProperty(const std::string& name) const
{
    static Property EMPTY_PROPERTY;
    if (_properties.end() == _properties.find(name))
        return EMPTY_PROPERTY;
    else
        return _properties.at(name);
}

void Effect::setProperty(const std::string& name, const Property& property)
{
    _properties[name] = property;
}

void Effect::_updateDefineBitOrder(const ValueMap& nameValues)
{
    for (auto& tmplDefs : nameValues)
    {
        if (_defineBitOrder.find(tmplDefs.first) == _defineBitOrder.end())
        {
            _sharedDefineList.push_back(tmplDefs.first);
            _defineBitOrder[tmplDefs.first] = _sharedDefineList.size();
        }
    }
}

void Effect::generateKey()
{
    // Update global order when has new define.
    _updateDefineBitOrder(_cachedNameValues);
    
    _definesKey = 0;
    for (auto& tmplDefs : _cachedNameValues) {
        uint32_t value = tmplDefs.second.asUnsignedInt();
        CCASSERT(value <= 1,"Define value can't greater than 1");
        value <<= _defineBitOrder[tmplDefs.first];
        _definesKey |= value;
    }
    
    // Reserve 8 bit for the OR operation with program id in ProgramLib.
    _definesKey <<= 8;
}

void Effect::copy(Effect& effect)
{
    _hash = effect._hash;
    auto& otherTech = effect._techniques;
    for (auto it = otherTech.begin(); it != otherTech.end(); it ++)
    {
        auto tech = new Technique();
        tech->autorelease();
        tech->copy(**it);
        _techniques.pushBack(tech);
    }
    _defineTemplates = effect._defineTemplates;
    _cachedNameValues = effect._cachedNameValues;
    _properties = effect._properties;
    _definesKey = effect._definesKey;
}

RENDERER_END
