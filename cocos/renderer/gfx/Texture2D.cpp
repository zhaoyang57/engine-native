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

#include "Texture2D.h"
#include "DeviceGraphics.h"
#include "GFXUtils.h"

#include "base/CCGLUtils.h"

RENDERER_BEGIN

Texture2D::Texture2D()
{
//    RENDERER_LOGD("Construct Texture2D: %p", this);
}

Texture2D::~Texture2D()
{
//    RENDERER_LOGD("Destruct Texture2D: %p", this);
}

bool Texture2D::init(DeviceGraphics* device, Options& options)
{
    bool ok = Texture::init(device);
    if (ok)
    {
        _target = GL_TEXTURE_2D;
        GL_CHECK(glGenTextures(1, &_glID));

        if (options.images.empty())
            options.images.push_back(Image());

        update(options);
    }
    return ok;
}

void Texture2D::update(const Options& options)
{
    bool genMipmap = _hasMipmap;

    _width = options.width;
    _height = options.height;
    _anisotropy = options.anisotropy;
    _minFilter = options.minFilter;
    _magFilter = options.magFilter;
    _mipFilter = options.mipFilter;
    _wrapS = options.wrapS;
    _wrapT = options.wrapT;
    _glFormat = options.glFormat;
    _glInternalFormat = options.glInternalFormat;
    _glType = options.glType;
    _compressed = options.compressed;
    _bpp = options.bpp;

    // check if generate mipmap
    _hasMipmap = options.hasMipmap;
    genMipmap = options.hasMipmap;

    if (options.images.size() > 1)
    {
        genMipmap = false; //REFINE: is it true here?
        uint16_t maxLength = options.width > options.height ? options.width : options.height;
        if (maxLength >> (options.images.size() - 1) != 1)
            RENDERER_LOGE("texture-2d mipmap is invalid, should have a 1x1 mipmap.");
    }

    // NOTE: get pot after _width, _height has been assigned.
    bool pot = isPow2(_width) && isPow2(_height);
    if (!pot)
        genMipmap = false;

    GL_CHECK(glActiveTexture(GL_TEXTURE0));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, _glID));
    if (!options.images.empty())
        setMipmap(options.images, options.flipY, options.premultiplyAlpha);

    setTexInfo();

    if (genMipmap)
    {
        GL_CHECK(glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST));
        GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
    }
    _device->restoreTexture(0);
}

void Texture2D::updateSubImage(const SubImageOption& option)
{
    GL_CHECK(glActiveTexture(GL_TEXTURE0));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, _glID));
    setSubImage(option);
    _device->restoreTexture(0);
}

void Texture2D::updateImage(const ImageOption& option)
{
    GL_CHECK(glActiveTexture(GL_TEXTURE0));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, _glID));
    setImage(option);
    _device->restoreTexture(0);
}

// Private methods:

void Texture2D::setSubImage(const SubImageOption& option)
{
    bool flipY = option.flipY;
    bool premultiplyAlpha = option.premultiplyAlpha;

    //Set the row align only when mipmapsNum == 1 and the data is uncompressed
    GLint aligment = 1;
    if (!_hasMipmap && !_compressed && _bpp > 0)
    {
        unsigned int bytesPerRow = option.width * _bpp / 8;

        if (bytesPerRow % 8 == 0)
            aligment = 8;
        else if (bytesPerRow % 4 == 0)
            aligment = 4;
        else if (bytesPerRow % 2 == 0)
            aligment = 2;
        else
            aligment = 1;
    }

    GL_CHECK(ccPixelStorei(GL_UNPACK_ALIGNMENT, aligment));

    GL_CHECK(ccPixelStorei(GL_UNPACK_FLIP_Y_WEBGL, flipY));
    GL_CHECK(ccPixelStorei(GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL, premultiplyAlpha));

    ccFlipYIfNeeded(option.imageData, (uint32_t)option.imageDataLength, option.height);
    ccPremultiptyAlphaIfNeeded(option.imageData, (uint32_t)option.imageDataLength, _glFormat, _glType);

    if (_compressed)
    {
        glCompressedTexSubImage2D(GL_TEXTURE_2D,
                                  option.level,
                                  option.x,
                                  option.y,
                                  option.width,
                                  option.height,
                                  _glFormat,
                                  option.imageDataLength,
                                  option.imageData);
    }
    else
    {
        GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D,
                                 option.level,
                                 option.x,
                                 option.y,
                                 option.width,
                                 option.height,
                                 _glFormat,
                                 _glType, option.imageData));
    }
}

void Texture2D::setImage(const ImageOption& option)
{
    const auto& img = option.image;

    bool flipY = option.flipY;
    bool premultiplyAlpha = option.premultiplyAlpha;

    //Set the row align only when mipmapsNum == 1 and the data is uncompressed
    GLint aligment = 1;
    unsigned int bytesPerRow = option.width * _bpp / 8;
    if (_hasMipmap && !_compressed && _bpp > 0)
    {
        if (bytesPerRow % 8 == 0)
            aligment = 8;
        else if (bytesPerRow % 4 == 0)
            aligment = 4;
        else if (bytesPerRow % 2 == 0)
            aligment = 2;
        else
            aligment = 1;
    }

    GL_CHECK(ccPixelStorei(GL_UNPACK_ALIGNMENT, aligment));
    GL_CHECK(ccPixelStorei(GL_UNPACK_FLIP_Y_WEBGL, flipY));
    GL_CHECK(ccPixelStorei(GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL, premultiplyAlpha));

    ccFlipYIfNeeded(img.data, (uint32_t)img.length, option.height);
    ccPremultiptyAlphaIfNeeded(img.data, (uint32_t)img.length, _glFormat, _glType);

    if (_compressed)
    {
        glCompressedTexImage2D(GL_TEXTURE_2D,
                               option.level,
                               _glInternalFormat,
                               option.width,
                               option.height,
                               0,
                               (GLsizei)img.length,
                               img.data);
    }
    else
    {
        GL_CHECK(glTexImage2D(GL_TEXTURE_2D,
                     option.level,
                     _glInternalFormat,
                     option.width,
                     option.height,
                     0,
                     _glFormat,
                     _glType,
                     img.data));
    }
}

void Texture2D::setMipmap(const std::vector<Image>& images, bool isFlipY, bool isPremultiplyAlpha)
{
    ImageOption options;
    options.width = _width;
    options.height = _height;
    options.flipY = isFlipY;
    options.premultiplyAlpha = isPremultiplyAlpha;
    options.level = 0;

    for (size_t i = 0, len = images.size(); i < len; ++i)
    {
        options.level = (GLint)i;
        options.width = _width >> i;
        options.height = _height >> i;
        options.image = images[i];
        setImage(options);
    }
}

void Texture2D::setTexInfo()
{
    bool pot = isPow2(_width) && isPow2(_height);

    // WebGL1 doesn't support all wrap modes with NPOT textures
    if (!pot && (_wrapS != WrapMode::CLAMP || _wrapT != WrapMode::CLAMP))
    {
        RENDERER_LOGW("WebGL1 doesn\'t support all wrap modes with NPOT textures");
        _wrapS = WrapMode::CLAMP;
        _wrapT = WrapMode::CLAMP;
    }

    Filter mipFilter = _hasMipmap ? _mipFilter : Filter::NONE;
    if (!pot && mipFilter != Filter::NONE)
    {
        RENDERER_LOGW("NPOT textures do not support mipmap filter");
        mipFilter = Filter::NONE;
    }

    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter(_minFilter, mipFilter)));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter(_magFilter, Filter::NONE)));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)_wrapS));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)_wrapT));

    //REFINE:    let ext = this._device.ext('EXT_texture_filter_anisotropic');
//    if (ext) {
//        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, ext.TEXTURE_MAX_ANISOTROPY_EXT, this._anisotropy));
//    }
}

RENDERER_END
