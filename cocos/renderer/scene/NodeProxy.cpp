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

#include "NodeProxy.hpp"

#include <string>

#include "ModelBatcher.hpp"
#include "../renderer/Scene.h"
#include "base/ccMacros.h"
#include "cocos/scripting/js-bindings/jswrapper/SeApi.h"
#include "cocos/scripting/js-bindings/manual/jsb_conversions.hpp"
#include "cocos/scripting/js-bindings/auto/jsb_renderer_auto.hpp"
#include <math.h>

RENDERER_BEGIN

int NodeProxy::parentOpacityDirty = 0;
int NodeProxy::_worldMatDirty = 0;

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

SystemHandle* NodeProxy::getHandle(const std::string& sysid)
{
    auto it = _handles.find(sysid);
    if (it != _handles.end())
    {
        return it->second;
    }
    return nullptr;
}

void NodeProxy::updateJSTRS(se::Object* trs)
{
    se::ScriptEngine::getInstance()->clearException();
    se::AutoHandleScope hs;
    
    if (_jsTRS != nullptr)
    {
        _jsTRS->unroot();
        _jsTRS->decRef();
    }
    
    trs->root();
    trs->incRef();
    _jsTRS = trs;
    _jsTRSData = nullptr;
    size_t length;
    trs->getTypedArrayData((uint8_t**)(&_jsTRSData), &length);
}

void NodeProxy::getPosition(cocos2d::Vec3* out) const
{
    out->x = _jsTRSData[1];
    out->y = _jsTRSData[2];
    out->z = _jsTRSData[3];
}

void NodeProxy::getRotation(cocos2d::Quaternion* out) const
{
    out->x = _jsTRSData[4];
    out->y = _jsTRSData[5];
    out->z = _jsTRSData[6];
    out->w = _jsTRSData[7];
}

void NodeProxy::getScale(cocos2d::Vec3* out) const
{
    out->x = _jsTRSData[8];
    out->y = _jsTRSData[9];
    out->z = _jsTRSData[10];
}

void NodeProxy::getWorldPosition(cocos2d::Vec3* out) const
{
    if (_jsTRSData != nullptr)
    {
        getPosition(out);
        
        cocos2d::Vec3 pos;
        cocos2d::Quaternion rot;
        cocos2d::Vec3 scale;
        NodeProxy* curr = _parent;
        while (curr != nullptr)
        {
            curr->getPosition(&pos);
            curr->getRotation(&rot);
            curr->getScale(&scale);
            
            out->multiply(scale);
            out->transformQuat(rot);
            out->add(pos);
            curr = curr->getParent();
        }
    }
}

void NodeProxy::getWorldRT(cocos2d::Mat4* out) const
{
    if (_jsTRSData != nullptr)
    {
        cocos2d::Vec3 opos(_jsTRSData[1], _jsTRSData[2], _jsTRSData[3]);
        cocos2d::Quaternion orot(_jsTRSData[4], _jsTRSData[5], _jsTRSData[6], _jsTRSData[7]);
        
        cocos2d::Vec3 pos;
        cocos2d::Quaternion rot;
        cocos2d::Vec3 scale;
        NodeProxy* curr = _parent;
        while (curr != nullptr)
        {
            curr->getPosition(&pos);
            curr->getRotation(&rot);
            curr->getScale(&scale);
            
            opos.multiply(scale);
            opos.transformQuat(rot);
            opos.add(pos);
            orot.multiply(rot);
            curr = curr->getParent();
        }
        out->setIdentity();
        out->translate(opos);
        cocos2d::Mat4 quatMat;
        cocos2d::Mat4::createRotation(orot, &quatMat);
        out->multiply(quatMat);
    }
}

void NodeProxy::setOpacity(uint8_t opacity)
{
    if (_opacity != opacity)
    {
        _opacity = opacity;
        _realOpacity = opacity;
        _opacityUpdated = true;
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
    uint32_t flag = _jsTRSData[0];
    if (flag & _TRANSFORM)
    {
        _localMat.setIdentity();

        // Transform = Translate * Rotation * Scale;
        cocos2d::Quaternion q(_jsTRSData[4], _jsTRSData[5], _jsTRSData[6], _jsTRSData[7]);
        _localMat.translate(_jsTRSData[1], _jsTRSData[2], _jsTRSData[3]);
        _localMat.rotate(q);
        _localMat.scale(_jsTRSData[8], _jsTRSData[9], _jsTRSData[10]);
        _jsTRSData[0] = 0;

        _matrixUpdated = true;
    }
}

void NodeProxy::visitAsRoot(ModelBatcher* batcher, Scene* scene)
{
    _worldMatDirty = 0;
    parentOpacityDirty = 0;
    visit(batcher, scene);
}

void NodeProxy::visit(ModelBatcher* batcher, Scene* scene)
{
    bool worldMatUpdated = false;
    bool parentOpacityUpdated = false;

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
        parentOpacityDirty++;
        _opacityUpdated = false;
        parentOpacityUpdated = true;
    }
    
    uint8_t opacity = _opacity;
    if (_parent != nullptr && parentOpacityDirty > 0)
    {
        float parentOpacity = _parent->getRealOpacity() / 255.0f;
        opacity = (uint8_t)(_opacity * parentOpacity);
        _realOpacity = opacity;
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
        parentOpacityDirty--;
    }
}

RENDERER_END
