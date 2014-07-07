//
//  mainCity.h
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-4-18.
//
//

#ifndef __LengendOfFightDemons__mainCity__
#define __LengendOfFightDemons__mainCity__

#include <iostream>
#include "cocos2d.h"
#include "editor-support/cocostudio/DictionaryHelper.h"
#include "ui/CocosGUI.h"
#include "ApplicationContext.h"
#include "ccMacros.h"
#include "MVCFrame.h"
#include "SimpleAudioEngine.h"
#include "gameTool.h"

#define CLOUD_COUNT 8
#define CLOUD_DEPTH 300
#define DELAY_SECOND 3

#define SHOW_BAG                  "show_bag"
#define SHOW_HERO                 "show_hero"
#define SHOW_COPY                 "show_copy"
#define SHOW_STRENGTH             "show_strength"
#define SHOW_UNION                "show_union"
#define SHOW_SIGN                 "show_sign"
#define SHOW_FIRST_RECHARGE       "show_first_recharge"
#define SHOW_LUCKY_TURN           "show_lucky_turn"
#define SHOW_LEVEL_GIFT_PACKAGE   "show_level_gift_package"
#define SHOW_SET                  "show_set"
#define SHOW_SPIRIT               "show_spirit"
#define SHOW_MINE                 "show_mine"
#define SHOW_RUNE                 "show_rune"
#define SHOW_TRAVEN               "show_traven"
#define SHOW_BLACKMISTY           "show_blacksimty"
#define ADD_SLIVER                "add_sliver"
#define ADD_GOLD                  "add_gold"
#define ADD_FOOD                  "add_food"



class MainCityScene:public mvc::OrionPanel
{
public:
    static cocos2d::Scene* createScene();
    MainCityScene();
    ~MainCityScene();
    CREATE_FUNC(MainCityScene);
    void addPanel(mvc::OrionPanel* panel);
<<<<<<< .mine
    virtual void update(float dt);
    virtual void onExit();
=======
    void showCity();
    void hideCity();
>>>>>>> .r799
private:
    bool virtual init();
    //void onBuildClickHandler(cocos2d::Ref* target,cocos2d::ui::TouchEventType type);
private:
    cocos2d::Sprite* bgLayer;
    cocos2d::Sprite* cloudLayer;
    cocos2d::Sprite* buildLayer;
    cocos2d::Sprite* effectLayer;
    cocos2d::Sprite* panelContainer;
    cocos2d::Sprite* resourceLayer;
};

//class PanelManager:public cocos2d::Sprite
//{
//public:
//    DEFINE_SIGANL_INSTANCE(PanelManager);
//    
//};

class MainCitySceneMediator:public mvc::Mediator
{
public:
    MainCitySceneMediator(MainCityScene* scene);
private:
    MainCityScene* scene;
};

class MainCityEffect:public cocos2d::Layer
{
public:
    CREATE_FUNC(MainCityEffect);
private:
    bool virtual init();
};

class Cloud:public cocos2d::Node
{
public:
    cocos2d::Sprite* getContent();
    CREATE_FUNC(Cloud);
    static Cloud* initWithZ3D(int z3d);
private:
    cocos2d::Sprite* content;
    
    int dir;
    int DEPTFH=300;
    double z3d=DEPTFH*3;
    float _width;
    float _contentWidth;
    int delayCount=0;
    virtual bool init();
    virtual void update(float delta);
    
};

class CloudLayer:public cocos2d::Layer
{
public:
    CREATE_FUNC(CloudLayer);
    ~CloudLayer();
private:
    virtual bool init();
    int cloudCount;
    void createCloud();
};

class Resource:public mvc::OrionPanel
{
public:
    CREATE_FUNC(Resource);
    virtual bool init();
public:
    cocos2d::ui::Button* sliverAddBtn;
    cocos2d::ui::Button* goldAddBtn;
    cocos2d::ui::Button* foodAddBtn;
    cocos2d::ui::Text* sliverLabel;
    cocos2d::ui::Text* goldLabel;
    cocos2d::ui::Text* foodLabel;
    cocos2d::ui::Button* setBtn;

};

class ResourseMediator:public mvc::Mediator
{
public:
    ResourseMediator(Resource* res);
    ~ResourseMediator();
    void showResourceInfo();
private:
    Resource* resource;
};

class MainUI:public mvc::OrionPanel
{
public:
    cocos2d::ui::Layout* bottomLayout;
    cocos2d::ui::Button* heroBtn;
    cocos2d::ui::Button* bagBtn;
    cocos2d::ui::Button* copyBtn;
    cocos2d::ui::Button* strengthBtn;
    cocos2d::ui::Button* unionBtn;
    
    cocos2d::ui::Layout* rightLayout;
    cocos2d::ui::Button* signBtn;
    cocos2d::ui::Button* rechargeBtn1;
    cocos2d::ui::Button* luckTurnBtn;
    cocos2d::ui::Button* levelGiftPackageBtn;
    
    cocos2d::ui::Button* controlBtn;
    cocos2d::ui::ImageView* showLabel;
    cocos2d::ui::ImageView* hideLabel;
    
    cocos2d::ui::Layout* userinfoLayout;
    cocos2d::ui::Button* rechargeBtn2;
    cocos2d::ui::Text* nameLabel;
    cocos2d::ui::Text* levelLabel;
    cocos2d::ui::ImageView* heroAvatar;
    
//    cocos2d::ui::Layout* resourceLayout;
//    cocos2d::ui::Button* sliverAddBtn;
//    cocos2d::ui::Button* goldAddBtn;
//    cocos2d::ui::Button* foodAddBtn;
//    cocos2d::ui::Text* sliverLabel;
//    cocos2d::ui::Text* goldLabel;
//    cocos2d::ui::Text* foodLabel;
    
    //cocos2d::ui::Button* setBtn;
    
    cocos2d::ui::ImageView* spiritBtn;
    cocos2d::ui::ImageView* mineBtn;
    cocos2d::ui::ImageView* tarvenBtn;
    cocos2d::ui::ImageView* blacksmithyBtn;
    cocos2d::ui::ImageView* runeBtn;
public:
    CREATE_FUNC(MainUI);
private:
    
    bool showFunBar;
    bool isMoving;
    const float FUNBAR_MOVE_TIME = .3;

private:
    virtual bool init();
    void onChildClick(cocos2d::Ref* target,cocos2d::ui::TouchEventType type);
    void updateFunBarStatus();
    void foldFunBar();
    void unFlodBar();
    cocos2d::ui::Button* getButtonFromeWidget(cocos2d::ui::Layout* layout,std::string name);
    cocos2d::ui::ImageView* getImageViewFromeWidget(cocos2d::ui::Layout* layout,std::string name,bool addListaner=false);
};

class MainUIMediator:public mvc::Mediator
{
public:
    MainUIMediator(MainUI* uiInstance);
private:
    MainUI* ui;
    void showMainUIInfo();
};

#endif /* defined(__LengendOfFightDemons__mainCity__) */