//
//  LOFDActor.h
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-8.
//
//

#ifndef __LengendOfFightDemons__LOFDActor__
#define __LengendOfFightDemons__LOFDActor__

#include "lengendOfFightDemons.h"
#include "MSVectorMap.h"
#include "LOFDActorState.h"
#include "LOFDCampContext.h"
#include "LOFDActorBehavior.h"
#include "LOFDActorAi.h"
#include "LOFDScene.h"
#include "LOFDSkillController.h"
#include "ActorEvent.h"
#include <vector>

#define ACTOR_ANIMATION_GAP_TIME        0.1f

NS_LOFD_BEGIN
class ActorBehavior;
class MapScene;
class AIBehavior;
class SkillDef;
class SkillData;
class SkillCoolDownData;
class ActorSkillContext : public cocos2d::Ref
{
public:
    ActorSkillContext(){
        isSkilling = false;
        skillData = nullptr;
    };
    ~ActorSkillContext(){};
    /******技能******/
    lofd::SkillDef * skillDef;
    /******是否在技能中******/
    bool isSkilling;
    lofd::SkillData * skillData;
};

class ActorSkillController : public cocos2d::Ref
{
public:
    ActorSkillController(){};
    ~ActorSkillController(){};
    
    cocos2d::Vector<lofd::ActorSkillContext *> actorSkillContexts;
    /****当前焦点的技能*****/
    lofd::ActorSkillContext * currentFocuse;
};

/*******人物血条******/
class ActorBloodBar : public cocos2d::Sprite
{
public:
    ActorBloodBar(){};
    ~ActorBloodBar(){};
    
    CREATE_FUNC(ActorBloodBar);
};

class ActorData : public moonsugar::MapActorData
{
public:
    ActorData(){
        this->stateContext = new lofd::StateContext();
        this->aiStateContext = new lofd::StateContext();
        currentFocus = nullptr;
        this->isAI = false;
        this->isRange = true;
        this->isTrack = false;
        this->isTarget = false;
        this->isDead = false;
        this->isDeading = false;
        this->deadTime = 0;
        isCurrentFocus = false;
        currentWaitSkill = -1;
    };
    ~ActorData(){
        stopActorAllActions();
        disposeSkillCoolDownDatas();
        delete stateContext;
        delete aiStateContext;
        this->actorEntry->release();
        this->actorContainer->release();
        this->bloodTimer->release();
        this->actorDef = nullptr;
        actorStateGroupDef = nullptr;
        actorBehavior = nullptr;
        campContext = nullptr;
        aiBehavior = nullptr;
        currentFocus = nullptr;
    };
    
    int actorId;
    /********标示唯一性********/
    int tagId;
    /******状态上下文********/
    lofd::StateContext * stateContext;
    /******ai stateContext*****/
    lofd::StateContext * aiStateContext;
    /******camp context******/
    lofd::CampDef * campContext;
    /*******ActorStateDef******/
    lofd::ActorStateGroupDef * actorStateGroupDef;
    /********ActorDef*********/
    lofd::ActorDef * actorDef;
    /*******ActorBehaviorDef*****/
    lofd::ActorBehavior * actorBehavior;
    /*********AIBehaviorDef******/
    lofd::AIBehavior * aiBehavior;
    bool isAI;
    /*********标记是否接受范围检测事件********/
    bool isRange;
    /*******ActorEntry********/
    cocos2d::Sprite * actorEntry;
    /********ActorTotalContainer******/
    cocos2d::Sprite * actorContainer;
    /********BloodTimer********/
    cocos2d::ProgressTimer * bloodTimer;
    cocos2d::Map<std::string, cocos2d::Animate *> animationDic;
    /********ActorPropertyData*******/
    lofd::ActorPropertyData * actorPropertyData;
    
    lofd::MapScene * mapScene;
    
    //======ai==========
    /********当前范围内的actors********/
    cocos2d::Vector<lofd::ActorData *> currentRangeActors;
    lofd::ActorData * currentFocus;
    bool isCurrentFocus;
    /********当前焦点的目标点***********/
    cocos2d::Point targetPoint;
    
    int currentFocusSkill;
    int currentWaitSkill;
    lofd::SkillDef * currentSkillDef;
    lofd::ActorSkillController * actorSkillController;
    /******技能冷却及其他数据******/
    cocos2d::Vector<cocos2d::Ref *> skillCoolDownDatas;
    void onSkillCoolDownComplete(lofd::SkillDef * skillDefValue);
    void disposeSkillCoolDownDatas();
    lofd::SkillCoolDownData * getSkillCoolDownData(int skillId);
    
//    //======skill========
//    /******
//     **技能数据列表，actor当前角色所释放的技能列表**
//     *****/
    cocos2d::Vector<cocos2d::Ref *> skillDatas;
//
//    void skill(lofd::SkillData * skillDataValue);
//    void skillTotalEffectEnd(lofd::SkillData * skillDataValue){};
    //std::vector<lofd::LifeCycleController *> skillDatas;
    //cocos2d::Vector<lofd::LifeCycleController *> skillDatas;
    void skill(lofd::SkillData * skillDataValue);
    void skillTotalEffectEnd(lofd::SkillData * skillDataValue);
    
    void runAction(std::string actionTag);
    void runActionComplete();
    
    void move(cocos2d::Point endPoint, moonsugar::VectorMap * vectorMapValue);
    void moveComplete();
    
    /********死亡*******/
    void dead();
    void deadComplete();
    bool isDead;
    bool isDeading;
    float deadTime;
    
    int getActorFromCurrentRangeActors(int actorTagId);
    /**********跟随ActorData*********/
    void track();
    void trackComplete();
    bool isTrack;
    
    /*********目标target*********/
    void target();
    void targetComplete();
    bool isTarget;
    
    virtual void update(float dt);
    virtual void trackUpdate();
    virtual void targetUpdate();
    virtual void deadUpdate(float dt);
    
    virtual void stopActorAllActions();
    
    cocos2d::Rect * getBound();
    
    /******焦点新技能时使用*****/
    void changeSkillFocus(int skillId);
    /*****技能监听*****/
    cocos2d::EventListenerCustom * skillListener;
    void addGlobalListeners();
    void removeGlobalListeners();
    void onSkillTouchedHandler(cocos2d::EventCustom * event);
    
    void addtionHp(float value);
    void reduceHp(float value);
    
    void deleteCurrentRangeActors(lofd::ActorData * actorDataValue);
    
    CREATE_FUNC(ActorData);
};

class ActorControllerUtils
{
public:
    static ActorData * createActorDataById(int actorId);
    static void resetActorIdIndex();
};

NS_LOFD_END;

#endif /* defined(__LengendOfFightDemons__LOFDActor__) */
