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

#include "../Macro.h"
#include "../Types.h"

#include "GraphicsHandle.h"
#include "RenderTarget.h"
#include "base/CCData.h"

#include <vector>

RENDERER_BEGIN

class DeviceGraphics;

/**
 * @addtogroup gfx
 * @{
 */

/**
 * The base texture class
 */
class Texture : public RenderTarget
{
public:
    /**
     * @enum Filter
     * Texture filter modes
     */
    enum class Filter : int8_t
    {
        NONE = -1,
        NEAREST = 0,
        LINEAR = 1
    };

    /**
     * @enum Filter
     * Texture wrap modes
     */
    enum class WrapMode : uint16_t
    {
        REPEAT = GL_REPEAT,
        CLAMP = GL_CLAMP_TO_EDGE,
        MIRROR = GL_MIRRORED_REPEAT
    };

    /**
     * @enum Format
     * Texture formats
     */
    enum class Format : uint8_t
    {
        BEGIN = 0,
    // compress formats
        RGB_DXT1 = 0,
        RGBA_DXT1 = 1,
        RGBA_DXT3 = 2,
        RGBA_DXT5 = 3,
        RGB_ETC1 = 4,
        RGB_PVRTC_2BPPV1 = 5,
        RGBA_PVRTC_2BPPV1 = 6,
        RGB_PVRTC_4BPPV1 = 7,
        RGBA_PVRTC_4BPPV1 = 8,
    //
    // normal formats
        A8 = 9,
        L8 = 10,
        L8_A8 = 11,
        R5_G6_B5 = 12,
        R5_G5_B5_A1 = 13,
        R4_G4_B4_A4 = 14,
        RGB8 = 15,                      // each channel has 8 bits
        RGBA8 = 16,                     // each channel has 8 bits
        RGB16F = 17,                    // each channel has 16 bits
        RGBA16F = 18,                   // each channel has 16 bits
        RGB32F = 19,                    // each channel has 32 bits
        RGBA32F = 20,                   // each channel has 32 bits
        R32F = 21,
        _111110F = 22,
        SRGB = 23,
        SRGBA = 24,
    //
    // depth formats
        D16 = 25,
        END = 25
    //
    };
    
    /**
     * @struct Image
     * Raw image data
     */
    struct Image
    {
        uint8_t* data = nullptr;
        size_t length = 0;
    };

    /**
     * @struct Options
     * Texture setting options including format, width, height, wrap mode, filter etc.
     */
    struct Options
    {
        /**
         * Image mipmaps
         */
        std::vector<Image> images;
        /**
         * The maximum anisotropy for the texture
         */
        int32_t anisotropy = 1;
        /**
         * The internal format specifying the color components in the texture
         */
        GLenum glInternalFormat = GL_RGBA;
        /**
         * The pixel format of the texel data
         */
        GLenum glFormat = GL_RGB;
        /**
         * The data type of the texel data
         */
        GLenum glType = GL_UNSIGNED_BYTE;
        /**
         * The width of the texture
         */
        uint16_t width = 4;
        /**
         * The height of the texture
         */
        uint16_t height = 4;
        uint8_t bpp = 0;
        
        /**
         * The wrapping function for texture coordinate s
         */
        WrapMode wrapS = WrapMode::REPEAT;
        /**
         * The wrapping function for texture coordinate t
         */
        WrapMode wrapT = WrapMode::REPEAT;
        /**
         * The texture minification filter
         */
        Filter minFilter = Filter::LINEAR;
        /**
         * The texture magnification filter
         */
        Filter magFilter = Filter::LINEAR;
        /**
         * The texture filter for mipmaps
         */
        Filter mipFilter = Filter::LINEAR;
        
        /**
         * Specifies whether the texture have mipmaps
         */
        bool hasMipmap = false;
        /**
         * Specifies whether the texture if flipped vertically
         */
        bool flipY = true;
        /**
         * Specifies whether the texture have alpha premultiplied
         */
        bool premultiplyAlpha = false;
        /**
         * Specifies whether the texture is compressed
         */
        bool compressed = false;
    };

    /**
     * @struct ImageOption
     * The informations of Image which indicates the mipmap level, width, height, filpY and premultiplyAlpha
     */
    struct ImageOption
    {
        Image image;
        int32_t level = 0;
        uint16_t width = 4;
        uint16_t height = 4;

        bool flipY = false;
        bool premultiplyAlpha = false;
    };

    /**
     * @struct SubImageOption
     * The informations of sub image which indicates the area to update, mipmap level, filpY and premultiplyAlpha
     */
    struct SubImageOption
    {
        SubImageOption(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t level, bool flipY, bool premultiplyAlpha)
        : x(x)
        , y(y)
        , width(width)
        , height(height)
        , level(level)
        , flipY(flipY)
        , premultiplyAlpha(premultiplyAlpha)
        {}
        
        SubImageOption() {}

        uint32_t imageDataLength = 0;
        uint16_t x = 0;
        uint16_t y = 0;
        uint16_t width = 0;
        uint16_t height = 0;
        uint8_t* imageData = nullptr;
        uint8_t level = 0;
        bool flipY = false;
        bool premultiplyAlpha = false;
    };

    /**
     * Gets the target gl location
     */
    inline GLuint getTarget() const { return _target; }
    /**
     * Gets the width of texture
     */
    inline uint16_t getWidth() const { return _width; }
    /**
     * Gets the height of texture
     */
    inline uint16_t getHeight() const { return _height; }

protected:
    
    static GLenum glFilter(Filter filter, Filter mipFilter = Filter::NONE);
    
    static bool isPow2(int32_t v) {
        return !(v & (v - 1)) && (!!v);
    }

    Texture();
    virtual ~Texture();

    bool init(DeviceGraphics* device);

    DeviceGraphics* _device;
    GLint _anisotropy;
    GLuint _target;
    
    WrapMode _wrapS;
    WrapMode _wrapT;
    uint16_t _width;
    uint16_t _height;
    uint8_t _bpp = 0;

    Filter _minFilter;
    Filter _magFilter;
    Filter _mipFilter;
    GLenum _glInternalFormat;
    GLenum _glFormat;
    GLenum _glType;

    bool _hasMipmap;
    bool _compressed;
};

// end of gfx group
/// @}

RENDERER_END
