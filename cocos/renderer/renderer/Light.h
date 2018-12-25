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
#include <string>
#include "base/ccTypes.h"
#include "math/Mat4.h"
#include "../Macro.h"
#include "../Types.h"
#include "View.h"

RENDERER_BEGIN

class FrameBuffer;
class Texture2D;
class RenderBuffer;
class DeviceGraphics;

/**
 * @addtogroup renderer
 * @{
 */

/**
 *  @brief Manage scene light.
 */
class Light : public Ref
{
public:
    /**
     *  @brief Light type.
     */
    enum class LightType
    {
        /**
         * Directional light
         */
        DIRECTIONAL,
        /**
         * Point light
         */
        POINT,
        /**
         * Spot light
         */
        SPOT
    };
    
    /**
     *  @brief Shadow type.
     */
    enum class ShadowType
    {
        /**
         * No shadow
         */
        NONE,
        /**
         * Hard shadow
         */
        HARD,
        /**
         * Soft shadow
         */
        SOFT
    };
    
    /**
     *  @brief The default constructor.
     */
    Light();
    
    /**
     *  @brief Sets the light color.
     */
    void setColor(float r, float g, float b);
    /**
     *  @brief Gets the light color.
     */
    inline Color3F getColor() const { return _color; }
    /**
     *  @brief Sets the light intensity.
     */
    void setIntensity(float val);
    /**
     *  @brief Gets the light intensity.
     */
    inline float getIntensity() const { return _intensity; }
    /**
     *  @brief Sets the light type.
     */
    inline void setType(LightType type) { _type = type; }
    /**
     *  @brief Gets the light type.
     */
    inline LightType getType() const { return _type; }
    /**
     *  @brief Sets the spot light angle.
     */
    void setSpotAngle(float val);
    /**
     *  @brief Gets the spot light angle.
     */
    inline float getSpotAngle() const { return _spotAngle; }
    
    /**
     *  @brief Sets the light attenuation exponent.
     */
    void setSpotExp(float val);
    /**
     *  @brief Gets the light attenuation exponent.
     */
    inline float getSpotExp() const { return _spotExp; }
    /**
     *  @brief Sets spot or point light range.
     */
    inline void setRange(float val) { _range = val; }
    /**
     *  @brief Gets spot or point light range.
     */
    inline float getRange() const { return _range; }
    /**
     *  @brief Gets the shadow type.
     */
    void setShadowType(ShadowType val);
    /**
     *  @brief Gets the shadow type.
     */
    inline ShadowType getShadowType() const { return _shadowType; }
    /**
     *  @brief Gets the shadow map texture.
     */
    inline Texture2D* getShadowMap() const { return _shadowMap; }
    /**
     *  @brief Gets the view projection matrix.
     */
    inline Mat4 getViewPorjMatrix() const { return _viewProjMatrix; }
    /**
     *  @brief Gets the view projection matrix.
     */
    inline const Mat4& getViewProjMatrix() const { return _viewProjMatrix; }
    /**
     *  @brief Sets the resolution of shadow
     */
    void setShadowResolution(uint32_t val);
    /**
     *  @brief Gets the resolution of shadow
     */
    inline uint32_t getShadowResolution() const { return _shadowResolution; }
    
    /**
     *  @brief Sets the shadow bias setting.
     */
    inline void setShadowBias(float val) { _shadowBias = val; }
    /**
     *  @brief Gets the shadow bias setting.
     */
    inline float getShadowBias() const { return _shadowBias; }
    /**
     *  @brief Sets the shadow darkness setting.
     */
    inline void setShadowDarkness(uint32_t val) { _shadowDarkness = val; }
    /**
     *  @brief Gets the shadow darkness setting.
     */
    inline uint32_t getShadowDarkness() const { return _shadowDarkness; }
    
    /**
     *  @brief Sets the min depth of shadow.
     */
    inline void setShadowMinDepth(float val) { _shadowMinDepth = val; }
    /**
     *  @brief Gets the min depth of shadow.
     */
    float getShadowMinDepth() const;
    /**
     *  @brief Sets the max depth of shadow.
     */
    inline void setShadowMaxDepth(float val) { _shadowMaxDepth = val; }
    /**
     *  @brief Gets the max depth of shadow.
     */
    float getShadowMaxDepth() const;
    /**
     *  @brief Sets the shadow scale.
     */
    inline void setShadowScale(float val) { _shadowScale = val; }
    /**
     *  @brief Gets the shadow scale.
     */
    inline float getShadowScale() const { return _shadowScale; }
    /**
     *  @brief Sets the frustum edge fall off.
     */
    inline void setFrustumEdgeFalloff(uint32_t val) { _frustumEdgeFalloff = val; }
    /**
     *  @brief Gets the frustum edge fall off.
     */
    inline uint32_t getFrustumEdgeFalloff() const { return _frustumEdgeFalloff; }
    /**
     *  @brief Sets the world matrix.
     */
    void setWorldMatrix(const Mat4& worldMatrix);
    /**
     *  @brief Gets the world matrix.
     */
    inline const Mat4& getWorldMatrix() const { return _worldMatrix; }
    /**
     *  @brief Extract light informations to view.
     */
    void extractView(View& out, const std::vector<std::string>& stages);
    /**
     *  @brief Update light and generate new shadow map.
     */
    void update(DeviceGraphics* device);
    
private:
    void updateLightPositionAndDirection();
    void generateShadowMap(DeviceGraphics* device);
    void destroyShadowMap();
    void computeSpotLightViewProjMatrix(Mat4& matView, Mat4& matProj) const;
    void computeDirectionalLightViewProjMatrix(Mat4& matView, Mat4& matProj) const;
    void computePointLightViewProjMatrix(Mat4& matView, Mat4& matProj) const;
    
    LightType _type = LightType::DIRECTIONAL;
    Color3F _color = {1.f, 1.f, 1.f};
    float _intensity = 1.f;
    
    // used for spot and point light
    float _range = 1.f;
    // used for spot light, default to 60 degrees
    float _spotAngle = RENDERER_PI / 3;
    float _spotExp = 1.f;
    // cached for uniform
    Vec3 _directionUniform;
    Vec3 _positionUniform;
    Vec3 _colorUniform = {_color.r * _intensity, _color.g * _intensity, _color.b * _intensity};
    float _spotUniform[2] = { std::cos(_spotAngle) * 0.5f, _spotExp};
    
    // shadow params
    ShadowType _shadowType = ShadowType::NONE;
    FrameBuffer* _shadowFrameBuffer = nullptr;
    Texture2D* _shadowMap = nullptr;
    bool _shadowMapDirty = false;
    RenderBuffer* _shadowDepthBuffer = nullptr;
    uint32_t _shadowResolution = 1024;
    float _shadowBias = 0.00005f;
    float _shadowDarkness = 1.f;
    float _shadowMinDepth = 1;
    float _shadowMaxDepth = 1000;
    float _shadowScale = 50; // maybe need to change it if the distance between shadowMaxDepth and shadowMinDepth is small.
    uint32_t _frustumEdgeFalloff = 0; // used by directional and spot light.
    Mat4 _viewProjMatrix;
    float _spotAngleScale = 1; // used for spot light.
    uint32_t _shadowFustumSize = 80; // used for directional light.
    
    Mat4 _worldMatrix;
    Mat4 _worldRT;
};

// end of renderer group
/// @}

RENDERER_END
