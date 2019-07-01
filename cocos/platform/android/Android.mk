LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dxandroid_static

LOCAL_MODULE_FILENAME := libcocos2dandroid

LOCAL_SRC_FILES := \
CCDevice-android.cpp \
CCFileUtils-android.cpp \
CCApplication-android.cpp \
CCCanvasRenderingContext2D-android.cpp \
jni/JniImp.cpp \
jni/JniHelper.cpp \

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/.. \
                    $(LOCAL_PATH)/../.. \
                    $(LOCAL_PATH)/../../.. \
                    $(LOCAL_PATH)/../../../external/android/$(TARGET_ARCH_ABI)/include/v8/libc++

LOCAL_EXPORT_LDLIBS := -lGLESv2 \
                       -lEGL \
                       -llog \
                       -landroid

LOCAL_STATIC_LIBRARIES := v8_static

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_WHOLE_STATIC_LIBRARIES += android_support
endif

include $(BUILD_STATIC_LIBRARY)
