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

#include "Camera.h"
#include "gfx/FrameBuffer.h"
#include "math/MathUtil.h"

RENDERER_BEGIN

Camera::Camera()
{
    _cachedView.color = _color;
    _cachedView.depth = _depth;
    _cachedView.clearFlags = _clearFlags;
    _cachedView.stencil = _stencil;
}

Camera::~Camera()
{
    RENDERER_SAFE_RELEASE(_framebuffer);
    RENDERER_SAFE_RELEASE(_node);
}

void Camera::setFrameBuffer(FrameBuffer* framebuffer)
{
    RENDERER_SAFE_RELEASE(_framebuffer);
    _framebuffer = framebuffer;
    RENDERER_SAFE_RETAIN(_framebuffer);
    
    _cachedView.frameBuffer = _framebuffer;
}

void Camera::setWorldMatrix(const Mat4& worldMatrix)
{
    Quaternion rotation;
    worldMatrix.decompose(nullptr, &rotation, &_worldPos);

    Mat4::createTranslation(_worldPos, &_worldRTInv);
    _worldRTInv.rotate(rotation);
    _worldRTInv.inverse();
}

void Camera::setColor(float r, float g, float b, float a)
{
    _color.set(r, g, b, a);
    _cachedView.color = _color;
}

void Camera::setDepth(float depth)
{
    _depth = depth;
    _cachedView.depth = _depth;
}

void Camera::setStencil(int stencil)
{
    _stencil = stencil;
    _cachedView.stencil = _stencil;
}

void Camera::setClearFlags(uint8_t flags )
{
    _clearFlags = flags;
    _cachedView.clearFlags = _clearFlags;
}

void Camera::setRect(float x, float y, float w, float h)
{
    _rect.set(x, y, w, h);
}

void Camera::setStages(const std::vector<std::string>& stages)
{
    _stages = stages;
    _cachedView.stages = _stages;
}


void Camera::setNode(NodeProxy* node)
{
    if (_node != nullptr)
    {
        _node->release();
    }
    _node = node;
    if (_node != nullptr)
    {
        _node->retain();
    }
}

const View& Camera::extractView( int width, int height)
{
    // can optimize it if node's position is not changed
    
    // rect
    _cachedView.rect.set(_rect.x * width,
                 _rect.y * height,
                 _rect.w * width,
                 _rect.h * height);

    // view matrix
    //REFINE:
    _node->getWorldRT(&_worldRTInv);
    _cachedView.matView.set(_worldRTInv.getInversed());

    // projecton matrix
    float aspect = (float)width / height;
    if (ProjectionType::PERSPECTIVE == _projection)
        Mat4::createPerspective(_fov / 3.1415926 * 180, aspect, _near, _far, &_cachedView.matProj);
    else
    {
        float x = _orthoHeight * aspect;
        float y = _orthoHeight;
        Mat4::createOrthographicOffCenter(-x, x, -y, y, _near, _far, &_cachedView.matProj);
    }

    // view projection
    Mat4::multiply(_cachedView.matProj, _cachedView.matView, &_cachedView.matViewProj);
    _cachedView.matInvViewPorj.set(_cachedView.matViewProj.getInversed());
    
    // culling mask
    _cachedView.cullingMask = _cullingMask;
    _cachedView.cullingByID = true;
    
    return _cachedView;
}

Vec3& Camera::screenToWorld(Vec3& out, const Vec3& screenPos, int width, int height) const
{
    float aspect = (float)width / height;
    float cx = _rect.x * width;
    float cy = _rect.y * height;
    float cw = _rect.w * width;
    float ch = _rect.h * height;
    
    // projection matrix
    Mat4 matProj;
    if (ProjectionType::PERSPECTIVE == _projection)
        Mat4::createPerspective(_fov / 3.1415926 * 180, aspect, _near, _far, &matProj);
    else
    {
        float x = _orthoHeight * aspect;
        float y = _orthoHeight;
        Mat4::createOrthographic(-x, x, -y, y, &matProj);
    }
    
    _node->getWorldRT(&(const_cast<Camera*>(this)->_worldRTInv));
    const_cast<Camera*>(this)->_worldRTInv.inverse();
    
    // view projection
    Mat4 matViewProj;
    Mat4::multiply(matProj, _worldRTInv, &matViewProj);
    
    // invert view projection
    Mat4 matInvViewProj = matViewProj.getInversed();
    
    if (ProjectionType::PERSPECTIVE == _projection)
    {
        // Caculate screen pos in far clip plane.
        out.set((screenPos.x - cx) * 2.0f / cw - 1.0f,
                (screenPos.y - cy) * 2.0f / ch - 1.0f,
                1.0f);
        
        // Transform to world position.
        matInvViewProj.transformPoint(&out);
        _node->getWorldPosition(&(const_cast<Camera*>(this)->_worldPos));
        Vec3 tmpVec3 = _worldPos;
        out = tmpVec3.lerp(out, MathUtil::lerp(_near / _far, 1, screenPos.z));
    }
    else
    {
        out.set((screenPos.x - cx) * 2.0f / cw - 1.0f,
                (screenPos.y - cy) * 2.0f / ch - 1.0f,
                screenPos.z * 2.0f - 1.0f);
        
        // Transform to world position.
        matInvViewProj.transformPoint(&out);
    }
    
    return out;
}

Vec3& Camera::worldToScreen(Vec3& out, const Vec3& worldPos, int width, int height) const
{
    float aspect = (float)width / height;
    float cx = _rect.x * width;
    float cy = _rect.y * height;
    float cw = _rect.w * width;
    float ch = _rect.h * height;
    
    // projection matrix
    Mat4 matProj;
    if (ProjectionType::PERSPECTIVE == _projection)
        Mat4::createPerspective(_fov, aspect, _near, _far, &matProj);
    else
    {
        float x = _orthoHeight * aspect;
        float y = _orthoHeight;
        Mat4::createOrthographic(-x, x, -y, y, &matProj);
    }
    
    _node->getWorldRT(&(const_cast<Camera*>(this)->_worldRTInv));
    const_cast<Camera*>(this)->_worldRTInv.inverse();
    // view projection
    Mat4 matViewProj;
    Mat4::multiply(matProj, _worldRTInv, &matViewProj);
    
    matViewProj.transformPoint(worldPos, &out);
    out.x = cx + (out.x + 1) * 0.5f * cw;
    out.y = cy + (out.y + 1) * 0.5f * ch;
    out.z = out.z * 0.5 + 0.5;
    
    return out;
}

RENDERER_END
