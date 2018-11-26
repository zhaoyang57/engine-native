LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := editor_support_static

LOCAL_MODULE_FILENAME := libeditorsupport

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
../scripting/js-bindings/manual/jsb_helper.cpp \
EffectPool.cpp \
EditorRenderHandle.cpp \
IOBuffer.cpp \
editor-adapter.cpp \
TypeArrayPool.cpp \
IOTypeArray.cpp \
EditorManager.cpp \
../scripting/js-bindings/auto/jsb_cocos2dx_editor_support_auto.cpp

ifeq ($(USE_SPINE),1)
LOCAL_SRC_FILES += \
spine/Animation.c \
spine/AnimationState.c \
spine/AnimationStateData.c \
spine/Atlas.c \
spine/AtlasAttachmentLoader.c \
spine/Attachment.c \
spine/AttachmentLoader.c \
spine/Bone.c \
spine/BoneData.c \
spine/BoundingBoxAttachment.c \
spine/Event.c \
spine/EventData.c \
spine/IkConstraint.c \
spine/IkConstraintData.c \
spine/Json.c \
spine/MeshAttachment.c \
spine/PathAttachment.c \
spine/PathConstraint.c \
spine/PathConstraintData.c \
spine/RegionAttachment.c \
spine/Skeleton.c \
spine/SkeletonBinary.c \
spine/SkeletonBounds.c \
spine/SkeletonData.c \
spine/SkeletonJson.c \
spine/Skin.c \
spine/Slot.c \
spine/SlotData.c \
spine/TransformConstraint.c \
spine/TransformConstraintData.c \
spine/VertexAttachment.c \
spine/extension.c \
spine-creator-support/AttachmentVertices.cpp \
spine-creator-support/CreatorAttachmentLoader.cpp \
spine-creator-support/SpineAnimation.cpp \
spine-creator-support/SpineRenderer.cpp \
spine-creator-support/spine-cocos2dx.cpp \
../scripting/js-bindings/manual/jsb_spine_manual.cpp \
../scripting/js-bindings/auto/jsb_cocos2dx_spine_auto.cpp
endif # USE_SPINE

ifeq ($(USE_DRAGONBONES),1)
LOCAL_SRC_FILES += \
dragonBones/animation/Animation.cpp \
dragonBones/animation/AnimationState.cpp \
dragonBones/animation/BaseTimelineState.cpp \
dragonBones/animation/TimelineState.cpp \
dragonBones/animation/WorldClock.cpp \
dragonBones/armature/Armature.cpp \
dragonBones/armature/Bone.cpp \
dragonBones/armature/Constraint.cpp \
dragonBones/armature/DeformVertices.cpp \
dragonBones/armature/Slot.cpp \
dragonBones/armature/TransformObject.cpp \
dragonBones/core/BaseObject.cpp \
dragonBones/core/DragonBones.cpp \
dragonBones/event/EventObject.cpp \
dragonBones/factory/BaseFactory.cpp \
dragonBones/geom/Point.cpp \
dragonBones/geom/Transform.cpp \
dragonBones/model/AnimationConfig.cpp \
dragonBones/model/AnimationData.cpp \
dragonBones/model/ArmatureData.cpp \
dragonBones/model/BoundingBoxData.cpp \
dragonBones/model/CanvasData.cpp \
dragonBones/model/ConstraintData.cpp \
dragonBones/model/DisplayData.cpp \
dragonBones/model/DragonBonesData.cpp \
dragonBones/model/SkinData.cpp \
dragonBones/model/TextureAtlasData.cpp \
dragonBones/model/UserData.cpp \
dragonBones/parser/DataParser.cpp \
dragonBones/parser/BinaryDataParser.cpp \
dragonBones/parser/JSONDataParser.cpp \
dragonbones-creator-support/CCArmatureDisplay.cpp \
dragonbones-creator-support/CCFactory.cpp \
dragonbones-creator-support/CCSlot.cpp \
dragonbones-creator-support/CCTextureAtlasData.cpp \
../scripting/js-bindings/manual/jsb_dragonbones_manual.cpp \
../scripting/js-bindings/auto/jsb_cocos2dx_dragonbones_auto.cpp
endif # USE_DRAGONBONES

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
						   $(LOCAL_PATH)/.. \
						   $(LOCAL_PATH)/../..

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
					$(LOCAL_PATH)/../.. \
                    $(LOCAL_PATH)/../../external/android/$(TARGET_ARCH_ABI)/include/v8 \
					$(LOCAL_PATH)/../../external/sources/

include $(BUILD_STATIC_LIBRARY)