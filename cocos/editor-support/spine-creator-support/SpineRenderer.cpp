/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
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

#include "spine-creator-support/SpineRenderer.h"
#include "spine/extension.h"
#include "spine-creator-support/AttachmentVertices.h"
#include "spine-creator-support/CreatorAttachmentLoader.h"
#include <algorithm>
#include "base/CCScheduler.h"
#include "MiddlewareMacro.h"
#include "renderer/renderer/Pass.h"
#include "renderer/renderer/Technique.h"
#include "MiddlewareRenderHandle.h"

USING_NS_CC;
USING_NS_MW;

using std::min;
using std::max;

using namespace spine;
using namespace cocos2d;
using namespace cocos2d::renderer;

static const std::string techStage = "transparent";
static const std::string textureKey = "texture";

SpineRenderer* SpineRenderer::create ()
{
    SpineRenderer* skeleton = new SpineRenderer();
    skeleton->autorelease();
    return skeleton;
}

SpineRenderer* SpineRenderer::createWithData (spSkeletonData* skeletonData, bool ownsSkeletonData)
{
	SpineRenderer* node = new SpineRenderer(skeletonData, ownsSkeletonData);
	node->autorelease();
	return node;
}

SpineRenderer* SpineRenderer::createWithFile (const std::string& skeletonDataFile, spAtlas* atlas, float scale)
{
	SpineRenderer* node = new SpineRenderer(skeletonDataFile, atlas, scale);
	node->autorelease();
	return node;
}

SpineRenderer* SpineRenderer::createWithFile (const std::string& skeletonDataFile, const std::string& atlasFile, float scale)
{
	SpineRenderer* node = new SpineRenderer(skeletonDataFile, atlasFile, scale);
	node->autorelease();
	return node;
}

void SpineRenderer::initialize ()
{
    // SpineRenderer::initalize may be invoked twice, need to check whether _worldVertics is already allocated to avoid memory leak.
    if (_worldVertices == nullptr)
        _worldVertices = new float[1000]; // Max number of vertices per mesh.
    
    beginSchedule();
}

void SpineRenderer::beginSchedule()
{
    MiddlewareManager::getInstance()->addTimer(this);
}

void SpineRenderer::onEnable()
{
    beginSchedule();
}

void SpineRenderer::onDisable()
{
    stopSchedule();
}

void SpineRenderer::stopSchedule()
{
    MiddlewareManager::getInstance()->removeTimer(this);
}

void SpineRenderer::setSkeletonData (spSkeletonData *skeletonData, bool ownsSkeletonData)
{
	_skeleton = spSkeleton_create(skeletonData);
	_ownsSkeletonData = ownsSkeletonData;
}

SpineRenderer::SpineRenderer ()
{
}

SpineRenderer::SpineRenderer (spSkeletonData *skeletonData, bool ownsSkeletonData)
{
	initWithData(skeletonData, ownsSkeletonData);
}

SpineRenderer::SpineRenderer (const std::string& skeletonDataFile, spAtlas* atlas, float scale)
{
	initWithJsonFile(skeletonDataFile, atlas, scale);
}

SpineRenderer::SpineRenderer (const std::string& skeletonDataFile, const std::string& atlasFile, float scale)
{
	initWithJsonFile(skeletonDataFile, atlasFile, scale);
}

SpineRenderer::~SpineRenderer ()
{
	if (_ownsSkeletonData) spSkeletonData_dispose(_skeleton->data);
	spSkeleton_dispose(_skeleton);
	if (_atlas) spAtlas_dispose(_atlas);
	if (_attachmentLoader) spAttachmentLoader_dispose(_attachmentLoader);
	delete [] _worldVertices;
    
    if (_debugBuffer)
    {
        delete _debugBuffer;
        _debugBuffer = nullptr;
    }
    
    CC_SAFE_RELEASE(_nodeProxy);
    stopSchedule();
}

void SpineRenderer::initWithData (spSkeletonData* skeletonData, bool ownsSkeletonData)
{
	setSkeletonData(skeletonData, ownsSkeletonData);
	initialize();
}

void SpineRenderer::initWithJsonFile (const std::string& skeletonDataFile, spAtlas* atlas, float scale)
{
    _atlas = atlas;
	_attachmentLoader = SUPER(CreatorAttachmentLoader_create(_atlas));

	spSkeletonJson* json = spSkeletonJson_createWithLoader(_attachmentLoader);
	json->scale = scale;
	spSkeletonData* skeletonData = spSkeletonJson_readSkeletonDataFile(json, skeletonDataFile.c_str());
	CCASSERT(skeletonData, json->error ? json->error : "Error reading skeleton data.");
	spSkeletonJson_dispose(json);

	setSkeletonData(skeletonData, true);
	initialize();
}

void SpineRenderer::initWithJsonFile (const std::string& skeletonDataFile, const std::string& atlasFile, float scale)
{
	_atlas = spAtlas_createFromFile(atlasFile.c_str(), 0);
	CCASSERT(_atlas, "Error reading atlas file.");

	_attachmentLoader = SUPER(CreatorAttachmentLoader_create(_atlas));

	spSkeletonJson* json = spSkeletonJson_createWithLoader(_attachmentLoader);
	json->scale = scale;
	spSkeletonData* skeletonData = spSkeletonJson_readSkeletonDataFile(json, skeletonDataFile.c_str());
	CCASSERT(skeletonData, json->error ? json->error : "Error reading skeleton data file.");
	spSkeletonJson_dispose(json);

	setSkeletonData(skeletonData, true);
	initialize();
}
    
void SpineRenderer::initWithBinaryFile (const std::string& skeletonDataFile, spAtlas* atlas, float scale)
{
    _atlas = atlas;
    _attachmentLoader = SUPER(CreatorAttachmentLoader_create(_atlas));
    
    spSkeletonBinary* binary = spSkeletonBinary_createWithLoader(_attachmentLoader);
    binary->scale = scale;
    spSkeletonData* skeletonData = spSkeletonBinary_readSkeletonDataFile(binary, skeletonDataFile.c_str());
    CCASSERT(skeletonData, binary->error ? binary->error : "Error reading skeleton data file.");
    spSkeletonBinary_dispose(binary);
    
    setSkeletonData(skeletonData, true);
    initialize();
}

void SpineRenderer::initWithBinaryFile (const std::string& skeletonDataFile, const std::string& atlasFile, float scale)
{
    _atlas = spAtlas_createFromFile(atlasFile.c_str(), 0);
    CCASSERT(_atlas, "Error reading atlas file.");
    
    _attachmentLoader = SUPER(CreatorAttachmentLoader_create(_atlas));
    
    spSkeletonBinary* binary = spSkeletonBinary_createWithLoader(_attachmentLoader);
    binary->scale = scale;
    spSkeletonData* skeletonData = spSkeletonBinary_readSkeletonDataFile(binary, skeletonDataFile.c_str());
    CCASSERT(skeletonData, binary->error ? binary->error : "Error reading skeleton data file.");
    spSkeletonBinary_dispose(binary);
    
    setSkeletonData(skeletonData, true);
    initialize();
}

void SpineRenderer::update (float deltaTime)
{
    // avoid other place call update.
    auto mgr = MiddlewareManager::getInstance();
    if (!mgr->isUpdating) return;
    
    if (!_paused)
    {
        spSkeleton_update(_skeleton, deltaTime * _timeScale);
    }
    
    if (_nodeProxy == nullptr)
    {
        return;
    }
    
    MiddlewareRenderHandle* renderHandle = (MiddlewareRenderHandle*)_nodeProxy->getHandle("render");
    if (renderHandle == nullptr)
    {
        return;
    }
    renderHandle->reset();
    
    _skeleton->r = _nodeColor.r / (float)255;
    _skeleton->g = _nodeColor.g / (float)255;
    _skeleton->b = _nodeColor.b / (float)255;
    _skeleton->a = _nodeProxy->getRealOpacity() / (float)255;
    
    Color4F color;
    AttachmentVertices* attachmentVertices = nullptr;
    middleware::IOBuffer& vb = mgr->vb;
    middleware::IOBuffer& ib = mgr->ib;
    
    BlendFactor preBlendSrc = BlendFactor::ZERO;
    BlendFactor preBlendDst = BlendFactor::ONE;
    BlendFactor curBlendSrc = BlendFactor::ONE;
    BlendFactor curBlendDst = BlendFactor::ZERO;
    
    int preTextureIndex = -1;
    int curTextureIndex = -1;
    
    int curISegLen = 0;
    
    int debugSlotsLen = 0;
    int materialLen = 0;
    int indexStart = 0;
    
    middleware::Texture2D* texture = nullptr;
    
    if (_debugSlots || _debugBones)
    {
        // If enable debug draw,then init debug buffer.
        if (_debugBuffer == nullptr)
        {
            _debugBuffer = new IOTypedArray(se::Object::TypedArrayType::FLOAT32, MAX_DEBUG_BUFFER_SIZE);
        }
        _debugBuffer->reset();
        
        if (_debugSlots)
        {
            //reserved 4 bytes to save debug slots len
            _debugBuffer->writeUint32(0);
        }
    }
    
    indexStart = (uint32_t)ib.getCurPos()/sizeof(unsigned short);
    
    for (int i = 0, n = _skeleton->slotsCount; i < n; ++i)
    {
        spSlot* slot = _skeleton->drawOrder[i];
        if (!slot->attachment) continue;
        
        switch (slot->attachment->type)
        {
            case SP_ATTACHMENT_REGION:
            {
                spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
                spRegionAttachment_computeWorldVertices(attachment, slot->bone, _worldVertices);
                attachmentVertices = getAttachmentVertices(attachment);
                color.r = attachment->r;
                color.g = attachment->g;
                color.b = attachment->b;
                color.a = attachment->a;
                
                if(_debugSlots)
                {
                    _debugBuffer->writeFloat32(_worldVertices[0]);
                    _debugBuffer->writeFloat32(_worldVertices[1]);
                    _debugBuffer->writeFloat32(_worldVertices[2]);
                    _debugBuffer->writeFloat32(_worldVertices[3]);
                    _debugBuffer->writeFloat32(_worldVertices[4]);
                    _debugBuffer->writeFloat32(_worldVertices[5]);
                    _debugBuffer->writeFloat32(_worldVertices[6]);
                    _debugBuffer->writeFloat32(_worldVertices[7]);
                    debugSlotsLen+=8;
                }
                
                break;
            }
            case SP_ATTACHMENT_MESH:
            {
                spMeshAttachment* attachment = (spMeshAttachment*)slot->attachment;
                spMeshAttachment_computeWorldVertices(attachment, slot, _worldVertices);
                attachmentVertices = getAttachmentVertices(attachment);
                color.r = attachment->r;
                color.g = attachment->g;
                color.b = attachment->b;
                color.a = attachment->a;
                break;
            }
            default:
                continue;
        }
        
        switch (slot->data->blendMode)
        {
            case SP_BLEND_MODE_ADDITIVE:
                curBlendSrc = _premultipliedAlpha ? BlendFactor::ONE : BlendFactor::SRC_ALPHA;
                curBlendDst = BlendFactor::ONE;
                break;
            case SP_BLEND_MODE_MULTIPLY:
                curBlendSrc = BlendFactor::DST_COLOR;
                curBlendDst = BlendFactor::ONE_MINUS_SRC_ALPHA;
                break;
            case SP_BLEND_MODE_SCREEN:
                curBlendSrc = BlendFactor::ONE;
                curBlendDst = BlendFactor::ONE_MINUS_SRC_COLOR;
                break;
            default:
                curBlendSrc = _premultipliedAlpha ? BlendFactor::ONE : BlendFactor::SRC_ALPHA;
                curBlendDst = BlendFactor::ONE_MINUS_SRC_ALPHA;
        }
        
        texture = attachmentVertices->_texture;
        curTextureIndex = texture->getRealTextureIndex();
        
        auto vbSize = attachmentVertices->_triangles->vertCount * sizeof(middleware::V2F_T2F_C4B);
        auto isFull = vb.checkSpace(vbSize, true);
        
        // If texture or blendMode change or vertex is more than 65535, will change material.
        if (preTextureIndex != curTextureIndex || preBlendDst != curBlendDst || preBlendSrc != curBlendSrc || isFull)
        {
            indexStart += curISegLen;
            
            double curHash = curTextureIndex + (int)curBlendSrc + (int)curBlendDst;
            
            auto nowEffectIdx = materialLen;
            Effect* renderEffect = renderHandle->getEffect(nowEffectIdx);
            Technique::Parameter* param = nullptr;
            Pass* pass = nullptr;
            
            if (renderEffect)
            {
                double renderHash = renderEffect->getHash();
                if (abs(renderHash - curHash) >= 0.01)
                {
                    param = (Technique::Parameter*)&(renderEffect->getProperty(textureKey));
                    Technique* tech = renderEffect->getTechnique(techStage);
                    Vector<Pass*>& passes = (Vector<Pass*>&)tech->getPasses();
                    pass = *(passes.begin());
                }
            }
            else
            {
                auto tplEffect = texture->getNativeEffect();
                if (tplEffect == nullptr)
                {
                    cocos2d::log("SpineRender:update get effect failed");
                    renderHandle->reset();
                    return;
                }
                auto effect = new cocos2d::renderer::Effect();
                effect->autorelease();
                effect->copy(*tplEffect);
                
                Technique* tech = effect->getTechnique(techStage);
                Vector<Pass*>& passes = (Vector<Pass*>&)tech->getPasses();
                pass = *(passes.begin());
                
                renderHandle->updateNativeEffect(nowEffectIdx, effect);
                renderEffect = effect;
            }
            
            if (param)
            {
                param->setTexture(texture->getNativeTexture());
            }
            
            if (pass)
            {
                pass->setBlend(BlendOp::ADD, curBlendSrc, curBlendDst,
                               BlendOp::ADD, curBlendSrc, curBlendDst);
            }
            
            renderEffect->updateHash(curHash);
            
            preTextureIndex = curTextureIndex;
            preBlendDst = curBlendDst;
            preBlendSrc = curBlendSrc;
            
            // Clear index segmentation count,prepare to next segmentation.
            curISegLen = 0;
            materialLen++;
        }
        
        // Calculation vertex color.
        color.a *= _skeleton->a * slot->a * 255;
        float multiplier = _premultipliedAlpha ? slot->a * 255 : 255;
        color.r *= _skeleton->r * slot->r * multiplier;
        color.g *= _skeleton->g * slot->g * multiplier;
        color.b *= _skeleton->b * slot->b * multiplier;
        
        for (int v = 0, w = 0, vn = attachmentVertices->_triangles->vertCount; v < vn; ++v, w += 2)
        {
            V2F_T2F_C4B* vertex = attachmentVertices->_triangles->verts + v;
            vertex->vertices.x = _worldVertices[w];
            vertex->vertices.y = _worldVertices[w + 1];
            vertex->colors.r = (GLubyte)color.r;
            vertex->colors.g = (GLubyte)color.g;
            vertex->colors.b = (GLubyte)color.b;
            vertex->colors.a = (GLubyte)color.a;
        }
        
        // Fill MiddlewareManager vertex buffer
        auto vertexOffset = vb.getCurPos() / sizeof(middleware::V2F_T2F_C4B);
        vb.writeBytes((char*)attachmentVertices->_triangles->verts, vbSize);
        
		auto ibSize = attachmentVertices->_triangles->indexCount * sizeof(unsigned short);
		ib.checkSpace(ibSize, true);
        if (vertexOffset > 0)
        {
            for (int ii = 0, nn = attachmentVertices->_triangles->indexCount; ii < nn; ii++)
            {
                ib.writeUint16(attachmentVertices->_triangles->indices[ii] + vertexOffset);
            }
        }
        else
        {
            ib.writeBytes((char*)attachmentVertices->_triangles->indices, ibSize);
        }
        
        curISegLen += attachmentVertices->_triangles->indexCount;
        renderHandle->updateIA(materialLen - 1, indexStart, curISegLen);
    }
    
    if (_debugSlots)
    {
        _debugBuffer->writeFloat32(0, debugSlotsLen);
    }
    
    if (_debugBones)
    {
        _debugBuffer->writeFloat32(_skeleton->bonesCount*4);
        for (int i = 0, n = _skeleton->bonesCount; i < n; i++)
        {
            spBone *bone = _skeleton->bones[i];
            float x = bone->data->length * bone->a + bone->worldX;
            float y = bone->data->length * bone->c + bone->worldY;
            _debugBuffer->writeFloat32(bone->worldX);
            _debugBuffer->writeFloat32(bone->worldY);
            _debugBuffer->writeFloat32(x);
            _debugBuffer->writeFloat32(y);
        }
    }
    
    if ((_debugSlots || _debugBones) &&  _debugBuffer->isOutRange())
    {
        _debugBuffer->writeFloat32(0, 0);
        _debugBuffer->writeFloat32(sizeof(float), 0);
        cocos2d::log("Spine debug data is too large,debug buffer has no space to put in it!!!!!!!!!!");
        cocos2d::log("You can adjust MAX_DEBUG_BUFFER_SIZE in MiddlewareMacro");
    }
}

AttachmentVertices* SpineRenderer::getAttachmentVertices (spRegionAttachment* attachment) const
{
    return (AttachmentVertices*)attachment->rendererObject;
}

AttachmentVertices* SpineRenderer::getAttachmentVertices (spMeshAttachment* attachment) const
{
    return (AttachmentVertices*)attachment->rendererObject;
}

void SpineRenderer::updateWorldTransform ()
{
	spSkeleton_updateWorldTransform(_skeleton);
}

void SpineRenderer::setToSetupPose ()
{
	spSkeleton_setToSetupPose(_skeleton);
}

void SpineRenderer::setBonesToSetupPose ()
{
	spSkeleton_setBonesToSetupPose(_skeleton);
}

void SpineRenderer::setSlotsToSetupPose ()
{
	spSkeleton_setSlotsToSetupPose(_skeleton);
}

spBone* SpineRenderer::findBone (const std::string& boneName) const
{
	return spSkeleton_findBone(_skeleton, boneName.c_str());
}

spSlot* SpineRenderer::findSlot (const std::string& slotName) const
{
	return spSkeleton_findSlot(_skeleton, slotName.c_str());
}

bool SpineRenderer::setSkin (const std::string& skinName)
{
	return spSkeleton_setSkinByName(_skeleton, skinName.empty() ? 0 : skinName.c_str()) ? true : false;
}

bool SpineRenderer::setSkin (const char* skinName)
{
	return spSkeleton_setSkinByName(_skeleton, skinName) ? true : false;
}

spAttachment* SpineRenderer::getAttachment (const std::string& slotName, const std::string& attachmentName) const
{
	return spSkeleton_getAttachmentForSlotName(_skeleton, slotName.c_str(), attachmentName.c_str());
}

bool SpineRenderer::setAttachment (const std::string& slotName, const std::string& attachmentName)
{
	return spSkeleton_setAttachment(_skeleton, slotName.c_str(), attachmentName.empty() ? 0 : attachmentName.c_str()) ? true : false;
}

bool SpineRenderer::setAttachment (const std::string& slotName, const char* attachmentName)
{
	return spSkeleton_setAttachment(_skeleton, slotName.c_str(), attachmentName) ? true : false;
}

spSkeleton* SpineRenderer::getSkeleton () const
{
	return _skeleton;
}

void SpineRenderer::setTimeScale (float scale)
{
	_timeScale = scale;
}

float SpineRenderer::getTimeScale () const
{
	return _timeScale;
}

void SpineRenderer::paused(bool value)
{
    _paused = value;
}

void SpineRenderer::setColor (cocos2d::Color4B& color)
{
    _nodeColor = color;
}

void SpineRenderer::setDebugBonesEnabled (bool enabled)
{
    _debugBones = enabled;
}

void SpineRenderer::setDebugSlotsEnabled (bool enabled)
{
    _debugSlots = enabled;
}
    
void SpineRenderer::setOpacityModifyRGB (bool value)
{
    _premultipliedAlpha = value;
}

bool SpineRenderer::isOpacityModifyRGB () const
{
    return _premultipliedAlpha;
}
