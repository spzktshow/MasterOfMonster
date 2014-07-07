//
//  LOFDActor.cpp
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-8.
//
//

#include "LOFDActor.h"
#include "LOFDConfigManager.h"
#include "TypeConver.h"
#include "FileFormat.h"
#include "LOFDActorState.h"
#include "MSVectorMap.h"
#include "LOFDSkillData.h"
#include "SkillEvent.h"
#include "ActorEvent.h"

NS_LOFD_BEGIN

/********ActorData*********/
void ActorData::runAction(std::string actionTag)
{
    lofd::ActorStateDef * actorStateDef = this->actorStateGroupDef->getActorStateMatchByTag(actionTag);
    //cocos2d::log("%s", actionTag.c_str());
    actorEntry->setAnchorPoint(cocos2d::Point(actorStateDef->relativePoint->x, actorStateDef->relativePoint->y));
    cocos2d::Animate * animate = animationDic.at(actorStateDef->tag);
    cocos2d::FiniteTimeAction * actionDone = cocos2d::CallFunc::create(CC_CALLBACK_0(ActorData::runActionComplete, this));
    if (actorStateDef->round > 0)
    {
        cocos2d::Sequence *sq = cocos2d::Sequence::create(animate, actionDone, NULL);
        this->actorEntry->runAction(sq);
    }
    else
    {
        cocos2d::RepeatForever * repeatf;
        repeatf = cocos2d::RepeatForever::create(animate);
        this->actorEntry->runAction(repeatf);
    }
}

void ActorData::runActionComplete()
{
    if (this->isDead) return;
    //
    std::string cacheState = this->stateContext->currentState->itemName;
    lofd::ActorStateData * actorStateData = (lofd::ActorStateData*)this->stateContext->currentState;
    std::string cacheOperationType = actorStateData->operationType;
    
    constellation::BehaviorEvent * event = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_CANCEL_CURRENT);
    lofd::ActorBehaviorData * data = new lofd::ActorBehaviorData;
    data->actorData = this;
    event->behaviorData = data;
    this->actorBehavior->root->execute(event);
    delete data;
    delete event;
    
    if (cacheState == LOFD_ACTOR_STATE_ATK1)
    {
        constellation::BehaviorEvent * event = new constellation::BehaviorEvent(LOFD_BEHAVIOR_EVENT_ATK1_END);
        lofd::AIBehaviorDynamicData * actorAIBehaviorData = new lofd::AIBehaviorDynamicData();
        actorAIBehaviorData->actorData = this;
        actorAIBehaviorData->mapSceneData = mapScene->mapSceneData;
        actorAIBehaviorData->operationType = cacheOperationType;
        actorAIBehaviorData->mapScene = mapScene;
        event->behaviorData = actorAIBehaviorData;
        
        this->aiBehavior->root->execute(event);
        
        delete event;
        delete actorAIBehaviorData;
        event = nullptr;
        actorAIBehaviorData = nullptr;
    }
    else if (cacheState == LOFD_ACTOR_STATE_ATK2)
    {
        constellation::BehaviorEvent * event = new constellation::BehaviorEvent(LOFD_BEHAVIOR_EVENT_ATK2_END);
        lofd::AIBehaviorDynamicData * actorAIBehaviorData = new lofd::AIBehaviorDynamicData();
        actorAIBehaviorData->actorData = this;
        actorAIBehaviorData->mapSceneData = mapScene->mapSceneData;
        actorAIBehaviorData->operationType = cacheOperationType;
        actorAIBehaviorData->mapScene = mapScene;
        event->behaviorData = actorAIBehaviorData;
        
        this->aiBehavior->root->execute(event);
        
        delete event;
        delete actorAIBehaviorData;
        event = nullptr;
        actorAIBehaviorData = nullptr;
    }
    else if (cacheState == LOFD_ACTOR_STATE_SKILL1)
    {
        constellation::BehaviorEvent * event = new constellation::BehaviorEvent(LOFD_BEHAVIOR_EVENT_SKILL1_END);
        lofd::AIBehaviorDynamicData * actorAIBehaviorData = new lofd::AIBehaviorDynamicData();
        actorAIBehaviorData->actorData = this;
        actorAIBehaviorData->mapSceneData = mapScene->mapSceneData;
        actorAIBehaviorData->operationType = cacheOperationType;
        actorAIBehaviorData->mapScene = mapScene;
        event->behaviorData = actorAIBehaviorData;
        
        this->aiBehavior->root->execute(event);
        
        delete event;
        delete actorAIBehaviorData;
        event = nullptr;
        actorAIBehaviorData = nullptr;
    }
}

void ActorData::skill(lofd::SkillData * skillDataValue)
{
    lofd::SkillData * skillData = skillDataValue;
    lofd::SkillDef * skillDef = skillData->skillDef;
    lofd::SkillCoolDownData * skillCoolDownData = this->getSkillCoolDownData(skillDef->skillId);
    if (skillCoolDownData->isCoolDown) return;
    
    skillCoolDownData->startCoolDown();
    
    if (this->isCurrentFocus)
    {
        cocos2d::EventDispatcher * eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
        cocos2d::EventCustom event(SKILL_COOLDOWN_START);
        lofd::SkillEventParam * skillEventParam = new lofd::SkillEventParam();
        skillEventParam->skillDef = skillDef;
        skillEventParam->actorTag = this->tagId;
        event.setUserData(skillEventParam);
        eventDispatcher->dispatchEvent(&event);
    }

    this->changeSkillFocus(this->actorDef->skillIds.at(0));
    skillData->retain();
    skillData->startLifeCycle();
}

void ActorData::skillTotalEffectEnd(lofd::SkillData * skillDataValue)
{
    lofd::SkillData * skillData = skillDataValue;
    skillData->endLifeCycle();
    skillData->release();
}

void ActorData::move(cocos2d::Point endPoint, moonsugar::VectorMap *vectorMapValue)
{
    moonsugar::VectorPoint * startVPoint = new moonsugar::VectorPoint(new cocos2d::Point(this->actorContainer->getPosition()));
    moonsugar::VectorPoint * endVPoint = new moonsugar::VectorPoint(new cocos2d::Point(endPoint));
    cocos2d::Vector<moonsugar::VectorPoint *> paths = moonsugar::VectorUtils::findPath(startVPoint, endVPoint, vectorMapValue);
    long length = paths.size();
    if (length > 0)
    {
        //direct
        bool isFlip = moonsugar::VectorUtils::calculateRowVector(startVPoint->point, endVPoint->point);
        this->actorEntry->setFlippedX(!isFlip);
        cocos2d::Vector<cocos2d::FiniteTimeAction *> actions;
        float distance;
        for (int i = 0; i < length; i ++) {
            if (i < paths.size() - 1)
            {
                distance = moonsugar::VectorUtils::calculateDistance(paths.at(i), paths.at(i + 1));
                cocos2d::MoveTo * moveBy = cocos2d::MoveTo::create(distance / actorDef->speed/ 60, *paths.at(i + 1)->point);
                actions.pushBack(moveBy);
            }
        }
        cocos2d::FiniteTimeAction * moveDone = cocos2d::CallFunc::create(CC_CALLBACK_0(ActorData::moveComplete, this));
        actions.pushBack(moveDone);
        cocos2d::Sequence * seq = cocos2d::Sequence::create(actions);
        this->actorContainer->runAction(seq);
    }
}

void ActorData::moveComplete()
{
    lofd::ActorStateData * actorStateData = (lofd::ActorStateData *)stateContext->currentState;
    std::string cacheOperationType = actorStateData->operationType;
    constellation::BehaviorEvent * behaviorEvent = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_CANCEL_MOVE);
    lofd::ActorBehaviorData * actorBehaviorData = new lofd::ActorBehaviorData();
    actorBehaviorData->actorData = this;
    behaviorEvent->behaviorData = actorBehaviorData;
    this->actorBehavior->root->execute(behaviorEvent);
    delete actorBehaviorData;
    delete behaviorEvent;
    actorBehaviorData = nullptr;
    behaviorEvent = nullptr;
    
    behaviorEvent = new constellation::BehaviorEvent(LOFD_BEHAVIOR_EVENT_MOVE_END);
    lofd::AIBehaviorDynamicData * actorAIBehaviorData = new lofd::AIBehaviorDynamicData();
    actorAIBehaviorData->actorData = this;
    actorAIBehaviorData->mapSceneData = mapScene->mapSceneData;
    actorAIBehaviorData->operationType = cacheOperationType;
    behaviorEvent->behaviorData = actorAIBehaviorData;
    this->aiBehavior->root->execute(behaviorEvent);
    delete behaviorEvent;
    delete actorAIBehaviorData;
    behaviorEvent = nullptr;
    actorAIBehaviorData = nullptr;
}

void ActorData::track()
{
    //
    if (this->isTrack) return;
    
    lofd::SkillConfig * skillConfig = lofd::ConfigManager::getInstance()->skillConfig;
    lofd::SkillDef * skillDef = skillConfig->getSkillDefById(this->currentFocusSkill);
    
    cocos2d::Point * leftPoint = new cocos2d::Point(this->currentFocus->actorContainer->getPositionX(), this->currentFocus->actorContainer->getPositionY());
    cocos2d::Point * rightPoint = new cocos2d::Point(this->actorContainer->getPositionX(), this->actorContainer->getPositionY());
    
    float distance = moonsugar::VectorUtils::calculateDistance(leftPoint, rightPoint);
    bool isFlip = moonsugar::VectorUtils::calculateRowVector(rightPoint, leftPoint);
    this->actorEntry->setFlippedX(!isFlip);
    
    if (skillDef->skillRangeDef->type == SKILL_RANGE_TYPE_R)
    {
        lofd::SkillRangeRDef * skillRangeRDef = dynamic_cast<lofd::SkillRangeRDef *>(skillDef->skillRangeDef);
        if (distance <= skillRangeRDef->r)
        {
            trackComplete();
            return;
        }
    }
    
    this->isTrack = true;
    this->onEnter();
    this->scheduleUpdate();
}

void ActorData::trackComplete()
{
    this->unscheduleUpdate();
    this->isTrack = false;
    this->onExit();
    
    lofd::ActorStateData * actorStateData = dynamic_cast<lofd::ActorStateData *>(this->stateContext->currentState);
    std::string cacheOperationType = actorStateData->operationType;
    
    constellation::BehaviorEvent * behaviorEvent = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_CANCEL_TRACK);
    lofd::ActorBehaviorData * actorBehaviorData = new lofd::ActorBehaviorData();
    actorBehaviorData->actorData = this;
    actorBehaviorData->operationType = cacheOperationType;
    behaviorEvent->behaviorData = actorBehaviorData;
    this->actorBehavior->root->execute(behaviorEvent);
    delete actorBehaviorData;
    delete behaviorEvent;
    
    behaviorEvent = new constellation::BehaviorEvent(LOFD_BEHAVIOR_EVENT_TRACK_END);
    lofd::AIBehaviorDynamicData * aiBehaviorDynamicData = new lofd::AIBehaviorDynamicData;
    aiBehaviorDynamicData->actorData = this;
    aiBehaviorDynamicData->operationType = cacheOperationType;
    behaviorEvent->behaviorData = aiBehaviorDynamicData;
    this->aiBehavior->root->execute(behaviorEvent);
    delete behaviorEvent;
    delete aiBehaviorDynamicData;
}

void ActorData::target()
{
    if (this->isTarget) return;
    this->isTarget = true;
    this->onEnter();
    this->scheduleUpdate();
}

void ActorData::dead()
{
    this->isDead = true;
//    this->deadTime = 0;
//    this->onEnter();
//    this->scheduleUpdate();
    
    cocos2d::EventCustom event(ACTOR_EVENT_DEAD);
    event.setUserData(this);
    cocos2d::EventDispatcher * eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    eventDispatcher->dispatchEvent(&event);
    
    cocos2d::Animate * animate = animationDic.at(LOFD_ACTOR_STATE_DEAD_EFFECT);
    cocos2d::FiniteTimeAction * actionDone = cocos2d::CallFunc::create(CC_CALLBACK_0(ActorData::deadComplete, this));
    cocos2d::Sequence *sq = cocos2d::Sequence::create(animate, actionDone, NULL);
    auto deadSprite = cocos2d::Sprite::create();
    deadSprite->setAnchorPoint(this->actorEntry->getAnchorPoint());
    this->actorContainer->addChild(deadSprite);
    deadSprite->runAction(sq);
}

void ActorData::deadComplete()
{
//    this->unscheduleUpdate();
//    this->onExit();
    
    cocos2d::EventCustom event(ACTOR_EVENT_DEAD_COMPLETE);
    event.setUserData(this);
    cocos2d::EventDispatcher * eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    eventDispatcher->dispatchEvent(&event);
}

void ActorData::targetComplete()
{
    this->unscheduleUpdate();
    this->isTarget = false;
    this->onExit();
    
    lofd::ActorStateData * actorStateData = (lofd::ActorStateData*)this->stateContext->currentState;
    std::string cacheOperationType = actorStateData->operationType;
    
    constellation::BehaviorEvent * behaviorEvent = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_CANCEL_TARGET);
    lofd::ActorBehaviorData * actorBehaviorData = new lofd::ActorBehaviorData();
    actorBehaviorData->actorData = this;
    actorBehaviorData->operationType = cacheOperationType;
    behaviorEvent->behaviorData = actorBehaviorData;
    this->actorBehavior->root->execute(behaviorEvent);
    delete actorBehaviorData;
    delete behaviorEvent;
    actorBehaviorData = nullptr;
    behaviorEvent = nullptr;
    
    behaviorEvent = new constellation::BehaviorEvent(LOFD_BEHAVIOR_EVENT_TARGET_END);
    lofd::AIBehaviorDynamicData * aiBehaviorDynamicData = new lofd::AIBehaviorDynamicData;
    aiBehaviorDynamicData->actorData = this;
    aiBehaviorDynamicData->operationType = cacheOperationType;
    behaviorEvent->behaviorData = aiBehaviorDynamicData;
    this->aiBehavior->root->execute(behaviorEvent);
    delete behaviorEvent;
    delete aiBehaviorDynamicData;
}

cocos2d::Rect * ActorData::getBound()
{
    cocos2d::Point aPoint = this->actorContainer->getAnchorPoint();
    lofd::ActorStateDef * actorStateDef = this->actorStateGroupDef->getActorStateMatchByTag(this->stateContext->currentState->itemName);
    
    float minX = this->actorContainer->getPositionX() - (aPoint.x * actorStateDef->rect->size.width);
    float minY = this->actorContainer->getPositionY() - (aPoint.y * actorStateDef->rect->size.height);
    float maxX = this->actorContainer->getPositionX() + ((1 - aPoint.x) * actorStateDef->rect->size.width);
    float maxY = this->actorContainer->getPositionY() + ((1 - aPoint.y) * actorStateDef->rect->size.height);
    
    //cocos2d::log("minx %f, miny %f, maxX %f, maxY %f", minX, minY, maxX, maxY);
    float width = maxX - minX;
    float height = maxY - minY;
    cocos2d::Rect * rect = new cocos2d::Rect(minX, minY, width, height);
    return rect;
}

void ActorData::update(float dt)
{
    if (this->isTrack) this->trackUpdate();
    if (this->isTarget) this->targetUpdate();
    if (this->isDead) this->deadUpdate(dt);
}

void ActorData::targetUpdate()
{
    if (this->tagId == 1)
    {
        cocos2d::log("fdafd");
    }
    lofd::SkillConfig * skillConfig = lofd::ConfigManager::getInstance()->skillConfig;
    lofd::SkillDef * skillDef = skillConfig->getSkillDefById(this->currentFocusSkill);
    
    float dy = this->targetPoint.y - this->actorContainer->getPositionY();
    float dx = this->targetPoint.x - this->actorContainer->getPositionX();
    cocos2d::Point * leftPoint = new cocos2d::Point(this->actorContainer->getPositionX(), this->actorContainer->getPositionY());
    cocos2d::Point * rightPoint = new cocos2d::Point(this->targetPoint.x, this->targetPoint.y);
    float distance = moonsugar::VectorUtils::calculateDistance(leftPoint, rightPoint);
    bool isFlip = moonsugar::VectorUtils::calculateRowVector(leftPoint, rightPoint);
    delete leftPoint;
    delete rightPoint;
    this->actorEntry->setFlippedX(!isFlip);
    if (skillDef->skillRangeDef->type == SKILL_RANGE_TYPE_R)
    {
        lofd::SkillRangeRDef * skillRangeRDef = dynamic_cast<lofd::SkillRangeRDef *>(skillDef->skillRangeDef);
        if (distance <= skillRangeRDef->r)
        {
            targetComplete();
            return;
        }
    }
    
    double angle = atan2(dy, dx);
    double vx = cos(angle) * this->actorDef->speed;
    double vy = sin(angle) * this->actorDef->speed;
    cocos2d::Point p(this->actorContainer->getPositionX() + vx, this->actorContainer->getPositionY() + vy);
    this->actorContainer->setPosition(p);
}

void ActorData::trackUpdate()
{
    if (this->tagId == 1)
    {
        cocos2d::log("fdafds");
    }
    lofd::SkillConfig * skillConfig = lofd::ConfigManager::getInstance()->skillConfig;
    lofd::SkillDef * skillDef = skillConfig->getSkillDefById(this->currentFocusSkill);
    
    float dy = this->currentFocus->actorContainer->getPositionY() - this->actorContainer->getPositionY();
    float dx = this->currentFocus->actorContainer->getPositionX() - this->actorContainer->getPositionX();
    cocos2d::Point * leftPoint = new cocos2d::Point(this->currentFocus->actorContainer->getPositionX(), this->currentFocus->actorContainer->getPositionY());
    cocos2d::Point * rightPoint = new cocos2d::Point(this->actorContainer->getPositionX(), this->actorContainer->getPositionY());
    
    float distance = moonsugar::VectorUtils::calculateDistance(leftPoint, rightPoint);
    bool isFlip = moonsugar::VectorUtils::calculateRowVector(rightPoint, leftPoint);
    delete leftPoint;
    delete rightPoint;
    this->actorEntry->setFlippedX(!isFlip);
    
    if (skillDef->skillRangeDef->type == SKILL_RANGE_TYPE_R)
    {
        lofd::SkillRangeRDef * skillRangeRDef = dynamic_cast<lofd::SkillRangeRDef *>(skillDef->skillRangeDef);
        if (distance <= skillRangeRDef->r)
        {
            trackComplete();
            return;
        }
    }
    
    double angle = atan2(dy, dx);
    double vx = cos(angle) * this->actorDef->speed;
    double vy = sin(angle) * this->actorDef->speed;
    cocos2d::Point p(this->actorContainer->getPositionX() + vx, this->actorContainer->getPositionY() + vy);
    this->actorContainer->setPosition(p);
}

void ActorData::deadUpdate(float dt)
{
    this->deadTime += dt;
    
    if (this->deadTime >= 5)
    {
        this->deadComplete();
    }
}

int ActorData::getActorFromCurrentRangeActors(int actorTagId)
{
    long n = this->currentRangeActors.size();
    for (int i = 0; i < n; i ++) {
        lofd::ActorData * actorData = this->currentRangeActors.at(i);
        if (actorData->tagId == actorTagId) return i;
    }
    return -1;
}

void ActorData::stopActorAllActions()
{
    this->isTrack = false;
    this->isTarget = false;
    this->unscheduleUpdate();
    this->onExit();
}

void ActorData::changeSkillFocus(int skillId)
{
    this->currentFocusSkill = skillId;
    lofd::SkillConfig * skillConfig = lofd::ConfigManager::getInstance()->skillConfig;
    this->currentSkillDef = skillConfig->getSkillDefById(this->currentFocusSkill);
}

void ActorData::addGlobalListeners()
{
    this->skillListener = cocos2d::EventListenerCustom::create(SKILL_BUTTON_TOUCHED, CC_CALLBACK_1(ActorData::onSkillTouchedHandler, this));
    cocos2d::EventDispatcher * eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithFixedPriority(this->skillListener, 1);
}

void ActorData::removeGlobalListeners()
{
    cocos2d::EventDispatcher * eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    eventDispatcher->removeEventListener(this->skillListener);
}

void ActorData::onSkillTouchedHandler(cocos2d::EventCustom * event)
{
    lofd::SkillDef * skillDef = static_cast<lofd::SkillDef *>(event->getUserData());
    this->changeSkillFocus(skillDef->skillId);
}

void ActorData::onSkillCoolDownComplete(lofd::SkillDef *skillDefValue)
{
    //人物的某个技能cd完成
    //cocos2d::log("tag:%d, coolDown:%d", this->tagId, skillDefValue->skillId);
    cocos2d::EventDispatcher * eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    cocos2d::EventCustom event(SKILL_COOLDOWN_END);
    lofd::SkillEventParam * skillEventParam = new lofd::SkillEventParam();
    skillEventParam->skillDef = skillDefValue;
    skillEventParam->actorTag = this->tagId;
    event.setUserData(skillEventParam);
    eventDispatcher->dispatchEvent(&event);
}

lofd::SkillCoolDownData * ActorData::getSkillCoolDownData(int skillId)
{
    long n = this->skillCoolDownDatas.size();
    for (int i = 0; i < n; i ++) {
        lofd::SkillCoolDownData * skillCoolDownData = dynamic_cast<lofd::SkillCoolDownData *>(skillCoolDownDatas.at(i));
        if (skillCoolDownData->skillDef->skillId == skillId)
        {
            return skillCoolDownData;
        }
    }
    return nullptr;
}

void ActorData::disposeSkillCoolDownDatas()
{
    long n = this->skillCoolDownDatas.size();
    for (int i = 0; i < n; i ++) {
        lofd::SkillCoolDownData * skillCoolDownData = dynamic_cast<lofd::SkillCoolDownData *>(this->skillCoolDownDatas.at(i));
        skillCoolDownData->onExit();
    }
    skillCoolDownDatas.clear();
}

void ActorData::addtionHp(float value)
{
    if (this->isDead) return;
    
    this->actorPropertyData->currentHp += value;
    int percent = actorPropertyData->currentHp / actorPropertyData->hp * 100;
    bloodTimer->setPercentage(percent);
    
    if (this->actorPropertyData->currentHp <= 0)
    {
        //this->dead();
        constellation::BehaviorEvent * event = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_DEAD);
        lofd::ActorBehaviorData * data = new lofd::ActorBehaviorData();
        data->actorData = this;
        event->behaviorData = data;
        this->actorBehavior->root->execute(event);
        delete event;
        delete data;
    }
}

void ActorData::reduceHp(float value)
{
    if (this->isDead) return;
    
    this->actorPropertyData->currentHp -= value;
    int percent = actorPropertyData->currentHp / actorPropertyData->hp * 100;
    bloodTimer->setPercentage(percent);
    
    cocos2d::EventCustom reduceHp(ACTOR_EVENT_HURT);
    lofd::ActorHurtEventParam * actorHurtEventParam = new lofd::ActorHurtEventParam();
    actorHurtEventParam->hurt = value;
    actorHurtEventParam->location = this->actorContainer->getPosition();
    actorHurtEventParam->location.y += 300;
    reduceHp.setUserData(actorHurtEventParam);
    cocos2d::EventDispatcher * eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    eventDispatcher->dispatchEvent(&reduceHp);
    
    if (this->actorPropertyData->currentHp <= 0)
    {
        //this->dead();
        constellation::BehaviorEvent * event = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_DEAD);
        lofd::ActorBehaviorData * data = new lofd::ActorBehaviorData();
        data->actorData = this;
        event->behaviorData = data;
        int result = this->actorBehavior->root->execute(event);
        if (result != 1)
        {
            cocos2d::log("死亡不成功");
        }
        delete event;
        delete data;
    }
}

void ActorData::deleteCurrentRangeActors(lofd::ActorData * actorDataValue)
{
    long n = this->currentRangeActors.size();
    std::vector<int> indexs;
    for (int i = 0; i < n; i ++) {
        if (this->currentRangeActors.at(i)->tagId == actorDataValue->tagId)
        {
            indexs.push_back(i);
        }
    }
    n = indexs.size();
    for (int j = 0; j < n; j ++) {
        this->currentRangeActors.erase(indexs.at(j));
    }
}
/*************ActorControllerUtils**********/
static int tagIdIndex = 0;

void ActorControllerUtils::resetActorIdIndex()
{
    tagIdIndex = 0;
}

ActorData * ActorControllerUtils::createActorDataById(int actorId)
{
    tagIdIndex ++;
    //actorData
    ActorData * actorData = ActorData::create();
    actorData->actorId = actorId;
    actorData->tagId = tagIdIndex;
    //stateData
    //lofd::ActorStateData * stateData = new lofd::ActorStateData(LOFD_ACTOR_STATE_IDLE, LOFD_ACTOR_STATE_OPERATION_TYPE_AI);
    lofd::ActorStateData * stateData = lofd::ActorStateData::create(LOFD_ACTOR_STATE_IDLE, LOFD_ACTOR_STATE_OPERATION_TYPE_AI);
    actorData->stateContext->insertStateData(stateData);
    //actor state config
    lofd::ActorConfig * actorConfig = ConfigManager::getInstance()->actorConfig;
    lofd::ActorDef * actorDef = actorConfig->getActorDefById(actorId);
    actorData->actorDef = actorDef;
    lofd::ActorStateConfig * actorStateConfig = ConfigManager::getInstance()->actorStateConfig;
    lofd::ActorStateGroupDef * groupDef = actorStateConfig->getActorStateGroupDefByStateId(actorDef->stateId);
    actorData->actorStateGroupDef = groupDef;
    //actor behavior config
    lofd::ActorActionConfig * actorActionConfig = ConfigManager::getInstance()->actorActionConfig;
    lofd::ActorBehavior * actorBehavior = actorActionConfig->getActorBehaviorById(actorDef->behaviorId);
    //camp config
    lofd::CampConfig * campConfig = ConfigManager::getInstance()->campConfig;
    lofd::CampDef * campDef = campConfig->getCampDefById(actorDef->campId);
    actorData->campContext = campDef;
    //ai config;
    lofd::AIConfig * aiConfig = ConfigManager::getInstance()->aiConfig;
    lofd::AIBehavior * aiBehaviorDef = aiConfig->getAIBehaviorById(actorDef->aiId);
    actorData->aiBehavior = aiBehaviorDef;
    //ai state data
    lofd::ActorStateData * aiStateData = lofd::ActorStateData::create(LOFD_AI_STATE_AI_IDLE, LOFD_ACTOR_STATE_OPERATION_TYPE_AI);
    actorData->aiStateContext->insertStateData(aiStateData);
    
    cocos2d::Map<std::string, cocos2d::Animate *> animations;
    long n = groupDef->states.size();
    int i = 0;
    for (i = 0; i < n; i ++) {
        lofd::ActorStateDef * actorStateDef = groupDef->states.at(i);
        cocos2d::Vector<cocos2d::SpriteFrame *> frames;
        cocos2d::Animation * animation;
        for (int j = 0; j < actorStateDef->frames; j ++) {
            std::string name = moonsugar::TypeConver::intToString(groupDef->stateId);
            name += actorStateDef->tag;
            name += FILE_NAME_CONNECT;
            name += moonsugar::TypeConver::filterStr(j, 4);
            name += FILE_PNG;
            cocos2d::SpriteFrame * sf = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
            frames.pushBack(sf);
        }
        animation = cocos2d::Animation::createWithSpriteFrames(frames, ACTOR_ANIMATION_GAP_TIME);
        cocos2d::Animate * animate = cocos2d::Animate::create(animation);
        animations.insert(actorStateDef->tag, animate);
    }
    cocos2d::Vector<cocos2d::SpriteFrame *> frames;
    cocos2d::Animation * animation;
    for (int j = 0; j < 16; j ++) {
        std::string name = "dead";
        name += FILE_NAME_CONNECT;
        name += moonsugar::TypeConver::filterStr(j, 4);
        name += FILE_PNG;
        cocos2d::SpriteFrame * sf = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
        frames.pushBack(sf);
    }
    animation = cocos2d::Animation::createWithSpriteFrames(frames, ACTOR_ANIMATION_GAP_TIME);
    cocos2d::Animate * animate = cocos2d::Animate::create(animation);
    animations.insert(LOFD_ACTOR_STATE_DEAD_EFFECT, animate);
    
    cocos2d::Sprite * actorEntry = cocos2d::Sprite::create();
    cocos2d::Sprite * actorContainer = cocos2d::Sprite::create();
    actorContainer->setAnchorPoint(cocos2d::Point(.5, 0));
    actorContainer->addChild(actorEntry);
    //shadow
    cocos2d::SpriteFrame * sf = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("shadow.png");
    cocos2d::Sprite * shadow = cocos2d::Sprite::createWithSpriteFrame(sf);
    actorContainer->addChild(shadow);
    //==============skill==============//
    lofd::SkillConfig * skillConfig = lofd::ConfigManager::getInstance()->skillConfig;
    n = actorData->actorDef->skillIds.size();
    for (i = 0; i < n; i ++)
    {
//        lofd::ActorRandomSkillGroupDef * actorRandomSkillGroupDef = actorData->actorDef->skills.at(i);
        int skillId = actorData->actorDef->skillIds.at(i);
        if (i == 0) actorData->changeSkillFocus(skillId);
        lofd::SkillDef * skillDef = skillConfig->getSkillDefById(skillId);
        lofd::SkillCoolDownData * skillCoolDownData = SkillCoolDownData::create();
        skillCoolDownData->skillDef = skillDef;
//        skillCoolDownData->actorRandSkillGroupDef = actorRandomSkillGroupDef;
        skillCoolDownData->actorDataCallBackPoint = actorData;
        skillCoolDownData->skillCoolDownCallBack = cocos2d::SEL_CallFuncO(&ActorData::onSkillCoolDownComplete);
        actorData->skillCoolDownDatas.pushBack(skillCoolDownData);
    }
    //==============test===============//
    lofd::ActorPropertyData * actorPropertyData = new lofd::ActorPropertyData;
    actorPropertyData->physicalAtt = 100;
    actorPropertyData->physicalDef = 100;
    actorPropertyData->magicAtt = 100;
    actorPropertyData->magicDef = 100;
    actorPropertyData->hit = 50;
    actorPropertyData->physicalImmuneInjury = 10;
    actorPropertyData->magicImmuneInjury = 10;
    actorPropertyData->hp = 1000;
    actorPropertyData->currentHp = 1000;
    //blood
    cocos2d::SpriteFrame * bloodSF = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("blood.png");
    cocos2d::SpriteFrame * bloodBgSF = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("bloodBg.png");
    cocos2d::Sprite * blood = cocos2d::Sprite::createWithSpriteFrame(bloodSF);
    cocos2d::Sprite * bloodBg = cocos2d::Sprite::createWithSpriteFrame(bloodBgSF);
    cocos2d::ProgressTimer * bloodTimer = cocos2d::ProgressTimer::create(blood);
    actorContainer->addChild(bloodBg);
    bloodBg->setPositionY(280);
    bloodTimer->setType(cocos2d::ProgressTimer::Type::BAR);
    bloodTimer->setMidpoint(cocos2d::Point(0, 0.5));
    bloodTimer->setBarChangeRate(cocos2d::Point(1, 0));
    int percent = actorPropertyData->currentHp / actorPropertyData->hp * 100;
    bloodTimer->setPercentage(percent);
    actorContainer->addChild(bloodTimer);
    bloodTimer->setPositionY(280);
    actorData->bloodTimer = bloodTimer;
    
    auto label = cocos2d::LabelTTF::create(moonsugar::TypeConver::intToString(actorData->tagId), "Arial", 24);
    label->setPositionY(-20);

    actorData->actorPropertyData = actorPropertyData;
    actorData->animationDic = animations;
    actorData->actorEntry = actorEntry;
    actorData->actorContainer = actorContainer;
    
        actorData->actorContainer->addChild(label);
    actorData->actorBehavior = actorBehavior;
    
    actorEntry->retain();
    actorContainer->retain();
    bloodTimer->retain();
    return actorData;
}

NS_LOFD_END;