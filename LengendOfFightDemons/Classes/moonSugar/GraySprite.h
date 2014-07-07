//
//  GraySprite.h
//  IsoFarm
//
//  Created by RockLee on 14-6-5.
//
//

#ifndef __IsoFarm__GraySprite__
#define __IsoFarm__GraySprite__

#include "cocos2d.h"
USING_NS_CC;
template <class spriteType>
class ShaderSpriteCreator
{
public:
    static spriteType* createSprite(const std::string& filename)
    {
        spriteType* ret = spriteType::create();
        ret->setTexture(filename);
        ret->initShader();
        ret->setBackgroundNotification();
        return ret;
    };
    
    static spriteType* createSprite(cocos2d::Texture2D *texture)
    {
        spriteType* ret = spriteType::create();
        ret->setTexture(texture);
        ret->initShader();
        ret->setBackgroundNotification();
        return ret;
    };
};

class ShaderSprite : public Sprite
{
public:
    ShaderSprite(){};
    ~ShaderSprite(){};
    
    virtual void initShader()
    {
        GLchar * fragSource = (GLchar*) String::createWithContentsOfFile(
                                                                         FileUtils::getInstance()->fullPathForFilename(_fragSourceFile).c_str())->getCString();
        auto program = new GLProgram();
        program->initWithByteArrays(ccPositionTextureColor_vert, fragSource);
        setShaderProgram(program);
        program->release();
        
        CHECK_GL_ERROR_DEBUG();
        
        program->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
        program->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
        program->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        
        CHECK_GL_ERROR_DEBUG();
        
        program->link();
        
        CHECK_GL_ERROR_DEBUG();
        
        program->updateUniforms();
        
        CHECK_GL_ERROR_DEBUG();
        
        buildCustomUniforms();
        
        CHECK_GL_ERROR_DEBUG();
    };
    void setBackgroundNotification()
    {
#if CC_ENABLE_CACHE_TEXTURE_DATA
        auto listener = EventListenerCustom::create(EVENT_COME_TO_FOREGROUND, [this](EventCustom* event){
            this->setShaderProgram(nullptr);
            this->initShader();
        });
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif
    };
    
    virtual void draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated) override
    {
        _renderCommand.init(_globalZOrder);
        _renderCommand.func = CC_CALLBACK_0(ShaderSprite::onDraw, this, transform, transformUpdated);
        renderer->addCommand(&_renderCommand);
    };
    void listenBackToForeground(Ref *obj);
    
protected:
    virtual void buildCustomUniforms() = 0;
    virtual void setCustomUniforms() = 0;
protected:
    std::string _fragSourceFile;
    
protected:
    CustomCommand _renderCommand;
    void onDraw(const kmMat4 &transform, bool transformUpdated)
    {
        auto shader = getShaderProgram();
        shader->use();
        shader->setUniformsForBuiltins(transform);
        
        setCustomUniforms();
        
        GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX );
        GL::blendFunc(_blendFunc.src, _blendFunc.dst);
        GL::bindTexture2D( getTexture()->getName());
        
        //
        // Attributes
        //
#define kQuadSize sizeof(_quad.bl)
        size_t offset = (size_t)&_quad;
        
        // vertex
        int diff = offsetof( V3F_C4B_T2F, vertices);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
        
        // texCoods
        diff = offsetof( V3F_C4B_T2F, texCoords);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
        
        // color
        diff = offsetof( V3F_C4B_T2F, colors);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 4);
    };
    
};

class GreyScaleSprite : public ShaderSprite, public ShaderSpriteCreator<GreyScaleSprite>
{
public:
    CREATE_FUNC(GreyScaleSprite);
    GreyScaleSprite()
    {
        _fragSourceFile = "Shaders/example_greyScale.fsh";
    };
protected:
    virtual void buildCustomUniforms(){};
    virtual void setCustomUniforms(){};
};

#endif /* defined(__IsoFarm__GraySprite__) */
