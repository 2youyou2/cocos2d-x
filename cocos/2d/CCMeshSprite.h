/****************************************************************************
 Copyright (c) 2011      ForzeField Studios S.L.
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.
 
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
#ifndef __CCMOTION_SPRITE_H__
#define __CCMOTION_SPRITE_H__

#include "base/CCProtocols.h"
#include "renderer/CCTexture2D.h"
#include "base/ccTypes.h"
#include "2d/CCNode.h"
#include "renderer/CCCustomCommand.h"
#ifdef EMSCRIPTEN
#include "CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN

/**
 * @addtogroup misc_nodes
 * @{
 */

/** MeshSprite.
 Creates a trailing path.
 */
class CC_DLL MeshSprite : public Node, public TextureProtocol
#ifdef EMSCRIPTEN
, public GLBufferedNode
#endif // EMSCRIPTEN
{
public:
    /** creates and initializes a motion streak with fade in seconds, minimum segments, stroke's width, color, texture filename */
    static MeshSprite* create(int rows, int columns, const std::string& path);
    /** creates and initializes a motion streak with fade in seconds, minimum segments, stroke's width, color, texture */
    static MeshSprite* create(int rows, int columns, Texture2D* texture);
    
    
    void setVertice(int row, int column, const Point &p);
    Point getVertice(int row, int column);
    
    void setTexCoord(int row, int column, const Point &tex);
    Point getTexCoord(int row, int column);
    
    
    /** color used for the tint */
    void tintWithColor(const Color3B& colors);
    
    /** Remove all living segments of the ribbon */
    void reset();
    
    /**
     * @js NA
     * @lua NA
     */
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    /**
     * @js NA
     * @lua NA
     */
    virtual void update(float delta) override;
    virtual Texture2D* getTexture() const override;
    virtual void setTexture(Texture2D *texture) override;
    /**
     * @js NA
     * @lua NA
     */
    virtual void setBlendFunc(const BlendFunc &blendFunc) override;
    /**
     * @js NA
     * @lua NA
     */
    virtual const BlendFunc& getBlendFunc() const override;
    virtual GLubyte getOpacity() const override;
    virtual void setOpacity(GLubyte opacity) override;
    virtual void setOpacityModifyRGB(bool value) override;
    virtual bool isOpacityModifyRGB() const override;
    
CC_CONSTRUCTOR_ACCESS:
    MeshSprite();
    virtual ~MeshSprite();
    
    /** initializes a motion streak with fade in seconds, minimum segments, stroke's width, color and texture filename */
    bool initWithFade(int rows, int columns, const std::string& path);
    
    /** initializes a motion streak with fade in seconds, minimum segments, stroke's width, color and texture  */
    bool initWithFade(int rows, int columns, Texture2D* texture);
    
    
protected:
    //renderer callback
    void onDraw(const Mat4 &transform, uint32_t flags);
    
    /** texture used for the motion streak */
    Texture2D* _texture;
    BlendFunc _blendFunc;
    
    unsigned int _rows;
    unsigned int _columns;
    unsigned int _total;
    unsigned int _indiceNumber;
    
    bool _dirty;
    
    // Opengl
    Vec2* _vertices;
    GLubyte* _colorPointer;
    Tex2F* _texCoords;
    
    V2F_C4B_T2F* _datas;
    GLushort*           _indices;
    GLuint              _buffersVBO[2];
    GLuint              _VAOname;
    
    CustomCommand _customCommand;
    
private:
    CC_DISALLOW_COPY_AND_ASSIGN(MeshSprite);
    
    void setupIndices();
    void setupVBO();
    void setupVBOandVAO();
    void mapBuffers();
};

// end of misc_nodes group
/// @}

NS_CC_END

#endif //__CCMOTION_STREAK_H__
