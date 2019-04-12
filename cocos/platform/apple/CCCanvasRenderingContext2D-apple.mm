#include "platform/CCCanvasRenderingContext2D.h"
#include "base/ccTypes.h"
#include "base/csscolorparser.hpp"

#include "cocos/scripting/js-bindings/jswrapper/SeApi.h"
#include "cocos/scripting/js-bindings/manual/jsb_platform.h"

#import <Foundation/Foundation.h>

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#import <Cocoa/Cocoa.h>
#else
#import <CoreText/CoreText.h>

#define NSBezierPath UIBezierPath
#define NSFont UIFont
#define NSColor UIColor
#define NSSize CGSize
#define NSZeroSize CGSizeZero
#define NSPoint CGPoint
#define NSMakePoint CGPointMake
#define NSMakeRect CGRectMake
#define NSButtLineCapStyle kCGLineCapButt
#define NSRoundLineCapStyle kCGLineCapRound
#define NSSquareLineCapStyle kCGLineCapSquare
#define NSMiterLineJoinStyle kCGLineJoinMiter
#define NSRoundLineJoinStyle kCGLineJoinRound
#define NSBevelLineJoinStyle kCGLineJoinBevel

#endif

#include <regex>

enum class CanvasTextAlign {
    LEFT,
    CENTER,
    RIGHT
};

enum class CanvasTextBaseline {
    TOP,
    MIDDLE,
    BOTTOM
};

@interface CanvasRenderingContext2DImpl : NSObject {
    NSFont* _font;
    NSMutableDictionary* _tokenAttributesDict;
    NSString* _fontName;
    NSString* _lineCap;
    NSString* _lineJoin;
    CGFloat _fontSize;
    CGFloat _width;
    CGFloat _height;
    CGContextRef _context;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    NSGraphicsContext* _currentGraphicsContext;
    NSGraphicsContext* _oldGraphicsContext;
#else
    CGContextRef _oldContext;
    CGAffineTransform _transform;
    CGBlendMode _blendMode;
#endif
    
    CGColorSpaceRef _colorSpace;
    cocos2d::Data _imageData;
    NSBezierPath* _path;

    CanvasTextAlign _textAlign;
    CanvasTextBaseline _textBaseLine;
    cocos2d::Color4F _fillStyle;
    cocos2d::Color4F _strokeStyle;
    float _lineWidth;
    bool _bold;
    std::vector<float> _lineDashPattern;
    float _lineDashOffset;
    float _miterLimit;
    cocos2d::Data _strokePatternData;
    float _strokePatternImageWidth;
    float _strokePatternImageHeight;
    NSColor *_strokePattern;
    cocos2d::Data _fillPatternData;
    float _fillPatternImageWidth;
    float _fillPatternImageHeight;
    NSColor *_fillPattern;
    CGGradientRef _fillLinearGradient;
    CGPoint _fillLinearGradientStart;
    CGPoint _fillLinearGradientEnd;
    CGGradientRef _strokeLinearGradient;
    CGPoint _strokeLinearGradientStart;
    CGPoint _strokeLinearGradientEnd;
    CGGradientRef _fillRadialGradient;
    CGPoint _fillRadialGradientStart;
    CGPoint _fillRadialGradientEnd;
    float _fillRadialGradientStartR;
    float _fillRadialGradientEndR;
    CGGradientRef _strokeRadialGradient;
    CGPoint _strokeRadialGradientStart;
    CGPoint _strokeRadialGradientEnd;
    float _strokeRadialGradientStratR;
    float _strokeRadialGradientEndR;
    NSColor *_shadowColor;
    float _shadowBlur;
    float _shadowOffsetX;
    float _shadowOffsetY;
    float _globalAlpha;
}

@property (nonatomic, assign) CGFloat width;
@property (nonatomic, assign) CGFloat height;
@property (nonatomic, strong) NSFont* font;
@property (nonatomic, strong) NSMutableDictionary* tokenAttributesDict;
@property (nonatomic, strong) NSString* fontName;
@property (nonatomic, strong) NSString* lineCap;
@property (nonatomic, strong) NSString* lineJoin;
@property (nonatomic, assign) CanvasTextAlign textAlign;
@property (nonatomic, assign) CanvasTextBaseline textBaseLine;
@property (nonatomic, assign) float lineWidth;
@property (nonatomic, assign) float lineDashOffset;
@property (nonatomic, assign) float miterLimit;
@property (nonatomic, assign) cocos2d::Data strokePatternData;
@property (nonatomic, assign) float strokePatternImageWidth;
@property (nonatomic, assign) float strokePatternImageHeight;
@property (nonatomic, strong) NSColor *strokePattern;
@property (nonatomic, assign) cocos2d::Data fillPatternData;
@property (nonatomic, assign) float fillPatternImageWidth;
@property (nonatomic, assign) float fillPatternImageHeight;
@property (nonatomic, strong) NSColor *fillPattern;
@property (nonatomic, assign) CGGradientRef fillLinearGradient;
@property (nonatomic, assign) CGPoint fillLinearGradientStart;
@property (nonatomic, assign) CGPoint fillLinearGradientEnd;
@property (nonatomic, assign) CGGradientRef strokeLinearGradient;
@property (nonatomic, assign) CGPoint strokeLinearGradientStart;
@property (nonatomic, assign) CGPoint strokeLinearGradientEnd;
@property (nonatomic, assign) CGGradientRef strokeRadialGradient;
@property (nonatomic, assign) CGPoint strokeRadialGradientStart;
@property (nonatomic, assign) CGPoint strokeRadialGradientEnd;
@property (nonatomic, assign) float strokeRadialGradientStartR;
@property (nonatomic, assign) float strokeRadialGradientEndR;
@property (nonatomic, assign) CGGradientRef fillRadialGradient;
@property (nonatomic, assign) CGPoint fillRadialGradientStart;
@property (nonatomic, assign) CGPoint fillRadialGradientEnd;
@property (nonatomic, assign) float fillRadialGradientStartR;
@property (nonatomic, assign) float fillRadialGradientEndR;
@property (nonatomic, assign) float shadowBlur;
@property (nonatomic, assign) float shadowOffsetX;
@property (nonatomic, assign) float shadowOffsetY;
@property (nonatomic, assign) float globalAlpha;

@end

@implementation CanvasRenderingContext2DImpl

@synthesize width = _width;
@synthesize height = _height;
@synthesize font = _font;
@synthesize tokenAttributesDict = _tokenAttributesDict;
@synthesize fontName = _fontName;
@synthesize textAlign = _textAlign;
@synthesize textBaseLine = _textBaseLine;
@synthesize lineWidth = _lineWidth;
@synthesize lineDashOffset = _lineDashOffset;
@synthesize miterLimit = _miterLimit;
@synthesize strokePatternData = _strokePatternData;
@synthesize strokePatternImageWidth = _strokePatternImageWidth;
@synthesize strokePatternImageHeight = _strokePatternImageHeight;
@synthesize strokePattern = _strokePattern;
@synthesize fillPatternData = _fillPatternData;
@synthesize fillPatternImageWidth = _fillPatternImageWidth;
@synthesize fillPatternImageHeight = _fillPatternImageHeight;
@synthesize fillPattern = _fillPattern;
@synthesize fillLinearGradient = _fillLinearGradient;
@synthesize fillLinearGradientStart = _fillLinearGradientStart;
@synthesize fillLinearGradientEnd = _fillLinearGradientEnd;
@synthesize strokeLinearGradient = _strokeLinearGradient;
@synthesize strokeLinearGradientStart = _strokeLinearGradientStart;
@synthesize strokeLinearGradientEnd = _strokeLinearGradientEnd;
@synthesize strokeRadialGradient = _strokeRadialGradient;
@synthesize strokeRadialGradientStart = _strokeRadialGradientStart;
@synthesize strokeRadialGradientEnd = _strokeRadialGradientEnd;
@synthesize strokeRadialGradientStartR = _strokeRadialGradientStratR;
@synthesize strokeRadialGradientEndR = _strokeRadialGradientEndR;
@synthesize fillRadialGradient = _fillRadialGradient;
@synthesize fillRadialGradientStart = _fillRadialGradientStart;
@synthesize fillRadialGradientEnd = _fillRadialGradientEnd;
@synthesize fillRadialGradientStartR = _fillRadialGradientStartR;
@synthesize fillRadialGradientEndR = _fillRadialGradientEndR;
@synthesize shadowBlur = _shadowBlur;
@synthesize shadowOffsetX = _shadowOffsetX;
@synthesize shadowOffsetY = _shadowOffsetY;
@synthesize globalAlpha = _globalAlpha;

-(id) init {
    if (self = [super init]) {
        _width = _height = 0;
        _context = nil;
        _colorSpace = nil;
        [self _resetSetting];
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        _currentGraphicsContext = nil;
        _oldGraphicsContext = nil;
#endif
        _path = [NSBezierPath bezierPath];
        [_path retain];
        [self updateFontWithName:@"Arial" fontSize:30 bold:false];
    }

    return self;
}

-(void) dealloc {
    self.font = nil;
    self.tokenAttributesDict = nil;
    self.fontName = nil;
    CGColorSpaceRelease(_colorSpace);
    // release the context
    CGContextRelease(_context);
    [_path release];
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    [_currentGraphicsContext release];
#endif
    _lineDashPattern.clear();
    // release pattern
    [self _clearApplyStrokeStyle];
    [self _clearApplyFillStyle];
    [super dealloc];
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC

-(NSFont*) _createSystemFont {
    NSFontTraitMask mask = NSUnitalicFontMask;
    if (_bold) {
        mask |= NSBoldFontMask;
    }
    else {
        mask |= NSUnboldFontMask;
    }

    NSFont* font = [[NSFontManager sharedFontManager]
                    fontWithFamily:_fontName
                    traits:mask
                    weight:0
                    size:_fontSize];

    if (font == nil) {
        const auto& familyMap = getFontFamilyNameMap();
        auto iter = familyMap.find([_fontName UTF8String]);
        if (iter != familyMap.end()) {
            font = [[NSFontManager sharedFontManager]
               fontWithFamily: [NSString stringWithUTF8String:iter->second.c_str()]
               traits: mask
               weight: 0
               size: _fontSize];
        }
    }

    if (font == nil) {
        font = [[NSFontManager sharedFontManager]
                fontWithFamily: @"Arial"
                traits: mask
                weight: 0
                size: _fontSize];
    }
    return font;
}

#else

-(UIFont*) _createSystemFont {
    UIFont* font = nil;

    if (_bold) {
        font = [UIFont fontWithName:[_fontName stringByAppendingString:@"-Bold"] size:_fontSize];
    }
    else {
        font = [UIFont fontWithName:_fontName size:_fontSize];
    }

    if (font == nil) {
        const auto& familyMap = getFontFamilyNameMap();
        auto iter = familyMap.find([_fontName UTF8String]);
        if (iter != familyMap.end()) {
            font = [UIFont fontWithName:[NSString stringWithUTF8String:iter->second.c_str()] size:_fontSize];
        }
    }

    if (font == nil) {
        if (_bold) {
            font = [UIFont boldSystemFontOfSize:_fontSize];
        } else {
            font = [UIFont systemFontOfSize:_fontSize];
        }
    }
    return font;
}

#endif

-(void) updateFontWithName: (NSString*)fontName fontSize: (CGFloat)fontSize bold: (bool)bold{
    _fontSize = fontSize;
    _bold = bold;

    self.fontName = fontName;
    self.font = [self _createSystemFont];

    NSMutableParagraphStyle* paragraphStyle = [[[NSMutableParagraphStyle alloc] init] autorelease];
    paragraphStyle.lineBreakMode = NSLineBreakByTruncatingTail;
    [paragraphStyle setAlignment:NSTextAlignmentCenter];

    // color
    NSColor* foregroundColor = [NSColor colorWithRed:1.0f
                                               green:1.0f
                                                blue:1.0f
                                               alpha:1.0f];

    // attribute
    self.tokenAttributesDict = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                                foregroundColor, NSForegroundColorAttributeName,
                                                _font, NSFontAttributeName,
                                                paragraphStyle, NSParagraphStyleAttributeName, nil];
}

-(void) recreateBufferWithWidth:(NSInteger) width height:(NSInteger) height {
    _width = width = width > 0 ? width : 1;
    _height = height = height > 0 ? height : 1;
    NSUInteger textureSize = width * height * 4;
    unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * textureSize);
    memset(data, 0, textureSize);
    _imageData.fastSet(data, textureSize);

    if (_context != nil)
    {
        CGContextRelease(_context);
        _context = nil;
    }

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    if (_currentGraphicsContext != nil)
    {
        [_currentGraphicsContext release];
        _currentGraphicsContext = nil;
    }
#endif

    // draw text
    _colorSpace  = CGColorSpaceCreateDeviceRGB();
    _context = CGBitmapContextCreate(data,
                                     width,
                                     height,
                                     8,
                                     width * 4,
                                     _colorSpace,
                                     kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    if (nil == _context)
    {
        CGColorSpaceRelease(_colorSpace); //REFINE: HOWTO RELEASE?
        _colorSpace = nil;
    }

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    _currentGraphicsContext = [NSGraphicsContext graphicsContextWithCGContext:_context flipped: NO];
    [_currentGraphicsContext retain];
#else
    // move Y rendering to the top of the image
    CGContextTranslateCTM(_context, 0.0f, _height);

    //NOTE: NSString draws in UIKit referential i.e. renders upside-down compared to CGBitmapContext referential
    CGContextScaleCTM(_context, 1.0f, -1.0f);
#endif
    [self _resetSetting];
}

-(NSSize) measureText:(NSString*) text {

    NSAttributedString* stringWithAttributes = [[[NSAttributedString alloc] initWithString:text
                                                             attributes:_tokenAttributesDict] autorelease];

    NSSize textRect = NSZeroSize;
    textRect.width = CGFLOAT_MAX;
    textRect.height = CGFLOAT_MAX;

    NSSize dim = [stringWithAttributes boundingRectWithSize:textRect options:(NSStringDrawingOptions)(NSStringDrawingUsesLineFragmentOrigin) context:nil].size;

    dim.width = ceilf(dim.width);
    dim.height = ceilf(dim.height);

    return dim;
}

-(NSPoint) convertDrawPoint:(NSPoint) point text:(NSString*) text {
    // The parameter 'point' is located at left-bottom position.
    // Need to adjust 'point' according 'text align' & 'text base line'.
    NSSize textSize = [self measureText:text];

    if (_textAlign == CanvasTextAlign::CENTER)
    {
        point.x -= textSize.width / 2.0f;
    }
    else if (_textAlign == CanvasTextAlign::RIGHT)
    {
        point.x -= textSize.width;
    }

    if (_textBaseLine == CanvasTextBaseline::TOP)
    {
        point.y += _fontSize;
    }
    else if (_textBaseLine == CanvasTextBaseline::MIDDLE)
    {
        point.y += _fontSize / 2.0f;
    }

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    // We use font size to calculate text height, but 'drawPointAt' method on macOS is based on
    // the real font height and in bottom-left position, add the adjust value to make the text inside text rectangle.
    point.y += (textSize.height - _fontSize) / 2.0f;

    // The origin on macOS is bottom-left by default, so we need to convert y from top-left origin to bottom-left origin.
    point.y = _height - point.y;
#else
    // The origin of drawing text on iOS is from top-left, but now we get bottom-left,
    // So, we need to substract the font size to convert 'point' to top-left.
    point.y -= _fontSize;

    // We use font size to calculate text height, but 'drawPointAt' method on iOS is based on
    // the real font height and in top-left position, substract the adjust value to make text inside text rectangle.
    point.y -= (textSize.height - _fontSize) / 2.0f;
#endif
    return point;
}

-(void) fillText:(NSString*) text x:(CGFloat) x y:(CGFloat) y maxWidth:(CGFloat) maxWidth {
    if (text.length == 0)
        return;

    NSPoint drawPoint = [self convertDrawPoint:NSMakePoint(x, y) text:text];

    NSMutableParagraphStyle* paragraphStyle = [[[NSMutableParagraphStyle alloc] init] autorelease];
    paragraphStyle.lineBreakMode = NSLineBreakByTruncatingTail;

    [_tokenAttributesDict removeObjectForKey:NSStrokeWidthAttributeName];
    [_tokenAttributesDict removeObjectForKey:NSStrokeColorAttributeName];

    [_tokenAttributesDict setObject:paragraphStyle forKey:NSParagraphStyleAttributeName];
    NSColor *textColor = [NSColor colorWithRed:_fillStyle.r
                                         green:_fillStyle.g
                                          blue:_fillStyle.b
                                         alpha:_fillStyle.a];
    if (_fillPattern) {
        textColor = _fillPattern;
    }
    [_tokenAttributesDict setObject:textColor
                             forKey:NSForegroundColorAttributeName];

    [self saveContext];

    // text color
    CGContextSetFillColorWithColor(_context, [textColor CGColor]);
    CGContextSetShouldSubpixelQuantizeFonts(_context, false);
    CGContextBeginTransparencyLayerWithRect(_context, CGRectMake(0, 0, _width, _height), nullptr);
    CGContextSetTextDrawingMode(_context, kCGTextFill);

    if (_shadowColor) {
        CGContextSetShadowWithColor(_context,
                                    CGSizeMake(_shadowOffsetX, _shadowOffsetY),
                                    _shadowBlur,
                                    _shadowColor.CGColor);
    }
    [self _applyCTM];
    CGContextSetAlpha(_context, _globalAlpha);

    NSAttributedString *stringWithAttributes =[[[NSAttributedString alloc] initWithString:text
                                                                               attributes:_tokenAttributesDict] autorelease];

    [stringWithAttributes drawAtPoint:drawPoint];


    CGContextEndTransparencyLayer(_context);

    [self restoreContext];
}

-(void) strokeText:(NSString*) text x:(CGFloat) x y:(CGFloat) y maxWidth:(CGFloat) maxWidth {
    if (text.length == 0)
        return;

    NSPoint drawPoint = [self convertDrawPoint:NSMakePoint(x, y) text:text];

    NSMutableParagraphStyle* paragraphStyle = [[[NSMutableParagraphStyle alloc] init] autorelease];
    paragraphStyle.lineBreakMode = NSLineBreakByTruncatingTail;

    [_tokenAttributesDict removeObjectForKey:NSForegroundColorAttributeName];

    [_tokenAttributesDict setObject:paragraphStyle forKey:NSParagraphStyleAttributeName];
    [_tokenAttributesDict setObject:[NSNumber numberWithFloat: _lineWidth * 2]
                            forKey:NSStrokeWidthAttributeName];
    NSColor *textColor = [NSColor colorWithRed:_strokeStyle.r
                                         green:_strokeStyle.g
                                          blue:_strokeStyle.b
                                         alpha:_strokeStyle.a];
    if (_strokePattern) {
        textColor = _strokePattern;
    }
    [_tokenAttributesDict setObject:textColor
                             forKey:NSStrokeColorAttributeName];

    [self saveContext];

    // text color
    CGContextSetFillColorWithColor(_context, [textColor CGColor]);

    CGContextSetShouldSubpixelQuantizeFonts(_context, false);
    CGContextBeginTransparencyLayerWithRect(_context, CGRectMake(0, 0, _width, _height), nullptr);

    CGContextSetTextDrawingMode(_context, kCGTextStroke);
    
    if (_shadowColor) {
        CGContextSetShadowWithColor(_context,
                                    CGSizeMake(_shadowOffsetX, _shadowOffsetY),
                                    _shadowBlur,
                                    _shadowColor.CGColor);
    }
    [self _applyCTM];
    CGContextSetAlpha(_context, _globalAlpha);

    NSAttributedString *stringWithAttributes =[[[NSAttributedString alloc] initWithString:text
                                                                               attributes:_tokenAttributesDict] autorelease];

    [stringWithAttributes drawAtPoint:drawPoint];

    CGContextEndTransparencyLayer(_context);

    [self restoreContext];
}

-(void) setFillStyleWithRed:(CGFloat) r green:(CGFloat) g blue:(CGFloat) b alpha:(CGFloat) a {
    _fillStyle.r = r;
    _fillStyle.g = g;
    _fillStyle.b = b;
    _fillStyle.a = a;
    [self _clearApplyFillStyle];
}

-(void) setStrokeStyleWithRed:(CGFloat) r green:(CGFloat) g blue:(CGFloat) b alpha:(CGFloat) a {
    _strokeStyle.r = r;
    _strokeStyle.g = g;
    _strokeStyle.b = b;
    _strokeStyle.a = a;
    [self _clearApplyStrokeStyle];
}

-(void) setShadowColorWithR:(float)r g:(float)g b:(float)b a:(float)a {
    _shadowColor = [NSColor colorWithRed:r
                                   green:g
                                    blue:b
                                   alpha:a];
}

-(const cocos2d::Data&) getDataRef {
    return _imageData;
}

-(void) scaleX:(float)x y:(float)y {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    _transform = CGAffineTransformScale(_transform, x, y);
#endif
}

-(void) rotate:(float)angle {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    _transform = CGAffineTransformRotate(_transform, angle);
#endif
}

-(void) translateX:(float)x y:(float)y {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    _transform = CGAffineTransformTranslate(_transform, x, y);
#endif
}

-(void) transformA:(float)a b:(float)b c:(float)c d:(float)d e:(float)e f:(float)f {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    _transform = CGAffineTransformConcat(_transform, CGAffineTransformMake(a, b, c, d, e, f));
#endif
}

-(void) setTransformA:(float)a b:(float)b c:(float)c d:(float)d e:(float)e f:(float)f {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    _transform = CGAffineTransformIdentity;
    _transform = CGAffineTransformConcat(_transform, CGAffineTransformMake(a, b, c, d, e, f));
#endif
}

-(void) resetTransform {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    _transform = CGAffineTransformIdentity;
#endif
}

-(void) clearRect:(CGRect) rect {
    if (_imageData.isNull())
        return;

    rect.origin.x = floor(rect.origin.x);
    rect.origin.y = floor(rect.origin.y);
    rect.size.width = floor(rect.size.width);
    rect.size.height = floor(rect.size.height);

    if (rect.origin.x < 0) rect.origin.x = 0;
    if (rect.origin.y < 0) rect.origin.y = 0;

    if (rect.size.width < 1 || rect.size.height < 1)
        return;
    CGContextClearRect(_context, rect);
}

-(void) clip:(NSString *)rule {
    BOOL odd = NO;
    if ([@"evenodd" isEqualToString:rule]) {
        odd = YES;
    }
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    CGContextAddPath(_context, [_path CGPath]);
    if (odd) {
        CGContextEOClip(_context);
    } else {
        CGContextClip(_context);
    }
#endif
}

-(void) fillRect:(CGRect) rect {
    [self saveContext];

    [self _setFillSetting];
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    CGRect tmpRect = CGRectMake(rect.origin.x, _height - rect.origin.y - rect.size.height, rect.size.width, rect.size.height);
    [NSBezierPath fillRect:tmpRect];
#else
    NSBezierPath* path = [NSBezierPath bezierPathWithRect:rect];
    [path fill];
#endif
    [self restoreContext];
}

-(void) saveContext {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    // save the old graphics context
    _oldGraphicsContext = [NSGraphicsContext currentContext];
    // store the current context
    [NSGraphicsContext setCurrentContext:_currentGraphicsContext];
    // push graphics state to stack
    [NSGraphicsContext saveGraphicsState];
    [[NSGraphicsContext currentContext] setShouldAntialias:YES];
#else
    // save the old graphics context
    _oldContext = UIGraphicsGetCurrentContext();
    // store the current context
    UIGraphicsPushContext(_context);
    CGContextSaveGState(_context);
#endif
}

-(void) restoreContext {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    // pop the context
    [NSGraphicsContext restoreGraphicsState];
    // reset the old graphics context
    [NSGraphicsContext setCurrentContext:_oldGraphicsContext];
    _oldGraphicsContext = nil;
#else
    // pop the context
    CGContextRestoreGState(_context);
    // reset the old graphics context
    UIGraphicsPopContext();
    _oldContext = nil;
#endif
}

-(void) beginPath {
    if (!_path) {
        _path = [NSBezierPath bezierPath];
        [_path retain];
    }
    [_path removeAllPoints];
}

-(void) closePath {
    [_path closePath];
}

-(void) stroke {
    [self saveContext];
    [self _setStrokeSetting];
    [_path stroke];
    [self restoreContext];
}

-(void) fill {
    [self saveContext];
    [self _setFillSetting];
    [_path fill];
    [self restoreContext];
}

-(void) rectWithX:(float)x y:(float)y width:(float)width height:(float)height {
    if (!_path) {
        [self beginPath];
    }
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    [_path appendBezierPathWithRect:NSMakeRect(x, y, width, height)];
#else
    [_path appendPath:[UIBezierPath bezierPathWithRect:NSMakeRect(x, y, width, height)]];
#endif
}

-(void) quadraticCurveToControlPointX:(float)cpx controlPointY:(float)cpy x:(float)x y:(float)y {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    [_path addQuadCurveToPoint:NSMakePoint(x, y) controlPoint:NSMakePoint(cpx, cpy)];
#endif
}

-(void) bezierCurveToX1:(float)x1 y1:(float)y1 x2:(float)x2 y2:(float)y2 x3:(float)x3 y3:(float)y3 {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    [_path addCurveToPoint:NSMakePoint(x3, y3) controlPoint1:NSMakePoint(x1, y1) controlPoint2:NSMakePoint(x2, y2)];
#endif
}

-(void) arcWithX:(float)x y:(float)y radius:(float)radius startAngle:(float)startAngle endAngle:(float)endAngle anticlockwise:(BOOL)anticlockwise {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    [_path addArcWithCenter:CGPointMake(x, y)
                     radius:radius
                 startAngle:startAngle
                   endAngle:endAngle
                  clockwise:!anticlockwise];
#endif
}

-(void) arcToX1:(float)x1 y1:(float)y1 x2:(float)x2 y2:(float)y2 radius:(float)radius {
    if ([_path isEmpty]) {
        return;
    }
    CGPoint currentPoint = [_path currentPoint];
    float a1 = currentPoint.y - y1;
    float b1 = currentPoint.x - x1;
    float a2 = y2 - y1;
    float b2 = x2 - x1;
    float mm = fabsf(a1 * b2 - b1 * a2);
    if (mm < FLT_MIN || radius == 0) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
        [_path addLineToPoint:NSMakePoint(x1, y1)];
#endif
        return;
    }
    
    float dd = a1 * a1 + b1 * b1;
    float cc = a2 * a2 + b2 * b2;
    float tt = a1 * a2 + b1 * b2;
    float k1 = (float) (radius * sqrt(dd) / mm);
    float k2 = (float) (radius * sqrt(cc) / mm);
    float j1 = k1 * tt / dd;
    float j2 = k2 * tt / cc;
    float cx = k1 * b2 + k2 * b1;
    float cy = k1 * a2 + k2 * a1;
    float px = b1 * (k2 + j1);
    float py = a1 * (k2 + j1);
    float qx = b2 * (k1 + j2);
    float qy = a2 * (k1 + j2);
    float startAngle = (float) atan2(py - cy, px - cx);
    float endAngle = (float) atan2(qy - cy, qx - cx);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    [_path addArcWithCenter:CGPointMake(cx + x1, cy + y1)
                     radius:radius
                 startAngle:startAngle
                   endAngle:endAngle
                  clockwise:!(b1 * a2 > b2 * a1)];
#endif
}

static void _readEllipsePointFromBezier(void *info, const CGPathElement *element){
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    UIBezierPath *bezierPath = (UIBezierPath *)info;
    CGPathElementType type = element->type;
    CGPoint *points = element->points;
    switch (type) {
        case kCGPathElementMoveToPoint:
        {
            CGPoint pos = points[0];
            if ([bezierPath isEmpty]) {
                [bezierPath moveToPoint:pos];
            } else {
                [bezierPath addLineToPoint:pos];
            }
        }
            break;
        case kCGPathElementAddLineToPoint:
        {
            CGPoint pos = points[0];
            [bezierPath addLineToPoint:pos];
        }
            break;
        case kCGPathElementAddQuadCurveToPoint:
        {
            CGPoint cp = points[0];
            CGPoint pos = points[1];
            [bezierPath addQuadCurveToPoint:pos controlPoint:cp];
        }
            break;
        case kCGPathElementAddCurveToPoint:
        {
            CGPoint cp1 = points[0];
            CGPoint cp2 = points[1];
            CGPoint pos = points[2];
            [bezierPath addCurveToPoint:pos controlPoint1:cp1 controlPoint2:cp2];
        }
            break;
        case kCGPathElementCloseSubpath:
        {
            [bezierPath closePath];
        }
            break;
            
        default:
            break;
    }
#endif
}

-(void) ellipseWithX:(float)x y:(float)y radiusX:(float)radiusX radiusY:(float)radiusY rotation:(float)rotation startAngle:(float)startAngle endAngle:(float)endAngle anticlockwise:(BOOL)anticlockwise {
    if (!anticlockwise && endAngle - startAngle >= 2 * M_PI) {
        endAngle = 2 * M_PI - startAngle;
    } else if (anticlockwise && startAngle - endAngle >= 2 * M_PI) {
        startAngle = 2 * M_PI - endAngle;
    }
    float scaleX = radiusX / radiusY;
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    UIBezierPath *arcPath = [UIBezierPath bezierPathWithArcCenter:CGPointMake(x, y)
                                                           radius:radiusY
                                                       startAngle:startAngle
                                                         endAngle:endAngle
                                                        clockwise:!anticlockwise];
    // translate
    CGAffineTransform transform = CGAffineTransformMakeTranslation(-x, -y);
    [arcPath applyTransform:transform];
    // scale
    transform = CGAffineTransformMakeScale(scaleX, 1.0);
    [arcPath applyTransform:transform];
    // rotation
    transform = CGAffineTransformMakeRotation(rotation);
    [arcPath applyTransform:transform];
    // translate
    transform = CGAffineTransformMakeTranslation(x, y);
    [arcPath applyTransform:transform];
    CGPathApply(arcPath.CGPath, (__bridge void *)_path, _readEllipsePointFromBezier);
#endif
}

-(void) setGlobalCompositeOperation:(NSString *)compositeOperation {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    _blendMode = kCGBlendModeNormal;
    if ([@"xor" isEqualToString:compositeOperation]) {
        _blendMode = kCGBlendModeXOR;
    } else if ([@"overlay" isEqualToString:compositeOperation]) {
        _blendMode = kCGBlendModeOverlay;
    } else if ([@"darken" isEqualToString:compositeOperation]) {
        _blendMode = kCGBlendModeDarken;
    } else if ([@"lighten" isEqualToString:compositeOperation]) {
        _blendMode = kCGBlendModeLighten;
    } else if ([@"screen" isEqualToString:compositeOperation]) {
        _blendMode = kCGBlendModeScreen;
    } else if ([@"destination-over" isEqualToString:compositeOperation]) {
        _blendMode = kCGBlendModeDestinationOver;
    } else if ([@"destination-in" isEqualToString:compositeOperation]) {
        _blendMode = kCGBlendModeDestinationIn;
    } else if ([@"destination-out" isEqualToString:compositeOperation]) {
        _blendMode = kCGBlendModeDestinationOut;
    } else if ([@"destination-atop" isEqualToString:compositeOperation]) {
        _blendMode = kCGBlendModeDestinationAtop;
    } else if ([@"source-in" isEqualToString:compositeOperation]) {
        _blendMode = kCGBlendModeSourceIn;
    } else if ([@"source-out" isEqualToString:compositeOperation]) {
        _blendMode = kCGBlendModeSourceOut;
    } else if ([@"source-atop" isEqualToString:compositeOperation]) {
        _blendMode = kCGBlendModeSourceAtop;
    }
    CGContextSetBlendMode(_context, _blendMode);
#endif
}

-(void) setLineCap:(NSString *)lineCap {
    _lineCap = lineCap;
}

-(void) setLineJoin:(NSString *)lineJoin {
    _lineJoin = lineJoin;
}

-(void) setLineDash:(const std::vector<float> &)lineDash {
    _lineDashPattern.clear();
    _lineDashPattern.assign(lineDash.begin(), lineDash.end());
    if (lineDash.size() % 2 > 0) {
        _lineDashPattern.insert(_lineDashPattern.end(), lineDash.begin(), lineDash.end());
    }
}

-(const std::vector<float> &) getLineDash {
    return _lineDashPattern;
}

-(void) moveToX: (float) x y:(float) y {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    [_path moveToPoint: NSMakePoint(x, _height - y)];
#else
    [_path moveToPoint: NSMakePoint(x, y)];
#endif
}

-(void) lineToX: (float) x y:(float) y {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    [_path lineToPoint: NSMakePoint(x, _height - y)];
#else
    [_path addLineToPoint: NSMakePoint(x, y)];
#endif
}

-(void) drawImage:(const cocos2d::Data &)imageData sx:(float)sx sy:(float)sy sw:(float)sw sh:(float)sh dx:(float)dx dy:(float)dy dw:(float)dw dh:(float)dh ow:(float)ow oh:(float)oh {
    [self saveContext];
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    // generate CGImageRef
    CGDataProviderRef provider = CGDataProviderCreateWithData(nullptr, imageData.getBytes(), imageData.getSize(), nullptr);
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    size_t bitsPerComponent = 8;
    size_t bitsPerPixel = bitsPerComponent * 4;
    size_t bytesPerRow = ow * 4;
    CGImageRef imgRef = CGImageCreate(ow,
                                      oh,
                                      bitsPerComponent,
                                      bitsPerPixel,
                                      bytesPerRow,
                                      colorSpaceRef,
                                      kCGImageAlphaPremultipliedLast,
                                      provider,
                                      nullptr,
                                      false,
                                      kCGRenderingIntentDefault);
    // the transform should be applied at first
    [self _applyCTM];
    CGContextSetAlpha(_context, _globalAlpha);
    // image is upside-down
    CGContextScaleCTM(_context, 1.0f, -1.0f);
    CGContextTranslateCTM(_context, 0.0f, -_height);
    // the area be drew
    CGRect clip = CGRectMake(sx, sy, sw, sh);
    CGImageRef imgClipRef = CGImageCreateWithImageInRect(imgRef, clip);
    CGImageRelease(imgRef);
    // the position when draw on canvas
    // because translate the context so y shoule set to _height - dh - dy
    CGRect rect = CGRectMake(dx, _height - dh - dy, dw, dh);
    CGContextDrawImage(_context, rect, imgClipRef);
    CGImageRelease(imgClipRef);
    CGColorSpaceRelease(colorSpaceRef);
    CGDataProviderRelease(provider);
#endif
    [self restoreContext];
}

static void _drawPatternTile(const cocos2d::Data & patternImageData,
                                            float width,
                                            float height,
                                            CGContextRef context) {
    
    CGDataProviderRef provider = CGDataProviderCreateWithData(nullptr, patternImageData.getBytes(), patternImageData.getSize(), nullptr);
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    size_t bitsPerComponent = 8;
    size_t bitsPerPixel = bitsPerComponent * 4;
    size_t bytesPerRow = width * 4;
    CGImageRef patternImage = CGImageCreate(width,
                                            height,
                                            bitsPerComponent,
                                            bitsPerPixel,
                                            bytesPerRow,
                                            colorSpaceRef,
                                            kCGImageAlphaPremultipliedLast,
                                            provider,
                                            nullptr,
                                            false,
                                            kCGRenderingIntentDefault);
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    UIImage *image = [UIImage imageWithCGImage:(CGImage *)patternImage];
    CGRect rect = CGRectMake(0,
                             0,
                             image.size.width,
                             image.size.height);
    CGContextTranslateCTM(context, 0, image.size.height);
    CGContextScaleCTM(context, 1.0, -1.0);
    CGContextDrawImage(context, rect, [image CGImage]);
#endif
    CGImageRelease(patternImage);
    CGColorSpaceRelease(colorSpaceRef);
    CGDataProviderRelease(provider);
}

static void _drawLinearGradientTile(CGGradientRef gradient, float width, float height, CGPoint startPoint, CGPoint endPoint, CGContextRef context) {
    CGContextDrawLinearGradient(context, gradient, startPoint, endPoint, kCGGradientDrawsBeforeStartLocation | kCGGradientDrawsAfterEndLocation);
}

static void _drawRadialGradientTile(CGGradientRef gradient, float width, float height, CGPoint startPoint, float startR, CGPoint endPoint, float endR, CGContextRef context) {
    CGContextDrawRadialGradient (context,
                                 gradient,
                                 startPoint,
                                 startR,
                                 endPoint,
                                 endR,
                                 kCGGradientDrawsBeforeStartLocation | kCGGradientDrawsAfterEndLocation);
}

static void _drawStrokePatternTile(void *info, CGContextRef context) {
    CanvasRenderingContext2DImpl *impl = (CanvasRenderingContext2DImpl *)info;
    if (!impl) {
        return;
    }
    cocos2d::Data patternImageData = impl.strokePatternData;
    float patternImageWidth = impl.strokePatternImageWidth;
    float patternImageHeight = impl.strokePatternImageHeight;
    _drawPatternTile(patternImageData,
                     patternImageWidth,
                     patternImageHeight,
                     context);

}

static void _drawFillPatternTile(void *info, CGContextRef context) {
    CanvasRenderingContext2DImpl *impl = (CanvasRenderingContext2DImpl *)info;
    if (!impl) {
        return;
    }
    cocos2d::Data patternImageData = impl.fillPatternData;
    float patternImageWidth = impl.fillPatternImageWidth;
    float patternImageHeight = impl.fillPatternImageHeight;
    _drawPatternTile(patternImageData,
                     patternImageWidth,
                     patternImageHeight,
                     context);
    
}

static void _drawFillLinearGradientTile(void *info, CGContextRef context) {
    CanvasRenderingContext2DImpl *impl = (CanvasRenderingContext2DImpl *)info;
    if (!impl) {
        return;
    }
    CGGradientRef gradient = [impl fillLinearGradient];
    CGPoint startPoint = [impl fillLinearGradientStart];
    CGPoint endPoint = [impl fillLinearGradientEnd];
    CGFloat width = [impl width];
    CGFloat height = [impl height];
    _drawLinearGradientTile(gradient, width, height, startPoint, endPoint, context);
}

static void _drawStrokeLinearGradientTile(void *info, CGContextRef context) {
    CanvasRenderingContext2DImpl *impl = (CanvasRenderingContext2DImpl *)info;
    if (!impl) {
        return;
    }
    CGGradientRef gradient = [impl strokeLinearGradient];
    CGPoint startPoint = [impl strokeLinearGradientStart];
    CGPoint endPoint = [impl strokeLinearGradientEnd];
    CGFloat width = [impl width];
    CGFloat height = [impl height];
    _drawLinearGradientTile(gradient, width, height, startPoint, endPoint, context);
}

static void _drawFillRadialGradientTile(void *info, CGContextRef context) {
    CanvasRenderingContext2DImpl *impl = (CanvasRenderingContext2DImpl *)info;
    if (!impl) {
        return;
    }
    CGGradientRef gradient = [impl fillRadialGradient];
    CGPoint startPoint = [impl fillRadialGradientStart];
    float startR = [impl fillRadialGradientStartR];
    CGPoint endPoint = [impl fillRadialGradientEnd];
    float endR = [impl fillRadialGradientEndR];
    CGFloat width = [impl width];
    CGFloat height = [impl height];
    _drawRadialGradientTile(gradient,
                            width,
                            height,
                            startPoint,
                            startR,
                            endPoint,
                            endR,
                            context);
}

static void _drawStrokeRadialGradientTile(void *info, CGContextRef context) {
    CanvasRenderingContext2DImpl *impl = (CanvasRenderingContext2DImpl *)info;
    if (!impl) {
        return;
    }
    CGGradientRef gradient = [impl strokeRadialGradient];
    CGPoint startPoint = [impl strokeRadialGradientStart];
    float startR = [impl strokeRadialGradientStartR];
    CGPoint endPoint = [impl strokeRadialGradientEnd];
    float endR = [impl strokeRadialGradientEndR];
    CGFloat width = [impl width];
    CGFloat height = [impl height];
    _drawRadialGradientTile(gradient,
                            width,
                            height,
                            startPoint,
                            startR,
                            endPoint,
                            endR,
                            context);
}

-(void) applyStylePatternWithIsFillStyle:(BOOL)isFillStyle rule:(NSString *)rule image:(const cocos2d::Data &)image width:(float)imageWidth height:(float)imageHeight {
    // handle param
    if (isFillStyle) {
        [self _clearApplyFillStyle];
        _fillPatternData = image;
        _fillPatternImageWidth = imageWidth;
        _fillPatternImageHeight = imageHeight;
    } else {
        [self _clearApplyStrokeStyle];
        _strokePatternData = image;
        _strokePatternImageWidth = imageWidth;
        _strokePatternImageHeight = imageHeight;
    }
    
    
    float xStep = imageWidth;
    float yStep = imageHeight;
    if ([@"repeat-x" isEqualToString:rule]) {
        yStep = 0;
    } else if ([@"repeat-y" isEqualToString:rule]) {
        xStep = 0;
    } else if ([@"no-repeat" isEqualToString:rule]) {
        xStep = 0;
        yStep = 0;
    }
    
    //填充模式回调函数结构体
    CGPatternCallbacks callback = {
        0,
        isFillStyle ? _drawFillPatternTile : _drawStrokePatternTile,
        NULL
    };
    
    NSColor *colorPattern = [self _generateColorWithCallBack:callback
                                                       xStep:xStep
                                                       yStep:yStep
                                                       width:imageWidth
                                                      height:imageHeight];
    if (isFillStyle) {
        _fillPattern = colorPattern;
    } else {
        _strokePattern = colorPattern;
    }
}

-(void) applyStyleLinearGradientWithIsFillStyle:(BOOL)isFillStyle x0:(float)x0 y0:(float)y0 x1:(float)x1 y1:(float)y1 pos:(const std::vector<float> &)posVec color:(const std::vector<std::string> &)colorVec {
    unsigned long size = posVec.size();
    if (size < 1) {
        return;
    }
    CGFloat positions[size];
    CGFloat components[size * 4];
    CSSColorParser::Color color;
    for (int i = 0; i < size; i++) {
        positions[i] = posVec[i];
        color = CSSColorParser::parse(colorVec[i]);
        components[i * 4] = color.r;
        components[i * 4 + 1] = color.g;
        components[i * 4 + 2] = color.b;
        components[i * 4 + 3] = color.a;
    }
    CGColorSpaceRef myColorspace = CGColorSpaceCreateDeviceRGB();
    CGGradientRef linearGradient = CGGradientCreateWithColorComponents(myColorspace,
                                                                       components,
                                                                       positions,
                                                                       size);
    CGColorSpaceRelease(myColorspace);
    
    CGPatternCallbacks callback = {
        0,
        isFillStyle ? _drawFillLinearGradientTile : _drawStrokeLinearGradientTile,
        NULL
    };
    NSColor *colorPattern = [self _generateColorWithCallBack:callback
                                                       xStep:0
                                                       yStep:0
                                                       width:_width
                                                      height:_height];
    CGPoint startPoint = CGPointMake(x0, y0);
    CGPoint endPoint = CGPointMake(x1, y1);
    if (isFillStyle) {
        [self _clearApplyFillStyle];
        _fillLinearGradient = linearGradient;
        _fillLinearGradientStart = startPoint;
        _fillLinearGradientEnd = endPoint;
        _fillPattern = colorPattern;
    } else {
        [self _clearApplyStrokeStyle];
        _strokeLinearGradient = linearGradient;
        _strokeLinearGradientStart = startPoint;
        _strokeLinearGradientEnd = endPoint;
        _strokePattern = colorPattern;
    }
}

-(void) applyStyleRadialGradientWithIsFillStyle:(BOOL)isFillStyle x0:(float)x0 y0:(float)y0 r0:(float)r0 x1:(float)x1 y1:(float)y1 r1:(float)r1 pos:(const std::vector<float> &)posVec color:(const std::vector<std::string> &)colorVec {
    unsigned long size = posVec.size();
    if (size < 1) {
        return;
    }
    CGFloat positions[size];
    CGFloat components[size * 4];
    CSSColorParser::Color color;
    for (int i = 0; i < size; i++) {
        positions[i] = posVec[i];
        color = CSSColorParser::parse(colorVec[i]);
        components[i * 4] = color.r;
        components[i * 4 + 1] = color.g;
        components[i * 4 + 2] = color.b;
        components[i * 4 + 3] = color.a;
    }
    CGColorSpaceRef myColorspace = CGColorSpaceCreateDeviceRGB();
    CGGradientRef gradient = CGGradientCreateWithColorComponents(myColorspace,
                                                                       components,
                                                                       positions,
                                                                       size);
    CGColorSpaceRelease(myColorspace);
    
    CGPatternCallbacks callback = {
        0,
        isFillStyle ? _drawFillRadialGradientTile : _drawStrokeRadialGradientTile,
        NULL
    };
    NSColor *colorPattern = [self _generateColorWithCallBack:callback
                                                       xStep:0
                                                       yStep:0
                                                       width:_width
                                                      height:_height];
    CGPoint startPoint = CGPointMake(x0, y0);
    CGPoint endPoint = CGPointMake(x1, y1);
    if (isFillStyle) {
        [self _clearApplyFillStyle];
        _fillRadialGradient = gradient;
        _fillRadialGradientStart = startPoint;
        _fillRadialGradientStartR = r0;
        _fillRadialGradientEnd = endPoint;
        _fillRadialGradientEndR = r1;
        _fillPattern = colorPattern;
    }else {
        [self _clearApplyStrokeStyle];
        _strokeRadialGradient = gradient;
        _strokeRadialGradientStart = startPoint;
        _strokeRadialGradientStratR = r0;
        _strokeRadialGradientEnd = endPoint;
        _strokeRadialGradientEndR = r1;
        _strokePattern = colorPattern;
    }
}

-(NSColor *) _generateColorWithCallBack:(CGPatternCallbacks)callback xStep:(float)xStep yStep:(float)yStep width:(float)width height:(float)height {
    CGAffineTransform trans = CGAffineTransformMakeTranslation(0, _height);
    trans = CGAffineTransformScale(trans, 1.0, -1.0f);
    CGPatternRef patternRef = CGPatternCreate(self,
                                              CGRectMake(0, 0, width, height),
                                              trans,
                                              xStep,
                                              yStep,
                                              kCGPatternTilingNoDistortion,
                                              YES,
                                              &callback);
    CGColorSpaceRef colorSpacePattern = CGColorSpaceCreatePattern(NULL);
    CGFloat alpha = 1;
    CGColorRef colorRef = CGColorCreateWithPattern(colorSpacePattern, patternRef, &alpha);
    NSColor *colorPattern = [NSColor colorWithCGColor:colorRef];
    CGColorRelease(colorRef);
    CGColorSpaceRelease(colorSpacePattern);
    CGPatternRelease(patternRef);
    return colorPattern;
}

-(void) _fillImageData:(const cocos2d::Data &)imageData width:(float)imageWidth height:(float)imageHeight offsetX:(float)offsetX offsetY:(float)offsetY {
    // check param
    if (_width < 1.0f || _height < 1.0f) {
        // do nothing
        return;
    }
    if (offsetX < 0) {
        @throw [NSException exceptionWithName:NSInvalidArgumentException reason:@"x must be >= 0" userInfo:nil];
    }
    if (offsetY < 0) {
        @throw [NSException exceptionWithName:NSInvalidArgumentException reason:@"y must be >= 0" userInfo:nil];
    }
    if (imageWidth == 0 || imageHeight == 0) {
        // do nothing
        return;
    }
    if (imageWidth < 0) {
        @throw [NSException exceptionWithName:NSInvalidArgumentException reason:@"width must be >= 0" userInfo:nil];
    }
    if (imageHeight < 0) {
        @throw [NSException exceptionWithName:NSInvalidArgumentException reason:@"height must be >= 0" userInfo:nil];
    }
    if (offsetX + imageWidth > _width) {
        @throw [NSException exceptionWithName:NSInvalidArgumentException reason:@"x + width must be <= total width" userInfo:nil];
    }
    if (offsetY + imageHeight > _height) {
        @throw [NSException exceptionWithName:NSInvalidArgumentException reason:@"y + height must be <= total height" userInfo:nil];
    }
    
    uint8_t *fillColors = imageData.getBytes();
    uint8_t *imageDataTemp = _imageData.getBytes();
    uint32_t yBegin = offsetY;
    uint32_t yEnd = offsetY + imageHeight;
    uint32_t bytesPerRow = imageWidth * 4;
    uint32_t index;
    for (uint32_t yIndex = yBegin; yIndex < yEnd; ++yIndex)
    {
        index = (_width * (yIndex - offsetY)) * 4;
        memcpy(imageDataTemp + yIndex * bytesPerRow, fillColors + index, bytesPerRow);
    }
}

-(void) _applyCTM {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    CGContextConcatCTM(_context, _transform);
#endif
}

-(void) _setStrokeSetting {
    NSColor* color = [NSColor colorWithRed:_strokeStyle.r green:_strokeStyle.g blue:_strokeStyle.b alpha:_strokeStyle.a];
    if (_strokePattern) {
        color = _strokePattern;
    }
    [color setStroke];
    [_path setLineWidth: _lineWidth];
    // shadow
    if (_shadowColor) {
        CGContextSetShadowWithColor(_context,
                                    CGSizeMake(_shadowOffsetX, _shadowOffsetY),
                                    _shadowBlur,
                                    _shadowColor.CGColor);
    }
    // cap
    if ([@"square" isEqualToString:_lineCap]) {
        [_path setLineCapStyle:NSSquareLineCapStyle];
    } else if ([@"round" isEqualToString:_lineCap]) {
        [_path setLineCapStyle:NSRoundLineCapStyle];
    } else {
        [_path setLineCapStyle:NSButtLineCapStyle];
    }
    // join
    if ([@"bevel" isEqualToString:_lineJoin]) {
        [_path setLineJoinStyle:NSBevelLineJoinStyle];
    } else if ([@"round" isEqualToString:_lineJoin]) {
        [_path setLineJoinStyle:NSRoundLineJoinStyle];
    } else {
        [_path setLineJoinStyle:NSMiterLineJoinStyle];
    }
    // dash
    NSInteger lineDashCount = _lineDashPattern.size();
    if (lineDashCount > 0) {
        CGFloat lineDash[lineDashCount];
        for (int i = 0; i < lineDashCount; i++) {
            lineDash[i] = _lineDashPattern[i];
        }
        [_path setLineDash:lineDash count:lineDashCount phase:_lineDashOffset];
    } else {
        [_path setLineDash:NULL count:0 phase:0];
    }
    // miterLimit
    [_path setMiterLimit:_miterLimit];
    [self _applyCTM];
    CGContextSetAlpha(_context, _globalAlpha);
}

-(void) _setFillSetting {
    NSColor* color = [NSColor colorWithRed:_fillStyle.r green:_fillStyle.g blue:_fillStyle.b alpha:_fillStyle.a];
    if (_fillPattern) {
        color = _fillPattern;
    }
    [color setFill];
    if (_shadowColor) {
        CGContextSetShadowWithColor(_context,
                                    CGSizeMake(_shadowOffsetX, _shadowOffsetY),
                                    _shadowBlur,
                                    _shadowColor.CGColor);
    }
    [self _applyCTM];
    CGContextSetAlpha(_context, _globalAlpha);
}

-(void) _clearApplyStrokeStyle {
    // reset pattern
    _strokePatternData.clear();
    _strokePatternImageWidth = 0;
    _strokePatternImageHeight = 0;
    if (_strokePattern) {
        _strokePattern = nil;
    }
    // reset linear gradient
    if (_strokeLinearGradient) {
        CGGradientRelease(_strokeLinearGradient);
        _strokeLinearGradient = nullptr;
    }
    // reset radial gradient
    if (_strokeRadialGradient) {
        CGGradientRelease(_strokeRadialGradient);
        _strokeRadialGradient = nullptr;
    }
}

-(void) _clearApplyFillStyle {
    // reset pattern
    _fillPatternData.clear();
    _fillPatternImageWidth = 0;
    _fillPatternImageHeight = 0;
    if (_fillPattern) {
        _fillPattern = nil;
    }
    // reset linear gradient
    if (_fillLinearGradient) {
        CGGradientRelease(_fillLinearGradient);
        _fillLinearGradient = nullptr;
    }
    // reset radial gradient
    if (_fillRadialGradient) {
        CGGradientRelease(_fillRadialGradient);
        _fillRadialGradient = nullptr;
    }
}
-(void) _resetSetting {
    _textAlign = CanvasTextAlign::LEFT;
    _textBaseLine = CanvasTextBaseline::BOTTOM;
    _lineCap = @"butt";
    _lineJoin = @"miter";
    _lineWidth = 1.0;
    _fillStyle.r = 0;
    _fillStyle.g = 0;
    _fillStyle.b = 0;
    _fillStyle.a = 1;
    _strokeStyle.r = 0;
    _strokeStyle.g = 0;
    _strokeStyle.b = 0;
    _strokeStyle.a = 1;
    _lineDashOffset = 0;
    _lineDashPattern.clear();
    _miterLimit = 10.f;
    [self _clearApplyStrokeStyle];
    [self _clearApplyFillStyle];
    _globalAlpha = 1.0f;
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#else
    _transform = CGAffineTransformIdentity;
    _blendMode = kCGBlendModeNormal;
#endif
}

@end

NS_CC_BEGIN
// CanvasRenderingContext2D

static std::map<std::string, bool> s_globalCompositeOperationMap;

CanvasRenderingContext2D::CanvasRenderingContext2D(float width, float height)
: __width(width)
, __height(height)
{
//    SE_LOGD("CanvasRenderingContext2D constructor: %p, width: %f, height: %f\n", this, width, height);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_maxTextureSize);
    _impl = [[CanvasRenderingContext2DImpl alloc] init];
    recreateBufferIfNeeded();
    
    s_globalCompositeOperationMap["source-over"] = true;
    s_globalCompositeOperationMap["source-in"] = true;
    s_globalCompositeOperationMap["source-out"] = true;
    s_globalCompositeOperationMap["source-atop"] = true;
    s_globalCompositeOperationMap["destination-over"] = true;
    s_globalCompositeOperationMap["destination-in"] = true;
    s_globalCompositeOperationMap["destination-out"] = true;
    s_globalCompositeOperationMap["destination-atop"] = true;
    s_globalCompositeOperationMap["xor"] = true;
    s_globalCompositeOperationMap["screen"] = true;
    s_globalCompositeOperationMap["overlay"] = true;
    s_globalCompositeOperationMap["darken"] = true;
    s_globalCompositeOperationMap["lighten"] = true;
}

CanvasRenderingContext2D::~CanvasRenderingContext2D()
{
//    SE_LOGD("CanvasRenderingContext2D destructor: %p\n", this);
    [_impl release];
}

bool CanvasRenderingContext2D::recreateBufferIfNeeded()
{
    if (_isBufferSizeDirty)
    {
        _isBufferSizeDirty = false;
//        SE_LOGD("CanvasRenderingContext2D::recreateBufferIfNeeded %p, w: %f, h:%f\n", this, __width, __height);
        if (__width > _maxTextureSize || __height > _maxTextureSize) {
            return false;
        }
        [_impl recreateBufferWithWidth: __width height:__height];
        if (_canvasBufferUpdatedCB != nullptr)
            _canvasBufferUpdatedCB([_impl getDataRef]);
    }
    return true;
}

void CanvasRenderingContext2D::clearRect(float x, float y, float width, float height)
{
//    SE_LOGD("CanvasGradient::clearRect: %p, %f, %f, %f, %f\n", this, x, y, width, height);
    if (recreateBufferIfNeeded()) {
        [_impl clearRect:CGRectMake(x, y, width, height)];
    } else {
        SE_LOGE("[ERROR] CanvasRenderingContext2D clearRect width:%d, height:%d is out of GL_MAX_TEXTURE_SIZE",
                __width, __height);
    }
}

void CanvasRenderingContext2D::fillRect(float x, float y, float width, float height)
{
    if (recreateBufferIfNeeded()) {
        [_impl fillRect:CGRectMake(x, y, width, height)];

        if (_canvasBufferUpdatedCB != nullptr)
        {
            _canvasBufferUpdatedCB([_impl getDataRef]);
        }
    } else {
        SE_LOGE("[ERROR] CanvasRenderingContext2D fillRect width:%d, height:%d is out of GL_MAX_TEXTURE_SIZE",
                __width, __height);
    }
}

void CanvasRenderingContext2D::strokeRect(float x, float y, float width, float height)
{
    if (recreateBufferIfNeeded()) {
        [_impl rectWithX:x y:y width:width height:height];
        [_impl stroke];
        if (_canvasBufferUpdatedCB != nullptr) {
            _canvasBufferUpdatedCB([_impl getDataRef]);
        }
    } else {
        SE_LOGE("[ERROR] CanvasRenderingContext2D strokeRect width:%f, height:%f is out of GL_MAX_TEXTURE_SIZE",
                __width, __height);
    }
}

void CanvasRenderingContext2D::fillText(const std::string& text, float x, float y, float maxWidth)
{
//    SE_LOGD("CanvasRenderingContext2D(%p)::fillText: %s, %f, %f, %f\n", this, text.c_str(), x, y, maxWidth);
    if (text.empty())
        return;

    if (recreateBufferIfNeeded()) {
        [_impl fillText:[NSString stringWithUTF8String:text.c_str()] x:x y:y maxWidth:maxWidth];
        if (_canvasBufferUpdatedCB != nullptr)
            _canvasBufferUpdatedCB([_impl getDataRef]);
    } else {
        SE_LOGE("[ERROR] CanvasRenderingContext2D fillText width:%d, height:%d is out of GL_MAX_TEXTURE_SIZE",
                __width, __height);
    }
}

void CanvasRenderingContext2D::strokeText(const std::string& text, float x, float y, float maxWidth)
{
//    SE_LOGD("CanvasRenderingContext2D(%p)::strokeText: %s, %f, %f, %f\n", this, text.c_str(), x, y, maxWidth);
    if (text.empty())
        return;
    if (recreateBufferIfNeeded()) {
        [_impl strokeText:[NSString stringWithUTF8String:text.c_str()] x:x y:y maxWidth:maxWidth];

        if (_canvasBufferUpdatedCB != nullptr)
            _canvasBufferUpdatedCB([_impl getDataRef]);
    } else {
        SE_LOGE("[ERROR] CanvasRenderingContext2D strokeText width:%d, height:%d is out of GL_MAX_TEXTURE_SIZE",
                __width, __height);
    }
}

cocos2d::Size CanvasRenderingContext2D::measureText(const std::string& text)
{
//    SE_LOGD("CanvasRenderingContext2D::measureText: %s\n", text.c_str());
    CGSize size = [_impl measureText: [NSString stringWithUTF8String:text.c_str()]];
    return cocos2d::Size(size.width, size.height);
}

void CanvasRenderingContext2D::save()
{
    [_impl saveContext];
}

void CanvasRenderingContext2D::beginPath()
{
    [_impl beginPath];
}

void CanvasRenderingContext2D::closePath()
{
    [_impl closePath];
}

void CanvasRenderingContext2D::moveTo(float x, float y)
{
    [_impl moveToX:x y:y];
}

void CanvasRenderingContext2D::lineTo(float x, float y)
{
    [_impl lineToX:x y:y];
}

void CanvasRenderingContext2D::quadraticCurveTo(float x1, float y1, float x2, float y2)
{
    [_impl quadraticCurveToControlPointX:x1 controlPointY:y1 x:x2 y:y2];
}

void CanvasRenderingContext2D::bezierCurveTo(float x1, float y1, float x2, float y2, float x3, float y3)
{
    [_impl bezierCurveToX1:x1 y1:y1 x2:x2 y2:y2 x3:x3 y3:y3];
}

void CanvasRenderingContext2D::arc(float x1, float y1, float radius, float startAngle, float endAngle, bool anticlockwise)
{
    [_impl arcWithX:x1
                  y:y1
             radius:radius
         startAngle:startAngle
           endAngle:endAngle
      anticlockwise:anticlockwise];
}

void CanvasRenderingContext2D::arcTo(float x1, float y1, float x2, float y2, float radius)
{
    [_impl arcToX1:x1
                y1:y1
                x2:x2
                y2:y2
            radius:radius];
}

void CanvasRenderingContext2D::stroke()
{
    [_impl stroke];

    if (_canvasBufferUpdatedCB != nullptr)
        _canvasBufferUpdatedCB([_impl getDataRef]);
}

void CanvasRenderingContext2D::fill()
{
    if (recreateBufferIfNeeded()) {
        [_impl fill];
        
        if (_canvasBufferUpdatedCB != nullptr) {
            _canvasBufferUpdatedCB([_impl getDataRef]);
        }
    } else {
        SE_LOGE("[ERROR] CanvasRenderingContext2D fill width:%f height:%f is out of GL_MAX_TEXTURE_SIZE",
                __width, __height);
    }
}

void CanvasRenderingContext2D::rect(float x, float y, float w, float h)
{
    [_impl rectWithX:x y:y width:w height:h];
}

void CanvasRenderingContext2D::restore()
{
    [_impl restoreContext];
}

void CanvasRenderingContext2D::setCanvasBufferUpdatedCallback(const CanvasBufferUpdatedCallback& cb)
{
    _canvasBufferUpdatedCB = cb;
}

void CanvasRenderingContext2D::set__width(float width)
{
//    SE_LOGD("CanvasRenderingContext2D::set__width: %f\n", width);
    __width = width;
    _isBufferSizeDirty = true;
    recreateBufferIfNeeded();
}

void CanvasRenderingContext2D::set__height(float height)
{
//    SE_LOGD("CanvasRenderingContext2D::set__height: %f\n", height);
    __height = height;
    _isBufferSizeDirty = true;
    recreateBufferIfNeeded();
}

void CanvasRenderingContext2D::set_lineWidthInternal(float lineWidth)
{
    _lineWidthInternal = lineWidth;
    _impl.lineWidth = _lineWidthInternal;
}

void CanvasRenderingContext2D::set_lineCap(const std::string& lineCap)
{
    if (lineCap != "butt" && lineCap != "round" &&lineCap != "square") {
        return;
    }
    this->_lineCap = lineCap;
    [_impl setLineCap:[NSString stringWithUTF8String:lineCap.c_str()]];
}

void CanvasRenderingContext2D::set_lineJoin(const std::string& lineJoin)
{
    if (lineJoin != "bevel" && lineJoin != "round" &&lineJoin != "miter") {
        return;
    }
    this->_lineJoin = lineJoin;
    [_impl setLineJoin:[NSString stringWithUTF8String:lineJoin.c_str()]];
}

void CanvasRenderingContext2D::set_font(const std::string& font)
{
    if (_font != font)
    {
        _font = font;

        std::string boldStr;
        std::string fontName = "Arial";
        std::string fontSizeStr = "30";

        // support get font name from `60px American` or `60px "American abc-abc_abc"`
        std::regex re("(bold)?\\s*((\\d+)([\\.]\\d+)?)px\\s+([\\w-]+|\"[\\w -]+\"$)");
        std::match_results<std::string::const_iterator> results;
        if (std::regex_search(_font.cbegin(), _font.cend(), results, re))
        {
            boldStr = results[1].str();
            fontSizeStr = results[2].str();
            fontName = results[5].str();
        }

        CGFloat fontSize = atof(fontSizeStr.c_str());
        [_impl updateFontWithName:[NSString stringWithUTF8String:fontName.c_str()] fontSize:fontSize bold:!boldStr.empty()];
    }
}

void CanvasRenderingContext2D::set_textAlign(const std::string& textAlign)
{
//    SE_LOGD("CanvasRenderingContext2D::set_textAlign: %s\n", textAlign.c_str());
    if (textAlign == "left")
    {
        _impl.textAlign = CanvasTextAlign::LEFT;
    }
    else if (textAlign == "center" || textAlign == "middle")
    {
        _impl.textAlign = CanvasTextAlign::CENTER;
    }
    else if (textAlign == "right")
    {
        _impl.textAlign = CanvasTextAlign::RIGHT;
    }
    else
    {
        assert(false);
    }
}

void CanvasRenderingContext2D::set_textBaseline(const std::string& textBaseline)
{
//    SE_LOGD("CanvasRenderingContext2D::set_textBaseline: %s\n", textBaseline.c_str());
    if (textBaseline == "top")
    {
        _impl.textBaseLine = CanvasTextBaseline::TOP;
    }
    else if (textBaseline == "middle")
    {
        _impl.textBaseLine = CanvasTextBaseline::MIDDLE;
    }
    else if (textBaseline == "bottom" || textBaseline == "alphabetic") //REFINE:, how to deal with alphabetic, currently we handle it as bottom mode.
    {
        _impl.textBaseLine = CanvasTextBaseline::BOTTOM;
    }
    else
    {
        assert(false);
    }
}

void CanvasRenderingContext2D::set_fillStyleInternal(const std::string& fillStyle)
{
    CSSColorParser::Color color = CSSColorParser::parse(fillStyle);
    [_impl setFillStyleWithRed:color.r/255.0f green:color.g/255.0f blue:color.b/255.0f alpha:color.a];
//    SE_LOGD("CanvasRenderingContext2D::set_fillStyle: %s, (%d, %d, %d, %f)\n", fillStyle.c_str(), color.r, color.g, color.b, color.a);
}

void CanvasRenderingContext2D::set_strokeStyleInternal(const std::string& strokeStyle)
{
    CSSColorParser::Color color = CSSColorParser::parse(strokeStyle);
    [_impl setStrokeStyleWithRed:color.r/255.0f green:color.g/255.0f blue:color.b/255.0f alpha:color.a];
}

void CanvasRenderingContext2D::set_globalCompositeOperation(const std::string& globalCompositeOperation)
{
    if(s_globalCompositeOperationMap.find(globalCompositeOperation) == s_globalCompositeOperationMap.end()) {
        return;
    }
    this->_globalCompositeOperation = globalCompositeOperation;
    [_impl setGlobalCompositeOperation:[NSString stringWithUTF8String:globalCompositeOperation.c_str()]];
}

void CanvasRenderingContext2D::_fillImageData(const Data& imageData, float imageWidth, float imageHeight, float offsetX, float offsetY)
{
    [_impl _fillImageData:imageData width:imageWidth height:imageHeight offsetX:offsetX offsetY:offsetY];
    if (_canvasBufferUpdatedCB != nullptr) {
        _canvasBufferUpdatedCB([_impl getDataRef]);
    }
}

// transform

void CanvasRenderingContext2D::translate(float x, float y)
{
    [_impl translateX:x y:y];
}

void CanvasRenderingContext2D::scale(float x, float y)
{
    [_impl scaleX:x y:y];
}

void CanvasRenderingContext2D::rotate(float angle)
{
    [_impl rotate:angle];
}

void CanvasRenderingContext2D::transform(float a, float b, float c, float d, float e, float f)
{
    [_impl transformA:a b:b c:c d:d e:e f:f];
}

void CanvasRenderingContext2D::setTransform(float a, float b, float c, float d, float e, float f)
{
    [_impl setTransformA:a b:b c:c d:d e:e f:f];
}

void CanvasRenderingContext2D::resetTransform()
{
    [_impl resetTransform];
}

void CanvasRenderingContext2D::setLineDash(std::vector<float>& arr) {
    [_impl setLineDash:arr];
}

std::vector<float>& CanvasRenderingContext2D::getLineDash() {
    static std::vector<float> tmp;
    tmp.clear();
    std::vector<float> lineDash = [_impl getLineDash];
    for (int i = 0; i < lineDash.size(); i++) {
        tmp.push_back(lineDash[i]);
    }
    return tmp;
}

void CanvasRenderingContext2D::set_lineDashOffsetInternal(float offset)
{
    this->_lineDashOffsetInternal = offset;
    [_impl setLineDashOffset:offset];
}

void CanvasRenderingContext2D::set_miterLimitInternal(float limit)
{
    this->_miterLimitInternal = limit;
    [_impl setMiterLimit:limit];
}

void CanvasRenderingContext2D::drawImage(const Data &image, float sx, float sy, float sw, float sh,
                                         float dx, float dy, float dw, float dh, float ow, float oh) {
    [_impl drawImage:image sx:sx sy:sy sw:sw sh:sh dx:dx dy:dy dw:dw dh:dh ow:ow oh:oh];
    if (_canvasBufferUpdatedCB != nullptr) {
        _canvasBufferUpdatedCB([_impl getDataRef]);
    }
}

void CanvasRenderingContext2D::set_shadowColor(const std::string& shadowColor)
{
    CSSColorParser::Color color = CSSColorParser::parse(shadowColor);
    _shadowColor = shadowColor;
    [_impl setShadowColorWithR:color.r/255.0
                             g:color.g/255.0
                             b:color.b/255.0
                             a:color.a];
}

void CanvasRenderingContext2D::set_shadowBlurInternal(float blur)
{
    _shadowBlurInternal = blur;
    [_impl setShadowBlur:blur*0.5];
}

void CanvasRenderingContext2D::set_shadowOffsetXInternal(float offsetX)
{
    _shadowOffsetXInternal = offsetX;
    [_impl setShadowOffsetX:offsetX];
}

void CanvasRenderingContext2D::set_shadowOffsetYInternal(float offsetY)
{
    _shadowOffsetYInternal = offsetY;
    [_impl setShadowOffsetY:-offsetY];
}

void CanvasRenderingContext2D::ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle, float endAngle, bool antiClockWise) {
    [_impl ellipseWithX:x y:y radiusX:radiusX
                radiusY:radiusY
               rotation:rotation
             startAngle:startAngle
               endAngle:endAngle
          anticlockwise:antiClockWise];
}

void CanvasRenderingContext2D::clip(std::string rule) {
    [_impl clip:[NSString stringWithUTF8String:rule.c_str()]];
}

void CanvasRenderingContext2D::_applyStyle_LinearGradient(bool isFillStyle, float x0, float y0, float x1, float y1, std::vector<float>& pos, std::vector<std::string>& color) {
    [_impl applyStyleLinearGradientWithIsFillStyle:isFillStyle
                                                x0:x0
                                                y0:y0
                                                x1:x1
                                                y1:y1
                                               pos:pos
                                             color:color];
}

void CanvasRenderingContext2D::_applyStyle_RadialGradient(bool isFillStyle, float x0, float y0, float r0, float x1, float y1, float r1, std::vector<float>& pos, std::vector<std::string>& color) {
    [_impl applyStyleRadialGradientWithIsFillStyle:isFillStyle
                                                x0:x0
                                                y0:y0
                                                r0:r0
                                                x1:x1
                                                y1:y1
                                                r1:r1
                                               pos:pos
                                             color:color];
}

void CanvasRenderingContext2D::_applyStyle_Pattern(bool isFillStyle, std::string rule, const Data& image, float width, float height) {
    [_impl applyStylePatternWithIsFillStyle:isFillStyle
                                       rule:[NSString stringWithUTF8String:rule.c_str()]
                                      image:image
                                      width:width
                                     height:height];
}

void CanvasRenderingContext2D::set_globalAlphaInternal(float alpha) {
    if(alpha < 0 || alpha > 1) {
        return;
    }
    this->_globalAlphaInternal = alpha;
    [_impl setGlobalAlpha:alpha];
}

NS_CC_END
