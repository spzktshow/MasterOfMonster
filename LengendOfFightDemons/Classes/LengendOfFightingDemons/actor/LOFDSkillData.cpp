
//
//  LOFDSkillData.cpp
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-19.
//
//


#include "LOFDSkillData.h"
#include "TypeConver.h"
#include "FileFormat.h"
#include "BehaviorTree.h"
#include "LOFDActorBehavior.h"
#include "LOFDConfigManager.h"

NS_LOFD_BEGIN

EffectLocationData * create(lofd::EffectLocationDef * locationDefValue)
{
    EffectLocationData * effectLocationData = new EffectLocationData(locationDefValue);
    if (effectLocationData)
    {
        effectLocationData->autorelease();
        return effectLocationData;
    }
    else
    {
        delete effectLocationData;
        effectLocationData = nullptr;
        return nullptr;
    }
}

/*****************SkillDataUtils*****************/
lofd::SkillStateData * SkillDataUtils::createSkillStateData(lofd::SkillStateDef *skillStateDefValue , lofd::SkillParamContext * skillParamContext)
{
//    lofd::SkillStateData * skillStateData = new lofd::SkillStateData();
    lofd::SkillStateData * skillStateData = lofd::SkillStateData::create();
    skillStateData->skillStateDef = skillStateDefValue;
    return skillStateData;
}

lofd::EffectLocationData * SkillDataUtils::createEffectLocationData(lofd::EffectLocationDef *effectLocationDefValue , lofd::SkillParamContext * skillParamContext)
{
    if (effectLocationDefValue->locationType == EFFECT_LOCATION_TYPE_TARGET)
    {
        lofd::EffectTargetLocationDef * effectTargetLocationDef = static_cast<lofd::EffectTargetLocationDef *>(effectTargetLocationDef);
        //lofd::EffectTargetLocationData * effectTargetLocationData = new lofd::EffectTargetLocationData(effectTargetLocationDef);
        lofd::EffectTargetLocationData * effectTargetLocationData = lofd::EffectTargetLocationData::create(effectTargetLocationDef);
        effectTargetLocationData->targetPoint = new cocos2d::Point(skillParamContext->targetPoint.x, skillParamContext->targetPoint.y);
        return effectTargetLocationData;
    }
    else if (effectLocationDefValue->locationType == EFFECT_LOCATION_TYPE_RELATIVE)
    {
        lofd::EffectRelativeLocationDef * effectRelativeLocationDef = static_cast<lofd::EffectRelativeLocationDef *>(effectLocationDefValue);
        //lofd::EffectRelativeLocationData * effectRelativeLocationData = new lofd::EffectRelativeLocationData(effectRelativeLocationDef);
        lofd::EffectRelativeLocationData * effectRelativeLocationData = lofd::EffectRelativeLocationData::create(effectRelativeLocationDef);
        return effectRelativeLocationData;
    }
    return nullptr;
}

lofd::EffectMovementData * SkillDataUtils::createEffectMovementData(lofd::EffectMovementDef *effectMovementDefValue, lofd::SkillParamContext * skillParamContext)
{
    if (effectMovementDefValue->movementType == EFFECT_MOVEMENT_TYPE_STILL)
    {
        //lofd::EffectMovementStillData * effectMovementStillData = new lofd::EffectMovementStillData();
        lofd::EffectMovementStillData * effectMovementStillData = lofd::EffectMovementStillData::create();
        effectMovementStillData->movementDef = effectMovementDefValue;
        return effectMovementStillData;
    }
    else if (effectMovementDefValue->movementType == EFFECT_MOVEMENT_TYPE_TARGET)
    {
        //lofd::EffectMovementTargetData * effectMovementTargetData = new lofd::EffectMovementTargetData();
        lofd::EffectMovementTargetData * effectMovementTargetData = lofd::EffectMovementTargetData::create();
        effectMovementTargetData->movementDef = effectMovementDefValue;
        return effectMovementTargetData;
    }
    else if (effectMovementDefValue->movementType == EFFECT_MOVEMENT_TYPE_TRACK)
    {
        //lofd::EffectMovementTrackData * effectMovementTrackData = new lofd::EffectMovementTrackData();
        lofd::EffectMovementTrackData * effectMovementTrackData = lofd::EffectMovementTrackData::create();
        effectMovementTrackData->movementDef = effectMovementDefValue;
        effectMovementTrackData->selfActorData = skillParamContext->actorData;
        effectMovementTrackData->actorData = skillParamContext->targetActorData;
        return effectMovementTrackData;
    }
    return nullptr;
}

lofd::EffectCoordinateData * SkillDataUtils::createEffectCoordinateData(lofd::EffectCoordinateDef *effectCoordinateDefValue, lofd::SkillParamContext * skillParamContext)
{
    if (effectCoordinateDefValue->coordinateType == EFFECT_COORDINATE_TYPE_SCENE)
    {
        //lofd::EffectSceneCoordinateData * sceneCoordinateData = new lofd::EffectSceneCoordinateData();
        lofd::EffectSceneCoordinateData * sceneCoordinateData = lofd::EffectSceneCoordinateData::create();
        sceneCoordinateData->coordinateDef = effectCoordinateDefValue;
        sceneCoordinateData->mapScene = skillParamContext->mapScene;
        sceneCoordinateData->actorData  = skillParamContext->actorData;
        return sceneCoordinateData;
    }
    else if (effectCoordinateDefValue->coordinateType == EFFECT_COORDINATE_TYPE_ACTOR)
    {
        //lofd::EffectActorCoordinateData * actorCoordinateData = new lofd::EffectActorCoordinateData();
        lofd::EffectActorCoordinateData * actorCoordinateData = lofd::EffectActorCoordinateData::create();
        actorCoordinateData->coordinateDef = effectCoordinateDefValue;
        actorCoordinateData->actorData = skillParamContext->targetActorData;
        return actorCoordinateData;
    }
    else if (effectCoordinateDefValue->coordinateType == EFFECT_COORDINATE_TYPE_SELF_ACTOR)
    {
        //lofd::EffectSelfActorCoordinateData * selfActorCoordinateData = new lofd::EffectSelfActorCoordinateData();
        lofd::EffectSelfActorCoordinateData * selfActorCoordinateData = lofd::EffectSelfActorCoordinateData::create();
        selfActorCoordinateData->coordinateDef = effectCoordinateDefValue;
        selfActorCoordinateData->selfActorData = skillParamContext->actorData;
        return selfActorCoordinateData;
    }
    return nullptr;
}

lofd::EffectViewData * SkillDataUtils::createEffectViewData(lofd::EffectViewDef *effectViewDefValue, lofd::SkillParamContext * skillParamContext)
{
    if (effectViewDefValue->animationType == EFFECT_VIEW_TYPE_NONE)
    {
        //lofd::EffectViewData * effectViewData = new lofd::EffectViewData(effectViewDefValue);
        lofd::EffectViewData * effectViewData = lofd::EffectViewData::create(effectViewDefValue);
        return effectViewData;
    }
    else if (effectViewDefValue->animationType == EFFECT_VIEW_TYPE_FRAME)
    {
        lofd::EffectFrameViewDef * effectFrameViewDef = static_cast<lofd::EffectFrameViewDef *>(effectViewDefValue);
        //lofd::EffectFrameViewData * effectFrameViewData = new lofd::EffectFrameViewData(effectFrameViewDef);
        lofd::EffectFrameViewData * effectFrameViewData = lofd::EffectFrameViewData::create(effectFrameViewDef);
        cocos2d::SpriteFrame * sf = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(effectFrameViewDef->frameName + FILE_PNG);
        effectFrameViewData->entry = cocos2d::Sprite::createWithSpriteFrame(sf);
        effectFrameViewData->entry->retain();
        effectFrameViewData->spriteFrame = sf;
        
        return effectFrameViewData;
    }
    else if (effectViewDefValue->animationType == EFFECT_VIEW_TYPE_FRAMES)
    {
        lofd::EffectFramesViewDef * effectFramesViewDef = static_cast<lofd::EffectFramesViewDef *>(effectViewDefValue);
//        lofd::EffectFramesViewData * effectFramesViewData = new lofd::EffectFramesViewData(effectFramesViewDef);
        lofd::EffectFramesViewData * effectFramesViewData = lofd::EffectFramesViewData::create(effectFramesViewDef);
        cocos2d::Vector<cocos2d::SpriteFrame *> sfs;
        for (int i = 0; i < effectFramesViewDef->frames; i ++) {
            std::string name = effectFramesViewDef->frameName;
            name += FILE_NAME_CONNECT;
            name += moonsugar::TypeConver::filterStr(i, FILE_NUM_BIT);
            name += FILE_PNG;
            cocos2d::SpriteFrame * sf = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
            sfs.pushBack(sf);
        }
        cocos2d::Animation * animation = cocos2d::Animation::createWithSpriteFrames(sfs, effectFramesViewDef->delay);
        cocos2d::Animate * animate = cocos2d::Animate::create(animation);
        effectFramesViewData->animate = animate;
        effectFramesViewData->entry = cocos2d::Sprite::create();
        effectFramesViewData->entry->retain();
        effectFramesViewData->animate->retain();
        return effectFramesViewData;
    }
    return nullptr;
}

lofd::EffectHitData * SkillDataUtils::createEffectHitData(lofd::EffectHitDef *effectHitDefValue, lofd::SkillParamContext * skillParamContext)
{
    if (effectHitDefValue->hitType == EFFECT_HIT_TYPE_NONE)
    {
        //lofd::EffectHitData * effectHitData = new lofd::EffectHitData(effectHitDefValue);
        lofd::EffectHitData * effectHitData = lofd::EffectHitData::create(effectHitDefValue);
        return effectHitData;
    }
    else if (effectHitDefValue->hitType == EFFECT_HIT_TYPE_FRAME)
    {
        lofd::EffectFrameHitDef * effectHitFrameDef = static_cast<lofd::EffectFrameHitDef*>(effectHitDefValue);
//        lofd::EffectFrameHitData * effectFrameHitData = new lofd::EffectFrameHitData(effectHitFrameDef);
        lofd::EffectFrameHitData * effectFrameHitData = lofd::EffectFrameHitData::create(effectHitFrameDef);
        return effectFrameHitData;
    }
    else if (effectHitDefValue->hitType == EFFECT_HIT_TYPE_FRAMES)
    {
        lofd::EffectFramesHitDef * effectFramesHitDef = static_cast<lofd::EffectFramesHitDef*>(effectHitDefValue);
//        lofd::EffectFramesHitData * effectFramesHitData = new lofd::EffectFramesHitData(effectFramesHitDef);
        lofd::EffectFramesHitData * effectFramesHitData = lofd::EffectFramesHitData::create(effectFramesHitDef);
        return effectFramesHitData;
    }
    return nullptr;
}

lofd::EffectStartLifeCycleData * SkillDataUtils::createEffectStartLifeCycleData(lofd::EffectStartLifeCycleDef *effectStartLifeCycleDefValue, lofd::SkillParamContext * skillParamContext)
{
    lofd::EffectOutputData * effectOutputData = SkillDataUtils::createEffectOutputData(effectStartLifeCycleDefValue->effectOutputDef, skillParamContext);
    if (effectStartLifeCycleDefValue->type == EFFECT_START_LIFE_CYCLE_DEPEND)
    {
        lofd::EffectStartLifeCycleDependDef * startLifeCycleDependDef = static_cast<lofd::EffectStartLifeCycleDependDef *>(effectStartLifeCycleDefValue);
//        lofd::EffectStartLifeCycleDependData * startLifeCycleDependData = new lofd::EffectStartLifeCycleDependData(startLifeCycleDependDef);
        lofd::EffectStartLifeCycleDependData * startLifeCycleDependData = lofd::EffectStartLifeCycleDependData::create(startLifeCycleDependDef);
        startLifeCycleDependData->effectOutputData = effectOutputData;
        startLifeCycleDependData->effectOutputData->retain();
        return startLifeCycleDependData;
    }
    else if (effectStartLifeCycleDefValue->type == EFFECT_START_LIFE_CYCLE_INTERVAL)
    {
        lofd::EffectStartLifeCycleIntervalDef * startLifeCycleIntervalDef = static_cast<lofd::EffectStartLifeCycleIntervalDef *>(effectStartLifeCycleDefValue);
//        lofd::EffectStartLifeCycleIntervalData * startLifeCycleIntervalData = new lofd::EffectStartLifeCycleIntervalData(startLifeCycleIntervalDef);
        lofd::EffectStartLifeCycleIntervalData * startLifeCycleIntervalData = lofd::EffectStartLifeCycleIntervalData::create(startLifeCycleIntervalDef);
        startLifeCycleIntervalData->effectOutputData = effectOutputData;
        startLifeCycleIntervalData->effectOutputData->retain();
        return startLifeCycleIntervalData;
    }
    return nullptr;
}

lofd::EffectEndLifeCycleData * SkillDataUtils::createEffectEndLifeCycleData(lofd::EffectEndLifeCycleDef *effectEndLifeCycleDefValue, lofd::SkillParamContext * skillParamContext)
{
    lofd::EffectOutputData * effectOutputData = SkillDataUtils::createEffectOutputData(effectEndLifeCycleDefValue->effectOutputDef, skillParamContext);
    if (effectEndLifeCycleDefValue->type == EFFECT_END_LIFE_CYCLE_HIT)
    {
        lofd::EffectEndLifeCycleHitDef * effectEndLifeCycleHitDef = static_cast<lofd::EffectEndLifeCycleHitDef *>(effectEndLifeCycleDefValue);
//        lofd::EffectEndLifeCycleHitData * effectEndLifeCycleHitData = new lofd::EffectEndLifeCycleHitData(effectEndLifeCycleHitDef);
        lofd::EffectEndLifeCycleHitData * effectEndLifeCycleHitData = lofd::EffectEndLifeCycleHitData::create(effectEndLifeCycleHitDef);
        effectEndLifeCycleHitData->effectOutputData = effectOutputData;
        effectEndLifeCycleHitData->effectOutputData->retain();
        return effectEndLifeCycleHitData;
    }
    else if (effectEndLifeCycleDefValue->type == EFFECT_END_LIFE_CYCLE_TIME)
    {
        lofd::EffectEndLifeCycleTimeDef * effectEndLifeCycleTimeDef = static_cast<lofd::EffectEndLifeCycleTimeDef *>(effectEndLifeCycleDefValue);
//        lofd::EffectEndLifeCycleTimeData * effectEndLifeCycleTimeData = new lofd::EffectEndLifeCycleTimeData(effectEndLifeCycleTimeDef);
        lofd::EffectEndLifeCycleTimeData * effectEndLifeCycleTimeData = lofd::EffectEndLifeCycleTimeData::create(effectEndLifeCycleTimeDef);
        effectEndLifeCycleTimeData->effectOutputData = effectOutputData;
        effectEndLifeCycleTimeData->effectOutputData->retain();
        return effectEndLifeCycleTimeData;
    }
    else if (effectEndLifeCycleDefValue->type == EFFECT_END_LIFE_CYCLE_DISTANCE)
    {
        lofd::EffectEndLifeCycleDistanceDef * effectEndLifeCycleDistanceDef = static_cast<lofd::EffectEndLifeCycleDistanceDef *>(effectEndLifeCycleDefValue);
//        lofd::EffectEndLifeCycleDistanceData * effectEndLifeCycleDistanceData = new lofd::EffectEndLifeCycleDistanceData(effectEndLifeCycleDistanceDef);
        lofd::EffectEndLifeCycleDistanceData * effectEndLifeCycleDistanceData = lofd::EffectEndLifeCycleDistanceData::create(effectEndLifeCycleDistanceDef);
        effectEndLifeCycleDistanceData->effectOutputData = effectOutputData;
        effectEndLifeCycleDistanceData->effectOutputData->retain();
        return effectEndLifeCycleDistanceData;
    }
    else if (effectEndLifeCycleDefValue->type == EFFECT_END_LIFE_CYCLE_TARGET)
    {
        lofd::EffectEndLifeCycleTargetDef * effectEndLifeCycleTargetDef = static_cast<lofd::EffectEndLifeCycleTargetDef *>(effectEndLifeCycleDefValue);
//        lofd::EffectEndLifeCycleTargetData * effectEndLifeCycleTargetData = new lofd::EffectEndLifeCycleTargetData(effectEndLifeCycleTargetDef);
        lofd::EffectEndLifeCycleTargetData * effectEndLifeCycleTargetData = lofd::EffectEndLifeCycleTargetData::create(effectEndLifeCycleTargetDef);
        effectEndLifeCycleTargetData->effectOutputData = effectOutputData;
        effectEndLifeCycleTargetData->effectOutputData->retain();
        return effectEndLifeCycleTargetData;
    }
    else if (effectEndLifeCycleDefValue->type == EFFECT_END_LIFE_CYCLE_TRACK)
    {
        lofd::EffectEndLifeCycleTrackDef * effectEndLifeCycleTrackDef = static_cast<lofd::EffectEndLifeCycleTrackDef *>
        (effectEndLifeCycleDefValue);
//        lofd::EffectEndLifeCycleTrackData * effectEndLifeCycleTrackData = new lofd::EffectEndLifeCycleTrackData(effectEndLifeCycleTrackDef);
        lofd::EffectEndLifeCycleTrackData * effectEndLifeCycleTrackData = lofd::EffectEndLifeCycleTrackData::create(effectEndLifeCycleTrackDef);
        effectEndLifeCycleTrackData->actorData = skillParamContext->targetActorData;
        effectEndLifeCycleTrackData->effectOutputData = effectOutputData;
        effectEndLifeCycleTrackData->effectOutputData->retain();
        return effectEndLifeCycleTrackData;
    }
    return nullptr;
}

lofd::EffectLifeCycleData * SkillDataUtils::createEffectLifeCycleData(lofd::EffectLifeCycleDef *effectLifeCycleDefValue, lofd::SkillParamContext * skillParamContext)
{
//    lofd::EffectLifeCycleData * lifeCycleData = new lofd::EffectLifeCycleData();
    lofd::EffectLifeCycleData * lifeCycleData = lofd::EffectLifeCycleData::create();
    lifeCycleData->effectLifeCycleDef = effectLifeCycleDefValue;
    lifeCycleData->startLifeCycleData = SkillDataUtils::createEffectStartLifeCycleData(effectLifeCycleDefValue->effectStartLifeCycleDef, skillParamContext);
    lifeCycleData->startLifeCycleData->retain();
    for (int i = 0; i < effectLifeCycleDefValue->effectEndLifeCycelDefs.size(); i ++)
    {
        lofd::EffectEndLifeCycleData * endLifeCycleData = SkillDataUtils::createEffectEndLifeCycleData(effectLifeCycleDefValue->effectEndLifeCycelDefs.at(i), skillParamContext);
        lifeCycleData->endLifeCycleDatas.pushBack(endLifeCycleData);
    }
    return lifeCycleData;
}

lofd::SkillEffectData * SkillDataUtils::createSkillEffectData(lofd::SkillEffectDef *skillEffectDefValue, lofd::SkillParamContext * skillParamContext)
{
//    lofd::SkillEffectData * skillEffectData = new lofd::SkillEffectData();
    lofd::SkillEffectData * skillEffectData = lofd::SkillEffectData::create();
    skillEffectData->skillEffectDef = skillEffectDefValue;
    
    skillEffectData->effectLocationData = SkillDataUtils::createEffectLocationData(skillEffectDefValue->locationDef, skillParamContext);
    skillEffectData->effectLocationData->retain();
    skillEffectData->effectMovementData = SkillDataUtils::createEffectMovementData(skillEffectDefValue->movementDef, skillParamContext);
    skillEffectData->effectMovementData->retain();
    skillEffectData->effectCoordinateData = SkillDataUtils::createEffectCoordinateData(skillEffectDefValue->coordinateDef, skillParamContext);
    skillEffectData->effectCoordinateData->retain();
    
    skillEffectData->effectViewData = SkillDataUtils::createEffectViewData(skillEffectDefValue->viewDef, skillParamContext);
    skillEffectData->effectViewData->retain();
    skillEffectData->effectHitData = SkillDataUtils::createEffectHitData(skillEffectDefValue->hitDef, skillParamContext);
    skillEffectData->effectHitData->retain();
    return skillEffectData;
}

lofd::SkillEffectContextData * SkillDataUtils::createSkillEffectContextData(lofd::SkillEffectContextDef *skillEffectContextDefValue, lofd::SkillParamContext * skillParamContext)
{
    lofd::SkillEffectContextData * contextData = lofd::SkillEffectContextData::create();
    contextData->skillEffectContextDef = skillEffectContextDefValue;
    contextData->skillEffectData = SkillDataUtils::createSkillEffectData(skillEffectContextDefValue->skillEffectDef, skillParamContext);
    contextData->skillEffectData->retain();
    contextData->effectLifeCycleData = SkillDataUtils::createEffectLifeCycleData(skillEffectContextDefValue->effectLifeCycleDef, skillParamContext);
    contextData->effectLifeCycleData->retain();
    return contextData;
}

lofd::SkillOperationContextData * SkillDataUtils::createSkillOperationContextData(lofd::SkillOperationContextDef *skillOperationContextDefValue, lofd::SkillParamContext * skillParamContext)
{
    if (skillOperationContextDefValue->operationType == SKILL_OPERATION_TYPE_NONE)
    {
//        lofd::SkillOperationContextData * operatonContextData = new lofd::SkillOperationContextData();
        lofd::SkillOperationContextData * operatonContextData = lofd::SkillOperationContextData::create();
        operatonContextData->operationContextDef = skillOperationContextDefValue;
        return operatonContextData;
    }
    else if (skillOperationContextDefValue->operationType == SKILL_OPERATION_TYPE_ACTOR)
    {
//        lofd::SkillOperationContextActorData * operationContextActorData = new lofd::SkillOperationContextActorData();
        lofd::SkillOperationContextActorData * operationContextActorData =  lofd::SkillOperationContextActorData::create();
        operationContextActorData->operationContextDef = skillOperationContextDefValue;
        return operationContextActorData;
    }
    else if (skillOperationContextDefValue->operationType == SKILL_OPERATION_TYPE_TARGET)
    {
//        lofd::SkillOperationContextTargetData * operationContextTargetData = new lofd::SkillOperationContextTargetData();
        lofd::SkillOperationContextTargetData * operationContextTargetData = lofd::SkillOperationContextTargetData::create();
        operationContextTargetData->operationContextDef = skillOperationContextDefValue;
        return operationContextTargetData;
    }
    return nullptr;
}

lofd::SkillData * SkillDataUtils::createSkillData(lofd::SkillDef *skillDefValue, lofd::SkillParamContext * skillParamContext)
{
//    lofd::SkillData * skillData = new lofd::SkillData();
    lofd::SkillData * skillData = lofd::SkillData::create();
    skillData->skillDef = skillDefValue;
    int index = rand() % skillData->skillDef->skillRandomContexts.size();
    lofd::SkillRandomContext * skillRandomContext = skillData->skillDef->skillRandomContexts.at(index);
    skillData->skillStateData = SkillDataUtils::createSkillStateData(skillRandomContext->stateDef, skillParamContext);
    skillData->skillStateData->retain();
    for (int i = 0; i < skillRandomContext->effectContextDefs.size(); i ++) {
        lofd::SkillEffectContextData * skillEffectContextData = SkillDataUtils::createSkillEffectContextData(skillRandomContext->effectContextDefs.at(i), skillParamContext);
        /*****TODO:需要解偶和*****/
        skillEffectContextData->skillData = skillData;
        
        skillData->effectContextDatas.pushBack(skillEffectContextData);
    }
    skillData->skillOperationContextData = SkillDataUtils::createSkillOperationContextData(skillDefValue->operationContextDef, skillParamContext);
    skillData->skillOperationContextData->retain();
    if (skillParamContext->actorData != nullptr)
    {
        skillData->setActor(skillParamContext->actorData);
    }
    if (skillParamContext->targetActorData != nullptr)
    {
        skillData->setTargetActor(skillParamContext->targetActorData);
    }
    return skillData;
}

lofd::EffectOutputData * SkillDataUtils::createEffectOutputData(lofd::EffectOutputDef *effectOutputDefValue, lofd::SkillParamContext *skillParamContext)
{
//    lofd::EffectOutputData * effectOutputData = new lofd::EffectOutputData;
    lofd::EffectOutputData * effectOutputData = lofd::EffectOutputData::create();
    effectOutputData->effectOutputDef = effectOutputDefValue;
    return effectOutputData;
}

int SkillDataUtils::checkHasEndLifeCycletype(lofd::EffectLifeCycleDef *effectLifeCycleDefValue, std::string checkType)
{
    for (int i = 0; i < effectLifeCycleDefValue->effectEndLifeCycelDefs.size(); i ++) {
        lofd::EffectEndLifeCycleDef * def = effectLifeCycleDefValue->effectEndLifeCycelDefs.at(i);
        if (def->type == checkType) return i;
    }
    return -1;
}
/************************************************/

/************SkillEffectContextData*******************/
void SkillEffectContextData::startLifeCycle()
{
    //cocos2d::log("skill effect startLifeCycle: %s", this->skillEffectContextDef->skillEffectDef->effectName.c_str());
    this->onEnter();
    this->scheduleUpdate();
    
    this->locateInit();
    if (this->skillEffectContextDef->skillEffectDef->viewDef->animationType == EFFECT_VIEW_TYPE_FRAME)
    {
        this->effectViewFrameInit();
    }
    else if(this->skillEffectContextDef->skillEffectDef->viewDef->animationType == EFFECT_VIEW_TYPE_FRAMES)
    {
        this->effectViewFramesInit();
    }
    float outputValue = 0;
    if (this->effectLifeCycleData->startLifeCycleData->effectStartLifeCycleDef->effectOutputDef->type == EFFECT_OUTPUT_ADDTION_BLOOD)
    {
        if (this->skillEffectData->skillEffectDef->coordinateDef->coordinateType == EFFECT_COORDINATE_TYPE_ACTOR)
        {
            outputValue = 50;
            lofd::EffectActorCoordinateData * effectActorCoordinateData = dynamic_cast<lofd::EffectActorCoordinateData *>(this->skillEffectData->effectCoordinateData);
//            lofd::ActorBehaviorHurtData * actorBehaviorHurtData = new lofd::ActorBehaviorHurtData();
//            actorBehaviorHurtData->actorData = effectActorCoordinateData->actorData;
//            actorBehaviorHurtData->hurtActorData = nullptr;
//            constellation::BehaviorEvent * event = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_HURT);
//            event->behaviorData = actorBehaviorHurtData;
//            effectActorCoordinateData->actorData->actorBehavior->root->execute(event);
//            delete event;
//            delete actorBehaviorHurtData;
            effectActorCoordinateData->actorData->reduceHp(outputValue);
        }
        else if (this->skillEffectData->skillEffectDef->coordinateDef->coordinateType == EFFECT_COORDINATE_TYPE_SELF_ACTOR)
        {

        }
    }
    else if (this->effectLifeCycleData->startLifeCycleData->effectStartLifeCycleDef->effectOutputDef->type == EFFECT_OUTPUT_REDUCE_BLOOD)
    {
        if (this->skillEffectData->skillEffectDef->coordinateDef->coordinateType == EFFECT_COORDINATE_TYPE_ACTOR)
        {
            outputValue = 50;
            lofd::EffectActorCoordinateData * effectActorCoordinateData = dynamic_cast<lofd::EffectActorCoordinateData *>(this->skillEffectData->effectCoordinateData);
//            lofd::ActorBehaviorHurtData * actorBehaviorHurtData = new lofd::ActorBehaviorHurtData();
//            actorBehaviorHurtData->actorData = effectActorCoordinateData->actorData;
//            actorBehaviorHurtData->hurtActorData = nullptr;
//            constellation::BehaviorEvent * event = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_HURT);
//            event->behaviorData = actorBehaviorHurtData;
//            effectActorCoordinateData->actorData->actorBehavior->root->execute(event);
//            delete event;
//            delete actorBehaviorHurtData;
            effectActorCoordinateData->actorData->reduceHp(outputValue);
        }
        else if (this->skillEffectData->skillEffectDef->coordinateDef->coordinateType == EFFECT_COORDINATE_TYPE_SELF_ACTOR)
        {

        }
    }
    this->isRunning = true;
}

void SkillEffectContextData::endLifeCycle()
{
    this->isComplete = true;
    this->isRunning = false;
    
    if (this->skillEffectContextDef->skillEffectDef->viewDef->animationType == EFFECT_VIEW_TYPE_FRAME)
    {
        this->effectViewFrameDispose();
    }
    else if(this->skillEffectContextDef->skillEffectDef->viewDef->animationType == EFFECT_VIEW_TYPE_FRAMES)
    {
        this->effectViewFramesDispose();
    }
    this->locateDispose();

    this->unscheduleUpdate();
    this->onExit();
}

void SkillEffectContextData::update(float dt)
{
    if (!this->isCompleteTag) this->endLifeCycleTimeUpdate(dt);
    if (!this->isCompleteTag) this->endLifeCyleDistanceUpdate(dt);
    if (!this->isCompleteTag) this->endLifeCycleTargetUpdate(dt);
    if (!this->isCompleteTag) this->endLifeCycleTrackUpdate(dt);
    
    if (!this->isCompleteTag) this->movementActor(dt);
    
    if (!this->isCompleteTag) this->frameHitUpdate(dt);
    if (!this->isCompleteTag) this->framesHitUpdate(dt);
    
    if (!this->isCompleteTag) currentLifeCycle += dt;
}

void SkillEffectContextData::effectViewFrameInit()
{
    lofd::EffectFrameViewData * effectFrameViewData = dynamic_cast<lofd::EffectFrameViewData *>(this->skillEffectData->effectViewData);
    effectFrameViewData->entry->setAnchorPoint(*effectFrameViewData->effectFrameViewDef->anchorPoint);
    //effectFrameViewData->entry->setSpriteFrame(effectFrameViewData->spriteFrame);
}

void SkillEffectContextData::effectViewFrameDispose()
{
    
}

void SkillEffectContextData::effectViewFramesInit()
{
    lofd::EffectFramesViewData * effectFramesViewData = static_cast<lofd::EffectFramesViewData *>(this->skillEffectData->effectViewData);
    effectFramesViewData->entry->setAnchorPoint(*effectFramesViewData->effectFramesViewDef->anchorPoint);
    effectFramesViewData->entry->stopAllActions();
    effectFramesViewData->entry->runAction(effectFramesViewData->animate);
}

void SkillEffectContextData::effectViewFramesDispose()
{
    //it looks like disposed in removeChild when calling locateDispose;
}

void SkillEffectContextData::effectViewFramesRunAnimation()
{
    
}

void SkillEffectContextData::effectViewFrameStopAnimation()
{
    
}

void SkillEffectContextData::locateInit()
{
    if (this->skillEffectContextDef->skillEffectDef->viewDef->animationType == EFFECT_VIEW_TYPE_NONE) return;
    
    if (this->skillEffectContextDef->skillEffectDef->coordinateDef->coordinateType == EFFECT_COORDINATE_TYPE_ACTOR)
    {
        lofd::EffectActorCoordinateData * effectActorCoordinateData = static_cast<lofd::EffectActorCoordinateData *>(this->skillEffectData->effectCoordinateData);
        effectActorCoordinateData->actorData->actorContainer->addChild(this->skillEffectData->effectViewData->entry);
        if (this->skillEffectContextDef->skillEffectDef->locationDef->locationType == EFFECT_LOCATION_TYPE_RELATIVE)
        {
            lofd::EffectRelativeLocationData * relativeLocationData = static_cast<lofd::EffectRelativeLocationData *>(this->skillEffectData->effectLocationData);
            if (effectActorCoordinateData->actorData->actorEntry->isFlippedX())
            {
                cocos2d::Point relativePoint(0 - relativeLocationData->relativeLocationDef->relativePoint->x, relativeLocationData->relativeLocationDef->relativePoint->y);
                this->skillEffectData->effectViewData->entry->setPosition(relativePoint);
                this->skillEffectData->effectViewData->entry->setFlippedX(true);
            }
            else
            {
                this->skillEffectData->effectViewData->entry->setPosition(*relativeLocationData->relativeLocationDef->relativePoint);
                this->skillEffectData->effectViewData->entry->setFlippedX(false);
            }
        }
        else if (this->skillEffectContextDef->skillEffectDef->locationDef->locationType == EFFECT_LOCATION_TYPE_TARGET)
        {
            
        }
    }
    else if (this->skillEffectContextDef->skillEffectDef->coordinateDef->coordinateType == EFFECT_COORDINATE_TYPE_SCENE)
    {
        lofd::EffectSceneCoordinateData * effectSceneCoordinateData = static_cast<lofd::EffectSceneCoordinateData *>(this->skillEffectData->effectCoordinateData);
        effectSceneCoordinateData->mapScene->getMapActorsLayer()->addChild(this->skillEffectData->effectViewData->entry);
        if (this->skillEffectContextDef->skillEffectDef->locationDef->locationType == EFFECT_LOCATION_TYPE_RELATIVE)
        {
            lofd::EffectRelativeLocationData * relativeLocationData = static_cast<lofd::EffectRelativeLocationData *>(this->skillEffectData->effectLocationData);
            cocos2d::Point p(effectSceneCoordinateData->actorData->actorContainer->getPositionX() + relativeLocationData->relativeLocationDef->relativePoint->x, effectSceneCoordinateData->actorData->actorContainer->getPositionY() + relativeLocationData->relativeLocationDef->relativePoint->y);
            this->skillEffectData->effectViewData->entry->setPosition(p);
        }
        else if (this->skillEffectContextDef->skillEffectDef->locationDef->locationType == EFFECT_LOCATION_TYPE_TARGET)
        {
            lofd::EffectTargetLocationData * targetLocationData = dynamic_cast<lofd::EffectTargetLocationData *>(this->skillEffectData->effectLocationData);
            this->skillEffectData->effectViewData->entry->setPosition(*targetLocationData->targetPoint);
        }
    }
    else if (this->skillEffectContextDef->skillEffectDef->coordinateDef->coordinateType == EFFECT_COORDINATE_TYPE_SELF_ACTOR)
    {
        lofd::EffectSelfActorCoordinateData * effectSelfActorCoordinateData = static_cast<lofd::EffectSelfActorCoordinateData *>(this->skillEffectData->effectCoordinateData);
        effectSelfActorCoordinateData->selfActorData->actorContainer->addChild(this->skillEffectData->effectViewData->entry);
        if (this->skillEffectContextDef->skillEffectDef->locationDef->locationType == EFFECT_LOCATION_TYPE_RELATIVE)
        {
            lofd::EffectRelativeLocationData * relativeLocationData = static_cast<lofd::EffectRelativeLocationData *>(this->skillEffectData->effectLocationData);
            if (effectSelfActorCoordinateData->selfActorData->actorEntry->isFlippedX())
            {
                cocos2d::Point relativePoint(0 - relativeLocationData->relativeLocationDef->relativePoint->x, relativeLocationData->relativeLocationDef->relativePoint->y);
                this->skillEffectData->effectViewData->entry->setPosition(relativePoint);
                this->skillEffectData->effectViewData->entry->setFlippedX(true);
            }
            else
            {
                this->skillEffectData->effectViewData->entry->setPosition(*relativeLocationData->relativeLocationDef->relativePoint);
                this->skillEffectData->effectViewData->entry->setFlippedX(false);
            }
        }
        else if (this->skillEffectContextDef->skillEffectDef->locationDef->locationType == EFFECT_LOCATION_TYPE_TARGET)
        {
            
        }
    }
}

void SkillEffectContextData::locateDispose()
{
    if (this->skillEffectContextDef->skillEffectDef->viewDef->animationType == EFFECT_VIEW_TYPE_NONE) return;
    
    if (this->skillEffectContextDef->skillEffectDef->coordinateDef->coordinateType == EFFECT_COORDINATE_TYPE_ACTOR)
    {
        lofd::EffectActorCoordinateData * effectActorCoordinateData = static_cast<lofd::EffectActorCoordinateData *>(this->skillEffectData->effectCoordinateData);
        effectActorCoordinateData->actorData->actorContainer->removeChild(this->skillEffectData->effectViewData->entry, true);
    }
    else if (this->skillEffectContextDef->skillEffectDef->coordinateDef->coordinateType == EFFECT_COORDINATE_TYPE_SCENE)
    {
        lofd::EffectSceneCoordinateData * effectSceneCoordinateData = static_cast<lofd::EffectSceneCoordinateData *>(this->skillEffectData->effectCoordinateData);
        effectSceneCoordinateData->mapScene->getMapActorsLayer()->removeChild(this->skillEffectData->effectViewData->entry, true);
    }
    else if (this->skillEffectContextDef->skillEffectDef->coordinateDef->coordinateType == EFFECT_COORDINATE_TYPE_SELF_ACTOR)
    {
        lofd::EffectSelfActorCoordinateData * effectSelfActorCoordinateData = static_cast<lofd::EffectSelfActorCoordinateData *>(this->skillEffectData->effectCoordinateData);
        effectSelfActorCoordinateData->selfActorData->actorContainer->removeChild(this->skillEffectData->effectViewData->entry, true);
    }
}

void SkillEffectContextData::movementActor(float dt)
{
    if (this->skillEffectData->skillEffectDef->movementDef->movementType != EFFECT_MOVEMENT_TYPE_TRACK) return;
    lofd::EffectMovementTrackData * effectMovementTrackData = dynamic_cast<lofd::EffectMovementTrackData *>(this->skillEffectData->effectMovementData);
    lofd::EffectMovementTrackDef * effectMovementTrackDef = dynamic_cast<lofd::EffectMovementTrackDef *>(effectMovementTrackData->movementDef);
    if (this->skillEffectData->skillEffectDef->locationDef->locationType == EFFECT_LOCATION_TYPE_RELATIVE)
    {
        lofd::EffectRelativeLocationDef * effectLocationDef = dynamic_cast<lofd::EffectRelativeLocationDef *>(this->skillEffectData->skillEffectDef->locationDef);
        cocos2d::Point actorContainerPoint(effectMovementTrackData->actorData->actorContainer->getPositionX() + effectLocationDef->relativePoint->x
                                           , effectMovementTrackData->actorData->actorContainer->getPositionY() + effectLocationDef->relativePoint->y);
        float dy = actorContainerPoint.y - this->skillEffectData->effectViewData->entry->getPositionY();
        float dx = actorContainerPoint.x - this->skillEffectData->effectViewData->entry->getPositionX();
        double angle = atan2(dy, dx);
        double vx = cos(angle) * effectMovementTrackDef->speed;
        double vy = sin(angle) * effectMovementTrackDef->speed;
        cocos2d::Point p(this->skillEffectData->effectViewData->entry->getPositionX() + vx, this->skillEffectData->effectViewData->entry->getPositionY() + vy);
        const double PI=3.1415926;
        if (actorContainerPoint.x >= this->skillEffectData->effectViewData->entry->getPositionX())
        {
            if (actorContainerPoint.y >= this->skillEffectData->effectViewData->entry->getPositionY())
            {
                angle = 2 * PI - angle;
                //cocos2d::log("one");
            }
            else
            {
                angle = 2 * PI - angle;
                //cocos2d::log("two");
            }
        }
        else
        {
            //第二
            if (actorContainerPoint.y >= this->skillEffectData->effectViewData->entry->getPositionY())
            {
                angle = 2 * PI - angle;
                //cocos2d::log("three");
            }
            else//第三
            {
                angle = 2 * PI - angle;
                //cocos2d::log("four");
            }
        }
        float rotaion = angle * 180 / 3.141596;
        this->skillEffectData->effectViewData->entry->setRotation(rotaion);
        this->skillEffectData->effectViewData->entry->setPosition(p);
    }
    else
    {
        
    }
}

void SkillEffectContextData::frameHitUpdate(float dt)
{
    if (this->skillEffectData->skillEffectDef->hitDef->hitType != EFFECT_HIT_TYPE_FRAME) return;
    
    lofd::EffectFrameHitData * effectFrameHitData = dynamic_cast<lofd::EffectFrameHitData *>(this->skillEffectData->effectHitData);
    
    if (effectFrameHitData->hasTrigger) return;
    
    if (this->currentLifeCycle >= effectFrameHitData->effectFrameHitDef->effectHitFrameDef->frameIndex)
    {
        cocos2d::Rect * hitRect = effectFrameHitData->effectFrameHitDef->effectHitFrameDef->hitRects.at(0);
        
        if (this->skillEffectData->skillEffectDef->locationDef->locationType == EFFECT_LOCATION_TYPE_TARGET)
        {
            lofd::EffectTargetLocationData * effectTargetLocationData = dynamic_cast<lofd::EffectTargetLocationData *>(this->skillEffectData->effectLocationData);
            cocos2d::Rect checkRect(hitRect->origin.x + effectTargetLocationData->targetPoint->x, hitRect->origin.y + effectTargetLocationData->targetPoint->y, hitRect->size.width, hitRect->size.height);
            if (this->skillEffectData->skillEffectDef->coordinateDef->coordinateType == EFFECT_COORDINATE_TYPE_SCENE)
            {
                lofd::EffectSceneCoordinateData * effectCoordinateData = dynamic_cast<lofd::EffectSceneCoordinateData *>(this->skillEffectData->effectCoordinateData);
                moonsugar::MapActorsLayer * mapActorsLayer = effectCoordinateData->mapScene->getMapActorsLayer();
                long n = mapActorsLayer->actorLayerData->mapActorDatas.size();
                for (int i = 0; i < n; i ++)
                {
                    lofd::ActorData * actorData = dynamic_cast<lofd::ActorData *>(mapActorsLayer->actorLayerData->mapActorDatas.at(i));
                    cocos2d::Rect * actorRect = actorData->getBound();
                    if (checkRect.intersectsRect(*actorRect))
                    {
                        //cocos2d::log("frameHitUpdate");
                        effectFrameHitData->hasTrigger = true;
                        modifyEndLifeCycleHit(actorData);
                    }
                }
            }
            else
            {
                
            }
        }
        else
        {
            
        }
    }
}

void SkillEffectContextData::framesHitUpdate(float dt)
{
    if (this->skillEffectData->skillEffectDef->hitDef->hitType != EFFECT_HIT_TYPE_FRAMES)
    {
        return;
    }
    
    lofd::EffectFramesHitData * effectFramesHitData = dynamic_cast<lofd::EffectFramesHitData *>(this->skillEffectData->effectHitData);
    
    if (effectFramesHitData->hasTriggerIndex >= effectFramesHitData->effectFramesHitDef->frames.size() - 1)
    {
        return;
    }
    
    if (this->skillEffectData->skillEffectDef->locationDef->locationType == EFFECT_LOCATION_TYPE_TARGET)
    {
        long n = effectFramesHitData->effectFramesHitDef->frames.size();
        for (int i = 0; i < n; i ++) {
            
            lofd::EffectHitFrameDef * hitFrameDef = effectFramesHitData->effectFramesHitDef->frames.at(i);
            if (hitFrameDef->frameIndex >= dt && effectFramesHitData->hasTriggerIndex < i) {
                effectFramesHitData->hasTriggerIndex = i;
                cocos2d::Rect * hitRect = hitFrameDef->hitRects.at(0);
                
                lofd::EffectTargetLocationData * effectTargetLocationData = dynamic_cast<lofd::EffectTargetLocationData *>(this->skillEffectData->effectLocationData);
                cocos2d::Rect checkRect(hitRect->origin.x + effectTargetLocationData->targetPoint->x, hitRect->origin.y + effectTargetLocationData->targetPoint->y, hitRect->size.width, hitRect->size.height);
                
                if (this->skillEffectData->skillEffectDef->coordinateDef->coordinateType == EFFECT_COORDINATE_TYPE_SCENE)
                {
                    lofd::EffectSceneCoordinateData * effectCoordinateData = dynamic_cast<lofd::EffectSceneCoordinateData *>(this->skillEffectData->effectCoordinateData);
                    moonsugar::MapActorsLayer * mapActorsLayer = effectCoordinateData->mapScene->getMapActorsLayer();
                    
                    long n = mapActorsLayer->actorLayerData->mapActorDatas.size();
                    for (int i = 0; i < n; i ++)
                    {
                        lofd::ActorData * actorData = dynamic_cast<lofd::ActorData *>(mapActorsLayer->actorLayerData->mapActorDatas.at(i));
                        cocos2d::Rect * actorRect = actorData->getBound();
                        if (checkRect.intersectsRect(*actorRect))
                        {
                            //cocos2d::log("frameHitUpdate");
                            modifyEndLifeCycleHit(actorData);
                        }
                    }
                }
                else
                {
                    
                }
            }
        }
    }
    
}

void SkillEffectContextData::modifyEndLifeCycleHit(lofd::ActorData * actorDataValue)
{
    /********每次都做hit+1处理*******/
    lofd::EffectLifeCycleDef * effectLifeCycleDef = this->skillEffectContextDef->effectLifeCycleDef;
    int index = SkillDataUtils::checkHasEndLifeCycletype(effectLifeCycleDef, EFFECT_END_LIFE_CYCLE_HIT);
    if (index != -1)
    {
        lofd::EffectEndLifeCycleHitData * data = static_cast<lofd::EffectEndLifeCycleHitData *>(this->effectLifeCycleData->endLifeCycleDatas.at(index));
                    //暂时写死加血伤血都是50，实际根据EffectOutputDef配置中的formulaId读取伤害公式
        float outputValue = 0;
        bool validOutput = false;
        if (data->effectOutputData->effectOutputDef->outputTargetType == EFFECT_OUTPUT_TARGET_TYPE_ALL)
        {
            validOutput = true;
        }
        else if (data->effectOutputData->effectOutputDef->outputTargetType == EFFECT_OUTPUT_TARGET_TYPE_ENEMY)
        {
            lofd::CampConfig * campConfig = lofd::ConfigManager::getInstance()->campConfig;
            lofd::CampDef * campDef = campConfig->getCampDefById(this->skillData->actor->actorDef->campId);
            lofd::CampRelationShipDef * campRelationShipDef = campDef->getDefByCampId(actorDataValue->actorDef->campId);
            if (campRelationShipDef->relationShip < 0)
            {
                validOutput = true;
            }
        }
        else if (data->effectOutputData->effectOutputDef->outputTargetType == EFFECT_OUTPUT_TARGET_TYPE_FRIENDLY)
        {
            lofd::CampConfig * campConfig = lofd::ConfigManager::getInstance()->campConfig;
            lofd::CampDef * campDef = campConfig->getCampDefById(this->skillData->actor->actorDef->campId);
            lofd::CampRelationShipDef * campRelationShipDef = campDef->getDefByCampId(actorDataValue->actorDef->campId);
            if (campRelationShipDef->relationShip > 0)
            {
                validOutput = true;
            }
        }
        if (validOutput)
        {
            data->currentHit ++;
            if (data->effectEndLifeCycleDef->effectOutputDef->type == EFFECT_OUTPUT_ADDTION_BLOOD)
            {
                
            }
            else if (data->effectEndLifeCycleDef->effectOutputDef->type == EFFECT_OUTPUT_REDUCE_BLOOD)
            {
                outputValue = 50;
//                lofd::ActorBehaviorHurtData * actorBehaviorHurtData = new lofd::ActorBehaviorHurtData();
//                actorBehaviorHurtData->actorData = actorDataValue;
//                actorBehaviorHurtData->hurtActorData = nullptr;
//                constellation::BehaviorEvent * event = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_HURT);
//                event->behaviorData = actorBehaviorHurtData;
//                actorDataValue->actorBehavior->root->execute(event);
//                delete event;
//                delete actorBehaviorHurtData;
                
                actorDataValue->reduceHp(outputValue);
            }
            else//none
            {
                
            }
            lofd::EffectEndLifeCycleHitDef * hitDef = static_cast<lofd::EffectEndLifeCycleHitDef *>(data->effectEndLifeCycleDef);
            if (hitDef->hitLimit != -1)
            {
                if (data->currentHit >= hitDef->hitLimit)
                {
                    //
                    this->isCompleteTag = true;
                    this->skillData->onSkillEffectContextDataComplete(this);
                }
            }
        }
    }
}

void SkillEffectContextData::endLifeCycleTargetUpdate(float dt)
{
    lofd::EffectLifeCycleDef * effectLifeCycleDef = this->skillEffectContextDef->effectLifeCycleDef;
    int index = SkillDataUtils::checkHasEndLifeCycletype(effectLifeCycleDef, EFFECT_END_LIFE_CYCLE_TARGET);
    if (index != -1)
    {
        lofd::EffectEndLifeCycleTargetData * data = static_cast<lofd::EffectEndLifeCycleTargetData *>(this->effectLifeCycleData->endLifeCycleDatas.at(index));
        
        if (this->skillEffectContextDef->skillEffectDef->viewDef->animationType == EFFECT_VIEW_TYPE_NONE) return;
        if (this->skillEffectData->effectViewData->entry->getPositionX() == data->target->x
            && this->skillEffectData->effectViewData->entry->getPositionY() == data->target->y)
        {
            //this->endLifeCycle();
            this->isCompleteTag = true;
            this->skillData->onSkillEffectContextDataComplete(this);
        }
    }
}

void SkillEffectContextData::endLifeCycleTimeUpdate(float dt)
{
    lofd::EffectLifeCycleDef * effectLifeCycleDef = this->skillEffectContextDef->effectLifeCycleDef;
    int index = SkillDataUtils::checkHasEndLifeCycletype(effectLifeCycleDef, EFFECT_END_LIFE_CYCLE_TIME);
    if (index != -1)
    {
        lofd::EffectEndLifeCycleTimeData * data = static_cast<lofd::EffectEndLifeCycleTimeData *>(this->effectLifeCycleData->endLifeCycleDatas.at(index));
        if (data->currentTime >= data->effectEndLifeCycleTimeDef->time)
        {
            //this->endLifeCycle();
            float outputValue = 0;
            if (data->effectOutputData->effectOutputDef->type == EFFECT_OUTPUT_REDUCE_BLOOD)
            {
                outputValue = 50;
                    
                if (this->skillEffectData->skillEffectDef->coordinateDef->coordinateType == EFFECT_COORDINATE_TYPE_ACTOR)
                {
                    lofd::EffectActorCoordinateData * effectActorCoordinateData = dynamic_cast<lofd::EffectActorCoordinateData *>(this->skillEffectData->effectCoordinateData);
//                    lofd::ActorBehaviorHurtData * actorBehaviorHurtData = new lofd::ActorBehaviorHurtData();
//                    actorBehaviorHurtData->actorData = effectActorCoordinateData->actorData;
//                    actorBehaviorHurtData->hurtActorData = nullptr;
//                    constellation::BehaviorEvent * event = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_HURT);
//                    event->behaviorData = actorBehaviorHurtData;
//                    effectActorCoordinateData->actorData->actorBehavior->root->execute(event);
//                    delete event;
//                    delete actorBehaviorHurtData;
                    
                    effectActorCoordinateData->actorData->reduceHp(outputValue);
                }
                else if (this->skillEffectData->skillEffectDef->coordinateDef->coordinateType == EFFECT_COORDINATE_TYPE_SELF_ACTOR)
                {
                        
                }
            }
            else if (data->effectOutputData->effectOutputDef->type == EFFECT_OUTPUT_ADDTION_BLOOD)
            {
                    
            }
            else//none
            {
                
            }
            this->isCompleteTag = true;
            this->skillData->onSkillEffectContextDataComplete(this);
        }
        data->currentTime += dt;
    }
}

void SkillEffectContextData::endLifeCyleDistanceUpdate(float dt)
{
    lofd::EffectLifeCycleDef * effectLifeCycleDef = this->skillEffectContextDef->effectLifeCycleDef;
    int index = SkillDataUtils::checkHasEndLifeCycletype(effectLifeCycleDef, EFFECT_END_LIFE_CYCLE_DISTANCE);
    if (index != -1)
    {
        lofd::EffectEndLifeCycleDistanceData * data = static_cast<lofd::EffectEndLifeCycleDistanceData *>(this->effectLifeCycleData->endLifeCycleDatas.at(index));
        if (data->currentDistance >= data->effectEndLifeCycleDistanceDef->distance)
        {
//            this->endLifeCycle();
            this->isCompleteTag = true;
            this->skillData->onSkillEffectContextDataComplete(this);
        }
    }
}

void SkillEffectContextData::endLifeCycleTrackUpdate(float dt)
{
    lofd::EffectLifeCycleDef * effectLifeCycleDef = this->skillEffectContextDef->effectLifeCycleDef;
    int index = SkillDataUtils::checkHasEndLifeCycletype(effectLifeCycleDef, EFFECT_END_LIFE_CYCLE_TRACK);
    if (index != -1)
    {
        //lofd::EffectEndLifeCycleTrackData * data = dynamic_cast<lofd::EffectEndLifeCycleTrackData *>(this->effectLifeCycleData->endLifeCycleDatas.at(index));
        if (this->skillEffectData->effectMovementData->movementDef->movementType == EFFECT_MOVEMENT_TYPE_TRACK)
        {
            lofd::EffectMovementTrackData * effectMovementTrackData = dynamic_cast<lofd::EffectMovementTrackData *>(this->skillEffectData->effectMovementData);
            //lofd::EffectMovementTrackDef * effectMovementTrackDef = dynamic_cast<lofd::EffectMovementTrackDef *>(effectMovementTrackData->movementDef);
            if (this->skillEffectData->skillEffectDef->locationDef->locationType == EFFECT_LOCATION_TYPE_RELATIVE)
            {
                lofd::EffectRelativeLocationDef * effectLocationDef = dynamic_cast<lofd::EffectRelativeLocationDef *>(this->skillEffectData->skillEffectDef->locationDef);
                cocos2d::Point actorContainerPoint(effectMovementTrackData->actorData->actorContainer->getPositionX() + effectLocationDef->relativePoint->x
                                                   , effectMovementTrackData->actorData->actorContainer->getPositionY() + effectLocationDef->relativePoint->y);
                float gapX = fabs(this->skillEffectData->effectViewData->entry->getPositionX() - actorContainerPoint.x);
                float gapY = fabs(this->skillEffectData->effectViewData->entry->getPositionY() - actorContainerPoint.y);
                lofd::EffectMovementTrackData * movementTrackData = dynamic_cast<lofd::EffectMovementTrackData *>(this->skillEffectData->effectMovementData);
                lofd::EffectMovementTrackDef * movementTrackDef = dynamic_cast<lofd::EffectMovementTrackDef *>(movementTrackData->movementDef);
                //cocos2d::log("gapX=%f, gapY=%f", gapX, gapY);
                if (gapX <= movementTrackDef->speed &&  gapY <= movementTrackDef->speed)
                {
                    this->isCompleteTag = true;
                    this->skillData->onSkillEffectContextDataComplete(this);
                }
            }
            else
            {
                
            }
        }
    }
}

/*****************************************************/

/*****************SkillData***************************/
void SkillData::startLifeCycle()
{
    this->unscheduleUpdate();
    this->onExit();
    this->onEnter();
    this->scheduleUpdate();
}

void SkillData::endLifeCycle()
{
    this->unscheduleUpdate();
    this->onExit();
}

void SkillData::onSkillEffectContextDataComplete(lofd::SkillEffectContextData *skillEffectContextDataValue)
{
    skillEffectContextDataValue->endLifeCycle();
    
    bool isTotalComplete = true;
    for (int i = 0; i < this->effectContextDatas.size(); i ++)
    {
        lofd::SkillEffectContextData * effectContextData = effectContextDatas.at(i);
        if (!effectContextData->isComplete)
        {
            isTotalComplete = false;
        }
        if (effectContextData->isRunning == false)
        {
            if (effectContextData->effectLifeCycleData->startLifeCycleData->effectStartLifeCycleDef->type == EFFECT_START_LIFE_CYCLE_DEPEND)
            {
                lofd::EffectStartLifeCycleDependDef * effectStartLifeCycleDependDef = static_cast<lofd::EffectStartLifeCycleDependDef *>(effectContextData->effectLifeCycleData->startLifeCycleData->effectStartLifeCycleDef);
                if (effectStartLifeCycleDependDef->dependId == skillEffectContextDataValue->skillEffectContextDef->skillEffectDef->effectId)
                {
                    effectContextData->startLifeCycle();
                }
            }
        }
    }
    if (isTotalComplete) {
        //this->endLifeCycle();
        this->skillStateData->actorData->skillTotalEffectEnd(this);
    }
}

void SkillData::update(float dt)
{
    for (int i = 0; i < this->effectContextDatas.size(); i ++)
    {
        lofd::SkillEffectContextData * effectContextData = effectContextDatas.at(i);
        if (!effectContextData->isRunning && !effectContextData->isComplete && effectContextData->effectLifeCycleData->startLifeCycleData->effectStartLifeCycleDef->type == EFFECT_START_LIFE_CYCLE_INTERVAL)
        {
            lofd::EffectStartLifeCycleIntervalDef * effectStartLifeCycleIntervalDef = static_cast<lofd::EffectStartLifeCycleIntervalDef *>(effectContextData->effectLifeCycleData->startLifeCycleData->effectStartLifeCycleDef);
            if (this->currentLifeCycle >= effectStartLifeCycleIntervalDef->startInterval) {
                effectContextData->startLifeCycle();
            }
        }
    }
    
    this->currentLifeCycle += dt;
}

void SkillData::setActor(lofd::ActorData * actorDataValue)
{
    if (actor != nullptr)
    {
        actor->release();
    }
    actor = actorDataValue;
    actor->retain();
    //cocos2d::log("actor reference count %d", actor->getReferenceCount());
}

void SkillData::setTargetActor(lofd::ActorData * targetActorDataValue)
{
    if (targetActor != nullptr)
    {
        targetActor->release();
    }
    targetActor = targetActorDataValue;
    targetActor->retain();
    //cocos2d::log("actor reference count %d", actor->getReferenceCount());
}

/******************************************************/
/******************SkillCoolDownData*******************/
void SkillCoolDownData::startCoolDown()
{
    if (this->skillDef->coolDown <= 0) return;
    this->onEnter();
//    this->schedule(cocos2d::SEL_SCHEDULE(&SkillCoolDownData::update), this->skillDef->coolDown);
    this->scheduleOnce(cocos2d::SEL_SCHEDULE(&SkillCoolDownData::update), this->skillDef->coolDown);
    //this->currentTime = 0;
    this->isCoolDown = true;
}

void SkillCoolDownData::update(float dt)
{
    //cocos2d::log("================SkillCoolDownData====%f", dt);
    this->isCoolDown = false;
    this->onExit();
    (this->actorDataCallBackPoint->*skillCoolDownCallBack)(this->skillDef);
//    this->currentTime += dt;
//    
//    if (this->currentTime >= this->skillDef->coolDown)
//    {
//        
//    }
}

//void SkillCoolDownData::coolDownComplete()
//{
//    this->isCoolDown = false;
//    
//    this->unscheduleUpdate();
//    this->onExit();
//    
//    (this->actorDataCallBackPoint->*skillCoolDownCallBack)(this->skillDef);
//}
/******************************************************/
NS_LOFD_END;