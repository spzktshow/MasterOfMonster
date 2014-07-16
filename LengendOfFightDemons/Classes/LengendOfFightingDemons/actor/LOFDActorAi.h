//
//  LOFDActorAi.h
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-11.
//
//

#ifndef __LengendOfFightDemons__LOFDActorAi__
#define __LengendOfFightDemons__LOFDActorAi__

#include "lengendOfFightDemons.h"
#include "BehaviorTree.h"
#include "MSVectorMap.h"
#include "LOFDActor.h"
#include "external/json/rapidjson.h"
#include "external/json/document.h"
#include "LOFDActorBehavior.h"
/*************State*****************/
/******ai******/
///
#define LOFD_AI_STATE_IDLE                          /**/"idle"//ai 空闲状态@state*///
#define LOFD_AI_STATE_PATROL                        /**/"patrol"//ai 巡逻状态@state*///
#define LOFD_AI_STATE_MOVE                          /**/"move"//ai 移动状态, 一般情况下是custom操作触发的状态@state*///
#define LOFD_AI_STATE_ATTACK                        /**/"attack"//ai 攻击@state*///
#define LOFD_AI_STATE_AI_TRACK                      /**/"aiTrack"//ai技能track状态（ai的状态和人物track状态不同，ai属于总的状态，生命周期一般在人物做完施法动作之后)@state///
#define LOFD_AI_STATE_AI_TARGET                     /**/"aiTarget"//ai技能target状态@state*///
#define LOFD_AI_STATE_AI_MOVE                       /**/"aiMove"//移动状态@state*///
#define LOFD_AI_STATE_AI_PATROL                     /**/"aiPatrol"//巡逻状态@state*///
#define LOFD_AI_STATE_AI_IDLE                       /**/"aiIdle"//空闲状态@state*///
#define LOFD_AI_STATE_AI_STATE_TRACK                /**/"aiStateTrack"//ai 静态track状态@state///

#define LOFD_BEHAVIOR_EVENT_ATTACK_END              /**/"attackEndEvent"//老的事件，可能需要废弃@event*///

#define LOFD_BEHAVIOR_EVENT_NEW_ACTOR_INTO_RANGE    /**/"newActorIntoRangeEvent"//actor进入range(ai事件)@event*///
#define LOFD_BEHAVIOR_EVENT_NEW_ACTOR_OUT_RANGE     /**/"newActorOutRangeEvent"//actor从range中离开(ai事件)@event*///

#define LOFD_BEHAVIOR_EVENT_AI_TRACK                /**/"aiTrackEvent"//*******释放track技能时向ai发送track事件(操作事件or策略事件)****@event*///
#define LOFD_BEHAVIOR_EVENT_AI_TARGET               /**/"aiTargetEvent"//*******释放target技能时向ai发送target事件(操作事件or策略事件)*****@event*///
#define LOFD_BEHAVIOR_EVENT_AI_STATIC_TRACK         /**/"aiStaticTrackEvent"//****释放track技能时向ai发送static track⌚️****@event*///
#define LOFD_BEHAVIOR_EVENT_AI_PATROL               /**/"aiPatrolEvent"//*******巡逻状态(循环状态)(操作事件or策略事件)*******@event*///
#define LOFD_BEHAVIOR_EVENT_AI_CANCEL_PATROL        /**/"aiCancelPatrolEvent"//***************@event*///
#define LOFD_BEHAVIOR_EVENT_AI_IDLE                 /**/"aiIdleEvent"//*******空闲状态(循环状态)(操作事件or策略事件)*******@event*///
#define LOFD_BEHAVIOR_EVENT_AI_MOVE                 /**/"aiMoveEvent"//*******释放移动(操作事件or策略事件)*********@event*///

#define LOFD_BEHAVIOR_EVENT_TRACK_END               /**/"trackEndEvent"//***人物跟踪目标完成(人物行为结束发送事件)***@event*///
#define LOFD_BEHAVIOR_EVENT_TARGET_END              /**/"targetEndEvent"//***人物目标完成(人物行为结束发送事件)***@event*///
#define LOFD_BEHAVIOR_EVENT_STATIC_TRACK_END        /**/"staticTrackEndEvent"//****人物静态瞄准目标完成***@event*///
#define LOFD_BEHAVIOR_EVENT_ATK1_END                /**/"atk1EndEvent"//****人物普通攻击动作完成(人物行为结束发送事件)***@event*///
#define LOFD_BEHAVIOR_EVENT_ATK2_END                /**/"atk2EndEvent"//****人物普通攻击动作2完成(人物行为结束发送事件)***@event*///
#define LOFD_BEHAVIOR_EVENT_SKILL1_END              /**/"skill1EndEvent"//****人物技能攻击动作1完成(人物行为结束发送事件)****@event*///
#define LOFD_BEHAVIOR_EVENT_MOVE_END                /**/"moveEndEvent"//******移动结束事件完成(人物行为结束发送事件)******@event*///
#define LOFD_BEHAVIOR_EVENT_ACTOR_DEAD              /**/"actorDeadEvent"//******人物死忘事件(人物行为结束发送事件)*****@event*///

/*************Parse*****************/
#define LOFD_AI_BEHAVIOR_PARSE_TACTICS              "tactics"
#define LOFD_AI_BEHAVIOR_PARSE_ID                   "id"
#define LOFD_AI_BEHAVIOR_PARSE_NODE                 "node"
#define LOFD_AI_BEHAVIOR_PARSE_NAME                 "name"

/*************Debug*****************/
#ifndef LOFD_AI_DEBUG
#define LOFD_AI_DEBUG                            2/******静态定义是否需要测试tagId******/
#endif

NS_LOFD_BEGIN
extern int testTagId;

class ActorData;
class SkillData;
class ActorAIBehaviorData : public constellation::BehaviorData
{
public:
    ActorAIBehaviorData(){
        actorData = nullptr;
        mapSceneData = nullptr;
    };
    ~ActorAIBehaviorData(){
        mapSceneData = nullptr;
        actorData = nullptr;
    };
    lofd::ActorData * actorData;
    moonsugar::MapSceneData * mapSceneData;
    std::string operationType;
};

class AIBehaviorDynamicData : public lofd::ActorAIBehaviorData
{
public:
    AIBehaviorDynamicData(){
        point = nullptr;
        currentOperationActorData = nullptr;
        mapScene = nullptr;
    };
    ~AIBehaviorDynamicData(){
        //delete point;
        point = nullptr;
        currentOperationActorData = nullptr;
        mapScene = nullptr;
    };
    
    /******move*******/
    cocos2d::Point * point;
    //cocos2d::Vector<lofd::ActorData *> rangeChangeActors;
    //
    lofd::ActorData * currentOperationActorData;
    
    /*****track skill or normal track skill****/
    lofd::SkillData * skillData;
    //lofd::ActorData * selfData;
    moonsugar::MapScene * mapScene;
    /*****target point****/
    cocos2d::Point targetPoint;
};

class AIBehavior : public cocos2d::Ref
{
public:
    AIBehavior(){};
    ~AIBehavior(){
        delete root;
    };
    
    int aiId;
    std::string name;
    constellation::BehaviorNode * root;
};
/******************State*********************/
class AITrackOrTargetStateData : public lofd::ActorStateData
{
public:
    AITrackOrTargetStateData(std::string itemNameValue, std::string operationTypeValue):ActorStateData(itemNameValue, operationTypeValue){};
    ~AITrackOrTargetStateData(){
        selfActorData = nullptr;
    };
    /***cache selfActorData*****/
    lofd::ActorData * selfActorData;
    /*****地图信息******/
    moonsugar::MapScene * mapScene;
    
    cocos2d::Point targetPoint;
    
    static AITrackOrTargetStateData * create(std::string itemNameValue, std::string operationTypeValue)
    {
        lofd::AITrackOrTargetStateData * aiTrackOrTargetStateData = new lofd::AITrackOrTargetStateData(itemNameValue, operationTypeValue);
        if (aiTrackOrTargetStateData)
        {
            aiTrackOrTargetStateData->autorelease();
            return aiTrackOrTargetStateData;
        }
        else
        {
            delete aiTrackOrTargetStateData;
            aiTrackOrTargetStateData = nullptr;
            return nullptr;
        }
    };
};
/*****************Selector*******************/
class AISelectorBehaviorNode : public constellation::SelectorBehaviorNode
{
public:
    AISelectorBehaviorNode(std::string typeValue):SelectorBehaviorNode(typeValue){};
    ~AISelectorBehaviorNode(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AISelectorBehaviorNode * create(std::string typeValue)
    {
        AISelectorBehaviorNode * aiSelectorBehaviorNode = new lofd::AISelectorBehaviorNode(typeValue);
        if (aiSelectorBehaviorNode)
        {
            aiSelectorBehaviorNode->autorelease();
            return aiSelectorBehaviorNode;
        }
        else
        {
            delete aiSelectorBehaviorNode;
            aiSelectorBehaviorNode = nullptr;
            return nullptr;
        }
    };
};

/****************Sequence*********************/
class AISequenceBehaviorNode : public constellation::SequenceBehaviorNode
{
public:
    AISequenceBehaviorNode(std::string typeValue):SequenceBehaviorNode(typeValue){};
    ~AISequenceBehaviorNode(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AISequenceBehaviorNode * create(std::string typeValue)
    {
        AISequenceBehaviorNode * aiSequenceBehaviorNode = new AISequenceBehaviorNode(typeValue);
        if (aiSequenceBehaviorNode)
        {
            aiSequenceBehaviorNode->autorelease();
            return aiSequenceBehaviorNode;
        }
        else
        {
            delete aiSequenceBehaviorNode;
            aiSequenceBehaviorNode = nullptr;
            return nullptr;
        }
    };
};
/******************Action********************/
///
#define LOFD_AI_BehaviorTree_actionType_randMoveTarget                /**/"behaviorTreeActionTypeRandMoveTarget"//随机移动@action///
#define LOFD_DYNAMIC_KEY_RAND_TARGET         "randTarget"
class AIRandMoveTarget : public constellation::ActionBehaviorNode
{
public:
    AIRandMoveTarget(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){
    };
    ~AIRandMoveTarget(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIRandMoveTarget * create(std::string typeValue, std::string actionTypeValue)
    {
        AIRandMoveTarget * aiRandMoveTarget = new AIRandMoveTarget(typeValue, actionTypeValue);
        if (aiRandMoveTarget)
        {
            aiRandMoveTarget->autorelease();
            return aiRandMoveTarget;
        }
        else
        {
            delete aiRandMoveTarget;
            aiRandMoveTarget = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BehaviorTree_actionType_cancelCurrentState          /**/"behaviorTreeActionTypeCancelCurrentState"//cancel当前状态@action///
class AICancelCurrentState : public constellation::ActionBehaviorNode
{
public:
    AICancelCurrentState(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~AICancelCurrentState(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AICancelCurrentState * create(std::string typeValue, std::string actionTypeValue)
    {
        AICancelCurrentState * aiCancelCurrentState = new AICancelCurrentState(typeValue, actionTypeValue);
        if (aiCancelCurrentState)
        {
            aiCancelCurrentState->autorelease();
            return aiCancelCurrentState;
        }
        else
        {
            delete aiCancelCurrentState;
            aiCancelCurrentState = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BehaviorTree_actionType_cancelStackCacheState       /**/"behaviorTreeActionTypeCancelStackCacheState"//取消栈保存@action
class AICancelStackCacheState : public constellation::ActionBehaviorNode
{
public:
    AICancelStackCacheState(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~AICancelStackCacheState(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AICancelStackCacheState * create(std::string typeValue, std::string actionTypeValue)
    {
        AICancelStackCacheState * aiCancelStackCacheState = new AICancelStackCacheState(typeValue, actionTypeValue);
        if (aiCancelStackCacheState)
        {
            aiCancelStackCacheState->autorelease();
            return aiCancelStackCacheState;
        }
        else
        {
            delete aiCancelStackCacheState;
            aiCancelStackCacheState = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BehaviorTree_actionType_getRangeCampLessThan        /**/"behaviorTreeActionTypeGetRangeCampLessThan"//获取camp值小于@action|campCampareValue-int///
class AIGetRangeCampLessThan : public constellation::ActionBehaviorNode
{
public:
    AIGetRangeCampLessThan(std::string typeValue, std::string actionTypeValue, int campareCampValue):ActionBehaviorNode(typeValue, actionTypeValue){
        campareCamp = campareCampValue;
    };
    ~AIGetRangeCampLessThan(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    int campareCamp;
    
    static AIGetRangeCampLessThan * create(std::string typeValue, std::string actionTypeValue, int campareCampValue)
    {
        AIGetRangeCampLessThan * aiGetRangeCampLessThan = new AIGetRangeCampLessThan(typeValue, actionTypeValue, campareCampValue);
        if (aiGetRangeCampLessThan)
        {
            aiGetRangeCampLessThan->autorelease();
            return aiGetRangeCampLessThan;
        }
        else
        {
            delete aiGetRangeCampLessThan;
            aiGetRangeCampLessThan = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BehaviorTree_actionType_getRecentlyActorFromRange   /**/"behaviorTreeActionTypeGetRecentlyActorFromRange"//获取距离最近的actor@action///
class AIGetRecentlyActorFromRange : public constellation::ActionBehaviorNode
{
public:
    AIGetRecentlyActorFromRange(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~AIGetRecentlyActorFromRange(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIGetRecentlyActorFromRange * create(std::string typeValue, std::string actionTypeValue)
    {
        AIGetRecentlyActorFromRange * aiGetRecentlyActorFromRange = new AIGetRecentlyActorFromRange(typeValue, actionTypeValue);
        if (aiGetRecentlyActorFromRange)
        {
            aiGetRecentlyActorFromRange->autorelease();
            return aiGetRecentlyActorFromRange;
        }
        else
        {
            delete aiGetRecentlyActorFromRange;
            aiGetRecentlyActorFromRange = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BehaviorTree_actionType_focusActor          /**/"behaviorTreeActionTypeFocusActor"//焦点actor@action///
class AIFocusActor : public constellation::ActionBehaviorNode
{
public:
    AIFocusActor(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~AIFocusActor(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIFocusActor * create(std::string typeValue, std::string actionTypeValue)
    {
        AIFocusActor * aiFocusActor = new AIFocusActor(typeValue, actionTypeValue);
        if (aiFocusActor)
        {
            aiFocusActor->autorelease();
            return aiFocusActor;
        }
        else
        {
            delete aiFocusActor;
            aiFocusActor = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BehaviorTree_actionType_cancelFocusActor        /**/"behaviorTreeActionTypeCancelFocusActor"//cancel当前的actor@action///
class AICancelFocusActor : public constellation::ActionBehaviorNode
{
public:
    AICancelFocusActor(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~AICancelFocusActor(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AICancelFocusActor * create(std::string typeValue, std::string actionTypeValue)
    {
        AICancelFocusActor * aiCancelFocusActor = new AICancelFocusActor(typeValue, actionTypeValue);
        if (aiCancelFocusActor)
        {
            aiCancelFocusActor->autorelease();
            return aiCancelFocusActor;
        }
        else
        {
            delete aiCancelFocusActor;
            aiCancelFocusActor = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BehaviorTree_actionType_moveTo   /**/"behaviorTreeActionTypeActionMoveTo"//move to(废弃)@action///
class AIMoveTo : public constellation::ActionBehaviorNode
{
public:
    AIMoveTo(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~AIMoveTo(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIMoveTo * create(std::string typeValue, std::string actionTypeValue)
    {
        AIMoveTo * aiMoveTo = new AIMoveTo(typeValue, actionTypeValue);
        if (aiMoveTo)
        {
            aiMoveTo->autorelease();
            return aiMoveTo;
        }
        else
        {
            delete aiMoveTo;
            aiMoveTo = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BehaviorTree_actionType_attack   /**/"behaviorTreeActionTypeAttack"//attack@action///
class AIAttack : public constellation::ActionBehaviorNode
{
public:
    AIAttack(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~AIAttack(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIAttack * create(std::string typeValue, std::string actionTypeValue)
    {
        AIAttack * aiAttack = new AIAttack(typeValue, actionTypeValue);
        if (aiAttack)
        {
            aiAttack->autorelease();
            return aiAttack;
        }
        else
        {
            delete aiAttack;
            aiAttack = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BehaviorTree_actionType_track      /**/"behaviorTreeActionTypeTrack"//track@action///
class AITrack : public constellation::ActionBehaviorNode
{
public:
    AITrack(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~AITrack(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AITrack * create(std::string typeValue, std::string actionTypeValue)
    {
        AITrack * aiTrack = new AITrack(typeValue, actionTypeValue);
        if (aiTrack)
        {
            aiTrack->autorelease();
            return aiTrack;
        }
        else
        {
            delete aiTrack;
            aiTrack = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BehaviorTree_actionType_staticTrack     /**/"behaviorTreeActionTypeStaticTrack"//staticTrack@action///
class AIStaticTrack : public constellation::ActionBehaviorNode
{
public:
    AIStaticTrack(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~AIStaticTrack(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIStaticTrack * create(std::string typeValue, std::string actionTypeValue)
    {
        AIStaticTrack * aiStaticTrack = new AIStaticTrack(typeValue, actionTypeValue);
        if (aiStaticTrack)
        {
            aiStaticTrack->autorelease();
            return aiStaticTrack;
        }
        else
        {
            delete aiStaticTrack;
            aiStaticTrack = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BehaviorTree_actionType_target       /**/"behaviorTreeActionTypeTarget"//target@action///
class AITarget : public constellation::ActionBehaviorNode
{
public:
    AITarget(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~AITarget(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AITarget * create(std::string typeValue, std::string actionTypeValue)
    {
        AITarget * aiTarget = new AITarget(typeValue, actionTypeValue);
        if (aiTarget)
        {
            aiTarget->autorelease();
            return aiTarget;
        }
        else
        {
            delete aiTarget;
            aiTarget = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BehaviorTree_actionType_skillCurrentFocusSkill          /**/"behaviorTreeActionTypeSkillCurrentFocusSkill"//释放当前焦点技能@action///
class AISkillCurrentFocusSkill : public constellation::ActionBehaviorNode
{
public:
    AISkillCurrentFocusSkill(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~AISkillCurrentFocusSkill(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AISkillCurrentFocusSkill * create(std::string typeValue, std::string actionTypeValue)
    {
        AISkillCurrentFocusSkill * aiSkillCurrentFocusSkill = new AISkillCurrentFocusSkill(typeValue, actionTypeValue);
        if (aiSkillCurrentFocusSkill)
        {
            aiSkillCurrentFocusSkill->autorelease();
            return aiSkillCurrentFocusSkill;
        }
        else
        {
            delete aiSkillCurrentFocusSkill;
            aiSkillCurrentFocusSkill = nullptr;
            return nullptr;
        }
    }
};
///
#define LOFD_AI_BehaviorTree_actionType_patrol                          /**/"behaviorTreeActionTypePatrol"//巡逻@action///
class AIPatrol : public constellation::ActionBehaviorNode
{
public:
    AIPatrol(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~AIPatrol(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIPatrol * create(std::string typeValue, std::string actionTypeValue)
    {
        AIPatrol * aiPatrol = new AIPatrol(typeValue, actionTypeValue);
        if (aiPatrol)
        {
            aiPatrol->autorelease();
            return aiPatrol;
        }
        else
        {
            delete aiPatrol;
            aiPatrol = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BehaviorTree_actionType_idle                            /**/"behaviorTreeActionTypeIdle"//空闲@action///
class AIIdle : public constellation::ActionBehaviorNode
{
public:
    AIIdle(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~AIIdle(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIIdle * create(std::string typeValue, std::string actionTypeValue)
    {
        AIIdle * aiIdle = new AIIdle(typeValue, actionTypeValue);
        if (aiIdle)
        {
            aiIdle->autorelease();
            return aiIdle;
        }
        else
        {
            delete aiIdle;
            aiIdle = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BehaviorTree_actionType_move                            /**/"behaviorTreeActionTypeMove"//ai move@action///
class AIMove : public constellation::ActionBehaviorNode
{
public:
    AIMove(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~AIMove(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIMove * create(std::string typeValue, std::string actionTypeValue)
    {
        AIMove * aiMove = new AIMove(typeValue, actionTypeValue);
        if (aiMove)
        {
            aiMove->autorelease();
            return aiMove;
        }
        else
        {
            delete aiMove;
            aiMove = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BehaviorTree_actionType_focusUnCoolDownSkill            /**/"behaviorTreeActionTypeFocusUnCoolDownSkill"//焦点未coolDown的技能@action///
class AIFocusUncoolDownSkill : public constellation::ActionBehaviorNode
{
public:
    AIFocusUncoolDownSkill(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~AIFocusUncoolDownSkill(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIFocusUncoolDownSkill * create(std::string typeValue, std::string actionTypeValue)
    {
        AIFocusUncoolDownSkill * aiFocusUncoolDownSkill = new AIFocusUncoolDownSkill(typeValue, actionTypeValue);
        if (aiFocusUncoolDownSkill)
        {
            aiFocusUncoolDownSkill->autorelease();
            return aiFocusUncoolDownSkill;
        }
        else
        {
            delete aiFocusUncoolDownSkill;
            aiFocusUncoolDownSkill = nullptr;
            return nullptr;
        }
    };
};
/******************Condition**********************/
class AIConditionBehaviorNodeCampareEventType : public constellation::ConditionBehaviorNodeCampareEventType
{
public:
    AIConditionBehaviorNodeCampareEventType(std::string typeValue, std::string conditionTypeValue, std::string campareEventTypeValue):ConditionBehaviorNodeCampareEventType(typeValue, conditionTypeValue, campareEventTypeValue){};
    ~AIConditionBehaviorNodeCampareEventType(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventvalue);
    
    static AIConditionBehaviorNodeCampareEventType * create(std::string typeValue, std::string conditionTypeValue, std::string campareEventTypeValue)
    {
        AIConditionBehaviorNodeCampareEventType * aiConditionBehaviorNodeCampareEventType = new AIConditionBehaviorNodeCampareEventType(typeValue, conditionTypeValue, campareEventTypeValue);
        if (aiConditionBehaviorNodeCampareEventType)
        {
            aiConditionBehaviorNodeCampareEventType->autorelease();
            return aiConditionBehaviorNodeCampareEventType;
        }
        else
        {
            delete aiConditionBehaviorNodeCampareEventType;
            aiConditionBehaviorNodeCampareEventType = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BEHAVIOR_PARSE_CAMPARE_CURRENT_STATE                /**/"campareCurrentState"//比较当前状态@condition|campareCurrentState-string*state///
class AIConditionBehaviorEventCampareCurrentState : public constellation::ConditionBehaviorNode
{
public:
    AIConditionBehaviorEventCampareCurrentState(std::string typeValue, std::string conditionTypeValue, std::string campareCurrentStateValue):ConditionBehaviorNode(typeValue, conditionTypeValue)
    {
        campareCurrentState = campareCurrentStateValue;
    };
    ~AIConditionBehaviorEventCampareCurrentState(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventvalue);
    
    std::string campareCurrentState;
    
    static AIConditionBehaviorEventCampareCurrentState * create(std::string typeValue, std::string conditionTypeValue, std::string campareCurrentStateValue)
    {
        AIConditionBehaviorEventCampareCurrentState * aiConditionBehaviorEventCampareCurrentState = new AIConditionBehaviorEventCampareCurrentState(typeValue, conditionTypeValue, campareCurrentStateValue);
        if (aiConditionBehaviorEventCampareCurrentState)
        {
            aiConditionBehaviorEventCampareCurrentState->autorelease();
            return aiConditionBehaviorEventCampareCurrentState;
        }
        else
        {
            delete aiConditionBehaviorEventCampareCurrentState;
            aiConditionBehaviorEventCampareCurrentState = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BEHAVIOR_TYPE_HAS_FOCUS         /**/"hasFocus"//是否当前有焦点@condition///
class AIConditionHasFocus : public constellation::ConditionBehaviorNode
{
public:
    AIConditionHasFocus(std::string typeValue, std::string conditionTypeValue):ConditionBehaviorNode(typeValue, conditionTypeValue){};
    ~AIConditionHasFocus(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIConditionHasFocus * create(std::string typeValue, std::string conditionTypeValue)
    {
        AIConditionHasFocus * aiConditionHasFocus = new AIConditionHasFocus(typeValue, conditionTypeValue);
        if (aiConditionHasFocus)
        {
            aiConditionHasFocus->autorelease();
            return aiConditionHasFocus;
        }
        else
        {
            delete aiConditionHasFocus;
            aiConditionHasFocus = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BEHAVIOR_TYPE_IS_FOCUS          /**/"isFocus"//是否焦点@condition///
class AIConditionIsFocus : public constellation::ConditionBehaviorNode
{
public:
    AIConditionIsFocus(std::string typeValue, std::string conditionTypeValue):ConditionBehaviorNode(typeValue, conditionTypeValue){};
    ~AIConditionIsFocus(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIConditionIsFocus * create(std::string typeValue, std::string conditionTypeValue)
    {
        AIConditionIsFocus * aiConditionIsFocus = new AIConditionIsFocus(typeValue, conditionTypeValue);
        if (aiConditionIsFocus)
        {
            aiConditionIsFocus->autorelease();
            return aiConditionIsFocus;
        }
        else
        {
            delete aiConditionIsFocus;
            aiConditionIsFocus = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BEHAVIOR_TYPE_IS_FOCUS_DEAD     /**/"isFocusDead"//是否焦点的已经死亡@condition///
class AIConditionIsFocusDead : public constellation::ConditionBehaviorNode
{
public:
    AIConditionIsFocusDead(std::string typeValue, std::string conditionTypeValue):ConditionBehaviorNode(typeValue, conditionTypeValue){};
    ~AIConditionIsFocusDead(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIConditionIsFocusDead * create(std::string typeValue, std::string conditionTypeValue)
    {
        AIConditionIsFocusDead * aiConditionIsFocusDead = new AIConditionIsFocusDead(typeValue, conditionTypeValue);
        if (aiConditionIsFocusDead)
        {
            aiConditionIsFocusDead->autorelease();
            return aiConditionIsFocusDead;
        }
        else
        {
            delete aiConditionIsFocusDead;
            aiConditionIsFocusDead = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BEHAVIOR_TYPE_ALLOW_AI          /**/"allowAI"//是否容许ai@condition///
class AIConditionAllowAI : public constellation::ConditionBehaviorNode
{
public:
    AIConditionAllowAI(std::string typeValue, std::string conditionTypeValue):ConditionBehaviorNode(typeValue, conditionTypeValue){};
    ~AIConditionAllowAI(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIConditionAllowAI * create(std::string typeValue, std::string conditionTypeValue)
    {
        AIConditionAllowAI * aiConditionAllowAI = new AIConditionAllowAI(typeValue, conditionTypeValue);
        if (aiConditionAllowAI)
        {
            aiConditionAllowAI->autorelease();
            return aiConditionAllowAI;
        }
        else
        {
            delete aiConditionAllowAI;
            aiConditionAllowAI = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BEHAVIOR_TYPE_CAMP_GREATER_THAN /**/"campGreaterThan"//比较camp值是否大于@condition|campCampareValue-int///
#define LOFD_AI_BEHAVIOR_TYPE_CAMP_EQUAL        /**/"campEqual"//比较camp值等于@condition|campCampareValue-int///
#define LOFD_AI_BEHAVIOR_TYPE_CAMP_LESS_THAN    /**/"campLessThan"//比较camp值小于@condition|campCampareValue-int///

#define LOFD_AI_BEHAVIOR_KEY_CAMP_CAMPARE_VALUE "campCampareValue"
class AIConditionCampGreaterThan : public constellation::ConditionBehaviorNode
{
public:
    AIConditionCampGreaterThan(std::string typeValue, std::string conditionTypeValue, int campareCampValue):ConditionBehaviorNode(typeValue, conditionTypeValue){
        campareCamp = campareCampValue;
    };
    ~AIConditionCampGreaterThan(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    int campareCamp;
    
    static AIConditionCampGreaterThan * create(std::string typeValue, std::string conditionTypeValue, int campareCampValue)
    {
        AIConditionCampGreaterThan * aiConditionCampGreaterThan = new AIConditionCampGreaterThan(typeValue, conditionTypeValue, campareCampValue);
        if (aiConditionCampGreaterThan)
        {
            aiConditionCampGreaterThan->autorelease();
            return aiConditionCampGreaterThan;
        }
        else
        {
            delete aiConditionCampGreaterThan;
            aiConditionCampGreaterThan = nullptr;
            return nullptr;
        }
    };
};

class AIConditionCampLessThan : public constellation::ConditionBehaviorNode
{
public:
    AIConditionCampLessThan(std::string typeValue, std::string conditionTypeValue, int campareCampValue):ConditionBehaviorNode(typeValue, conditionTypeValue){
        campareCamp = campareCampValue;
    };
    ~AIConditionCampLessThan(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    int campareCamp;
    
    static AIConditionCampLessThan * create(std::string typeValue, std::string conditionTypeValue, int campareCampValue)
    {
        AIConditionCampLessThan * aiConditionCampLessThan = new AIConditionCampLessThan(typeValue, conditionTypeValue, campareCampValue);
        if (aiConditionCampLessThan)
        {
            aiConditionCampLessThan->autorelease();
            return aiConditionCampLessThan;
        }
        else
        {
            delete aiConditionCampLessThan;
            aiConditionCampLessThan = nullptr;
            return nullptr;
        }
    };
};

class AIConditionCampEqual : public constellation::ConditionBehaviorNode
{
public:
    AIConditionCampEqual(std::string typeValue, std::string conditionTypeValue, int campareCampValue):ConditionBehaviorNode(typeValue, conditionTypeValue){
        campareCamp = campareCampValue;
    };
    ~AIConditionCampEqual(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    int campareCamp;
    
    static AIConditionCampEqual * create(std::string typeValue, std::string conditionTypeValue, int campareCampValue)
    {
        AIConditionCampEqual * aiConditionCampEqual = new AIConditionCampEqual(typeValue, conditionTypeValue, campareCampValue);
        if (aiConditionCampEqual)
        {
            aiConditionCampEqual->autorelease();
            return aiConditionCampEqual;
        }
        else
        {
            delete aiConditionCampEqual;
            aiConditionCampEqual = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BEHAVIOR_TYPE_IS_CURRENT_FOCUS_SKILL_IS_AUTO            /**/"isCurrentFocusSkillIsAuto"//是否当前得焦点技能为自动攻击@condition///
class AIConditionIsCurrentFocusSkillIsAuto : public constellation::ConditionBehaviorNode
{
public:
    AIConditionIsCurrentFocusSkillIsAuto(std::string typeValue, std::string conditionTypeValue):ConditionBehaviorNode(typeValue, conditionTypeValue){};
    ~AIConditionIsCurrentFocusSkillIsAuto(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIConditionIsCurrentFocusSkillIsAuto * create(std::string typeValue, std::string conditionTypeValue)
    {
        AIConditionIsCurrentFocusSkillIsAuto * aiConditionIsCurrentFocusSkillIsAuto = new AIConditionIsCurrentFocusSkillIsAuto(typeValue, conditionTypeValue);
        if (aiConditionIsCurrentFocusSkillIsAuto)
        {
            aiConditionIsCurrentFocusSkillIsAuto->autorelease();
            return aiConditionIsCurrentFocusSkillIsAuto;
        }
        else
        {
            delete aiConditionIsCurrentFocusSkillIsAuto;
            aiConditionIsCurrentFocusSkillIsAuto = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BEHAVIOR_TYPE_IS_AI_TO_CUSTOM                         /**/"isAiToCustom"//是否是ai类型事件进入了自定义事件@condition///
class AIConditionIsAiToCustom : public constellation::ConditionBehaviorNode
{
public:
    AIConditionIsAiToCustom(std::string typeValue, std::string conditionTypeValue):ConditionBehaviorNode(typeValue, conditionTypeValue){};
    ~AIConditionIsAiToCustom(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIConditionIsAiToCustom * create(std::string typeValue, std::string conditionTypeValue)
    {
        AIConditionIsAiToCustom * aiConditionIsAiToCustom = new AIConditionIsAiToCustom(typeValue, conditionTypeValue);
        if (aiConditionIsAiToCustom)
        {
            aiConditionIsAiToCustom->autorelease();
            return aiConditionIsAiToCustom;
        }
        else
        {
            delete aiConditionIsAiToCustom;
            aiConditionIsAiToCustom = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BEHAVIOR_TYPE_IS_AI_OPERATION                          /**/"isAIOperation"//判断事件类型是否是ai操作@condition///
class AIConditionIsAIOperation : public constellation::ConditionBehaviorNode
{
public:
    AIConditionIsAIOperation(std::string typeValue, std::string conditionTypeValue):ConditionBehaviorNode(typeValue, conditionTypeValue){};
    ~AIConditionIsAIOperation(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIConditionIsAIOperation * create(std::string typeValue, std::string conditionTypeValue)
    {
        AIConditionIsAIOperation * aiConditionIsAiOperation = new AIConditionIsAIOperation(typeValue, conditionTypeValue);
        if (aiConditionIsAiOperation)
        {
            aiConditionIsAiOperation->autorelease();
            return aiConditionIsAiOperation;
        }
        else
        {
            delete aiConditionIsAiOperation;
            aiConditionIsAiOperation = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BEHAVIOR_TYPE_IS_CUSTOM_OPERATION                       /**/"isCustomOperation"//判断事件类型是否是自定义操作@condition///
class AIConditionIsCustomOperation : public constellation::ConditionBehaviorNode
{
public:
    AIConditionIsCustomOperation(std::string typeValue, std::string conditionTypeValue):ConditionBehaviorNode(typeValue, conditionTypeValue){};
    ~AIConditionIsCustomOperation(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static AIConditionIsCustomOperation * create(std::string typeValue, std::string conditionTypeValue)
    {
        AIConditionIsCustomOperation * aiConditionIsCustomOperation = new AIConditionIsCustomOperation(typeValue, conditionTypeValue);
        if (aiConditionIsCustomOperation)
        {
            aiConditionIsCustomOperation->autorelease();
            return aiConditionIsCustomOperation;
        }
        else
        {
            delete aiConditionIsCustomOperation;
            aiConditionIsCustomOperation = nullptr;
            return nullptr;
        }
    };
};
///
#define LOFD_AI_BEHAVIOR_TYPE_CHECK_CURRENT_FOCUS_SKILL_OPERATION_TYPE                /**/"checkCurrentFocusSkillOperationType"//检测当前技能得操作类型@condition|checkSkillOperationType-string///
#define LOFD_AI_BEHAVIOR_KEY_CHECK_SKILL_OPERATION_TYPE                               "checkSkillOperationType"
class AIConditionCheckCurrentFocusSkillOperationType : public constellation::ConditionBehaviorNode
{
public:
    AIConditionCheckCurrentFocusSkillOperationType(std::string typeValue, std::string conditionTypeValue, std::string checkSkillOperationTypeValue):ConditionBehaviorNode(typeValue, conditionTypeValue){
        checkSkillOperationType = checkSkillOperationTypeValue;
    };
    ~AIConditionCheckCurrentFocusSkillOperationType(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    std::string checkSkillOperationType;
    
    static AIConditionCheckCurrentFocusSkillOperationType * create(std::string typeValue, std::string conditionTypeValue, std::string checkSkillOperationTypeValue)
    {
        AIConditionCheckCurrentFocusSkillOperationType * aiConditionCheckCurrentFocusSkillOperationType = new AIConditionCheckCurrentFocusSkillOperationType(typeValue, conditionTypeValue, checkSkillOperationTypeValue);
        if (aiConditionCheckCurrentFocusSkillOperationType)
        {
            aiConditionCheckCurrentFocusSkillOperationType->autorelease();
            return aiConditionCheckCurrentFocusSkillOperationType;
        }
        else
        {
            delete aiConditionCheckCurrentFocusSkillOperationType;
            aiConditionCheckCurrentFocusSkillOperationType = nullptr;
            return nullptr;
        }
    };
};
/************/
class AIConfig : public constellation::BehaviorTreeParse
{
public:
    AIConfig(){};
    ~AIConfig(){};
    
    cocos2d::Vector<lofd::AIBehavior*> tactics;
    void parse(std::string behaviorConfig);
    lofd::AIBehavior * getAIBehaviorById(int aiId);
    
    virtual constellation::ActionBehaviorNode * parseActionNode(const rapidjson::Value &nodeValue, std::string type, std::string name);
    virtual constellation::ConditionBehaviorNode * parseConditionNode(const rapidjson::Value &nodeValue, std::string type, std::string name);
    
    virtual constellation::SequenceBehaviorNode * parseSequenceNode(const rapidjson::Value &nodeValue, std::string type, std::string name);
    virtual constellation::SelectorBehaviorNode * parseSelectorNode(const rapidjson::Value &nodeValue, std::string type, std::string name);
};

NS_LOFD_END;

#endif /* defined(__LengendOfFightDemons__LOFDActorAi__) */
