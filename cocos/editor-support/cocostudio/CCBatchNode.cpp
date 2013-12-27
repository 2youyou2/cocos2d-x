/****************************************************************************
Copyright (c) 2013 cocos2d-x.org

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

#include "cocostudio/CCBatchNode.h"
#include "cocostudio/CCArmatureDefine.h"
#include "cocostudio/CCArmature.h"
#include "cocostudio/CCSkin.h"
#include "CCRenderer.h"
#include "CCGroupCommand.h"

using namespace cocos2d;

namespace cocostudio {

BatchNode *BatchNode::create()
{
    BatchNode *batchNode = new BatchNode();
    if (batchNode && batchNode->init())
    {
        batchNode->autorelease();
        return batchNode;
    }
    CC_SAFE_DELETE(batchNode);
    return nullptr;
}

BatchNode::BatchNode()
    : _textureAtlas(nullptr)
    , _texture(nullptr)
{
}

BatchNode::~BatchNode()
{
}

bool BatchNode::init()
{
    bool ret = Node::init();
    
    CC_SAFE_FREE(_textureAtlas);
    _textureAtlas = new TextureAtlas();
    _textureAtlas->initWithTexture(nullptr, 8);
    
    setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));

    _blendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;

    return ret;
}

void BatchNode::addChild(Node *pChild)
{
    Node::addChild(pChild);
}

void BatchNode::addChild(Node *child, int zOrder)
{
    Node::addChild(child, zOrder);
}

void BatchNode::addChild(Node *child, int zOrder, int tag)
{
    Node::addChild(child, zOrder, tag);
    Armature *armature = dynamic_cast<Armature *>(child);
    if (armature != nullptr)
    {
        armature->setBatchNode(this);
    }
}

void BatchNode::removeChild(Node* child, bool cleanup)
{
    Armature *armature = dynamic_cast<Armature *>(child);
    if (armature != nullptr)
    {
        armature->setBatchNode(nullptr);
    }

    Node::removeChild(child, cleanup);
}

void BatchNode::visit()
{
    // quick return if not visible. children won't be drawn.
    if (!_visible)
    {
        return;
    }
    kmGLPushMatrix();

    transform();
    sortAllChildren();
    draw();

    // reset for next frame
    _orderOfArrival = 0;

    kmGLPopMatrix();
}

void BatchNode::draw()
{
    if (_children.empty())
    {
        return;
    }

    CC_NODE_DRAW_SETUP();
    

    for(auto object : _children)
    {
        Armature *armature = dynamic_cast<Armature *>(object);
        if (armature)
        {
            armature->visit();
            _texture = armature->getTexture();
        }
        else
        {
            ((Node *)object)->visit();
        }
    }
    
    drawQuads();
}

void BatchNode::drawQuads()
{
    // Optimization: Fast Dispatch
    if( _textureAtlas->getTotalQuads() == 0 && _texture != nullptr)
    {
        return;
    }

    kmMat4 mv;
    kmGLGetMatrix(KM_GL_MODELVIEW, &mv);

    ssize_t size = _textureAtlas->getTotalQuads();
    for(ssize_t i = 0; i<size; i+= Renderer::VBO_SIZE-1)
    {
        ssize_t count = 0;
        if((size - i) >= Renderer::VBO_SIZE)
        {
            count = Renderer::VBO_SIZE - 1;
        }
        else
        {
            count = size - i;
        }
        
        V3F_C4B_T2F_Quad* quad = _textureAtlas->getQuads() + i*sizeof(V3F_C4B_T2F_Quad);
        
        QuadCommand* cmd = QuadCommand::getCommandPool().generateCommand();
        cmd->init(0,
                  _vertexZ,
                  _texture->getName(),
                  _shaderProgram,
                  _blendFunc,
                  quad,
                  count,
                  mv);
        Director::getInstance()->getRenderer()->addCommand(cmd);
    }
    
    _textureAtlas->removeAllQuads();
}

}
