//
//  LOFDActorBehavior.cpp
//  Vector
//
//  Created by RockLee on 14-4-4.
//
//

#include "LOFDActorBehavior.h"
#include "editor-support/cocostudio/DictionaryHelper.h"
#include "LOFDActorAi.h"
#include "LOFDSkillData.h"
#include "LOFDConfigManager.h"

NS_LOFD_BEGIN
class ActorAIBehaviorData;
class ActorData;
/*********ActorBehavior*******/
lofd::ActorBehavior::ActorBehavior()
{
    behaviorId = -1;
    root = nullptr;
    name = "";
}

ActorBehavior::~ActorBehavior()
{
    delete root;
}
/*******ActorConvertState**********/
int ActorConvertState::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    ActionBehaviorNode::execute(behaviorEventValue);
    //cocos2d::log("convert state %s", this->actionType.c_str());
    //lofd::ActorBehaviorData * behaviorData = (lofd::ActorBehaviorData *)behaviorEventValue->behaviorData;
    //behaviorData->actorData->currentState = this->convertState;
    return BEHAVIOR_RESULT_SUCCESS;
}

/************ActionCancelCurrentState**********/
int ActionCancelCurrentState::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    ActionBehaviorNode::execute(behaviorEventValue);
    lofd::ActorBehaviorData * behaviorData = (lofd::ActorBehaviorData *)behaviorEventValue->behaviorData;
    //cocos2d::log("Cancel current state %s", behaviorData->actorData->stateContext->currentState->itemName.c_str());
    behaviorData->actorData->stateContext->popStateDataChangeNext();
    behaviorData->actorData->actorEntry->stopAllActions();
    behaviorData->actorData->actorContainer->stopAllActions();
    behaviorData->actorData->stopActorAllActions();
    //cocos2d::log("itemName: %s", behaviorData->actorData->stateContext->currentState->itemName.c_str());
    if (behaviorData->actorData->stateContext->currentState->itemName == LOFD_ACTOR_STATE_ATK1
        || behaviorData->actorData->stateContext->currentState->itemName == LOFD_ACTOR_STATE_ATK2
        || behaviorData->actorData->stateContext->currentState->itemName == LOFD_ACTOR_STATE_SKILL1)
    {
        ActorSkillStateData * actorSkillStateData = dynamic_cast<ActorSkillStateData *>(behaviorData->actorData->stateContext->currentState);
        behaviorData->actorData->runAction(behaviorData->actorData->stateContext->currentState->itemName);
        behaviorData->actorData->skill(dynamic_cast<lofd::SkillData *>(actorSkillStateData->skillData));
    }
    else if (behaviorData->actorData->stateContext->currentState->itemName == LOFD_ACTOR_STATE_RUN)
    {
        behaviorData->actorData->runAction(behaviorData->actorData->stateContext->currentState->itemName);
        ActorRunStateData * actorRunStateData = dynamic_cast<ActorRunStateData *>(behaviorData->actorData->stateContext->currentState);
        behaviorData->actorData->move(actorRunStateData->endPoint, actorRunStateData->vectorMap);
    }
    else
    {
        behaviorData->actorData->runAction(behaviorData->actorData->stateContext->currentState->itemName);
    }
    return BEHAVIOR_RESULT_SUCCESS;
}

/***********ActionCancelCurrentStateNoChange********/
int ActionCancelCurrentStateNoChange::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    ActionBehaviorNode::execute(behaviorEventValue);
    lofd::ActorBehaviorData * behaviorData = (lofd::ActorBehaviorData *)behaviorEventValue->behaviorData;
    //cocos2d::log("Cancel current state %s", behaviorData->actorData->stateContext->currentState->itemName.c_str());
    behaviorData->actorData->stateContext->popStateDataChangeNext();
    behaviorData->actorData->actorEntry->stopAllActions();
    behaviorData->actorData->actorContainer->stopAllActions();
    behaviorData->actorData->stopActorAllActions();
    //cocos2d::log("itemName: %s", behaviorData->actorData->stateContext->currentState->itemName.c_str());
    return BEHAVIOR_RESULT_SUCCESS;
}

/*************ActionAtk1******************/
int ActionAtk1::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    ActionBehaviorNode::execute(behaviorEventValue);
    lofd::ActorBehaviorData * behaviorData = (lofd::ActorBehaviorData *)behaviorEventValue->behaviorData;
    //cocos2d::log("atk1 execute");
    //lofd::ActorStateData * stateData = new lofd::ActorStateData(LOFD_ACTOR_STATE_ATK1, behaviorData->operationType);
    lofd::ActorStateData * stateData = lofd::ActorStateData::create(LOFD_ACTOR_STATE_ATK1, behaviorData->operationType);
    behaviorData->actorData->stateContext->insertStateData(stateData);
    behaviorData->actorData->actorEntry->stopAllActions();
    behaviorData->actorData->actorContainer->stopAllActions();
    behaviorData->actorData->stopActorAllActions();
    behaviorData->actorData->runAction(stateData->itemName);
    return BEHAVIOR_RESULT_SUCCESS;
}

/************ActionIdle*******************/
int ActionIdle::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    ActionBehaviorNode::execute(behaviorEventValue);
    lofd::ActorBehaviorData * behaviorData = (lofd::ActorBehaviorData *)behaviorEventValue->behaviorData;
    //cocos2d::log("idle execute");
    //lofd::ActorStateData * stateData = new lofd::ActorStateData(LOFD_ACTOR_STATE_IDLE, behaviorData->operationType);
    lofd::ActorStateData * stateData = lofd::ActorStateData::create(LOFD_ACTOR_STATE_IDLE, behaviorData->operationType);
    behaviorData->actorData->stateContext->insertStateData(stateData);
    behaviorData->actorData->actorEntry->stopAllActions();
    behaviorData->actorData->actorContainer->stopAllActions();
    behaviorData->actorData->stopActorAllActions();
    behaviorData->actorData->runAction(stateData->itemName);
    return BEHAVIOR_RESULT_SUCCESS;
}

/*************ActionSkill*******************/
int ActionSkill::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    ActionBehaviorNode::execute(behaviorEventValue);
    lofd::ActorBehaviorSkillData * behaviorData = (lofd::ActorBehaviorSkillData *)behaviorEventValue->behaviorData;
    //cocos2d::log("skill execute");
    lofd::ActorSkillStateData * stateData = lofd::ActorSkillStateData::create(LOFD_ACTOR_STATE_SKILL1, behaviorData->operationType);
    behaviorData->actorData->stateContext->insertStateData(stateData);
    behaviorData->actorData->actorEntry->stopAllActions();
    behaviorData->actorData->actorContainer->stopAllActions();
    behaviorData->actorData->stopActorAllActions();
    behaviorData->actorData->runAction(stateData->itemName);
    behaviorData->actorData->skill(dynamic_cast<lofd::SkillData *>(stateData->skillData));
    return BEHAVIOR_RESULT_SUCCESS;
}

/************ActionRun***************************/
int ActionRun::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    ActionBehaviorNode::execute(behaviorEventValue);
    lofd::ActorBehaviorRunData * behaviorData = (lofd::ActorBehaviorRunData *)behaviorEventValue->behaviorData;
    //cocos2d::log("run execute");
    lofd::ActorRunStateData * stateData = lofd::ActorRunStateData::create(LOFD_ACTOR_STATE_RUN, behaviorData->operationType);
    stateData->vectorMap = behaviorData->vectorMap;
    stateData->endPoint = behaviorData->endPoint;
    behaviorData->actorData->stateContext->insertStateData(stateData);
    behaviorData->actorData->actorEntry->stopAllActions();
    behaviorData->actorData->actorContainer->stopAllActions();
    behaviorData->actorData->stopActorAllActions();
    behaviorData->actorData->runAction(stateData->itemName);
    behaviorData->actorData->move(behaviorData->endPoint, behaviorData->vectorMap);
    return BEHAVIOR_RESULT_SUCCESS;
}

/**************ActionTrack***************************/
int ActionTrack::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    ActionBehaviorNode::execute(behaviorEventValue);
    lofd::ActorBehaviorTrackData * behaviorData = (lofd::ActorBehaviorTrackData *)behaviorEventValue->behaviorData;
    //cocos2d::log("track execute");
    lofd::ActorTrackStateData * stateData = lofd::ActorTrackStateData::create(LOFD_ACTOR_STATE_TRACK, behaviorData->operationType);
    stateData->actorData = behaviorData->actorData;
    behaviorData->actorData->stateContext->insertStateData(stateData);
    behaviorData->actorData->actorEntry->stopAllActions();
    behaviorData->actorData->actorContainer->stopAllActions();
    behaviorData->actorData->stopActorAllActions();
    behaviorData->actorData->runAction(LOFD_ACTOR_STATE_RUN);
    behaviorData->actorData->track();
    return BEHAVIOR_RESULT_SUCCESS;
}

/***************CreateActorBheaviorSkillData*****************/
int ActionCreateActorBehaviorSkillData::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    ActionBehaviorNode::execute(behaviorEventValue);
    lofd::ActorBehaviorTrackData * behaviorData = (lofd::ActorBehaviorTrackData *)behaviorEventValue->behaviorData;
    //cocos2d::log("create actor behavior skill data");
    lofd::SkillData * skillData = dynamic_cast<lofd::SkillData *>(behaviorData->skillData);
    int index = skillData->skillStateData->skillStateDef->tags.size() % rand();
    std::string tag = skillData->skillStateData->skillStateDef->tags.at(index);
    lofd::ActorSkillStateData * stateData = lofd::ActorSkillStateData::create(tag, behaviorData->operationType);
    stateData->actorData = behaviorData->actorData;
    stateData->skillData = behaviorData->skillData;
    behaviorData->actorData->stateContext->insertStateData(stateData);
    behaviorData->actorData->actorEntry->stopAllActions();
    behaviorData->actorData->actorContainer->stopAllActions();
    behaviorData->actorData->stopActorAllActions();
    return BEHAVIOR_RESULT_SUCCESS;
}

/****************ActionTrackSkill**********************/
int ActionTrackSkill::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    ActionBehaviorNode::execute(behaviorEventValue);
    lofd::ActorBehaviorSkillData * behaviorData = static_cast<lofd::ActorBehaviorSkillData *>(behaviorEventValue->behaviorData);
    //cocos2d::log("skill current focus skill");
    lofd::SkillData * skillData = dynamic_cast<lofd::SkillData *>(behaviorData->skillData);
    int index = rand() % skillData->skillStateData->skillStateDef->tags.size();
    std::string tag = skillData->skillStateData->skillStateDef->tags.at(index);
    lofd::ActorSkillStateData * stateData = lofd::ActorSkillStateData::create(tag, behaviorData->operationType);
    stateData->actorData = behaviorData->actorData;
    stateData->skillData = behaviorData->skillData;
    behaviorData->actorData->stateContext->insertStateData(stateData);
    behaviorData->actorData->actorEntry->stopAllActions();
    behaviorData->actorData->actorContainer->stopAllActions();
    behaviorData->actorData->stopActorAllActions();
    behaviorData->actorData->skill(skillData);
    behaviorData->actorData->runAction(behaviorData->actorData->stateContext->currentState->itemName);
    return BEHAVIOR_RESULT_SUCCESS;
}

/****************ActionTargetSkill**********************/
int ActionTargetSkill::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    ActionBehaviorNode::execute(behaviorEventValue);
    lofd::ActorBehaviorSkillData * behaviorData = static_cast<lofd::ActorBehaviorSkillData *>(behaviorEventValue->behaviorData);
    //cocos2d::log("skill current focus skill");
    lofd::SkillData * skillData = dynamic_cast<lofd::SkillData *>(behaviorData->skillData);
    int index = rand() % skillData->skillStateData->skillStateDef->tags.size();
    std::string tag = skillData->skillStateData->skillStateDef->tags.at(index);
    lofd::ActorSkillStateData * stateData = lofd::ActorSkillStateData::create(tag, behaviorData->operationType);
    stateData->actorData = behaviorData->actorData;
    stateData->skillData = behaviorData->skillData;
    behaviorData->actorData->stateContext->insertStateData(stateData);
    behaviorData->actorData->actorEntry->stopAllActions();
    behaviorData->actorData->actorContainer->stopAllActions();
    behaviorData->actorData->stopActorAllActions();
    behaviorData->actorData->skill(skillData);
    behaviorData->actorData->runAction(behaviorData->actorData->stateContext->currentState->itemName);
    return BEHAVIOR_RESULT_SUCCESS;
}

/**************ActionTarget*****************************/
int ActionTarget::execute(constellation::BehaviorEvent * behaviorEventValue)
{
    ActionBehaviorNode::execute(behaviorEventValue);
    lofd::ActorBehaviorTrackData * behaviorData = (lofd::ActorBehaviorTrackData *)behaviorEventValue->behaviorData;
    //cocos2d::log("target execute");
    lofd::ActorTargetStateData * stateData = lofd::ActorTargetStateData::create(LOFD_ACTOR_STATE_TARGET, behaviorData->operationType);
    stateData->actorData = behaviorData->actorData;
    behaviorData->actorData->stateContext->insertStateData(stateData);
    behaviorData->actorData->actorEntry->stopAllActions();
    behaviorData->actorData->actorContainer->stopAllActions();
    behaviorData->actorData->stopActorAllActions();
    behaviorData->actorData->runAction(LOFD_ACTOR_STATE_RUN);
    behaviorData->actorData->target();
    return BEHAVIOR_RESULT_SUCCESS;
}

/*************ActionHurt*********************************/
int ActionHurt::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    ActionBehaviorNode::execute(behaviorEventValue);
    lofd::ActorBehaviorHurtData * behaviorData = static_cast<lofd::ActorBehaviorHurtData *>(behaviorEventValue->behaviorData);
    //cocos2d::log("hurt execute");
    //lofd::ActorStateData * actorStateData = new lofd::ActorStateData(LOFD_ACTOR_STATE_HURT, behaviorData->operationType);
    lofd::ActorStateData * actorStateData = lofd::ActorStateData::create(LOFD_ACTOR_STATE_HURT, behaviorData->operationType);
    behaviorData->actorData->stateContext->insertStateData(actorStateData);
    behaviorData->actorData->actorEntry->stopAllActions();
    behaviorData->actorData->actorContainer->stopAllActions();
    behaviorData->actorData->stopActorAllActions();
    behaviorData->actorData->runAction(LOFD_ACTOR_STATE_HURT);
    return BEHAVIOR_RESULT_SUCCESS;
}

/**************ActionDead********************************/
int ActionDead::execute(constellation::BehaviorEvent * behaviorEventvalue)
{
    ActionBehaviorNode::execute(behaviorEventvalue);
    lofd::ActorBehaviorData * behaviorData = static_cast<lofd::ActorBehaviorData *>(behaviorEventvalue->behaviorData);
    //cocos2d::log("dead execute");
    //lofd::ActorStateData * actorStateData = new lofd::ActorStateData(LOFD_ACTOR_STATE_DEAD, behaviorData->operationType);
    lofd::ActorStateData * actorStateData = lofd::ActorStateData::create(LOFD_ACTOR_STATE_DEAD, behaviorData->operationType);
    behaviorData->actorData->stateContext->insertStateData(actorStateData);
    behaviorData->actorData->actorEntry->stopAllActions();
    behaviorData->actorData->actorContainer->stopAllActions();
    behaviorData->actorData->stopActorAllActions();
    behaviorData->actorData->runAction(LOFD_ACTOR_STATE_DEAD);
    behaviorData->actorData->dead();
    return BEHAVIOR_RESULT_SUCCESS;
}

/***********ConditionBehaviorNodeCampareCurrentState*******/
ConditionBehaviorNodeCampareCurrentState::~ConditionBehaviorNodeCampareCurrentState()
{
    
}

int ConditionBehaviorNodeCampareCurrentState::execute(constellation::BehaviorEvent *behaviorEventvalue)
{
    ConditionBehaviorNode::execute(behaviorEventvalue);
    lofd::ActorBehaviorData * behaviorData = (lofd::ActorBehaviorData *)behaviorEventvalue->behaviorData;
    if (behaviorData->actorData->stateContext->currentState->itemName == this->campareCurrentState) {
        return BEHAVIOR_RESULT_SUCCESS;
    }
    return BEHAVIOR_RESULT_FAILED;
}

/***********ConditionBehaviorNodeCampareEventType***********/
ConditionBehaviorEventCampareEventType::~ConditionBehaviorEventCampareEventType()
{
    
}

int ConditionBehaviorEventCampareEventType::execute(constellation::BehaviorEvent * behaviorEventvalue)
{
    ConditionBehaviorNode::execute(behaviorEventvalue);
    if (behaviorEventvalue->eventType == this->campareEventType)
    {
        return BEHAVIOR_RESULT_SUCCESS;
    }
    return BEHAVIOR_RESULT_FAILED;
}
/*******ActionActionConfig***********************/
void ActorActionConfig::parse(std::string behaviorConfig)
{
    rapidjson::Document dt;
    dt.Parse<0>(behaviorConfig.c_str());
    lofd::ActorBehavior * actorBehavior = nullptr;
    if (dt.HasParseError())
    {
        cocos2d::log("%s", "BehaviorNodeParse error");
    }
    else
    {
        //
        int n = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(dt, LOFD_ACTOR_BEHAVIOR_PARSE_ACTIONS);
        for (int i = 0; i < n; i ++)
        {
            actorBehavior = new lofd::ActorBehavior();
            const rapidjson::Value& actionValue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(dt, LOFD_ACTOR_BEHAVIOR_PARSE_ACTIONS, i);
            actorBehavior->behaviorId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actionValue, LOFD_ACTOR_BEHAVIOR_PARSE_ID);
            actorBehavior->name = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(actionValue, LOFD_ACTOR_BEHAVIOR_PARSE_NAME);
            const rapidjson::Value& nodeValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(actionValue, LOFD_ACTOR_BEHAVIOR_PARSE_NODE);
            actorBehavior->root = parseNode(nodeValue);
            actions.pushBack(actorBehavior);
        };
    }
}

lofd::ActorBehavior * ActorActionConfig::getActorBehaviorById(int behaviorId)
{
    for (int i = 0; i < actions.size(); i ++)
    {
        lofd::ActorBehavior * actorBehavior = actions.at(i);
        if (actorBehavior->behaviorId == behaviorId) return actorBehavior;
    }
    return nullptr;
}

constellation::ActionBehaviorNode * ActorActionConfig::parseActionNode(const rapidjson::Value &nodeValue, std::string type, std::string name)
{
    std::string actionType = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_ACTION_TYPE);
    constellation::ActionBehaviorNode * actionBehaviorNode = nullptr;
    if (BehaviorTree_actionType_actorConvertState == actionType)
    {
        std::string convertState = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(nodeValue, LOFD_ACTOR_BEHAVIOR_PARSE_CONVERT_STATE);
        actionBehaviorNode = new lofd::ActorConvertState(type, actionType, convertState);
    }
    else if (BehaviorTree_actionType_cancelCurrentState == actionType)
    {
        actionBehaviorNode = new lofd::ActionCancelCurrentState(type, actionType);
    }
    else if (BehaviorTree_actionType_atk1 == actionType)
    {
        actionBehaviorNode = new lofd::ActionAtk1(type, actionType);
    }
    else if (BehaviorTree_actionType_idle == actionType)
    {
        actionBehaviorNode = new lofd::ActionAtk1(type, actionType);
    }
    else if (BehaviorTree_actionType_run == actionType)
    {
        actionBehaviorNode = new lofd::ActionRun(type, actionType);
    }
    else if (BehaviorTree_actionType_skill1 == actionType)
    {
        actionBehaviorNode = new lofd::ActionSkill(type, actionType);
    }
    else if (BehaviorTree_actionType_stackActorSkillData == actionType)
    {
        actionBehaviorNode = new lofd::ActionCreateActorBehaviorSkillData(type, actionType);
    }
    else if (BehaviorTree_actionType_track == actionType)
    {
        actionBehaviorNode = new lofd::ActionTrack(type, actionType);
    }
    else if (BehaviorTree_actionType_target == actionType)
    {
        actionBehaviorNode = new lofd::ActionTarget(type, actionType);
    }
    else if (BehaviorTree_actionType_cancelCurrentStateNoChange == actionType)
    {
        actionBehaviorNode = new lofd::ActionCancelCurrentStateNoChange(type, actionType);
    }
    else if (BehaviorTree_actionType_hurt == actionType)
    {
        actionBehaviorNode = new lofd::ActionHurt(type, actionType);
    }
    else if (BehaviorTree_actionType_trackSkill == actionType)
    {
        actionBehaviorNode = new lofd::ActionTrackSkill(type, actionType);
    }
    else if (BehaviorTree_actionType_targetSkill == actionType)
    {
        actionBehaviorNode = new lofd::ActionTargetSkill(type, actionType);
    }
    else if (BehaviorTree_actionType_dead == actionType)
    {
        actionBehaviorNode = new lofd::ActionDead(type, actionType);
    }
    else
    {
        return  nullptr;
    }
    actionBehaviorNode->nodeName = name;
    return actionBehaviorNode;
}

constellation::ConditionBehaviorNode * ActorActionConfig::parseConditionNode(const rapidjson::Value &nodeValue, std::string type, std::string name)
{
    std::string conditionType = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_CONDITION_TYPE);
    constellation::ConditionBehaviorNode * conditionBehaviorNode = constellation::BehaviorTreeParse::parseConditionNode(nodeValue, type, name);
    if (conditionBehaviorNode) return conditionBehaviorNode;
    if (conditionType == LOFD_ACTOR_BEHAVIOR_PARSE_CAMPARE_CURRENT_STATE)
    {
        std::string campareCurrentState = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(nodeValue, LOFD_ACTOR_BEHAVIOR_PARSE_CAMPARE_CURRENT_STATE);
        conditionBehaviorNode = new lofd::ConditionBehaviorNodeCampareCurrentState(type, conditionType, campareCurrentState);
    }
    else
    {
        return nullptr;
    }
    conditionBehaviorNode->nodeName = name;
    return conditionBehaviorNode;
}
NS_LOFD_END;