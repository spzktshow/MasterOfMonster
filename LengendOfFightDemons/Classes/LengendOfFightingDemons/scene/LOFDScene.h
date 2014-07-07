//
//  LOFDScene.h
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-8.
//
//

#ifndef __LengendOfFightDemons__LOFDScene__
#define __LengendOfFightDemons__LOFDScene__

#include "lengendOfFightDemons.h"
#include "MSVectorMap.h"
#include "LOFDActor.h"
#include "LOFDDungeons.h"

NS_LOFD_BEGIN

#define SKILL_BUTTON_ALPHA_HALF         180
#define SKILL_BUTTON_ALPHA_MAX          255

#define SKILL_BUTTON_TOUCHED            "skillButtonTouched"

class MapScene;
class ActorData;
class SkillDef;

class SkillButton : public cocos2d::Sprite
{
public:
    SkillButton(){
        coolDownFrameIndex = -1;
        isTouched = false;
        isCoolDown = false;
        this->skillDef = nullptr;
    };
    ~SkillButton(){
        this->destoryButton();
    };
    
    lofd::SkillDef * skillDef;
    cocos2d::ProgressTimer * progressTimer;
    cocos2d::EventListenerTouchOneByOne * touchListener;
    
    /******技能按钮监听实体*****/
    cocos2d::Sprite * skillFrame;
    cocos2d::Sprite * skillLightFrame;
    cocos2d::Sprite * skillDarkFrame;
    
    virtual void startCoolDown();
    virtual void coolDownComplete();
    float coolDownFrameIndex;
    bool isCoolDown;
    
    virtual void createButton();
    virtual void destoryButton();
    
    virtual bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event);
    /*****标记touchBegan是否接触到按钮上*****/
    bool isBeganInSkillButton;
    
    virtual void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event);
    
    virtual void addListeners();
    virtual void removeListeners();
    /****标记按钮有没有touch*******/
    bool isTouched;
    
    CREATE_FUNC(SkillButton);
};

/*********debug text**********/
class DebugTextField : public cocos2d::Layer
{
public:
    DebugTextField(){};
    ~DebugTextField(){
        this->removeListeners();
    };
    
    CREATE_FUNC(DebugTextField);
    
    virtual bool init()
    {
        if (!cocos2d::Layer::init()) return false;
        textFieldTTF = cocos2d::TextFieldTTF::textFieldWithPlaceHolder("<click here for debug input>", "fonts/Marker Felt.ttf", 36);
        this->addChild(textFieldTTF);
        
        this->addListeners();
        return true;
    };
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event)
    {
        touchPoint = touch->getLocation();
        return true;
    };
    
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
    {
        cocos2d::Point endPos = touch->getLocation();
        
        const float gap = 20.0f;
        if (abs(endPos.x - touchPoint.x) > gap  || abs(endPos.y - touchPoint.y) > gap)
        {
            return;
        }
        
        auto point = convertTouchToNodeSpace(touch);
        cocos2d::Rect rect;
        rect.size = textFieldTTF->getContentSize();
        rect.origin.x = textFieldTTF->getPosition().x - textFieldTTF->getContentSize().width * .5;
        rect.origin.y = textFieldTTF->getPosition().y - textFieldTTF->getContentSize().height * .5;
        
        if (rect.containsPoint(point))
        {
            //
            textFieldTTF->attachWithIME();
        }
    };
    
    cocos2d::TextFieldTTF * textFieldTTF;
    cocos2d::Point touchPoint;
    
    cocos2d::EventListenerTouchOneByOne * touchListener;
    
    virtual void addListeners()
    {
        touchListener = cocos2d::EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = CC_CALLBACK_2(DebugTextField::onTouchBegan, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(DebugTextField::onTouchEnded, this);
        cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(this->touchListener, this);
    };
    virtual void removeListeners()
    {
        cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(this->touchListener);
    };
};

/*********伤害数字容器*********/
class HurtCount : public cocos2d::Sprite
{
public:
    HurtCount(){};
    ~HurtCount(){};
    
    void setCount(int countValue, cocos2d::Point location);
    
    void startAnimation();
    
    void endAnimation();
    
    CREATE_FUNC(HurtCount);

protected:
    int _count;
    cocos2d::Point _location;
};

class MapUILayer : public moonsugar::MapUILayer
{
public:
    MapUILayer(moonsugar::UILayerData * uiLayerData):moonsugar::MapUILayer(uiLayerData){};
    ~MapUILayer(){
        removeListeners();
    };
    
    static MapUILayer * create(moonsugar::UILayerData * uiLayerData);
    
    virtual bool init();
    
    void atk1CallBack(cocos2d::Ref * pSender);
    void skillCallBack(cocos2d::Ref * pSender);
    void addActorCallBack(cocos2d::Ref * pSender);
    
    lofd::ActorData * currentFocus;
    void changeFocus(lofd::ActorData * actorDataValue);
    lofd::MapScene * mapScene;
    
    lofd::SkillButton * skillButton1;
    lofd::SkillButton * skillButton2;
    lofd::SkillButton * skillButton3;
    lofd::SkillButton * skillButton4;
    
    /*****hasButtonTouched****/
    bool hasButtonTouched();
    /*****StartCD*******/
    void startCD(int skillId);
    
    cocos2d::EventListenerTouchOneByOne * touchListener;
    virtual void addListeners();
    virtual void removeListeners();
    
    virtual bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event);
    virtual void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event);
    /******按到的点*********/
    cocos2d::Point touchPoint;
    
    DebugTextField * debugTextField;
};

class MapActorsLayer : public moonsugar::MapActorsLayer
{
public:
    MapActorsLayer(moonsugar::ActorLayerData * actorLayerData):moonsugar::MapActorsLayer(actorLayerData),countCheckRange(0){};
    ~MapActorsLayer(){};
    
    virtual void draw(cocos2d::Renderer *renderer, const kmMat4& transform, bool transformUpdated);
    
    static MapActorsLayer * create(moonsugar::ActorLayerData * actorLayerDataValue);
    
    virtual bool init();
    lofd::MapScene * mapScene;
protected:
    virtual void onDraw(const kmMat4& transform, bool transformUpdated);
    cocos2d::CustomCommand _customCommand;
    
    int countCheckRange;
};

class MapEffectLayer : public moonsugar::MapEffectLayer
{
public:
    MapEffectLayer(moonsugar::EffectLayerData * effectLayerData):moonsugar::MapEffectLayer(effectLayerData){};
    ~MapEffectLayer(){
        this->removeContextListeners();
    };
    
    static MapEffectLayer * create(moonsugar::EffectLayerData * effectLayerDataValue);
    
    virtual bool init();
    
    /*******ActorHurtHandler*******/
    cocos2d::EventListenerCustom * actorHurtEventListener;
    virtual void onActorHurtHandler(cocos2d::EventCustom * event);
protected:
    virtual void addContextListeners();
    virtual void removeContextListeners();
};

class MapScene : public moonsugar::MapScene
{
public:
    MapScene(moonsugar::MapSceneData * mapSceneDataValue):moonsugar::MapScene(mapSceneDataValue){
        isDrawPath = false;
        this->currentFocus = nullptr;
    };
    ~MapScene(){
        this->removeContextListeners();
    };
    
    virtual bool init();
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    void onTouchEndedOrCancelledExecute(cocos2d::Touch * touch, cocos2d::Event *unused_event);
    
    static lofd::MapScene * create(moonsugar::MapSceneData * mapSceneDataValue);
    
    lofd::ActorData * currentFocus;
    /*****关卡配置*****/
    lofd::DungeonDef * dungeonDef;
    
    void changeFocus(lofd::ActorData * actorDataValue);
    /********可操控actors*******/
    cocos2d::Vector<lofd::ActorData *> operationActors;
    virtual void addOperationActor(lofd::ActorData * actorValue);
    virtual void removeOperationActor(lofd::ActorData * actorValue);
    
    virtual void addActor(lofd::ActorData * actorValue);
    virtual void removeActor(lofd::ActorData * actorValue);
    
    cocos2d::Point startPoint;
    /****绘制********/
    virtual void draw(cocos2d::Renderer *renderer, const kmMat4& transform, bool transformUpdated);
    
    cocos2d::Vector<moonsugar::VectorPoint *> tempPath;
    moonsugar::VectorPoint * tempStartPoint;
    moonsugar::VectorPoint * tempEndPoint;
    
    virtual void addContextListeners();
    virtual void removeContextListeners();
    
    /*******SkillHandler*******/
    cocos2d::EventListenerCustom * skillListener;
    virtual void onSkillHandler(cocos2d::EventCustom * event);
    /*******SkillCoolDownStartHandler******/
    cocos2d::EventListenerCustom * skillCoolDownListener;
    virtual void onSkillCoolDownStartHandler(cocos2d::EventCustom * event);
    /*******SkillCoolDownEndHandler*****/
    cocos2d::EventListenerCustom * skillCoolDownEndListener;
    virtual void onSkillCoolDownEndHandler(cocos2d::EventCustom * event);
    /*******ActorEventHandler******/
    cocos2d::EventListenerCustom * actorDeadEventListener;
    virtual void onActorDeadHandler(cocos2d::EventCustom * event);
    cocos2d::EventListenerCustom * actorDeadCompleteEventListener;
    virtual void onActorDeadCompleteHandler(cocos2d::EventCustom * event);
protected:
    bool isDrawPath;
    virtual moonsugar::MapUILayer * createMapUILayer(moonsugar::UILayerData * uiLayerData);
    virtual moonsugar::MapActorsLayer * createMapActorsLayer(moonsugar::ActorLayerData * actorLayerDataValue);
    virtual moonsugar::MapEffectLayer * createMapEffectLayer(moonsugar::EffectLayerData * effectLayerDataValue);
    
    virtual void onDraw(const kmMat4& transform, bool transformUpdated);
    cocos2d::CustomCommand _customCommand;
};

/*********以下部分都废弃***********/

class UISceneLayer : public cocos2d::Layer
{
public:
    UISceneLayer(){};
    ~UISceneLayer(){};
    
    /*******uiScene layer*******/
    lofd::MapScene * mapScene;
    
    CREATE_FUNC(UISceneLayer);
    
    virtual bool init();
};

class UIScene : public cocos2d::Scene
{
public:
    UIScene(){};
    ~UIScene(){};
    
    /******uiScene持有 mapScene的引用*****/
    lofd::MapScene * mapScene;
    
    lofd::UISceneLayer * uiSceneLayer;
    
    CREATE_FUNC(UIScene);
};

class BattleScene : public cocos2d::Scene
{
public:
    BattleScene(){};
    ~BattleScene(){};
    
    virtual bool init();
    
    static lofd::BattleScene * create();
    
    lofd::MapScene * mapScene;
    lofd::UIScene * uiScene;
};

NS_LOFD_END;

#endif /* defined(__LengendOfFightDemons__LOFDScene__) */
