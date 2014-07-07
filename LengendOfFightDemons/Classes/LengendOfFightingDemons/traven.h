//
//  traven.h
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-20.
//
//

#ifndef __LengendOfFightDemons__traven__
#define __LengendOfFightDemons__traven__

#include <iostream>
#include "MVCFrame.h"
#include "LOFDPubBeforeConfig.h"
#include "hero.h"

#define NS_TRAVEN_BEGIN      namespace traven {
#define NS_TRAVEN_END        }

#define COIN_NORMAL_ANIMATION_NAME          "coinNormalAnimation"
#define COIN_ONE_ANIMATION_NAME             "coinOneAnimation"
#define COIN_TWO_ANIMATION_NAME             "coinTwoAnimation"
#define COIN_RESOURCE                       "pubgoowillui_coin_100%02d.png" 

NS_TRAVEN_BEGIN

class HeroCard:public mvc::BaseItemRender
{
public:
    DEFINE_CREATE_FUNC_WITH_INIT(HeroCard);
    DEFINE_ITEMRENDER;
    int openCards = 9;
    float splitMoveTime = 0.0f;
    //    lofd::PubBeforeNode* def;
    hero::pubInfoModel* model;
public:
    void setItemModel(hero::pubInfoModel* node);
    void setRecruitStatus();
    void moveOffset(float offset);
    void setLayoutWidth(float width);
    void modifyPosition();
private:
    float MAX_SCALE = 1.0f;
    float MIDDLE_SCALE = 0.7f;
    float MIN_SCALE = 0.6f;
private:
    IMAGE_VIEW cardImage;
    IMAGE_VIEW frameImage;
    int lastIndex = -1;
    float scaleInfo[9] = {1.0f,1.2f,1.4f,1.2f,1.0f,1.0f,1.0f,1.0f,1.0f};
    float animationDuration = 2.0f;
    bool isMoving;
    float halfWidth;
    float layoutWidth;
};

class TravenPanel:public mvc::OrionPanel
{
public:
    ~TravenPanel();
    DEFINE_CREATE_SCENE;
    CREATE_FUNC(TravenPanel);
    bool init();
    void showHeroCardList();
private:
    int middleIndex = 2;
    HeroCard* currentSelectCard;
    IMAGE_VIEW      heroImage;
    BMFONT          heroNameBMF;
    TEXT            jobBMF;
    TEXT            locationBMF;
    TEXT            favorLabel;
    PROGRESS_BAR    powerBar;
    PROGRESS_BAR    quickBar;
    PROGRESS_BAR    phyBar;
    PROGRESS_BAR    intelBar;
    BUTTON          recruitBtn;
    BUTTON          favorBtn;
    LAYOUT          cardLayout;
    LAYOUT          skillLayout;
    DEFINE_ITME_GROUP;
    std::vector<HeroCard*> cardVec;
    int openLen = 9;
    
    float startX;
private:
    void moveCardContainer(float offset);
    void onSelectPubHero(hero::pubInfoModel* model);
    void modifyCardPosition();
};

class TravenMediator:public mvc::Mediator
{
public:
    TravenMediator(TravenPanel* instance);
private:
    TravenPanel* panel;
};

/********************************** guessPanel ************************************************/
typedef enum
{
    ZERO,
    ONE,
    TWO
}COIN_TYPE;

/*  获取好感度类型 */
typedef enum
{
    NORMAL,//一次
    AUTO//连续五次
}AUTO_TYPE;

typedef enum
{
    LEFT,
    RIGHT
}DILOG_DIR;

class SelfSprite;
class CardDilog;
class TravenGuessPanel:public mvc::OrionPanel
{
public:
    DEFINE_CREATE_FUNC_WITH_INIT(TravenGuessPanel);
    void setServerCoinType(int);
    void showHeroInfo();
public:
    COIN_TYPE  selectCoinType;
    AUTO_TYPE  autoType;
    TEXT                costLabel;
    TEXT                favorLabel;
    IMAGE_VIEW          selfHeroImage;
    IMAGE_VIEW          recruitHeroImage;
private:
    int currentResultCount;
    int serverCoinType;
    SelfSprite*    turnSpr;
    cocos2d::Sprite*    animateSprite;
    BUTTON              oneBtn;
    BUTTON              twoBtn;
    BUTTON              beginBtn;
    BUTTON              guessFiveBtn;
    BUTTON              winBtn;
    BUTTON              questionBtn;
    IMAGE_VIEW          leftDilogImage;
    IMAGE_VIEW          rightDilogImage;


//    TEXT                leftDilogLabel;
//    TEXT                rightDilogLabel;
    
    PROGRESS_BAR        luckBar;
    //cocos2d::Animate*   coinAnimate;
    int testCycleCount = -1;
    int stopIndex = -1;
    std::string lastCoinAnimateName;
    lofd::PubBeforeNode* selfDef;
    lofd::PubBeforeNode* recruitDef;
    /**是否赢了**/
    bool isWin;
    /**是否开始选择**/
    bool beginSelect;
    /***是否启用必胜***/
    bool willLiking;
    /****是否可以打断*****/
    bool canBreak;
    /***** 是否是第一局***/
    bool firstRound;
    
    CardDilog* leftDilog;
    CardDilog* rightDilog;
private:
    void showCoinAnimate(std::string style,bool add = false);
    void showSecondCoinAnimate(std::string style);
    void stopRunning();
    void winHandler();
    void loseHandler();
    void startGetFavor();
    void resetStatus();
    void showLeftDilog(std::string info,std::function<void()> callfunc = nullptr);
    void showRightDilog(std::string info,std::function<void()> callfunc = nullptr);
    void removeAllDilog();
    /***开始猜测行为**/
    void startGuess();
    void selectCoin();
    void onReallyShow();
};

class CardDilog:public mvc::BaseItemRender
{
public:
    DEFINE_CREATE_FUNC_WITH_INIT(CardDilog);
    void show(std::string info,DILOG_DIR _dir = DILOG_DIR::LEFT);
    void hide();
    std::function<void()> callbackFunc;
private:
    TEXT        label;
    IMAGE_VIEW  dilog;
    DILOG_DIR   dir;
    float scaleDuration;
};

class TravenGuessMediator:public mvc::Mediator
{
public:
    TravenGuessMediator(TravenGuessPanel* instance);
private:
    TravenGuessPanel*   panel;
private:
};

class SelfSprite:public cocos2d::Sprite
{
public:
    CREATE_FUNC(SelfSprite);
    bool init();
    int totalAnimateFrmes;
    virtual void setSpriteFrame(cocos2d::SpriteFrame* newFrame);
    void stopAtFrameIndex(int frameIndex);
    void stopAction();
    inline bool getIsRunning(){return _isRunning;}
    cocos2d::Action * runAction(cocos2d::Action* action);
private:
    int curFrameCount;
    int stopFrameIndex;
    bool _isRunning;
    bool firstSetFrame;
public:
    std::function<void(int)> frameCallBack;
};
/********************************** guessPanel ************************************************/


/********************************** server_command ************************************************/
class TravenServer:public mvc::BaseServer
{
public:
    
};

class TravenCommand:public mvc::MVCCommand
{
public:
    DEFINE_COMMAND_EXECUTE;
    CREATE_COMMAND(TravenCommand);
};
/********************************** server_command ************************************************/
NS_TRAVEN_END
#endif /* defined(__LengendOfFightDemons__traven__) */
