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
#include "math/Vec3.h"
#include "math/Mat4.h"
#include "../Macro.h"
#include "../Types.h"
#include "View.h"
#include "../scene/NodeProxy.hpp"

#ifdef  near
#undef near
#endif

#ifdef  far
#undef far
#endif

RENDERER_BEGIN

class FrameBuffer;

/**
 * @addtogroup renderer
 * @{
 */

/**
 *  @brief Manage scene camera.
 */
class Camera : public Ref
{
public:
    /**
     *  @brief The default constructor.
     */
    Camera();
    /**
     *  @brief The default destructor.
     */
    ~Camera();
    
    /**
     *  @brief Gets the camera type.
     */
    ProjectionType getType() const { return _projection; }
    /**
     *  @brief Sets the camera type.
     */
    inline void setType(ProjectionType value) { _projection = value; }
    /**
     *  @brief Gets the view height for orthographic camera.
     */
    inline float getOrthoHeight() const { return _orthoHeight; }
    /**
     *  @brief Sets the view height for orthographic camera.
     */
    inline void setOrthoHeight(float value) { _orthoHeight = value; }
    /**
     *  @brief Gets the field of view for the camera.
     */
    inline float getFov() const { return _fov; }
    /**
     *  @brief Sets the field of view for the camera.
     */
    inline void setFov(float fov) { _fov = fov; }
    /**
     *  @brief Gets the near clip plane.
     */
    inline float getNear() const { return _near; }
    /**
     *  @brief Sets the near clip plane.
     */
    inline void setNear(float near) { _near = near; }
    /**
     *  @brief Gets the far clip plane.
     */
    inline float getFar() const { return _far; }
    /**
     *  @brief Sets the far clip plane.
     */
    inline void setFar(float far) { _far = far; }
    /**
     *  @brief Gets the clear color.
     */
    inline void getColor(Color4F& out) const { out = _color; }
    /**
     *  @brief Sets the clear color.
     */
    void setColor(float r, float g, float b, float a);
    /**
     *  @brief Gets the depth of the camera, camera of smaller depth gets rendered earlier.
     */
    inline float getDepth() const { return _depth; }
    /**
     *  @brief Sets the depth of the camera, camera of smaller depth gets rendered earlier
     */
    void setDepth(float depth);
    /**
     *  @brief Gets the culling mask.
     */
    inline int getCullingMask() const { return _cullingMask; }
    /**
     *  @brief Sets the culling mask.
     */
    inline void setCullingMask(int mask) { _cullingMask = mask; }
    /**
     *  @brief Gets the stencil value to clear
     */
    inline int getStencil() const { return _stencil; }
    /**
     *  @brief Sets the stencil value to clear
     */
    void setStencil(int stencil);
    /**
     *  @brief Gets the clear flags including color, stencil and depth.
     */
    inline uint8_t getClearFlags() const { return _clearFlags; }
    /**
     *  @brief Sets the clear flags including color, stencil and depth.
     */
    void setClearFlags(uint8_t flags );
    /**
     *  @brief Gets rect.
     */
    inline Rect* getRect(Rect& out) const { out = _rect; return &out; }
    /**
     *  @brief Sets rect.
     */
    void setRect(float x, float y, float w, float h);
    /**
     *  @brief Gets stages.
     */
    inline const std::vector<std::string>& getStages() const { return _stages; }
    /**
     *  @brief Sets stages.
     */
    void setStages(const std::vector<std::string>& stages);
    /**
     *  @brief Gets the frame buffer as render target.
     */
    inline FrameBuffer* getFrameBuffer() const { return _framebuffer; }
    /**
     *  @brief Sets the frame buffer as render target.
     */
    void setFrameBuffer(FrameBuffer* framebuffer);
    /**
     *  @brief Sets the world matrix.
     */
    void setWorldMatrix(const Mat4& worldMatrix);
    /**
     *  @brief Extracts the camera info to view.
     */
    const View& extractView(int width, int height);
    /**
     *  @brief Transform a screen position to world in the current camera projection.
     */
    Vec3& screenToWorld(Vec3& out, const Vec3& screenPos, int width, int height) const;
    /**
     *  @brief Transform a world position to screen in the current camera projection.
     */
    Vec3& worldToScreen(Vec3& out, const Vec3& worldPos, int width, int height) const;
    /**
     *  @brief Sets the related node proxy which provids model matrix for camera.
     */
    void setNode(NodeProxy* node);
    /**
     *  @brief Gets the related node proxy which provids model matrix for camera.
     */
    inline NodeProxy* getNode() const { return _node; }
    /**
     *  @brief Sets the camera render priority.
     */
    void setPriority(int priority) { _priority = priority; }
    /**
     *  @brief Gets the camera render priority.
     */
    int getPriority() const { return _priority; }
private:
    NodeProxy* _node = nullptr;
    ProjectionType _projection = ProjectionType::PERSPECTIVE;
    
    // clear options
    Color4F _color = {0.2f, 0.3f, 0.47f, 1.f};
    float _depth = 1.f;
    int _stencil = 1;
    int _cullingMask = 1;
    uint8_t _clearFlags = ClearFlag::COLOR | ClearFlag::DEPTH;
    
    // stage & framebuffer
    std::vector<std::string> _stages;
    FrameBuffer* _framebuffer = nullptr;
    
    // projection properties
    float _near = 0.01f;
    float _far = 1000.0f;
    float _fov = RENDERER_PI / 4.0f;
    Rect _rect = {0, 0, 1, 1};
    
    // ortho properties
    float _orthoHeight = 10.f;
    
    Mat4 _worldRTInv;
    Vec3 _worldPos;
    
    View _cachedView;
    int _priority = 0;
};

// end of renderer group
/// @}

RENDERER_END
