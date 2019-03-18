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

#include "MaskRenderHandle.hpp"
#include "./ModelBatcher.hpp"
#include "./StencilManager.hpp"

RENDERER_BEGIN

MaskRenderHandle::MaskRenderHandle()
:_batcher(nullptr)
,_renderSubHandle(nullptr)
,_clearSubHandle(nullptr)
,_imageStencil(false)
,_inverted(false)
{
}

void MaskRenderHandle::handle(NodeProxy *node, ModelBatcher* batcher, Scene* scene)
{
    _batcher = batcher;
    _node = node;
    batcher->commit(node, (RenderHandle*)this);
    StencilManager* instance = StencilManager::getInstance();
    if (!_imageStencil && _renderSubHandle)
    {
        _renderSubHandle->handle(node, batcher, scene);
    }
    instance->enableMask();
};

void MaskRenderHandle::postHandle(NodeProxy *node, ModelBatcher *batcher, Scene *scene)
{
    _batcher = batcher;
    batcher->flush();
    StencilManager::getInstance()->exitMask();
}

void MaskRenderHandle::fillBuffers(MeshBuffer *buffer, int index, const Mat4 &worldMat)
{
    StencilManager* instance = StencilManager::getInstance();
    instance->pushMask(_inverted);
    instance->clear();
    _batcher->commitIA(_node, _clearSubHandle);
    instance->enterLevel();
    if(_imageStencil)
    {
        _batcher->setCurrentEffect(getEffect(index));
        RenderHandle::fillBuffers(buffer, index, worldMat);
        _batcher->flush();
    }
}

RENDERER_END
