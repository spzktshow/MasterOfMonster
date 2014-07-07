//
//  commonUI.cpp
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-21.
//
//

#include "commonUI.h"
#include "mvcDefine.h"
#include "resourceName.h"
bool BaseUIFrame::init()
{
    if(!Node::init())return false;
    return true;
}

void BaseUIFrame::setFrameSize(float width, float height,std::string title,TitleStyle titleStyle,std::string bgFrameName,cocos2d::ui::Widget* widget)
{
    
//    if(titleStyle == TitleStyle::BAG)
//        width = 500;
    
    colorLayer = cocos2d::LayerColor::create(cocos2d::Color4B(0,0,0,180), width, height);
    //this->addChild(colorLayer);
    
    if(bgFrameName != "")
    {
        auto bgFrame = cocos2d::Sprite::createWithSpriteFrameName(bgFrameName);
        bgFrame->setPosition(width / 2, height / 2);
        colorLayer->addChild(bgFrame);
    }
    
    if(widget != nullptr)
    {
        this->addChild(widget);
    }
    
    auto dotLint = cocos2d::Sprite::createWithSpriteFrameName(FRAME_UI_LINE_NAME);
    float _lineWidth = dotLint->getContentSize().width;
    float _lineHeight = dotLint->getContentSize().height;
    int lenH = width / _lineWidth;
    int lenV = height / _lineWidth;
    
    if(titleStyle == TitleStyle::BAG)
    {
        auto lineButtom = cocos2d::Sprite::createWithSpriteFrameName(FRAME_BAG_BUTTOM_LINE);
        lineButtom->setAnchorPoint(cocos2d::Point(0.0f,0.0f));
        float _x = (width - lineButtom->getContentSize().width) / 2;
        lineButtom->setPosition(_x, _lineHeight);
        this->addChild(lineButtom);
        
        auto lineTop = cocos2d::Sprite::createWithSpriteFrameName(FRAME_BAG_TOP_LINE);
        lineTop->setAnchorPoint(cocos2d::Point(0.0f,1.0f));
        float __x = (width - lineTop->getContentSize().width) / 2;
        lineTop->setPosition(__x, height - _lineHeight);
        this->addChild(lineTop);
    }

    
//    if(lenH * _lineWidth < width)
//        lenH++;
//    if(lenV * _lineWidth < height)
//        lenV++;
    
    for(int i = 0; i < lenH;i++)
    {
        auto _line = cocos2d::Sprite::createWithSpriteFrameName(FRAME_UI_LINE_NAME);
        _line->setAnchorPoint(cocos2d::Point(0.0f,1.0f));
        _line->setPosition(_lineWidth*i, height);
        this->addChild(_line);
    }
    
    for(int i = 0; i < lenH;i++)
    {
        auto _line = cocos2d::Sprite::createWithSpriteFrameName(FRAME_UI_LINE_NAME);
        _line->setAnchorPoint(cocos2d::Point(0.0f,0.0f));
        _line->setPosition(_lineWidth*i, 0);
        this->addChild(_line);
    }

    for(int i = 0; i < lenV;i++)
    {
        auto _line = cocos2d::Sprite::createWithSpriteFrameName(FRAME_UI_LINE_NAME);
        _line->setAnchorPoint(cocos2d::Point(0.0f,1.0f));
        _line->setRotation(-90);
        _line->setPosition(0,_lineWidth*i);
        this->addChild(_line);
    }
    
    for(int i = 0; i < lenV;i++)
    {
        auto _line = cocos2d::Sprite::createWithSpriteFrameName(FRAME_UI_LINE_NAME);
        _line->setAnchorPoint(cocos2d::Point(0.0f,0.0f));
        _line->setRotation(-90);
        _line->setPosition(width,_lineWidth*i);
        this->addChild(_line);
    }

    if(titleStyle == TitleStyle::NORMAL || titleStyle == TitleStyle::BAG)
    {
        auto titleImage = cocos2d::Sprite::createWithSpriteFrameName(FRAME_NORMAL_TITLE_NAME);
        titleImage->setAnchorPoint(cocos2d::Point(0.5f,1.0f));
        titleImage->setPosition(width/2, height);
        this->addChild(titleImage);
        
        auto bmf = cocos2d::ui::TextBMFont::create();
        bmf->setFntFile(GAME_FONT_FILE);
        bmf->setText(title.c_str());
        titleImage->addChild(bmf);
        bmf->setPosition(cocos2d::Point(titleImage->getContentSize().width/2,titleImage->getContentSize().height/2));
    }
    else if(titleStyle == TitleStyle::BEAUTIFUL)
    {
        auto leftLine = cocos2d::Sprite::createWithSpriteFrameName(FRAME_TITLE_LINE_NAME);
        leftLine->setAnchorPoint(cocos2d::Point(0.0f,1.0f));
        leftLine->setPosition(15, height);
        this->addChild(leftLine);
        
        auto rightLine = cocos2d::Sprite::createWithSpriteFrameName(FRAME_TITLE_LINE_NAME);
        rightLine->setAnchorPoint(cocos2d::Point(0.0f,1.0f));
        rightLine->setScaleX(-1);
        rightLine->setPosition(width - 15, height);
        this->addChild(rightLine);

        
        auto titleImage = cocos2d::Sprite::createWithSpriteFrameName(FRAME_BEAUTIFUL_TITLE_NAME);
        titleImage->setAnchorPoint(cocos2d::Point(0.5f,1.0f));
        titleImage->setPosition(width/2, height);
        this->addChild(titleImage);
        auto bmf = cocos2d::ui::TextBMFont::create();
        bmf->setFntFile(GAME_FONT_FILE);
        bmf->setText(title.c_str());
        titleImage->addChild(bmf);
        bmf->setPosition(cocos2d::Point(titleImage->getContentSize().width/2,titleImage->getContentSize().height/2));
    }
    
    auto leftTopCorner = cocos2d::Sprite::createWithSpriteFrameName(FRAME_UI_CORNER_NAME);
    leftTopCorner->setAnchorPoint(cocos2d::Point(0.0f,1.0f));
    leftTopCorner->setPosition(cocos2d::Point(0,height));
    this->addChild(leftTopCorner);
    
    auto leftButtomCorner = cocos2d::Sprite::createWithSpriteFrameName(FRAME_UI_CORNER_NAME);
    leftButtomCorner->setAnchorPoint(cocos2d::Point(0.0f,1.0f));
    leftButtomCorner->setRotation(-90);
    leftButtomCorner->setPosition(cocos2d::Point(0.0f,0.0f));
    this->addChild(leftButtomCorner);
    
    auto rightTopCorner = cocos2d::Sprite::createWithSpriteFrameName(FRAME_UI_CORNER_NAME);
    rightTopCorner->setAnchorPoint(cocos2d::Point(0.0f,1.0f));
    rightTopCorner->setRotation(90);
    rightTopCorner->setPosition(width, height);
    this->addChild(rightTopCorner);
    
    auto rightBottomCorner = cocos2d::Sprite::createWithSpriteFrameName(FRAME_UI_CORNER_NAME);
    rightBottomCorner->setAnchorPoint(cocos2d::Point(0.0f,1.0f));
    rightBottomCorner->setRotation(180);
    rightBottomCorner->setPosition(width, 0);
    this->addChild(rightBottomCorner);

    closeBtn = cocos2d::ui::Button::create();
    closeBtn->setTouchEnabled(true);
    closeBtn->loadTextureNormal(FRAME_CLOSE_NORMAL_NAME,cocos2d::ui::TextureResType::UI_TEX_TYPE_PLIST);
    closeBtn->loadTexturePressed(FRAME_CLOSE_PRESS_NAME,cocos2d::ui::TextureResType::UI_TEX_TYPE_PLIST);
    closeBtn->cocos2d::Node::setPosition(width - 10, height - 10);
    this->addChild(closeBtn);
}