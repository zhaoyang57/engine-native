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

#include <string>
#include <map>

#include "../Macro.h"
#include "base/CCRef.h"
#include "base/ccTypes.h"
#include "base/CCVector.h"
#include "math/CCMath.h"
#include "SystemHandle.hpp"

namespace se {
    class Object;
}

RENDERER_BEGIN

class ModelBatcher;
class Scene;

/**
 * @addtogroup scene
 * @{
 */

/**
 *  @brief NodeProxy correspond js CCNode.
 */
class NodeProxy : public Ref
{
public:
    /**
     *  @brief The default constructor.
     */
    NodeProxy();
    /**
     *  @brief The destructor.
     */
    ~NodeProxy();
    
    /**
     *  @brief Reset property value.
     */
    void reset();

    /// @{
    /// @name Children and Parent

    /**
     *  @brief Add child node proxy to the node proxy.
     *  @param[in] child A child node proxy pointer.
     */
    void addChild(NodeProxy * child);
    /**
     *  @brief Remove child node proxy from the node proxy.
     *  @param[in] child A child node proxy pointer.
     */
    void removeChild(NodeProxy* child);
    /**
     *  @brief Remove all child node proxy from the node proxy.
     */
    void removeAllChildren();
    
    /**
     *  @brief Set the node proxy parent.
     *  @param[in] parent.
     */
    inline void setParent(NodeProxy* parent) { _parent = parent; };
    /**
     *  @brief Get the node proxy parent.
     *  @return parent.
     */
    inline NodeProxy* getParent() const { return _parent; };
    /**
     *  @brief Get the node proxy all children.
     *  @return children container.
     */
    inline const Vector<NodeProxy*>& getChildren() const { return _children; };
    /**
     *  @brief Get the node proxy child count.
     *  @return child count.
     */
    inline size_t getChildrenCount() const { return _children.size(); };
    /**
     *  @brief Set the node proxy local zorder.
     *  @param[in] zOrder The value of zorder.
     */
    void setLocalZOrder(int zOrder);
    /**
     *  @brief Set children order dirty,set it true,will reorder children when visit node.
     */
    inline void setChildrenOrderDirty() { _childrenOrderDirty = true; };

    /// @} end of Children and Parent
    
    /**
     *  @brief update translate,rotation,scale data.
     *  @param[in] trs Js Float32Array object contain translate,rotation scale data.
     */
    void updateJSTRS(se::Object* trs);
    /**
     *  @brief Get world matrix.
     *  @return world matrix.
     */
    inline const cocos2d::Mat4& getWorldMatrix() const { return _worldMat; };
    
    /**
     *  @brief Get position value.
     *  @param[out] out The position value will set in the pointer.
     */
    void getPosition(cocos2d::Vec3* out) const;
    /**
     *  @brief Get rotation value.
     *  @param[out] out The rotation value will set in the pointer.
     */
    void getRotation(cocos2d::Quaternion* out) const;
    /**
     *  @brief Get scale value.
     *  @param[out] out The scale value will set in the pointer.
     */
    void getScale(cocos2d::Vec3* out) const;
    /**
     *  @brief Get world position value.
     *  @param[out] out The world position value will set in the pointer.
     */
    void getWorldPosition(cocos2d::Vec3* out) const;
    /**
     *  @brief Get world to local transform matrix.
     *  @param[out] out The matrix value will set in the pointer.
     */
    void getWorldRT(cocos2d::Mat4* out) const;
    
    /**
     *  @brief Get node opacity.
     */
    inline uint8_t getOpacity() const { return _opacity; };
    /**
     *  @brief Get groupID,use to culling node render if don't want to render the node.
     */
    inline int getGroupID() const { return _groupID; };
    /**
     *  @brief Set groupID.
     *  @param[in] groupID.
     */
    inline void setGroupID(int groupID) { _groupID = groupID; };
    
    /**
     *  @brief Get node proxy name,correspond to js CCNode name.
     *  @return name.
     */
    inline const std::string& getName() const { return _name; };
    /**
     *  @brief Set node proxy name,correspond to js CCNode name.
     *  @param[in] name.
     */
    inline void setName(const std::string& name) { _name = name; };
    
    /**
     *  @brief Add system handle into node proxy,then node will invoke per frame.
     *  @param[in] sysid The system handle id.
     *  @param[in] handle The system handle pointer.
     */
    void addHandle(const std::string& sysid, SystemHandle* handle);
    /**
     *  @brief Remove system handle from node proxy by system id.
     *  @param[in] sysid The system id.
     */
    void removeHandle(const std::string& sysid);
    /**
     *  @brief Get system handle by system id.
     *  @param[in] sysid The system id.
     */
    SystemHandle* getHandle(const std::string& sysid);
    
    /**
     *  @brief Begin to traverse all node proxy in scene.
     *  @param[in] batcher The ModelBatcher pointer which take charge collect render data.
     *  @param[in] scene The root node proxy.
     */
    void visitAsRoot(ModelBatcher* batcher, Scene* scene);
    
protected:
    void visit(ModelBatcher* batcher, Scene* scene);
    void childrenAlloc();
    void detachChild(NodeProxy* child, ssize_t childIndex);
    void reorderChildren();
    
    void updateFromJS();
    void updateMatrix();

private:
    static int _worldMatDirty;
    static int _parentOpacityDirty;
    static float _inheritOpacity;
    
    static const int _TRANSFORM = 1 << 0;
    static const int _UPDATE_RENDER_DATA = 1 << 1;
    static const int _OPACITY = 1 << 2;
    static const int _COLOR = 1 << 3;
    static const int _CHILDREN = 1 << 4;
    static const int _POST_UPDATE_RENDER_DATA = 1 << 5;
    
    bool _childrenOrderDirty = true;
    bool _matrixUpdated = false;
    bool _opacityUpdated = false;
    uint8_t _opacity = 0;
    int _localZOrder = 0;
    int _groupID = 0;

    cocos2d::Mat4 _localMat;
    cocos2d::Mat4 _worldMat;
    
    std::string _name;

    float* _jsTRSData;
    se::Object* _jsTRS;
    NodeProxy* _parent;                  ///< weak reference to parent node
    cocos2d::Vector<NodeProxy*> _children;        ///< array of children nodes
    
    std::map<std::string, SystemHandle*> _handles;
};

// end of scene group
/// @}

RENDERER_END
