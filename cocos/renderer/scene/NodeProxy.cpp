/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2009      Valentin Milea
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2017 Chukong Technologies Inc.

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

#include "NodeProxy.hpp"

#include <algorithm>
#include <string>
#include <regex>

#include "ModelBatcher.hpp"
#include "../renderer/Scene.h"
#include "base/ccMacros.h"
#include "cocos/scripting/js-bindings/jswrapper/SeApi.h"
#include "cocos/scripting/js-bindings/manual/jsb_conversions.hpp"
#include "cocos/scripting/js-bindings/auto/jsb_renderer_auto.hpp"

RENDERER_BEGIN

int NodeProxy::_worldMatDirty = 0;
int NodeProxy::_parentOpacityDirty = 0;
float NodeProxy::_inheritOpacity = 1.0;

NodeProxy::NodeProxy()
: _jsTRS(nullptr)
, _jsTRSData(nullptr)
, _parent(nullptr)
{
    _localMat = Mat4::IDENTITY;
    _worldMat = Mat4::IDENTITY;
}

NodeProxy::~NodeProxy()
{
    CCLOGINFO( "deallocing NodeProxy: %p", this );

    for (auto& child : _children)
    {
        child->_parent = nullptr;
    }
    
    if (_jsTRS != nullptr)
    {
        _jsTRS->unroot();
        _jsTRS->decRef();
        _jsTRS = nullptr;
    }
    _jsTRSData = nullptr;
}

void NodeProxy::reset()
{
    if (_parent)
    {
        _parent->removeChild(this);
    }
    removeAllChildren();
    _localZOrder = 0;
    _childrenOrderDirty = false;
    _groupID = 0;
    _localMat.setIdentity();
    _worldMat.setIdentity();
}

// lazy allocs
void NodeProxy::childrenAlloc()
{
    _children.reserve(4);
}

void NodeProxy::addChild(NodeProxy* child)
{
    CCASSERT(child != nullptr, "Argument must be non-nil");
    CCASSERT(child->_parent == nullptr, "child already added. It can't be added again");
    
    auto assertNotSelfChild
        ( [ this, child ]() -> bool
          {
              for ( NodeProxy* parent( getParent() ); parent != nullptr;
                    parent = parent->getParent() )
                  if ( parent == child )
                      return false;
              
              return true;
          } );
    (void)assertNotSelfChild;
    
    CCASSERT( assertNotSelfChild(),
              "A node cannot be the child of his own children" );
    
    if (_children.empty())
    {
        this->childrenAlloc();
    }
    _children.pushBack(child);
    _childrenOrderDirty = true;
    child->setParent(this);
}

void NodeProxy::detachChild(NodeProxy *child, ssize_t childIndex)
{
    // set parent nil at the end
    child->setParent(nullptr);
    _children.erase(childIndex);
}

void NodeProxy::removeChild(NodeProxy* child)
{
    // explicit nil handling
    if (_children.empty())
    {
        return;
    }

    ssize_t index = _children.getIndex(child);
    if( index != CC_INVALID_INDEX )
        this->detachChild( child, index );
}

void NodeProxy::removeAllChildren()
{
    // not using detachChild improves speed here
    for (const auto& child : _children)
    {
        // set parent nil at the end
        child->setParent(nullptr);
    }
    
    _children.clear();
}

void NodeProxy::setLocalZOrder(int zOrder)
{
    _localZOrder = zOrder;
    if (_parent != nullptr)
    {
        _parent->setChildrenOrderDirty();
    }
}

void NodeProxy::reorderChildren()
{
    if (_childrenOrderDirty)
    {
#if CC_64BITS
        std::sort(std::begin(_children), std::end(_children), [](NodeProxy* n1, NodeProxy* n2) {
            return (n1->_localZOrder < n2->_localZOrder);
        });
#else
        std::stable_sort(std::begin(_children), std::end(_children), [](NodeProxy* n1, NodeProxy* n2) {
            return n1->_localZOrder < n2->_localZOrder;
        });
#endif
        _childrenOrderDirty = false;
    }
}

void NodeProxy::addHandle(const std::string& sysid, SystemHandle* handle)
{
    _handles[sysid] = handle;
}

void NodeProxy::removeHandle(const std::string& sysid)
{
    _handles.erase(sysid);
}

void NodeProxy::generateTypedArray()
{
    if (_jsTRS == nullptr)
    {
        se::ScriptEngine::getInstance()->clearException();
        se::AutoHandleScope hs;
        
        se::Value jsVal;
        bool ok = native_ptr_to_seval<NodeProxy>((NodeProxy*)this, &jsVal);
        CCASSERT(ok, "NodeProxy_generateTypedArray : JS object missing");
        
        _jsTRS = se::Object::createTypedArray(se::Object::TypedArrayType::FLOAT32, nullptr, 4 * 10);
        // root it
        _jsTRS->root();
        // Pass to js
        jsVal.toObject()->setProperty("_trs", se::Value(_jsTRS));
        // Save to cpp
        size_t length;
        _jsTRS->getTypedArrayData((uint8_t**)(&_jsTRSData), &length);
    }
}

void NodeProxy::updateMatrix()
{
    if (_matrixUpdated || _worldMatDirty > 0)
    {
        // Update world matrix
        const cocos2d::Mat4& parentMat = _parent == nullptr ? cocos2d::Mat4::IDENTITY : _parent->getWorldMatrix();
        _worldMat.multiply(parentMat, _localMat, &_worldMat);
        _matrixUpdated = false;
    }
}

void NodeProxy::updateFromJS()
{
    uint8_t* uintData = (uint8_t*)_jsTRSData;
    if (uintData[0] > 0)
    {
        _localMat.setIdentity();
        cocos2d::Quaternion q(_jsTRSData[4], _jsTRSData[5], _jsTRSData[6], 1.0);
        _localMat.translate(_jsTRSData[1], _jsTRSData[2], _jsTRSData[3]);
        _localMat.scale(_jsTRSData[7], _jsTRSData[8], _jsTRSData[9]);
        _localMat.rotate(q);
        _matrixUpdated = true;
    }
    if (uintData[1] > 0)
    {
        _opacity = uintData[2];
        _opacityUpdated = true;
    }
}

void NodeProxy::visitAsRoot(ModelBatcher* batcher, Scene* scene)
{
    _worldMatDirty = 0;
    _parentOpacityDirty = 0;
    _inheritOpacity = 1.0;
    visit(batcher, scene);
}

void NodeProxy::visit(ModelBatcher* batcher, Scene* scene)
{
    bool worldMatUpdated = false;
    bool parentOpacityUpdated = false;
    uint8_t opacity = _opacity;
    if (_parent != nullptr && _parentOpacityDirty > 0)
    {
        opacity = (uint8_t)(_opacity * _inheritOpacity);
        _inheritOpacity = (float)opacity / 255;
    }
    
    reorderChildren();
    
    updateFromJS();
    
    if (_matrixUpdated)
    {
        _worldMatDirty++;
        worldMatUpdated = true;
    }
    updateMatrix();
    
    if (_opacityUpdated)
    {
        _parentOpacityDirty++;
        _opacityUpdated = false;
        parentOpacityUpdated = true;
    }
    
    for (const auto& handler : _handles)
    {
        handler.second->handle(this, batcher, scene);
    }
    
    for (const auto& child : _children)
    {
        child->visit(batcher, scene);
    }
    
    for (const auto& handler : _handles)
    {
        handler.second->postHandle(this, batcher, scene);
    }
    
    if (worldMatUpdated)
    {
        _worldMatDirty--;
    }
    if (parentOpacityUpdated)
    {
        _parentOpacityDirty--;
    }
}

RENDERER_END
