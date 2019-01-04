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

#include "Light.h"
#include <new>
#include "gfx/DeviceGraphics.h"
#include "gfx/Texture2D.h"
#include "gfx/RenderBuffer.h"
#include "gfx/FrameBuffer.h"

RENDERER_BEGIN

Light::Light()
{
}

void Light::setColor(float r, float g, float b)
{
    _color.set(r, g, b);
    _colorUniform.set(r * _intensity, g * _intensity, b * _intensity);
}

void Light::setIntensity(float val)
{
    _intensity = val;
    _colorUniform.set(_color.r * _intensity,
                      _color.g * _intensity,
                      _color.b * _intensity);
}

void Light::setSpotAngle(float val)
{
    _spotAngle = val;
    _spotUniform[0] = std::cos(_spotAngle * 0.5f);
}

void Light::setSpotExp(float val)
{
    _spotExp = val;
    _spotUniform[1] = _spotExp;
}

void Light::setShadowType(ShadowType val)
{
    if (ShadowType::NONE == _shadowType &&
        ShadowType::NONE != val)
    {
        _shadowMapDirty = true;
    }
    
    _shadowType = val;
}

void Light::setShadowResolution(uint32_t val)
{
    if (_shadowResolution != val)
    {
        _shadowMapDirty = true;
        _shadowResolution = val;
    }
}

float Light::getShadowMinDepth() const
{
    if (LightType::DIRECTIONAL == _type)
        return 1.0f;
    else
        return _shadowMinDepth;
}

float Light::getShadowMaxDepth() const
{
    if (LightType::DIRECTIONAL == _type)
        return 1.0f;
    else
        return _shadowMaxDepth;
}

void Light::setWorldMatrix(const Mat4& worldMatrix)
{
    _worldMatrix = worldMatrix;
    Quaternion rotation;
    Vec3 translation;
    _worldMatrix.decompose(nullptr, &rotation, &translation);
    Mat4::createRotation(rotation, &_worldRT);
    _worldRT.translate(translation);
}

void Light::extractView(View& out, const std::vector<std::string>& stages)
{
    out.shadowLight = const_cast<Light*>(this);
    out.rect = { 0, 0, (float)_shadowResolution, (float)_shadowResolution };
    
    // clear options
    out.color.set(1.f, 1.f, 1.f, 1.f);
    out.depth = 1;
    out.stencil = 1;
    out.clearFlags = ClearFlag::COLOR | ClearFlag::DEPTH;
    
    // stage & framebuffer
    out.stages = stages;
    out.frameBuffer = _shadowFrameBuffer;
    
    // view projection matrix
    switch (_type)
    {
        case LightType::SPOT:
            computeSpotLightViewProjMatrix(out.matView, out.matProj);
            break;
        case LightType::DIRECTIONAL:
            computeDirectionalLightViewProjMatrix(out.matView, out.matProj);
            break;
        case LightType::POINT:
            computePointLightViewProjMatrix(out.matView, out.matProj);
            break;
        default:
            RENDERER_LOGW("Shadow of this light type is not supported");
            break;
    }
    
    // view-porjection
    Mat4::multiply(out.matView, out.matProj, &out.matViewProj);
    out.matInvViewPorj = out.matViewProj.getInversed();
    
    _viewProjMatrix.set(out.matViewProj);
}

void Light::update(DeviceGraphics* device)
{
    updateLightPositionAndDirection();
    
    if (ShadowType::NONE == _shadowType)
        destroyShadowMap();
    else
    {
        destroyShadowMap();
        generateShadowMap(device);
        _shadowMapDirty = false;
    }
}

// private functions

void Light::updateLightPositionAndDirection()
{
    _worldMatrix.transformVector(Vec3(0, 0, -1.f), &_directionUniform);
    _positionUniform.set(_worldMatrix.m[12], _worldMatrix.m[13], _worldMatrix.m[14]);
}

void Light::generateShadowMap(DeviceGraphics* device)
{
    _shadowMap = new (std::nothrow) Texture2D();
    if (!_shadowMap)
        return;
    
    _shadowDepthBuffer = new (std::nothrow) RenderBuffer();
    if (!_shadowDepthBuffer)
    {
        _shadowMap->release();
        _shadowMap = nullptr;
        
        return;
    }
    
    _shadowFrameBuffer = new (std::nothrow) FrameBuffer();
    if (!_shadowFrameBuffer)
    {
        _shadowMap->release();
        _shadowDepthBuffer->release();
        _shadowMap = nullptr;
        _shadowDepthBuffer = nullptr;
        
        return;
    }
    
    Texture2D::Options options;
    options.width = _shadowResolution;
    options.height = _shadowResolution;
//    options.format = Texture2D::Format::RGBA8;
    options.wrapS = Texture2D::WrapMode::CLAMP;
    options.wrapT = Texture2D::WrapMode::CLAMP;
    _shadowMap->init(device, options);
    
    _shadowDepthBuffer->init(device, RenderBuffer::Format::D16, _shadowResolution, _shadowResolution);
    
    _shadowFrameBuffer->init(device, _shadowResolution, _shadowResolution);
    _shadowFrameBuffer->setColorBuffer(_shadowMap, 0);
    _shadowFrameBuffer->setDepthBuffer(_shadowDepthBuffer);
}

void Light::destroyShadowMap()
{
    if (_shadowMap)
    {
        _shadowMap->release();
        _shadowMap = nullptr;
        
        RENDERER_SAFE_RELEASE(_shadowDepthBuffer);
        _shadowDepthBuffer = nullptr;
        
        RENDERER_SAFE_RELEASE(_shadowFrameBuffer);
        _shadowFrameBuffer = nullptr;
    }
}

void Light::computeSpotLightViewProjMatrix(Mat4& matView, Mat4& matProj) const
{
    // view matrix
    matView = _worldRT.getInversed();
    
    // proj matrix
    Mat4::createPerspective(_spotAngle * _spotAngleScale,
                            1,
                            _shadowMinDepth,
                            _shadowMaxDepth,
                            &matProj);
}

void Light::computeDirectionalLightViewProjMatrix(Mat4& matView, Mat4& matProj) const
{
    // view matrix
    matView = _worldRT.getInversed();
    
    // proj matrix
    uint32_t halfSize = _shadowFustumSize / 2;
    Mat4::createOrthographic(-halfSize, halfSize, -halfSize, halfSize, &matProj);
}

void Light::computePointLightViewProjMatrix(Mat4& matView, Mat4& matProj) const
{
    //REFINE
}

RENDERER_END
