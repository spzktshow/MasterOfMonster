//
//  LOFDActorBehavior.h
//  Vector
//
//  Created by RockLee on 14-4-4.
//
//

#ifndef __Vector__LOFDActorBehavior__
#define __Vector__LOFDActorBehavior__

#include "lengendOfFightDemons.h"
#include "BehaviorTree.h"
#include "external/json/rapidjson.h"
#include "external/json/document.h"
#include "LOFDActorState.h"
#include "MSVectorMap.h"
#include "LOFDCampContext.h"
#include "LOFDActor.h"

#define LOFD_ACTOR_STATE_ATK1       "30"
#define LOFD_ACTOR_STATE_ATK2       "31"
#define LOFD_ACTOR_STATE_HURT       "50"
#define LOFD_ACTOR_STATE_IDLE       "10"
#define LOFD_ACTOR_STATE_RUN        "20"
#define LOFD_ACTOR_STATE_SKILL1     "40"
#define LOFD_ACTOR_STATE_TRACK      "21"
#define LOFD_ACTOR_STATE_TARGET     "22"
#define LOFD_ACTOR_STATE_DEAD       "60"

#define LOFD_ACTOR_STATE_DEAD_EFFECT "deadEffect"

#define LOFD_ACTOR_BEHAVIOR_EVENT_MOVE              "directEvent"
#define LOFD_ACTOR_BEHAVIOR_EVENT_CANCEL_MOVE       "cancelDirectEvent"
#define LOFD_ACTOR_BEHAVIOR_EVENT_IDLE              "idleEvent"
#define LOFD_ACTOR_BEHAVIOR_EVENT_ATK1              "atk1Event"
#define LOFD_ACTOR_BEHAVIOR_EVENT_CANCEL_ATK        "cancelAtkEvent"
#define LOFD_ACTOR_BEHAVIOR_EVENT_SKILL             "skillEvent"
#define LOFD_ACTOR_BEHAVIOR_EVENT_SKILL1             "skill1Event"
#define LOFD_ACTOR_BEHAVIOR_EVENT_CANCEL_SKILL      "cancelSkillEvent"

#define LOFD_ACTOR_BEHAVIOR_EVENT_CANCEL_CURRENT    "cancelCurrentEvent"
#define LOFD_ACTOR_BEHAVIOR_EVENT_TRACK             "trackEvent"
#define LOFD_ACTOR_BEHAVIOR_EVENT_TARGET            "targetEvent"
#define LOFD_ACTOR_BEHAVIOR_EVENT_CANCEL_TRACK      "cancelTrackEvent"
#define LOFD_ACTOR_BEHAVIOR_EVENT_TRACK_SKILL       "trackSkillEvent"
#define LOFD_ACTOR_BHEAVIOR_EVENT_TARGET_SKILL      "targetSkillEvent"
//#define LOFD_ACTOR_BEHAVIOR_EVENT_CANCEL_TRACK_SKILL    "cancelTrackSkillEvent"
#define LOFD_ACTOR_BEHAVIOR_EVENT_TARGET             "targetEvent"
#define LOFD_ACTOR_BEHAVIOR_EVENT_CANCEL_TARGET     "cancelTargetEvent"
#define LOFD_ACTOR_BEHAVIOR_EVENT_HURT              "hurtEvent"
#define LOFD_ACTOR_BEHAVIOR_EVENT_CANCEL_HURT       "caneclHurtEvent"
#define LOFD_ACTOR_BEHAVIOR_EVENT_DEAD              "deadEvent"

#define LOFD_ACTOR_BEHAVIOR_PARSE_ACTION            "action"
#define LOFD_ACTOR_BEHAVIOR_PARSE_ACTIONS           "actions"
#define LOFD_ACTOR_BEHAVIOR_PARSE_ID                "id"
#define LOFD_ACTOR_BEHAVIOR_PARSE_NODE              "node"
#define LOFD_ACTOR_BEHAVIOR_PARSE_NAME              "name"

/****************用户自定义操作****************/
#define LOFD_ACTOR_STATE_OPERATION_TYPE_CUSOTM      "custom"
/****************ai操作****************/
#define LOFD_ACTOR_STATE_OPERATION_TYPE_AI          "ai"
/****************系统操作**************/
#define LOFD_ACTOR_STATE_OPERATION_TYPE_SYSTEM      "system"

/****************operationMap***************/
/****************
                ai      custom      system
    ai(to)      o          x           o
    custom(to)  o          o           o
    system(to)  o          o           o
 ****************/
NS_LOFD_BEGIN
class ActorData;
class ActorBehavior : public cocos2d::Ref
{
public:
    ActorBehavior();
    ~ActorBehavior();
    
    int behaviorId;
    
    std::string name;
    
    constellation::BehaviorNode * root;
};

class ActorBehaviorData : public constellation::BehaviorData
{
public:
    ActorBehaviorData(){};
    ~ActorBehaviorData(){
        actorData = nullptr;
    };
    
    lofd::ActorData * actorData;
    std::string operationType;
};

class ActorBehaviorRunData : public lofd::ActorBehaviorData
{
public:
    ActorBehaviorRunData(){};
    ~ActorBehaviorRunData(){
        vectorMap = nullptr;
    };
    
    cocos2d::Point endPoint;
    moonsugar::VectorMap * vectorMap;
};

class ActorBehaviorSkillData : public lofd::ActorBehaviorData
{
public:
    ActorBehaviorSkillData(){};
    ~ActorBehaviorSkillData(){
        skillData = nullptr;
    };
    
    cocos2d::Ref * skillData;
};
\

class ActorBehaviorTrackData : public lofd::ActorBehaviorData
{
public:
    ActorBehaviorTrackData(){};
    ~ActorBehaviorTrackData(){
        skillData = nullptr;
    };
    
    //lofd::ActorData * targetActorData;
    
    cocos2d::Ref * skillData;
};

class ActorBehaviorTargetData : public lofd::ActorBehaviorData
{
public:
    ActorBehaviorTargetData(){};
    ~ActorBehaviorTargetData(){
        skillData = nullptr;
    }
    cocos2d::Ref * skillData;
};

class ActorBehaviorHurtData : public lofd::ActorBehaviorData
{
public:
    ActorBehaviorHurtData(){};
    ~ActorBehaviorHurtData(){};
    
    /*****伤害我的Actor*****/
    lofd::ActorData * hurtActorData;
};
/************ActorRunStateData**********/
class ActorRunStateData : public lofd::ActorStateData
{
public:
    ActorRunStateData(std::string itemNameValue, std::string operationTypeValue):ActorStateData(itemNameValue, operationTypeValue){
    };
    ~ActorRunStateData(){
        vectorMap = nullptr;
    };
    
    cocos2d::Point endPoint;
    moonsugar::VectorMap * vectorMap;
    
    static ActorRunStateData * create(std::string itemNameValue, std::string operationTypeValue)
    {
        ActorRunStateData * actorRunStateData = new ActorRunStateData(itemNameValue, operationTypeValue);
        if (actorRunStateData)
        {
            actorRunStateData->autorelease();
            return actorRunStateData;
        }
        else
        {
            delete actorRunStateData;
            actorRunStateData = nullptr;
            return actorRunStateData;
        }
    };
};

class ActorSkillStateData : public lofd::ActorStateData
{
public:
    ActorSkillStateData(std::string itemNameValue, std::string operationTypeValue):ActorStateData(itemNameValue, operationTypeValue)
    {};
    ~ActorSkillStateData()
    {
        skillData = nullptr;
        actorData = nullptr;
    };
    
    lofd::ActorData * actorData;
    cocos2d::Ref * skillData;
    
    static ActorSkillStateData * create(std::string itemNameValue, std::string operationTypeValue)
    {
        ActorSkillStateData * actorSkillStateData = new ActorSkillStateData(itemNameValue, operationTypeValue);
        if (actorSkillStateData)
        {
            actorSkillStateData->autorelease();
            return actorSkillStateData;
        }
        else
        {
            delete actorSkillStateData;
            actorSkillStateData = nullptr;
            return nullptr;
        }
    };
};

class ActorTrackStateData : public lofd::ActorStateData
{
public:
    ActorTrackStateData(std::string itemNameValue, std::string operationTypeValue):ActorStateData(itemNameValue, operationTypeValue)
    {
    };
    ~ActorTrackStateData(){
        actorData = nullptr;
    };
    
    lofd::ActorData * actorData;
    
    static ActorTrackStateData * create(std::string itemNameValue, std::string operationTypeValue)
    {
        ActorTrackStateData * actorTrackStateData = new ActorTrackStateData(itemNameValue, operationTypeValue);
        if (actorTrackStateData)
        {
            actorTrackStateData->autorelease();
            return actorTrackStateData;
        }
        else
        {
            delete actorTrackStateData;
            actorTrackStateData = nullptr;
            return nullptr;
        }
    };
};

class ActorTargetStateData : public lofd::ActorStateData
{
public:
    ActorTargetStateData(std::string itemNameValue, std::string operationTypeValue):ActorStateData(itemNameValue, operationTypeValue){};
    ~ActorTargetStateData(){
        actorData = nullptr;
    };
    
    lofd::ActorData * actorData;
    
    static ActorTargetStateData * create(std::string itemNameValue, std::string operationTypeValue)
    {
        ActorTargetStateData * actorTargetStateData = new ActorTargetStateData(itemNameValue, operationTypeValue);
        if (actorTargetStateData)
        {
            actorTargetStateData->autorelease();
            return actorTargetStateData;
        }
        else
        {
            delete actorTargetStateData;
            actorTargetStateData = nullptr;
            return nullptr;
        }
    };
};
/**********conditionNode********/
#define LOFD_ACTOR_BEHAVIOR_PARSE_CAMPARE_CURRENT_STATE     "campareCurrentState"
class ConditionBehaviorNodeCampareCurrentState : public constellation::ConditionBehaviorNode
{
public:
    ConditionBehaviorNodeCampareCurrentState(std::string typeValue, std::string conditionTypeValue, std::string campareCurretnStateValue):ConditionBehaviorNode(typeValue, conditionTypeValue){
        campareCurrentState = campareCurretnStateValue;
    };
    ~ConditionBehaviorNodeCampareCurrentState();
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventvalue);
    
    std::string campareCurrentState;
    
    static ConditionBehaviorNodeCampareCurrentState * create(std::string typeValue, std::string conditionTypeValue, std::string campareCurretnStateValue)
    {
        ConditionBehaviorNodeCampareCurrentState * conditionBehaviorNodeCampareCurrentState = new ConditionBehaviorNodeCampareCurrentState(typeValue, conditionTypeValue, campareCurretnStateValue);
        if (conditionBehaviorNodeCampareCurrentState)
        {
            conditionBehaviorNodeCampareCurrentState->autorelease();
            return conditionBehaviorNodeCampareCurrentState;
        }
        else
        {
            delete conditionBehaviorNodeCampareCurrentState;
            conditionBehaviorNodeCampareCurrentState = nullptr;
            return nullptr;
        }
    };
};

#define LOFD_ACTOR_BEHAVIOR_PARSE_CAMPARE_EVENT_TYPE        "compareEventType"
class ConditionBehaviorEventCampareEventType : public constellation::ConditionBehaviorNode
{
public:
    ConditionBehaviorEventCampareEventType(std::string typeValue, std::string conditionTypeValue, std::string campareEventTypeValue):ConditionBehaviorNode(typeValue, conditionTypeValue){
        campareEventType = campareEventTypeValue;
    };
    ~ConditionBehaviorEventCampareEventType();
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventvalue);
    
    std::string campareEventType;
    
    static ConditionBehaviorEventCampareEventType * create(std::string typeValue, std::string conditionTypeValue, std::string campareEventTypeValue)
    {
        ConditionBehaviorEventCampareEventType * conditionBehaviorEventCampareEventType = new ConditionBehaviorEventCampareEventType(typeValue, conditionTypeValue, campareEventTypeValue);
        if (conditionBehaviorEventCampareEventType)
        {
            conditionBehaviorEventCampareEventType->autorelease();
            return conditionBehaviorEventCampareEventType;
        }
        else
        {
            delete conditionBehaviorEventCampareEventType;
            conditionBehaviorEventCampareEventType = nullptr;
            return nullptr;
        }
    };
};

/*********actionNode*************/
#define BehaviorTree_actionType_actorConvertState           "behaviorTreeActionTypeActorConvertState"
#define LOFD_ACTOR_BEHAVIOR_PARSE_CONVERT_STATE             "convertState"
class ActorConvertState : public constellation::ActionBehaviorNode
{
public:
    ActorConvertState(std::string typeValue, std::string actionTypeValue, std::string convertStateValue):ActionBehaviorNode(typeValue, actionTypeValue){
        convertState = convertStateValue;
    };
    ~ActorConvertState(){
    
    };
    
    std::string convertState;
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static ActorConvertState * create(std::string typeValue, std::string actionTypeValue, std::string convertStateValue)
    {
        ActorConvertState * actorConvertState = new ActorConvertState(typeValue, actionTypeValue, convertStateValue);
        if (actorConvertState)
        {
            actorConvertState->autorelease();
            return actorConvertState;
        }
        else
        {
            delete actorConvertState;
            actorConvertState = nullptr;
            return nullptr;
        }
    };
};

#define BehaviorTree_actionType_cancelCurrentState             "behaviorTreeActionTypeCancelCurrentState"
class ActionCancelCurrentState : public constellation::ActionBehaviorNode
{
public:
    ActionCancelCurrentState(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){
    };
    ~ActionCancelCurrentState(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static ActionCancelCurrentState * create(std::string typeValue, std::string actionTypeValue)
    {
        ActionCancelCurrentState * actionCancelCurrentState = new ActionCancelCurrentState(typeValue, actionTypeValue);
        if (actionCancelCurrentState)
        {
            actionCancelCurrentState->autorelease();
            return actionCancelCurrentState;
        }
        else
        {
            delete actionCancelCurrentState;
            actionCancelCurrentState = nullptr;
            return nullptr;
        }
    };
};

#define BehaviorTree_actionType_cancelCurrentStateNoChange      "behaviorTreeActionTypeCancelCurrentStateNoChange"
class ActionCancelCurrentStateNoChange : public constellation::ActionBehaviorNode
{
public:
    ActionCancelCurrentStateNoChange(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue)
    {
        
    };
    ~ActionCancelCurrentStateNoChange(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static ActionCancelCurrentStateNoChange * create(std::string typeValue, std::string actionTypeValue)
    {
        ActionCancelCurrentStateNoChange * actionCancelCurrentStateNoChange = new ActionCancelCurrentStateNoChange(typeValue, actionTypeValue);
        if (actionCancelCurrentStateNoChange)
        {
            actionCancelCurrentStateNoChange->autorelease();
            return actionCancelCurrentStateNoChange;
        }
        else
        {
            delete actionCancelCurrentStateNoChange;
            actionCancelCurrentStateNoChange = nullptr;
            return nullptr;
        }
    };
};

#define BehaviorTree_actionType_atk1                        "behaviorTreeActionTypeAtk1"
class ActionAtk1 : public constellation::ActionBehaviorNode
{
public:
    ActionAtk1(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){
    };
    ~ActionAtk1(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static ActionAtk1 * create(std::string typeValue, std::string actionTypeValue)
    {
        ActionAtk1 * actionAtk1 = new ActionAtk1(typeValue, actionTypeValue);
        if (actionAtk1)
        {
            actionAtk1->autorelease();
            return actionAtk1;
        }
        else
        {
            delete actionAtk1;
            actionAtk1 = nullptr;
            return nullptr;
        }
    };
};

#define BehaviorTree_actionType_idle                        "behaviorTreeActionTypeIdle"
class ActionIdle : public constellation::ActionBehaviorNode
{
public:
    ActionIdle(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){
    };
    ~ActionIdle(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static ActionIdle * create(std::string typeValue, std::string actionTypeValue)
    {
        ActionIdle * actionIdle = new ActionIdle(typeValue, actionTypeValue);
        if (actionIdle)
        {
            actionIdle->autorelease();
            return actionIdle;
        }
        else
        {
            delete actionIdle;
            actionIdle = nullptr;
            return nullptr;
        }
    };
};

#define BehaviorTree_actionType_skill                       "behaviorTreeActionTypeSkill"
#define BehaviorTree_actionType_skill1                       "behaviorTreeActionTypeSkill1"
class ActionSkill : public constellation::ActionBehaviorNode
{
public:
    ActionSkill(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){
    };
    ~ActionSkill(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static ActionSkill * create(std::string typeValue, std::string actionTypeValue)
    {
        ActionSkill * actionSkill = new ActionSkill(typeValue, actionTypeValue);
        if (actionSkill)
        {
            actionSkill->autorelease();
            return actionSkill;
        }
        else
        {
            delete actionSkill;
            actionSkill = nullptr;
            return nullptr;
        }
    };
};

#define BehaviorTree_actionType_run                        "behaviorTreeActionTypeRun"
class ActionRun : public constellation::ActionBehaviorNode
{
public:
    ActionRun(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){
    };
    ~ActionRun(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static ActionRun * create(std::string typeValue, std::string actionTypeValue)
    {
        ActionRun * actionRun = new ActionRun(typeValue, actionTypeValue);
        if (actionRun)
        {
            actionRun->autorelease();
            return actionRun;
        }
        else
        {
            delete actionRun;
            actionRun = nullptr;
            return nullptr;
        }
    };
};

#define BehaviorTree_actionType_track                      "behaviorTreeActionTypeTrack"
class ActionTrack : public constellation::ActionBehaviorNode
{
public:
    ActionTrack(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){
    };
    
    ~ActionTrack(){};
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static ActionTrack * create(std::string typeValue, std::string actionTypeValue)
    {
        ActionTrack * actionTrack = new ActionTrack(typeValue, actionTypeValue);
        if (actionTrack)
        {
            actionTrack->autorelease();
            return actionTrack;
        }
        else
        {
            delete actionTrack;
            actionTrack = nullptr;
            return nullptr;
        }
    };
};

#define BehaviorTree_actionType_stackActorSkillData         "behaviorTreeActionTypeStackActorSkillData"
class ActionCreateActorBehaviorSkillData : public constellation::ActionBehaviorNode
{
public:
    ActionCreateActorBehaviorSkillData(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){
    };
    
    ~ActionCreateActorBehaviorSkillData(){};
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static ActionCreateActorBehaviorSkillData * create(std::string typeValue, std::string actionTypeValue)
    {
        ActionCreateActorBehaviorSkillData * actionCreateActorBehaviorSkillData =  new ActionCreateActorBehaviorSkillData(typeValue, actionTypeValue);
        if (actionCreateActorBehaviorSkillData)
        {
            actionCreateActorBehaviorSkillData->autorelease();
            return actionCreateActorBehaviorSkillData;
        }
        else
        {
            delete actionCreateActorBehaviorSkillData;
            actionCreateActorBehaviorSkillData = nullptr;
            return nullptr;
        }
    };
};

#define BehaviorTree_actionType_trackSkill          "behaviorTreeActionTypeTrackSkill"
class ActionTrackSkill : public constellation::ActionBehaviorNode
{
public:
    ActionTrackSkill(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~ActionTrackSkill(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static ActionTrackSkill * create(std::string typeValue, std::string actionTypeValue)
    {
        ActionTrackSkill * actionTrackSkill = new ActionTrackSkill(typeValue, actionTypeValue);
        if (actionTrackSkill)
        {
            actionTrackSkill->autorelease();
            return actionTrackSkill;
        }
        else
        {
            delete actionTrackSkill;
            actionTrackSkill = nullptr;
            return nullptr;
        }
    };
};

#define BehaviorTree_actionType_targetSkill         "behaviorTreeActionTypeTargetSkill"
class ActionTargetSkill : public constellation::ActionBehaviorNode
{
public:
    ActionTargetSkill(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~ActionTargetSkill(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static ActionTargetSkill * create(std::string typeValue, std::string actionTypeValue)
    {
        ActionTargetSkill * actionTargetSkill = new ActionTargetSkill(typeValue, actionTypeValue);
        if (actionTargetSkill)
        {
            actionTargetSkill->autorelease();
            return actionTargetSkill;
        }
        else
        {
            delete actionTargetSkill;
            actionTargetSkill = nullptr;
            return nullptr;
        }
    };
};

#define BehaviorTree_actionType_target                      "behaviorTreeActionTypeTarget"
class ActionTarget : public constellation::ActionBehaviorNode
{
public:
    ActionTarget(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){
    };
    
    ~ActionTarget(){};
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static ActionTarget * create(std::string typeValue, std::string actionTypeValue)
    {
        ActionTarget * actionTarget = new ActionTarget(typeValue, actionTypeValue);
        if (actionTarget)
        {
            actionTarget->autorelease();
            return actionTarget;
        }
        else
        {
            delete actionTarget;
            actionTarget = nullptr;
            return nullptr;
        }
    };
};

#define BehaviorTree_actionType_hurt                        "behaviorTreeActionTypeHurt"
class ActionHurt : public constellation::ActionBehaviorNode
{
public:
    ActionHurt(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~ActionHurt(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    static ActionHurt * create(std::string typeValue, std::string actionTypeValue)
    {
        ActionHurt * actionHurt = new ActionHurt(typeValue, actionTypeValue);
        if (actionHurt)
        {
            actionHurt->autorelease();
            return actionHurt;
        }
        else
        {
            delete actionHurt;
            actionHurt = nullptr;
            return nullptr;
        }
    };
};

#define BehaviorTree_actionType_dead                        "behaviorTreeActionTypeDead"
class ActionDead : public constellation::ActionBehaviorNode
{
public:
    ActionDead(std::string typeValue, std::string actionTypeValue):ActionBehaviorNode(typeValue, actionTypeValue){};
    ~ActionDead(){};
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventvalue);
    
    static ActionDead * create(std::string typeValue, std::string actionTypeValue)
    {
        ActionDead * actionDead = new ActionDead(typeValue, actionTypeValue);
        if (actionDead)
        {
            actionDead->autorelease();
            return actionDead;
        }
        else
        {
            delete actionDead;
            actionDead = nullptr;
            return nullptr;
        }
    };
};
/*******ActorActionConfig********/
class ActorActionConfig : public constellation::BehaviorTreeParse
{
public:
    ActorActionConfig(){};
    ~ActorActionConfig(){};
    
    void parse(std::string behaviorConfig);
    constellation::BehaviorNode * createNode(const rapidjson::Value& nodeValue);
    
    cocos2d::Vector<lofd::ActorBehavior*> actions;
    
    lofd::ActorBehavior * getActorBehaviorById(int behaviorId);
    
    virtual constellation::ActionBehaviorNode * parseActionNode(const rapidjson::Value &nodeValue, std::string type, std::string name);
    virtual constellation::ConditionBehaviorNode * parseConditionNode(const rapidjson::Value &nodeValue, std::string type, std::string name);
};
NS_LOFD_END;
#endif /* defined(__Vector__LOFDActorBehavior__) */
