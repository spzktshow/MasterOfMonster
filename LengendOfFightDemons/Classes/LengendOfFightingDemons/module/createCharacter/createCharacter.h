//
//  createCharacter.h
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-7.
//
//

#ifndef __LengendOfFightDemons__createCharacter__
#define __LengendOfFightDemons__createCharacter__

#include <iostream>
#include "MVCFrame.h"

#define NORMAL_CHARACTER_FILE               "creatcharacterui_picture_%d.png"
#define SELECT_CHARACTER_FILE               "creatcharacterui_pictureselect_%d.png"
/***************************************** 事件名 ******************************************************/
#define REQUIRE_ENTER_GAME                  "requireEnterGame"
#define REQUIRE_ENTER_GAME_RESPONSE         "Login_doActiveDataInit"
#define GET_HERO_NAME                       "getHeroName"
#define GET_HERO_NAME_RESPONSE              "getHeroNameResponse"

typedef enum
{
    STAND = 18,
    ATTACK,
}HeroActor;

typedef enum
{
    ONE = 1,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX
}CharacterType;

class Character
{
public:
    Character(cocos2d::ui::ImageView* instance,CharacterType type);
    ~Character(){image = nullptr;}
    void select(bool);
private:
    cocos2d::ui::ImageView* image;
    bool lastStatus;
    CharacterType _type;
};


class SelectCharacterPanel:public mvc::OrionPanel
{
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(SelectCharacterPanel);
    virtual bool init();
    void selectHero(CharacterType type);
    void updateSelectHero(CharacterType type=CharacterType::ONE);
    cocos2d::ui::TextField* nameText;
    int getCurrentSelectHeroID();
    
    void updateSelectHeroSex(int sex);
    int getSlectHeroSex();
private:
    
    void loadComplete();
    void showSkill(std::vector<int> skillList);
    
    cocos2d::ui::TextBMFont* typeBMF;
    cocos2d::ui::Text* desc1Text;
    cocos2d::ui::Text* desc2Text;
    cocos2d::ui::Layout* skillLayout;
    cocos2d::ui::Layout* heroLayout;
    cocos2d::ui::Button* diceBtn;
    cocos2d::ui::Button* enterGameBtn;
    
    cocos2d::ui::ImageView* p1;
    cocos2d::ui::ImageView* p2;
    cocos2d::ui::ImageView* p3;
    cocos2d::ui::ImageView* p4;
    cocos2d::ui::ImageView* p5;
    cocos2d::ui::ImageView* p6;
    cocos2d::ui::ImageView* nameBg;
    
    std::map<CharacterType,Character*> characterMap;
    
    std::map<int,std::map<int,int>> heroFrameMap;
    Character* curentSelectCharacter;
    cocos2d::Sprite* hero;
    CharacterType lastSelectCharacterType;
    HeroActor lastHeroActor;
    void showHeroAction(CharacterType heroType,HeroActor actorType);
    int heroActorFrame[6][2] = {{1,1},{2,2},{3,3},{8,16},{5,5},{6,6}};
    int heroIds[6] = {710010,710020,710030,710040,710050,710060};
    
    int lastSelectHeroSex = 1;//1男 2女
    void onTextFiledHandler(cocos2d::Ref* ref,cocos2d::ui::TextFiledEventType type);
    bool attackIME;
    int moveOffset = 420;
};

class SelectCharacterMediator:public mvc::Mediator
{
public:
    SelectCharacterMediator(SelectCharacterPanel* instance);
private:
    SelectCharacterPanel* panel;
};

class SelectCharacterServer:public mvc::BaseServer
{
public:
    CREATE_FUNC(SelectCharacterServer);
    SelectCharacterServer();
    void getHeroName();
    void enterGame(int heroID,std::string userName);
};

class SelectCharacterCommand:public mvc::MVCCommand
{
public:
    SelectCharacterCommand();
    CREATE_COMMAND(SelectCharacterCommand);
    virtual void execute(cocos2d::ValueMap* map);
private:
    SelectCharacterServer* server;
};

class HeroNameNode
{
public:
    int sex;
    std::string name;
};

class HeroNameConfig:public cocos2d::Ref
{
public:
    DEFINE_SIGANL_INSTANCE(HeroNameConfig);
    HeroNameConfig();
    
    std::string getHeroNameBySex(int sex);
private:
    std::vector<HeroNameNode*> manNameMap;
    std::vector<HeroNameNode*> womenNameMap;
};

#endif /* defined(__LengendOfFightDemons__createCharacter__) */
