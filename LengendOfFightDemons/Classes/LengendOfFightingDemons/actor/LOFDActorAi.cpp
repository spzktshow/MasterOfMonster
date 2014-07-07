//
//  LOFDActorAi.cpp
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-11.
//
//

#include "LOFDActorAi.h"
#include "editor-support/cocostudio/DictionaryHelper.h"
#include "LOFDSkillData.h"
#include "LOFDConfigManager.h"
#include "LOFDScene.h"

NS_LOFD_BEGIN
int AISelectorBehaviorNode::execute(constellation::BehaviorEvent *behaviorEventValue)
{
#if LOFD_AI_DEBUG
    lofd::AIBehaviorDynamicData * data = static_cast<lofd::AIBehaviorDynamicData *>(behaviorEventValue->behaviorData);
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("event %s selector node: %s handler", behaviorEventValue->eventType.c_str(), this->nodeName.c_str());
    }
#endif//LOFD_AI_DEBUG
    long n = children.size();
    for (int i = 0; i < n; i++) {
        int result = children.at(i)->execute(behaviorEventValue);
        if (result == BEHAVIOR_RESULT_SUCCESS || result == BEHAVIOR_RESULT_WAIT)
        {
#if LOFD_AI_DEBUG
            if (data->actorData->tagId == testTagId)
            {
                cocos2d::log("event %s selector node: %s return success", behaviorEventValue->eventType.c_str(), this->nodeName.c_str());
            }
#endif//LOFD_AI_DEBUG
            return result;
        }
    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("event %s selector node : %s return failded", behaviorEventValue->eventType.c_str(), this->nodeName.c_str());
    }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_FAILED;
}

int AISequenceBehaviorNode::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = static_cast<lofd::AIBehaviorDynamicData *>(behaviorEventValue->behaviorData);
    long n = children.size();
    int result = BEHAVIOR_RESULT_FAILED;
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("event %s sequence node: %s handler", behaviorEventValue->eventType.c_str(), this->nodeName.c_str());
    }
#endif//LOFD_AI_DEBUG
    for (int i = 0; i < n;i ++)
    {
        result = children.at(i)->execute(behaviorEventValue);
        if (result == BEHAVIOR_RESULT_FAILED)
        {
#if LOFD_AI_DEBUG
            if (data->actorData->tagId == testTagId)
            {
                cocos2d::log("event %s sequence node: %s return failed", behaviorEventValue->eventType.c_str(), this->nodeName.c_str());
            }
#endif//LOFD_AI_DEBUG
            return result;
        }
    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("event %s sequence node: %s return success", behaviorEventValue->eventType.c_str(), this->nodeName.c_str());
    }
#endif//LOFD_AI_DEBUG
    return result;
}

/*************Action*******************************/
int AIRandMoveTarget::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
    //
    float x = rand() % (int)data->mapSceneData->mapRect->size.width;
    float y = rand() % 400;
    data->point = new cocos2d::Point(x, y);
    return BEHAVIOR_RESULT_SUCCESS;
}

int AICancelCurrentState::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("cancel current state tagId=%d", data->actorData->tagId);
        cocos2d::log("ready cancel current state tagId=%d currentState=%s ", data->actorData->tagId, data->actorData->aiStateContext->currentState->itemName.c_str());
    }
#endif//LOFD_AI_DEBUG
    data->actorData->aiStateContext->popStateDataChangeNext();
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("complete cancel current state tagId=%d currentState=%s success", data->actorData->tagId, data->actorData->aiStateContext->currentState->itemName.c_str());
    }
#endif//LOFD_AI_DEBUG
    if (data->actorData->aiStateContext->currentState->itemName == LOFD_AI_STATE_AI_PATROL)
    {
        lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
        cocos2d::Point * point = nullptr;
        float x = rand() % (int)data->mapSceneData->mapRect->size.width;
        float y = rand() % 400;
        point = new cocos2d::Point(x, y);
        constellation::BehaviorEvent * actionBehaviorEvent = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_MOVE);
        lofd::ActorBehaviorRunData * runData = new lofd::ActorBehaviorRunData();
        runData->actorData = data->actorData;
        runData->endPoint = *point;
        runData->vectorMap = data->mapSceneData;
        runData->operationType = data->operationType;
        actionBehaviorEvent->behaviorData = runData;
        data->actorData->actorBehavior->root->execute(actionBehaviorEvent);
        delete actionBehaviorEvent;
        delete runData;
    }
    else if (data->actorData->aiStateContext->currentState->itemName == LOFD_AI_STATE_AI_IDLE)
    {
        lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
        constellation::BehaviorEvent * actionBehaviorEvent = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_IDLE);
        lofd::ActorBehaviorData * behaviorData = new lofd::ActorBehaviorData;
        behaviorData->actorData = data->actorData;
        behaviorData->operationType = data->operationType;
        actionBehaviorEvent->behaviorData = behaviorData;
        data->actorData->actorBehavior->root->execute(actionBehaviorEvent);
        delete actionBehaviorEvent;
        delete behaviorData;
    }
    return BEHAVIOR_RESULT_SUCCESS;
}

int AIGetRangeCampLessThan::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("get range camp less than tagId=%d handler", data->actorData->tagId);
        if (behaviorEventValue->eventType == LOFD_BEHAVIOR_EVENT_ATK1_END || behaviorEventValue->eventType == LOFD_BEHAVIOR_EVENT_ATK2_END || behaviorEventValue->eventType == LOFD_BEHAVIOR_EVENT_SKILL1_END)
        {
            cocos2d::log("%s", behaviorEventValue->eventType.c_str());
        }
        cocos2d::log("debug tagId=%d catched", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    
    float minDistance = 0;
    int minIndex = -1;
    for (int i = 0; i < data->actorData->currentRangeActors.size(); i ++)
    {
        lofd::ActorData * tempActorData = data->actorData->currentRangeActors.at(i);
        if (tempActorData->isDead) continue;
        lofd::CampRelationShipDef * def = data->actorData->campContext->getDefByCampId(tempActorData->campContext->campId);
        if (def->relationShip >= this->campareCamp) continue;
        cocos2d::Point * start = new cocos2d::Point(data->actorData->actorEntry->getPositionX(), data->actorData->actorEntry->getPositionY());
        cocos2d::Point * end = new cocos2d::Point(data->actorData->currentRangeActors.at(i)->actorEntry->getPositionX(), data->actorData->currentRangeActors.at(i)->actorEntry->getPositionY());
        float distance = moonsugar::VectorUtils::calculateDistance(start, end);
        if (minIndex == -1 || distance < minDistance) {
            minIndex = i;
            minDistance = distance;
        }
    }
    if (minIndex != -1)
    {
        data->currentOperationActorData = data->actorData->currentRangeActors.at(minIndex);
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("get range camp less than tagId=%d return success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_SUCCESS;
    }
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("get range camp less than tagId=%d return failed", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_FAILED;
}

int AIGetRecentlyActorFromRange::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
    float minDistance = 0;
    int minIndex = -1;
    for (int i = 0; i < data->actorData->currentRangeActors.size(); i ++)
    {
        cocos2d::Point * start = new cocos2d::Point(data->actorData->actorEntry->getPositionX(), data->actorData->actorEntry->getPositionY());
        cocos2d::Point * end = new cocos2d::Point(data->actorData->currentRangeActors.at(i)->actorEntry->getPositionX(), data->actorData->currentRangeActors.at(i)->actorEntry->getPositionY());
        lofd::ActorData * actorData = data->actorData->currentRangeActors.at(i);
        if (actorData->isDead) continue;
        float distance = moonsugar::VectorUtils::calculateDistance(start, end);
        if (minIndex == -1 || distance < minDistance) {
            minIndex = i;
            minDistance = distance;
        }
    }
    if (minIndex != -1)
    {
        data->currentOperationActorData = data->actorData->currentRangeActors.at(minIndex);
        //cocos2d::log("GotRecentlyActorFromRange");
        return BEHAVIOR_RESULT_SUCCESS;
    }
    //cocos2d::log("NoRecentlyActorFromRange");
    return BEHAVIOR_RESULT_FAILED;
}

int AIMoveTo::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
    //lofd::ActorStateData * stateData = new lofd::ActorStateData(LOFD_AI_STATE_MOVE, data->operationType);
    lofd::ActorStateData * stateData = lofd::ActorStateData::create(LOFD_AI_STATE_MOVE, data->operationType);
    data->actorData->aiStateContext->insertStateData(stateData);
    cocos2d::Point * target = data->point;
    constellation::BehaviorEvent * actionBehaviorEvent = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_MOVE);
    lofd::ActorBehaviorRunData * runData = new lofd::ActorBehaviorRunData();
    runData->actorData = data->actorData;
    runData->endPoint = *target;
    runData->vectorMap = data->mapSceneData;
    runData->operationType = LOFD_ACTOR_STATE_OPERATION_TYPE_AI;
    actionBehaviorEvent->behaviorData = runData;
    data->actorData->actorBehavior->root->execute(actionBehaviorEvent);
    delete runData;
    delete actionBehaviorEvent;
    return BEHAVIOR_RESULT_SUCCESS;
}

int AIFocusActor::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("actor tagId=%d focus actor tagId=%d", data->actorData->tagId, data->currentOperationActorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    if (data->currentOperationActorData != nullptr)
    {
        data->actorData->currentFocus = data->currentOperationActorData;
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            if (data->currentOperationActorData->isDead)
            {
                cocos2d::log("actor tagId=%d focus actor tagId=%d is dead", data->actorData->tagId, data->currentOperationActorData->tagId);
            }
            cocos2d::log("actor tagId=%d focus actor tagId=%d return success", data->actorData->tagId, data->currentOperationActorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_SUCCESS;
    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("actor tagId=%d focus actor tagId=%d return failed", data->actorData->tagId, data->currentOperationActorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_FAILED;
}

int AICancelFocusActor::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("cancel focus actor tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    data->actorData->currentFocus = nullptr;
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("cancel focus actor tagId=%d return success", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_SUCCESS;
}

int AIAttack::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ai attack tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    lofd::StateData * stateData = new lofd::StateData(LOFD_AI_STATE_ATTACK);
    data->actorData->aiStateContext->insertStateData(stateData);
    constellation::BehaviorEvent * actionBehaviorEvent = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_ATK1);
    lofd::ActorBehaviorData * actionData = new lofd::ActorBehaviorData;
    actionData->actorData = data->actorData;
    actionData->operationType = LOFD_ACTOR_STATE_OPERATION_TYPE_AI;
    actionBehaviorEvent->behaviorData = actionData;
    data->actorData->actorBehavior->root->execute(actionBehaviorEvent);
    delete actionData;
    delete actionBehaviorEvent;
    actionData = nullptr;
    actionBehaviorEvent = nullptr;
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ai attack tagId=%d success", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_SUCCESS;
}

int AITrack::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    //
    lofd::AIBehaviorDynamicData * data = static_cast<lofd::AIBehaviorDynamicData *>(behaviorEventValue->behaviorData);
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ai track tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    constellation::BehaviorEvent * actionBehaviorEvent = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_TRACK);
    lofd::ActorBehaviorTrackData * actorBehaviorTrackData = new lofd::ActorBehaviorTrackData();
    actorBehaviorTrackData->skillData = data->skillData;
    actorBehaviorTrackData->actorData = data->actorData;
    actionBehaviorEvent->behaviorData = actorBehaviorTrackData;
    actorBehaviorTrackData->operationType = data->operationType;
    //lofd::AITrackOrTargetStateData * stateData = new lofd::AITrackOrTargetStateData(LOFD_AI_STATE_AI_TRACK, data->operationType);
    lofd::AITrackOrTargetStateData * stateData = lofd::AITrackOrTargetStateData::create(LOFD_AI_STATE_AI_TRACK, data->operationType);
    stateData->selfActorData = data->actorData;
    stateData->mapScene = data->mapScene;
    data->actorData->aiStateContext->insertStateData(stateData);
    int result = data->actorData->actorBehavior->root->execute(actionBehaviorEvent);
//    if (result == BEHAVIOR_RESULT_SUCCESS)
//    {
//        lofd::AITrackStateData * stateData = new lofd::AITrackStateData(LOFD_AI_STATE_AI_TRACK, data->operationType);
//        stateData->selfActorData = data->actorData;
//        stateData->skillData = data->skillData;
//        stateData->mapScene = data->mapScene;
//        data->actorData->aiStateContext->insertStateData(stateData);
//    }
#if LOFD_AI_DEBUG
    if (result == BEHAVIOR_RESULT_FAILED)
    {

        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("ai track tagId=%d failed but ai state is refresh", data->actorData->tagId);
        }
    }
    else
    {
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("ai track tagId=%d success but ai state is refresh", data->actorData->tagId);
        }
    }
#endif//LOFD_AI_DEBUG
    delete actionBehaviorEvent;
    delete actorBehaviorTrackData;
    return result;
}

int AITarget::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = static_cast<lofd::AIBehaviorDynamicData *>(behaviorEventValue->behaviorData);
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ai target tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    constellation::BehaviorEvent * actionBehaviorEvent = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_TARGET);
    lofd::ActorBehaviorTargetData * actorBehaviorTrackData = new lofd::ActorBehaviorTargetData();
    actorBehaviorTrackData->skillData = data->skillData;
    actorBehaviorTrackData->actorData = data->actorData;
    actionBehaviorEvent->behaviorData = actorBehaviorTrackData;
    actorBehaviorTrackData->operationType = data->operationType;
//    lofd::AITrackOrTargetStateData * stateData = new lofd::AITrackOrTargetStateData(LOFD_AI_STATE_AI_TARGET, data->operationType);
    lofd::AITrackOrTargetStateData * stateData = lofd::AITrackOrTargetStateData::create(LOFD_AI_STATE_AI_TARGET, data->operationType);
    stateData->selfActorData = data->actorData;
    stateData->mapScene = data->mapScene;
    if (data->operationType == LOFD_ACTOR_STATE_OPERATION_TYPE_CUSOTM)
    {
        stateData->targetPoint = data->targetPoint;
    }
    else
    {
        cocos2d::Point point(data->actorData->currentFocus->actorContainer->getPositionX(), data->actorData->currentFocus->actorContainer->getPositionY());
        stateData->targetPoint = point;
    }
    data->actorData->targetPoint = stateData->targetPoint;
    data->actorData->aiStateContext->insertStateData(stateData);
    int result = data->actorData->actorBehavior->root->execute(actionBehaviorEvent);
    //    if (result == BEHAVIOR_RESULT_SUCCESS)
    //    {
    //        lofd::AITrackStateData * stateData = new lofd::AITrackStateData(LOFD_AI_STATE_AI_TRACK, data->operationType);
    //        stateData->selfActorData = data->actorData;
    //        stateData->skillData = data->skillData;
    //        stateData->mapScene = data->mapScene;
    //        data->actorData->aiStateContext->insertStateData(stateData);
    //    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        if (result == BEHAVIOR_RESULT_FAILED)
        {
            cocos2d::log("ai target tagId=%d failed but ai state is refresh", data->actorData->tagId);
        }
        else
        {
            cocos2d::log("ai target tagId=%d success but ai state is refresh", data->actorData->tagId);
        }
    }
#endif//LOFD_AI_DEBUG
    delete actionBehaviorEvent;
    delete actorBehaviorTrackData;
    return result;
}

int AISkillCurrentFocusSkill::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = static_cast<lofd::AIBehaviorDynamicData *>(behaviorEventValue->behaviorData);
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ai SkillCurrentFocusSkill tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    lofd::AITrackOrTargetStateData * stateData = dynamic_cast<lofd::AITrackOrTargetStateData *>(data->actorData->aiStateContext->currentState);

    lofd::SkillConfig * skillConfig = lofd::ConfigManager::getInstance()->skillConfig;
    lofd::SkillDef * skillDef = skillConfig->getSkillDefById(data->actorData->currentFocusSkill);
    
    lofd::SkillParamContext * skillParamContext = new lofd::SkillParamContext;
    skillParamContext->targetActorData = data->actorData->currentFocus;
    skillParamContext->targetPoint = stateData->targetPoint;
    skillParamContext->mapScene = static_cast<lofd::MapScene *>(stateData->mapScene);
    skillParamContext->actorData = data->actorData;
    lofd::SkillData * skillData = lofd::SkillDataUtils::createSkillData(skillDef, skillParamContext);
    if (skillDef->operationContextDef->operationType == SKILL_OPERATION_TYPE_ACTOR)
    {
        constellation::BehaviorEvent * actionBehaviorEvent = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_TRACK_SKILL);
        lofd::ActorBehaviorSkillData * actorBehaviorTrackData = new lofd::ActorBehaviorSkillData();
        actorBehaviorTrackData->actorData = data->actorData;
        delete skillParamContext;
        actorBehaviorTrackData->skillData = skillData;
        actionBehaviorEvent->behaviorData = actorBehaviorTrackData;
        actorBehaviorTrackData->operationType = data->operationType;
        int result = data->actorData->actorBehavior->root->execute(actionBehaviorEvent);
        delete actionBehaviorEvent;
        delete actorBehaviorTrackData;
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("ai SkillCurrentFocusSkill tagId=%d skill operation type actor success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return result;
    }
    else if (skillDef->operationContextDef->operationType == SKILL_OPERATION_TYPE_TARGET)
    {
        constellation::BehaviorEvent * actionBehaviorEvent = new constellation::BehaviorEvent(LOFD_ACTOR_BHEAVIOR_EVENT_TARGET_SKILL);
        lofd::ActorBehaviorSkillData * actorBehaviorTrackData = new lofd::ActorBehaviorSkillData();
        actorBehaviorTrackData->actorData = data->actorData;
        delete skillParamContext;
        actorBehaviorTrackData->skillData = skillData;
        actionBehaviorEvent->behaviorData = actorBehaviorTrackData;
        actorBehaviorTrackData->operationType = data->operationType;
        int result = data->actorData->actorBehavior->root->execute(actionBehaviorEvent);
        delete actionBehaviorEvent;
        delete actorBehaviorTrackData;
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("ai SkillCurrentFocusSkill tagId=%d skill operation type target success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return result;
    }
    else if (skillDef->operationContextDef->operationType == SKILL_OPERATION_TYPE_NONE)
    {
        constellation::BehaviorEvent * actionBehaviorEvent = new constellation::BehaviorEvent(LOFD_ACTOR_BHEAVIOR_EVENT_TARGET_SKILL);
        lofd::ActorBehaviorSkillData * actorBehaviorTrackData = new lofd::ActorBehaviorSkillData();
        actorBehaviorTrackData->actorData = data->actorData;
        delete skillParamContext;
        actorBehaviorTrackData->skillData = skillData;
        actionBehaviorEvent->behaviorData = actorBehaviorTrackData;
        actorBehaviorTrackData->operationType = data->operationType;
        int result = data->actorData->actorBehavior->root->execute(actionBehaviorEvent);
        delete actionBehaviorEvent;
        delete actorBehaviorTrackData;
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("ai SkillCurrentFocusSkill tagId=%d skill operation type none success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return result;
    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ai SkillCurrentFocusSkill tagId=%d skill operation type failed", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_FAILED;
}

int AIPatrol::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ai ai patrol tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    cocos2d::Point * point = nullptr;
    float x = rand() % (int)data->mapSceneData->mapRect->size.width;
    float y = rand() % 400;
    point = new cocos2d::Point(x, y);
    constellation::BehaviorEvent * actionBehaviorEvent = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_MOVE);
    lofd::ActorBehaviorRunData * runData = new lofd::ActorBehaviorRunData();
    runData->actorData = data->actorData;
    runData->endPoint = *point;
    runData->vectorMap = data->mapSceneData;
    runData->operationType = data->operationType;
    actionBehaviorEvent->behaviorData = runData;
    int result = data->actorData->actorBehavior->root->execute(actionBehaviorEvent);
    if (result == BEHAVIOR_RESULT_SUCCESS) {
        //lofd::ActorStateData * stateData = new lofd::ActorStateData(LOFD_AI_STATE_AI_PATROL, data->operationType);
        lofd::ActorStateData * stateData = lofd::ActorStateData::create(LOFD_AI_STATE_AI_PATROL, data->operationType);
        data->actorData->aiStateContext->insertStateDataBitAutoChange(stateData, 1);
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("ai ai patrol tagId=%d success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
    }
    else
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("ai ai patrol tagId=%d failed", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
    }
    delete runData;
    delete actionBehaviorEvent;
    return result;
}

int AIIdle::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ai ai idle tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    constellation::BehaviorEvent * actionBehaviorEvent = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_IDLE);
    lofd::ActorBehaviorData * behaviorData = new lofd::ActorBehaviorData;
    behaviorData->actorData = data->actorData;
    behaviorData->operationType = data->operationType;
    actionBehaviorEvent->behaviorData = behaviorData;
    int result = data->actorData->actorBehavior->root->execute(actionBehaviorEvent);
    if (result == BEHAVIOR_RESULT_SUCCESS)
    {
        //lofd::ActorStateData * stateData = new lofd::ActorStateData(LOFD_AI_STATE_AI_IDLE, data->operationType);
        lofd::ActorStateData * stateData = lofd::ActorStateData::create(LOFD_AI_STATE_AI_IDLE, data->operationType);
        data->actorData->aiStateContext->insertStateData(stateData);
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("ai ai idle tagId=%d success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
    }
    else
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("ai ai idle tagId=%d failed", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
    }
    delete behaviorData;
    delete actionBehaviorEvent;
    return result;
}

int AIMove::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = static_cast<lofd::AIBehaviorDynamicData *>(behaviorEventValue->behaviorData);
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ai ai move tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    constellation::BehaviorEvent * actionBehaviorEvent = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_MOVE);
    lofd::ActorBehaviorRunData * behaviorData = new lofd::ActorBehaviorRunData;
    behaviorData->actorData = data->actorData;
    behaviorData->vectorMap = data->mapSceneData;
    behaviorData->endPoint = *data->point;
    behaviorData->operationType = data->operationType;
    actionBehaviorEvent->behaviorData = behaviorData;
    int result = data->actorData->actorBehavior->root->execute(actionBehaviorEvent);
    if (result == BEHAVIOR_RESULT_SUCCESS)
    {
        //lofd::ActorStateData * stateData = new lofd::ActorStateData(LOFD_AI_STATE_AI_MOVE, data->operationType);
        lofd::ActorStateData * stateData = lofd::ActorStateData::create(LOFD_AI_STATE_AI_MOVE, data->operationType);
        data->actorData->aiStateContext->insertStateData(stateData);
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("ai ai move tagId=%d success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
    }
    else
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("ai ai move tagId=%d failed", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
    }
    delete behaviorData;
    delete actionBehaviorEvent;
    return result;
}

int AIFocusUncoolDownSkill::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = static_cast<lofd::AIBehaviorDynamicData *>(behaviorEventValue->behaviorData);
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ai ai focus uncool down skill tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    long n = data->actorData->skillCoolDownDatas.size();
    int index=  -1;
    for (int i = 1; i < n; i ++) {
        lofd::SkillCoolDownData * skillCoolDownData = dynamic_cast<lofd::SkillCoolDownData *>(data->actorData->skillCoolDownDatas.at(i));
        if (skillCoolDownData->isCoolDown != false)
        {
            continue;
        }
        index = i;
        break;
    }
    if (index == -1)
    {
        lofd::SkillCoolDownData * skillCoolDownData0 = dynamic_cast<lofd::SkillCoolDownData *>(data->actorData->skillCoolDownDatas.at(0));
        if (skillCoolDownData0->isCoolDown == false)
        {
            index = 0;
        }
    }

    if (index != -1)
    {
        lofd::SkillCoolDownData * skillCoolDownData = dynamic_cast<lofd::SkillCoolDownData *>(data->actorData->skillCoolDownDatas.at(index));
        data->actorData->currentFocusSkill = skillCoolDownData->skillDef->skillId;
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("ai ai focus uncool down skill tagId=%d return success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_SUCCESS;
    }
    else
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("ai ai focus uncool down skill tagId=%d return failed", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_FAILED;
    }

}
/******************Condition**********************/
int AIConditionBehaviorNodeCampareEventType::execute(constellation::BehaviorEvent *behaviorEventvalue)
{
    lofd::AIBehaviorDynamicData * data = static_cast<lofd::AIBehaviorDynamicData *>(behaviorEventvalue->behaviorData);
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("condition node campareEventType %s eventType=%s handler", this->nodeName.c_str(), this->campareEventType.c_str());
    }
#endif//LOFD_AI_DEBUG
    if (behaviorEventvalue->eventType == this->campareEventType)
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("condition node campareEventType %s eventType=%s return success", this->nodeName.c_str(), this->campareEventType.c_str());
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_SUCCESS;
    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("condition node campareEventType %s eventType=%s return failed", this->nodeName.c_str(), this->campareEventType.c_str());
    }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_FAILED;
}

int AIConditionHasFocus::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("condtion has focus tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    if (data->actorData->currentFocus != nullptr)
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("condtion has focus tagId=%d return success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_SUCCESS;
    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("condtion has focus tagId=%d return failed", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_FAILED;
}

int AIConditionIsFocus::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("condtion is focus tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    if (data->actorData->currentFocus != nullptr && data->actorData->currentFocus->tagId == data->currentOperationActorData->tagId)
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("condtion is focus tagId=%d return success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_SUCCESS;
    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("condtion is focus tagId=%d return failed", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_FAILED;
}

int AIConditionIsFocusDead::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("condtion is focus dead tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    if (data->actorData->currentFocus == nullptr || (data->actorData->currentFocus != nullptr && data->actorData->currentFocus->isDead))
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("condtion is focus dead tagId=%d return success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUGreturn BEHAVIOR_RESULT_SUCCESS;
        return BEHAVIOR_RESULT_SUCCESS;
    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("condtion is focus dead tagId=%d return failed", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_FAILED;
}

int AIConditionCampGreaterThan::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("condtion camp greater than tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    lofd::CampRelationShipDef * def = data->actorData->campContext->getDefByCampId(data->currentOperationActorData->campContext->campId);
    if (def == nullptr)
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("condtion camp greater than tagId=%d return failed", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_FAILED;
    }
    if (def->relationShip > this->campareCamp)
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("condtion camp greater than tagId=%d return success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
       return BEHAVIOR_RESULT_SUCCESS;
    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("condtion camp greater than tagId=%d return failed", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_FAILED;
}

int AIConditionCampEqual::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("condtion camp equal tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    lofd::CampRelationShipDef * def = data->actorData->campContext->getDefByCampId(data->currentOperationActorData->campContext->campId);
    if (def == nullptr)
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("condtion camp equal tagId=%d return failed", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_FAILED;
    }
    if (def->relationShip == this->campareCamp)
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("condtion camp equal tagId=%d return success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_SUCCESS;
    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("condtion camp equal tagId=%d return failed", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_FAILED;
}

int AIConditionCampLessThan::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("condtion camp less than tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    lofd::CampRelationShipDef * def = data->actorData->campContext->getDefByCampId(data->currentOperationActorData->campContext->campId);
    if (def == nullptr){
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("condtion camp less than tagId=%d return failed", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_FAILED;
    }
    if (def->relationShip < this->campareCamp)
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("condtion camp less than tagId=%d return success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_SUCCESS;
    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("condtion camp less than tagId=%d return failed", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_FAILED;
}

int AIConditionAllowAI::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = (lofd::AIBehaviorDynamicData *)behaviorEventValue->behaviorData;
    if (data->actorData->isAI) return BEHAVIOR_RESULT_SUCCESS;
    return BEHAVIOR_RESULT_FAILED;
}

int AIConditionBehaviorEventCampareCurrentState::execute(constellation::BehaviorEvent *behaviorEventvalue)
{
    lofd::AIBehaviorDynamicData * data = static_cast<lofd::AIBehaviorDynamicData *>(behaviorEventvalue->behaviorData);
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("event campare current state tagId=%d %s and %s", data->actorData->tagId, data->actorData->aiStateContext->currentState->itemName.c_str(), this->campareCurrentState.c_str());
    }
#endif//LOFD_AI_DEBUG
    if (data->actorData->aiStateContext->currentState->itemName == this->campareCurrentState)
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("event campare current state tagId=%d %s and %s return success", data->actorData->tagId, data->actorData->aiStateContext->currentState->itemName.c_str(), this->campareCurrentState.c_str());
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_SUCCESS;
    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("event campare current state tagId=%d %s and %s return failed", data->actorData->tagId, data->actorData->aiStateContext->currentState->itemName.c_str(), this->campareCurrentState.c_str());
    }

#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_FAILED;
}

int AIConditionIsCurrentFocusSkillIsAuto::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = static_cast<lofd::AIBehaviorDynamicData *>(behaviorEventValue->behaviorData);
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("eConditionIsCurrentFocusSkillIsAuto tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    lofd::SkillDef * skillDef = data->actorData->currentSkillDef;
    if (skillDef->isAuto)
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("eConditionIsCurrentFocusSkillIsAuto tagId=%d return success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_SUCCESS;
    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("eConditionIsCurrentFocusSkillIsAuto tagId=%d return failed", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_FAILED;
}

int AIConditionIsAiToCustom::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = static_cast<lofd::AIBehaviorDynamicData *>(behaviorEventValue->behaviorData);
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ConditionIsAiToCustom tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    lofd::ActorData * actorData = data->actorData;
    lofd::ActorStateData * actorStateData = dynamic_cast<lofd::ActorStateData *>(actorData->aiStateContext->currentState);
    if (data->operationType == LOFD_ACTOR_STATE_OPERATION_TYPE_AI && actorStateData->operationType == LOFD_ACTOR_STATE_OPERATION_TYPE_CUSOTM)
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("ConditionIsAiToCustom tagId=%d return success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_SUCCESS;
    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ConditionIsAiToCustom tagId=%d return failed", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_FAILED;
}

int AIConditionCheckCurrentFocusSkillOperationType::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = static_cast<lofd::AIBehaviorDynamicData *>(behaviorEventValue->behaviorData);
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ConditionCheckCurrentFocusSkillOperationType tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    lofd::SkillConfig * skillConfig = lofd::ConfigManager::getInstance()->skillConfig;
    lofd::SkillDef * skillDef = skillConfig->getSkillDefById(data->actorData->currentFocusSkill);
    if (skillDef->operationContextDef->operationType == this->checkSkillOperationType)
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("ConditionCheckCurrentFocusSkillOperationType tagId=%d return success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_SUCCESS;
    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ConditionCheckCurrentFocusSkillOperationType tagId=%d return failed", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_FAILED;
}

int AIConditionIsAIOperation::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = static_cast<lofd::AIBehaviorDynamicData *>(behaviorEventValue->behaviorData);
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ConditionIsAIOperation tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    if (data->operationType == LOFD_ACTOR_STATE_OPERATION_TYPE_AI)
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("ConditionIsAIOperation tagId=%d return success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_SUCCESS;
    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ConditionIsAIOperation tagId=%d return failed", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_FAILED;
}

int AIConditionIsCustomOperation::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    lofd::AIBehaviorDynamicData * data = static_cast<lofd::AIBehaviorDynamicData *>(behaviorEventValue->behaviorData);
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ConditionIsCustomOperation tagId=%d", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    if (data->operationType == LOFD_ACTOR_STATE_OPERATION_TYPE_CUSOTM)
    {
#if LOFD_AI_DEBUG
        if (data->actorData->tagId == testTagId)
        {
            cocos2d::log("ConditionIsCustomOperation tagId=%d return success", data->actorData->tagId);
        }
#endif//LOFD_AI_DEBUG
        return BEHAVIOR_RESULT_SUCCESS;
    }
#if LOFD_AI_DEBUG
    if (data->actorData->tagId == testTagId)
    {
        cocos2d::log("ConditionIsCustomOperation tagId=%d return failed", data->actorData->tagId);
    }
#endif//LOFD_AI_DEBUG
    return BEHAVIOR_RESULT_FAILED;
}
/*********AIConfig***************/
void AIConfig::parse(std::string behaviorConfig)
{
    rapidjson::Document dt;
    dt.Parse<0>(behaviorConfig.c_str());
    lofd::AIBehavior * aiBehavior = nullptr;
    if (dt.HasParseError())
    {
        cocos2d::log("%s", "AI Config error");
    }
    else
    {
        int n = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(dt, LOFD_AI_BEHAVIOR_PARSE_TACTICS);
        for (int i = 0; i < n; i ++) {
            aiBehavior = new lofd::AIBehavior();
            const rapidjson::Value& tacticsValue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(dt, LOFD_AI_BEHAVIOR_PARSE_TACTICS, i);
            aiBehavior->aiId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(tacticsValue, LOFD_AI_BEHAVIOR_PARSE_ID);
            aiBehavior->name = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(tacticsValue, LOFD_AI_BEHAVIOR_PARSE_NAME);
            const rapidjson::Value& nodeValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(tacticsValue, LOFD_AI_BEHAVIOR_PARSE_NODE);
            aiBehavior->root = parseNode(nodeValue);
            tactics.pushBack(aiBehavior);
        }
    }
}

lofd::AIBehavior * AIConfig::getAIBehaviorById(int aiId)
{
    for (int i = 0; i < tactics.size(); i++) {
        lofd::AIBehavior * aiBehavior = tactics.at(i);
        if (aiBehavior->aiId == aiId) return aiBehavior;
    }
    return nullptr;
}

constellation::SequenceBehaviorNode * AIConfig::parseSequenceNode(const rapidjson::Value &nodeValue, std::string type, std::string name)
{
    constellation::SequenceBehaviorNode * sequenceBehaviorNode = new lofd::AISequenceBehaviorNode(type);
    sequenceBehaviorNode->nodeName = name;
    int n = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_CHILDREN);
    for (int i = 0; i < n; i++) {
        const rapidjson::Value& childNodeValue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_CHILDREN, i);
        constellation::BehaviorNode * behaviorNode = parseNode(childNodeValue);
        sequenceBehaviorNode->children.pushBack(behaviorNode);
    }
    return sequenceBehaviorNode;
}

constellation::SelectorBehaviorNode * AIConfig::parseSelectorNode(const rapidjson::Value &nodeValue, std::string type, std::string name)
{
    constellation::SelectorBehaviorNode * selectorBehaviorNode = new lofd::AISelectorBehaviorNode(type);
    selectorBehaviorNode->nodeName = name;
    int n = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_CHILDREN);
    for (int i = 0; i < n; i++) {
        const rapidjson::Value& childNodeValue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_CHILDREN, i);
        constellation::BehaviorNode * behaviorNode = parseNode(childNodeValue);
        selectorBehaviorNode->children.pushBack(behaviorNode);
    }
    return selectorBehaviorNode;
}

constellation::ConditionBehaviorNode * AIConfig::parseConditionNode(const rapidjson::Value &nodeValue, std::string type, std::string name)
{
    std::string conditionType = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_CONDITION_TYPE);
    constellation::ConditionBehaviorNode * conditionBehaviorNode;// = constellation::BehaviorTreeParse::parseConditionNode(nodeValue, type, name);
    //if (conditionBehaviorNode) return conditionBehaviorNode;
    if (conditionType == LOFD_AI_BEHAVIOR_TYPE_HAS_FOCUS)
    {
        conditionBehaviorNode = new lofd::AIConditionHasFocus(type, conditionType);
    }
    else if (conditionType == LOFD_AI_BEHAVIOR_TYPE_IS_FOCUS)
    {
        conditionBehaviorNode = new lofd::AIConditionIsFocus(type, conditionType);
    }
    else if (conditionType == LOFD_AI_BEHAVIOR_TYPE_CAMP_EQUAL)
    {
        int campareValue = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(nodeValue, LOFD_AI_BEHAVIOR_KEY_CAMP_CAMPARE_VALUE);
        conditionBehaviorNode = new lofd::AIConditionCampEqual(type, conditionType, campareValue);
    }
    else if (conditionType == LOFD_AI_BEHAVIOR_TYPE_CAMP_GREATER_THAN)
    {
        int campareValue = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(nodeValue, LOFD_AI_BEHAVIOR_KEY_CAMP_CAMPARE_VALUE);
        conditionBehaviorNode = new lofd::AIConditionCampGreaterThan(type, conditionType, campareValue);
    }
    else if (conditionType == LOFD_AI_BEHAVIOR_TYPE_CAMP_LESS_THAN)
    {
        int campareValue = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(nodeValue, LOFD_AI_BEHAVIOR_KEY_CAMP_CAMPARE_VALUE);
        conditionBehaviorNode = new lofd::AIConditionCampLessThan(type, conditionType, campareValue);
    }
    else if (conditionType == LOFD_AI_BEHAVIOR_TYPE_ALLOW_AI)
    {
        conditionBehaviorNode = new lofd::AIConditionAllowAI(type, conditionType);
    }
    else if (conditionType == LOFD_AI_BEHAVIOR_PARSE_CAMPARE_CURRENT_STATE)
    {
        std::string campareCurrentState = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(nodeValue, LOFD_AI_BEHAVIOR_PARSE_CAMPARE_CURRENT_STATE);
        conditionBehaviorNode = new lofd::AIConditionBehaviorEventCampareCurrentState(type, conditionType, campareCurrentState);
    }
    else if (conditionType == LOFD_AI_BEHAVIOR_TYPE_IS_CURRENT_FOCUS_SKILL_IS_AUTO)
    {
        conditionBehaviorNode = new lofd::AIConditionIsCurrentFocusSkillIsAuto(type, conditionType);
    }
    else if (conditionType == LOFD_AI_BEHAVIOR_TYPE_IS_AI_TO_CUSTOM)
    {
        conditionBehaviorNode = new lofd::AIConditionIsAiToCustom(type, conditionType);
    }
    else if (conditionType == LOFD_AI_BEHAVIOR_TYPE_CHECK_CURRENT_FOCUS_SKILL_OPERATION_TYPE)
    {
        std::string checkSkillOperationType = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(nodeValue, LOFD_AI_BEHAVIOR_KEY_CHECK_SKILL_OPERATION_TYPE);
        conditionBehaviorNode = new lofd::AIConditionCheckCurrentFocusSkillOperationType(type, conditionType, checkSkillOperationType);
    }
    else if (conditionType == LOFD_AI_BEHAVIOR_TYPE_IS_AI_OPERATION)
    {
        conditionBehaviorNode = new lofd::AIConditionIsAIOperation(type, conditionType);
    }
    else if (conditionType == LOFD_AI_BEHAVIOR_TYPE_IS_CUSTOM_OPERATION)
    {
        conditionBehaviorNode = new lofd::AIConditionIsCustomOperation(type, conditionType);
    }
    else if (conditionType == LOFD_AI_BEHAVIOR_TYPE_IS_FOCUS_DEAD)
    {
        conditionBehaviorNode = new lofd::AIConditionIsFocusDead(type, conditionType);
    }
    else if (conditionType == CONDITION_CAMPARE_EVENT_TYPE) {
        std::string campareCurrentState = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(nodeValue, CONDITION_CAMPARE_EVENT_TYPE);
        conditionBehaviorNode = new lofd::AIConditionBehaviorNodeCampareEventType(type, conditionType, campareCurrentState);
    }
    else
    {
        return nullptr;
    }
    conditionBehaviorNode->nodeName = name;
    return conditionBehaviorNode;
}

constellation::ActionBehaviorNode * AIConfig::parseActionNode(const rapidjson::Value &nodeValue, std::string type, std::string name)
{
    std::string actionType = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_ACTION_TYPE);
    constellation::ActionBehaviorNode * actionBehaviorNode = nullptr;
    if (LOFD_AI_BehaviorTree_actionType_randMoveTarget == actionType)
    {
        actionBehaviorNode = new lofd::AIRandMoveTarget(type, actionType);
    }
    else if (LOFD_AI_BehaviorTree_actionType_cancelCurrentState == actionType)
    {
        actionBehaviorNode = new lofd::AICancelCurrentState(type, actionType);
    }
    else if (LOFD_AI_BehaviorTree_actionType_moveTo == actionType)
    {
        actionBehaviorNode = new lofd::AIMoveTo(type, actionType);
    }
    else if (LOFD_AI_BehaviorTree_actionType_getRangeCampLessThan == actionType)
    {
        int campareCamp = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(nodeValue, LOFD_AI_BEHAVIOR_KEY_CAMP_CAMPARE_VALUE);
        actionBehaviorNode = new lofd::AIGetRangeCampLessThan(type, actionType, campareCamp);
    }
    else if (LOFD_AI_BehaviorTree_actionType_getRecentlyActorFromRange == actionType)
    {
        actionBehaviorNode = new lofd::AIGetRecentlyActorFromRange(type, actionType);
    }
    else if (LOFD_AI_BehaviorTree_actionType_focusActor == actionType)
    {
        actionBehaviorNode = new lofd::AIFocusActor(type, actionType);
    }
    else if (LOFD_AI_BehaviorTree_actionType_cancelFocusActor == actionType)
    {
        actionBehaviorNode = new lofd::AICancelFocusActor(type, actionType);
    }
    else if (LOFD_AI_BehaviorTree_actionType_attack == actionType)
    {
        actionBehaviorNode = new lofd::AIAttack(type, actionType);
    }
    else if (LOFD_AI_BehaviorTree_actionType_track == actionType)
    {
        actionBehaviorNode = new lofd::AITrack(type, actionType);
    }
    else if (LOFD_AI_BehaviorTree_actionType_target == actionType)
    {
        actionBehaviorNode = new lofd::AITarget(type, actionType);
    }
    else if (LOFD_AI_BehaviorTree_actionType_skillCurrentFocusSkill == actionType)
    {
        actionBehaviorNode = new lofd::AISkillCurrentFocusSkill(type, actionType);
    }
    else if (LOFD_AI_BehaviorTree_actionType_patrol == actionType)
    {
        actionBehaviorNode = new lofd::AIPatrol(type, actionType);
    }
    else if (LOFD_AI_BehaviorTree_actionType_idle == actionType)
    {
        actionBehaviorNode = new lofd::AIIdle(type, actionType);
    }
    else if (LOFD_AI_BehaviorTree_actionType_move == actionType)
    {
        actionBehaviorNode = new lofd::AIMove(type, actionType);
    }
    else if (LOFD_AI_BehaviorTree_actionType_focusUnCoolDownSkill == actionType)
    {
        actionBehaviorNode = new lofd::AIFocusUncoolDownSkill(type, actionType);
    }
    else
    {
        return nullptr;
    }
    actionBehaviorNode->nodeName = name;
    return actionBehaviorNode;
}
NS_LOFD_END;