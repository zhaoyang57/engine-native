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

#include "platform/CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#endif

#ifndef RENDERER_BEGIN
#define RENDERER_BEGIN namespace cocos2d { namespace renderer {
#endif // RENDERER_BEGIN

#ifndef RENDERER_END
#define RENDERER_END }}
#endif // RENDERER_END

//#ifndef DISALLOW_COPY_ASSIGN_AND_MOVE
    #define CC_DISALLOW_COPY_ASSIGN_AND_MOVE(type) \
        type(const type&) = delete; \
        type& operator =(const type&) = delete; \
        type(type &&) = delete; \
        type& operator =(const type &&) = delete;
//#endif // DISALLOW_COPY_ASSIGN_AND_MOVE

#define RENDERER_LOG_TAG "renderer"
#define RENDERER_QUOTEME_(x) #x
#define RENDERER_QUOTEME(x) RENDERER_QUOTEME_(x)

#if defined(COCOS2D_DEBUG) && COCOS2D_DEBUG > 0
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define RENDERER_LOGV(fmt, ...) __android_log_print(ANDROID_LOG_VERBOSE, RENDERER_LOG_TAG, " (" RENDERER_QUOTEME(__LINE__) "): " fmt "\n", ##__VA_ARGS__)
#else
#define RENDERER_LOGV(fmt, ...) printf("V/" RENDERER_LOG_TAG " (" RENDERER_QUOTEME(__LINE__) "): " fmt "\n", ##__VA_ARGS__)
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#else
#define RENDERER_LOGV(fmt, ...) do {} while(false)
#endif // defined(COCOS2D_DEBUG) && COCOS2D_DEBUG > 0

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define RENDERER_LOGD(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, RENDERER_LOG_TAG, " (" RENDERER_QUOTEME(__LINE__) "): " fmt "\n", ##__VA_ARGS__)
#define RENDERER_LOGI(fmt, ...) __android_log_print(ANDROID_LOG_INFO, RENDERER_LOG_TAG, " (" RENDERER_QUOTEME(__LINE__) "): " fmt "\n", ##__VA_ARGS__)
#define RENDERER_LOGW(fmt, ...) __android_log_print(ANDROID_LOG_WARN, RENDERER_LOG_TAG, " (" RENDERER_QUOTEME(__LINE__) "): " fmt "\n", ##__VA_ARGS__)
#define RENDERER_LOGE(fmt, ...) __android_log_print(ANDROID_LOG_ERROR, RENDERER_LOG_TAG, " (" RENDERER_QUOTEME(__LINE__) "): " fmt "\n", ##__VA_ARGS__)
#else
#define RENDERER_LOGD(fmt, ...) printf("D/" RENDERER_LOG_TAG " (" RENDERER_QUOTEME(__LINE__) "): " fmt "\n", ##__VA_ARGS__)
#define RENDERER_LOGI(fmt, ...) printf("I/" RENDERER_LOG_TAG " (" RENDERER_QUOTEME(__LINE__) "): " fmt "\n", ##__VA_ARGS__)
#define RENDERER_LOGW(fmt, ...) printf("W/" RENDERER_LOG_TAG " (" RENDERER_QUOTEME(__LINE__) "): " fmt "\n", ##__VA_ARGS__)
#define RENDERER_LOGE(fmt, ...) printf("E/" RENDERER_LOG_TAG " (" RENDERER_QUOTEME(__LINE__) "): " fmt "\n", ##__VA_ARGS__)
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)


#define RENDERER_DEBUG 1 // REFINE: remove this

#define RENDERER_SAFE_RELEASE(p) do { if((p) != nullptr) (p)->release(); } while(false)
#define RENDERER_SAFE_RETAIN(p) do { if((p) != nullptr) (p)->retain(); } while(false)

#define RENDERER_DEFINE_CREATE_METHOD_0(clsName, initMethod) \
static clsName* create() \
{ \
    clsName* ret = new (std::nothrow) clsName(); \
    if (ret && ret->initMethod()) \
    { \
        ret->autorelease(); \
        return ret; \
    } \
    delete ret; \
    return nullptr; \
}

#define RENDERER_DEFINE_CREATE_METHOD_1(clsName, initMethod, arg0Type) \
static clsName* create(arg0Type arg0) \
{ \
    clsName* ret = new (std::nothrow) clsName(); \
    if (ret && ret->initMethod(arg0)) \
    { \
        ret->autorelease(); \
        return ret; \
    } \
    delete ret; \
    return nullptr; \
}

#define RENDERER_DEFINE_CREATE_METHOD_2(clsName, initMethod, arg0Type, arg1Type) \
static clsName* create(arg0Type arg0, arg1Type arg1) \
{ \
    clsName* ret = new (std::nothrow) clsName(); \
    if (ret && ret->initMethod(arg0, arg1)) \
    { \
        ret->autorelease(); \
        return ret; \
    } \
    delete ret; \
    return nullptr; \
}

#define RENDERER_DEFINE_CREATE_METHOD_3(clsName, initMethod, arg0Type, arg1Type, arg2Type) \
static clsName* create(arg0Type arg0, arg1Type arg1, arg2Type arg2) \
{ \
    clsName* ret = new (std::nothrow) clsName(); \
    if (ret && ret->initMethod(arg0, arg1, arg2)) \
    { \
        ret->autorelease(); \
        return ret; \
    } \
    delete ret; \
    return nullptr; \
}

#define RENDERER_DEFINE_CREATE_METHOD_4(clsName, initMethod, arg0Type, arg1Type, arg2Type, arg3Type) \
static clsName* create(arg0Type arg0, arg1Type arg1, arg2Type arg2, arg3Type arg3) \
{ \
    clsName* ret = new (std::nothrow) clsName(); \
    if (ret && ret->initMethod(arg0, arg1, arg2, arg3)) \
    { \
        ret->autorelease(); \
        return ret; \
    } \
    delete ret; \
    return nullptr; \
}

#define RENDERER_DEFINE_CREATE_METHOD_5(clsName, initMethod, arg0Type, arg1Type, arg2Type, arg3Type, arg4Type) \
static clsName* create(arg0Type arg0, arg1Type arg1, arg2Type arg2, arg3Type arg3, arg4Type arg4) \
{ \
    clsName* ret = new (std::nothrow) clsName(); \
    if (ret && ret->initMethod(arg0, arg1, arg2, arg3, arg4)) \
    { \
        ret->autorelease(); \
        return ret; \
    } \
    delete ret; \
    return nullptr; \
}

#define RENDERER_DEFINE_CREATE_METHOD_6(clsName, initMethod, arg0Type, arg1Type, arg2Type, arg3Type, arg4Type, arg5Type) \
static clsName* create(arg0Type arg0, arg1Type arg1, arg2Type arg2, arg3Type arg3, arg4Type arg4, arg5Type arg5) \
{ \
    clsName* ret = new (std::nothrow) clsName(); \
    if (ret && ret->initMethod(arg0, arg1, arg2, arg3, arg4, arg5)) \
    { \
        ret->autorelease(); \
        return ret; \
    } \
    delete ret; \
    return nullptr; \
}

// enum class to GLENUM
#define ENUM_CLASS_TO_GLENUM(value)  static_cast<GLenum>(value)

#define _GL_CHECK(_call) \
                do { \
                    _call; \
                    GLenum gl_err = glGetError(); \
                    if (0 != gl_err) \
                        RENDERER_LOGE(#_call "; GL error 0x%x: %s:%s", gl_err, glEnumName(gl_err), __FUNCTION__); \
                } while(false)


#if COCOS2D_DEBUG > 0
#   define GL_CHECK(_call)   _GL_CHECK(_call)
#else
#   define GL_CHECK(_call)   _call
#endif // BRENDERER_CONFIG_DEBUG

#ifndef RENDERER_PI
#define RENDERER_PI 3.1415926535897932385f
#endif // RENDERER_PI
