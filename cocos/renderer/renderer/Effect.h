/****************************************************************************
 Copyright (c) 2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#pragma once

#include <vector>
#include <unordered_map>
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
 *  @brief Fundamental class of material system, contains techniques, shader template define settings and uniform properties.
 */
class Effect : public Ref
{
public:
    
    using Property = Technique::Parameter;
    
    /**
     *  @brief The default constructor.
     */
    Effect();
    /**
     *  @brief The default destructor.
     */
    ~Effect();
    
    /**
     *  @brief Initialize with techniques, properties and define settings.
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
    /**
     *  @brief Gets all techniques.
     */
    const Vector<Technique*>& getTechniques() const { return _techniques; }
    /**
     *  @brief Gets define property value by name.
     */
    Value getDefineValue(const std::string& name) const;
    /**
     *  @brief Gets all define value.
     */
    const std::vector<ValueMap>& getDefines() const { return _defineTemplates; }
    /**
     *  @brief Sets define value.
     */
    void setDefineValue(const std::string& name, const Value& value);
    /**
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
    /**
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

    /**
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
    
    void generateKey();
};

// end of renderer group
/// @}

RENDERER_END
