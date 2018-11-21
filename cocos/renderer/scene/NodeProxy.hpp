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

class RenderFlow;

class NodeProxy : public Ref
{
public:
    NodeProxy();
    ~NodeProxy();
    
    void reset();

    /// @{
    /// @name Children and Parent

    void addChild(NodeProxy * child);
    void removeChild(NodeProxy* child);
    void removeAllChildren();
    
    inline void setParent(NodeProxy* parent) { _parent = parent; };
    inline NodeProxy* getParent() { return _parent; };
    inline const NodeProxy* getParent() const { return _parent; };
    
    inline const Vector<NodeProxy*>& getChildren() const { return _children; };
    inline ssize_t getChildrenCount() const { return _children.size(); };
    void setLocalZOrder(int zOrder);
    inline void setChildrenOrderDirty() { _childrenOrderDirty = true; };

    /// @} end of Children and Parent
    
    void generateJSMatrix();
    inline void setMatrixDirty() { _matDirty = true; };
    void updateLocalMatrixFromJS();
    inline const cocos2d::Mat4& getWorldMatrix() const { return _worldMat; };
    
    inline int getGroupID() const { return _groupID; };
    inline void setGroupID(int groupID) { _groupID = groupID; };
    
    inline const std::string& getName() const { return _name; };
    inline void setName(const std::string& name) { _name = name; };
    
    void addHandle(const std::string& sysid, SystemHandle* handle);
    void removeHandle(const std::string& sysid);
    
    void visitAsRoot(RenderFlow* flow);
    
private:
    void visit(RenderFlow* flow);
    void childrenAlloc();
    void detachChild(NodeProxy* child, ssize_t childIndex);
    void reorderChildren();
    
    void updateMatrix();

private:
    static int _worldMatDirty;
    
    bool _childrenOrderDirty;
    bool _matDirty;
    int _localZOrder;
    int _groupID;

    cocos2d::Mat4 _localMat;
    cocos2d::Mat4 _worldMat;
    
    std::string _name;

    uint32_t* _jsMatData;
    se::Object* _jsMatrix;
    NodeProxy* _parent;                  ///< weak reference to parent node
    cocos2d::Vector<NodeProxy*> _children;        ///< array of children nodes
    
    std::map<std::string, SystemHandle*> _handles;
};

RENDERER_END
