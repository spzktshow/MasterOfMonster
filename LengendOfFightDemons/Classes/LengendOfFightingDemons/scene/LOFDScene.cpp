//
//  LOFDScene.cpp
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-8.
//
//

#include "LOFDScene.h"
#include <math.h>
#include "LOFDConfigManager.h"
#include "LOFDSkillData.h"
#include "SkillEvent.h"
#include "LOFDActorAi.h"
#include "LOFDActorState.h"
#include "ActorEvent.h"
#include "TypeConver.h"
#include "FileFormat.h"
#include "LOFDActorBehavior.h"

NS_LOFD_BEGIN

int testTagId = -1;

/***************HurtCount****************/
void HurtCount::setCount(int countValue, cocos2d::Point location)
{
    this->_count = countValue;
    this->_location = location;
    
    std::string countStr = moonsugar::TypeConver::intToString(this->_count);
    int n = countStr.size();
    for (int i = 0; i < n; i ++) {
        char countChar = countStr.at(i);
        int count = moonsugar::TypeConver::charToInt(countChar);
        std::string hurtName = FILE_HURT_FILE;
        hurtName += FILE_NAME_CONNECT;
        hurtName += moonsugar::TypeConver::filterStr(count, FILE_NUM_BIT);
        hurtName += FILE_PNG;
        cocos2d::SpriteFrame * sf = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(hurtName);
        cocos2d::Sprite * sp = cocos2d::Sprite::createWithSpriteFrame(sf);
        sp->setPositionX(sf->getRect().size.width * i);
        this->addChild(sp);
        //sp->setCascadeColorEnabled(true);
    }
    this->setPosition(this->_location.x, this->_location.y);
    this->setScale(0.4);
}

void HurtCount::startAnimation()
{
    auto move = cocos2d::MoveBy::create(0.7, cocos2d::Point(0, 100));
    auto move_ease_inout = cocos2d::EaseInOut::create(move->clone(), 0.65f);
    auto scale = cocos2d::ScaleTo::create(.3, 1);
    cocos2d::FiniteTimeAction * moveDone = cocos2d::CallFunc::create(CC_CALLBACK_0(HurtCount::endAnimation, this));
    auto delay = cocos2d::DelayTime::create(0.25f);
    auto seq = cocos2d::Sequence::create(scale, move_ease_inout, delay, moveDone, NULL);
    auto alphaTrans = cocos2d::FadeOut::create(1);
    cocos2d::Vector<cocos2d::Node *> children = this->getChildren();
    int n = children.size();
    for (int i = 0; i < n; i ++) {
        cocos2d::Node * node = children.at(i);
        node->runAction(alphaTrans->clone());
    }
    this->runAction(seq);
}

void HurtCount::endAnimation()
{
    this->stopAllActions();
    if (this->getParent())
    {
        this->getParent()->removeChild(this);
    }
}
/****************SkillButton*****************/
void SkillButton::startCoolDown()
{
    if (!this->skillDef) return;
    cocos2d::ProgressTo * processTo = cocos2d::ProgressTo::create(this->skillDef->coolDown, 100);
    cocos2d::FiniteTimeAction * actionDone = cocos2d::CallFunc::create(CC_CALLBACK_0(SkillButton::coolDownComplete, this));
    cocos2d::Sequence * sq = cocos2d::Sequence::create(processTo, actionDone, NULL);
    progressTimer->runAction(sq);
    this->skillFrame->setOpacity(SKILL_BUTTON_ALPHA_HALF);
    this->skillLightFrame->setOpacity(SKILL_BUTTON_ALPHA_HALF);
    this->skillDarkFrame->setOpacity(SKILL_BUTTON_ALPHA_HALF);
    this->isCoolDown = true;
}

void SkillButton::coolDownComplete()
{
    this->isCoolDown = false;
    this->skillFrame->setOpacity(SKILL_BUTTON_ALPHA_MAX);
    this->skillLightFrame->setOpacity(SKILL_BUTTON_ALPHA_MAX);
    this->skillDarkFrame->setOpacity(SKILL_BUTTON_ALPHA_MAX);
    progressTimer->stopAllActions();
    progressTimer->setPercentage(100);
}

void SkillButton::createButton()
{
    skillFrame = cocos2d::Sprite::create("battleui_skillframe.png");
    skillLightFrame = cocos2d::Sprite::create("battleui_skillframe_dark.png");
    skillDarkFrame = cocos2d::Sprite::create("battleui_skillframe_light.png");
    this->addChild(skillLightFrame);
    progressTimer = cocos2d::ProgressTimer::create(skillDarkFrame);
    progressTimer->setType(cocos2d::ProgressTimer::Type::RADIAL);
    progressTimer->setPercentage(100);
    this->addChild(progressTimer);
    this->addChild(skillFrame);
    
    this->addListeners();
}

void SkillButton::destoryButton()
{
    this->removeListeners();
    touchListener->release();
    progressTimer->stopAllActions();
}

void SkillButton::addListeners()
{
    touchListener= cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(SkillButton::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(SkillButton::onTouchEnded, this);
    touchListener->retain();
    auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, skillFrame);
}

void SkillButton::removeListeners()
{
    auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    eventDispatcher->removeEventListener(touchListener);
}

bool SkillButton::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    cocos2d::log("skill button touch");
    cocos2d::Sprite * target = dynamic_cast<cocos2d::Sprite *>(event->getCurrentTarget());
    if (target != nullptr)
    {
        cocos2d::Point locationPoint = target->convertToNodeSpace(touch->getLocation());
        cocos2d::Size s = target->getContentSize();
        cocos2d::Rect rect(0, 0, s.width, s.height);
        if (rect.containsPoint(locationPoint))
        {
            this->isBeganInSkillButton = true;
        }
    }
    return true;
}

void SkillButton::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    cocos2d::log("skill button touch ended");
    cocos2d::Sprite * target = dynamic_cast<cocos2d::Sprite *>(event->getCurrentTarget());
    if (target != nullptr)
    {
        cocos2d::Point locationPoint = target->convertToNodeSpace(touch->getLocation());
        cocos2d::Size s = target->getContentSize();
        cocos2d::Rect rect(0, 0, s.width, s.height);
        if (rect.containsPoint(locationPoint))
        {
            if (this->isBeganInSkillButton)
            {
                this->isTouched = true;
                if (this->isCoolDown) return;
                //this->startCoolDown();
                this->isBeganInSkillButton = false;
                
                if (this->skillDef == nullptr)
                {
                    cocos2d::log("skillDef == nullptr");
                }
                else
                {
                    cocos2d::EventDispatcher * eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
                    cocos2d::EventCustom event(SKILL_BUTTON_TOUCHED);
                    event.setUserData(this->skillDef);
                    eventDispatcher->dispatchEvent(&event);
                }
                //
            }
        }
    }
}
/********************************************/
lofd::MapScene * MapScene::create(moonsugar::MapSceneData * mapSceneDataValue)
{
    lofd::MapScene * mapScene = new lofd::MapScene(mapSceneDataValue);
    if (mapScene && mapScene->init())
    {
        mapScene->autorelease();
        return mapScene;
    }
    else
    {
        delete mapScene;
        mapScene = nullptr;
        return nullptr;
    }
}

bool MapScene::init()
{
    if (!moonsugar::MapScene::init()) return false;
    this->addContextListeners();
    return true;
}

void MapScene::addContextListeners()
{
    this->skillListener = cocos2d::EventListenerCustom::create(SKILL_BUTTON_TOUCHED, CC_CALLBACK_1(MapScene::onSkillHandler, this));
    cocos2d::EventDispatcher * eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithFixedPriority(this->skillListener, 1);
    this->skillCoolDownListener = cocos2d::EventListenerCustom::create(SKILL_COOLDOWN_START, CC_CALLBACK_1(MapScene::onSkillCoolDownStartHandler, this));
    eventDispatcher->addEventListenerWithFixedPriority(this->skillCoolDownListener, 1);
    this->skillCoolDownEndListener = cocos2d::EventListenerCustom::create(SKILL_COOLDOWN_END, CC_CALLBACK_1(MapScene::onSkillCoolDownEndHandler, this));
    eventDispatcher->addEventListenerWithFixedPriority(this->skillCoolDownEndListener, 1);
    this->actorDeadEventListener = cocos2d::EventListenerCustom::create(ACTOR_EVENT_DEAD, CC_CALLBACK_1(MapScene::onActorDeadHandler, this));
    eventDispatcher->addEventListenerWithFixedPriority(this->actorDeadEventListener, 1);
    this->actorDeadCompleteEventListener = cocos2d::EventListenerCustom::create(ACTOR_EVENT_DEAD_COMPLETE, CC_CALLBACK_1(MapScene::onActorDeadCompleteHandler, this));
    eventDispatcher->addEventListenerWithFixedPriority(this->actorDeadCompleteEventListener, 1);
}

void MapScene::removeContextListeners()
{
    cocos2d::EventDispatcher * eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    eventDispatcher->removeEventListener(this->skillListener);
    eventDispatcher->removeEventListener(this->skillCoolDownListener);
    eventDispatcher->removeEventListener(this->skillCoolDownEndListener);
    eventDispatcher->removeEventListener(this->actorDeadEventListener);
    eventDispatcher->removeEventListener(this->actorDeadCompleteEventListener);
}

void MapScene::onActorDeadHandler(cocos2d::EventCustom *event)
{
    cocos2d::log("onActorDeadhandler");
    lofd::ActorData * actorData = static_cast<lofd::ActorData *>(event->getUserData());
    
    constellation::BehaviorEvent * behaviorEvent = new constellation::BehaviorEvent(LOFD_BEHAVIOR_EVENT_ACTOR_DEAD);
    lofd::AIBehaviorDynamicData * data = new lofd::AIBehaviorDynamicData;
    data->currentOperationActorData = actorData;
    data->operationType = LOFD_ACTOR_STATE_OPERATION_TYPE_SYSTEM;
    behaviorEvent->behaviorData = data;
    lofd::MapActorsLayer * mapActorsLayer = dynamic_cast<lofd::MapActorsLayer *>(this->getMapActorsLayer());
    long n = mapActorsLayer->actorLayerData->mapActorDatas.size();
    for (int i = 0; i < n; i ++) {
        lofd::ActorData * checkActorData = dynamic_cast<lofd::ActorData *>(mapActorsLayer->actorLayerData->mapActorDatas.at(i));
        if (checkActorData->tagId == actorData->tagId) continue;
        
        checkActorData->deleteCurrentRangeActors(actorData);
        
        data->actorData = checkActorData;
        data->mapScene = this;
        data->mapSceneData = this->mapSceneData;
        checkActorData->aiBehavior->root->execute(behaviorEvent);
    }
    delete behaviorEvent;
    delete data;
    //this->removeActor(actorData);
}

void MapScene::onActorDeadCompleteHandler(cocos2d::EventCustom *event)
{
    lofd::ActorData * actorData = static_cast<lofd::ActorData *>(event->getUserData());
    this->removeActor(actorData);
}

void MapScene::onSkillHandler(cocos2d::EventCustom * event)
{
    lofd::SkillDef * skillDef = static_cast<lofd::SkillDef *>(event->getUserData());
    if (skillDef->operationContextDef->operationType == SKILL_OPERATION_TYPE_ACTOR)
    {
        this->currentFocus->changeSkillFocus(skillDef->skillId);
        if (this->currentFocus->currentFocus != nullptr)
        {

            constellation::BehaviorEvent * event = new constellation::BehaviorEvent(LOFD_BEHAVIOR_EVENT_AI_TRACK);
            lofd::AIBehaviorDynamicData * aiBehaviorData = new lofd::AIBehaviorDynamicData;
            aiBehaviorData->actorData = this->currentFocus;
            aiBehaviorData->mapScene = this;
            aiBehaviorData->operationType = LOFD_ACTOR_STATE_OPERATION_TYPE_CUSOTM;
            event->behaviorData = aiBehaviorData;
            this->currentFocus->aiBehavior->root->execute(event);
            delete event;
            delete aiBehaviorData;
        }
    }
    else
    {
        this->currentFocus->currentWaitSkill = skillDef->skillId;
    }
}

void MapScene::onSkillCoolDownStartHandler(cocos2d::EventCustom *event)
{
    lofd::SkillEventParam * skillEventParam = static_cast<lofd::SkillEventParam *>(event->getUserData());
    if (!this->currentFocus) return;
    if (skillEventParam->actorTag == this->currentFocus->tagId)
    {
        lofd::MapUILayer * mapUILayer = dynamic_cast<lofd::MapUILayer *>(this->getMapUILayer());
        mapUILayer->startCD(skillEventParam->skillDef->skillId);
    }
    skillEventParam->release();
}

void MapScene::onSkillCoolDownEndHandler(cocos2d::EventCustom * event)
{
    lofd::SkillEventParam * skillEventParam = static_cast<lofd::SkillEventParam *>(event->getUserData());
    if (!this->currentFocus) return;
    if (skillEventParam->actorTag == this->currentFocus->tagId)
    {
        cocos2d::log("my skill %d has cool down end", skillEventParam->skillDef->skillId);
    }
    skillEventParam->release();
}

bool MapScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    if (!this->currentFocus) return moonsugar::MapScene::onTouchBegan(touch, unused_event);
    startPoint.setPoint(touch->getLocation().x, touch->getLocation().y);
    moonsugar::MapActorsLayer * mapActorsLayer = this->getMapActorsLayer();
    cocos2d::Point touchLocalPoint = mapActorsLayer->convertTouchToNodeSpace(touch);
    cocos2d::log("localPoint x=%f, y=%f", touchLocalPoint.x, touchLocalPoint.y);
    moonsugar::VectorPoint * startVPoint = new moonsugar::VectorPoint(new cocos2d::Point(currentFocus->actorEntry->getPosition()));
    moonsugar::VectorPoint * endVPoint = new moonsugar::VectorPoint(new cocos2d::Point(touchLocalPoint));
    cocos2d::Vector<moonsugar::VectorPoint *> paths = moonsugar::VectorUtils::findPath(startVPoint, endVPoint, this->mapSceneData);
    long length = paths.size();
    cocos2d::log("路径长度%d", (int)length);
    if (length > 0)
    {
        this->isDrawPath = true;
        tempPath = paths;
        tempStartPoint = startVPoint;
        tempEndPoint = endVPoint;
    }
    return moonsugar::MapScene::onTouchBegan(touch, unused_event);
}

void MapScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    this->isDrawPath = false;
    moonsugar::MapScene::onTouchMoved(touch, unused_event);
}

void MapScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    this->isDrawPath = false;
    if (!this->currentFocus) return;
    float dx = abs(touch->getLocation().x - startPoint.x);
    float dy = abs(touch->getLocation().y - startPoint.y);
    bool isHit = false;
    lofd::ActorData * actorData = nullptr;
    moonsugar::MapActorsLayer * mapActorsLayer = this->getMapActorsLayer();
    cocos2d::Point touchLocalPoint = mapActorsLayer->convertTouchToNodeSpace(touch);
    moonsugar::ActorLayerData * mapLayerData = static_cast<moonsugar::ActorLayerData *>(mapActorsLayer->mapLayerData);
    //这部分需要重构,现在需要根据当前焦点的技能操作类型来判断操作什么类型
    lofd::MapUILayer * mapUILayer = dynamic_cast<lofd::MapUILayer *>(this->getMapUILayer());
    if (mapUILayer->hasButtonTouched())
    {
        //hasButtonTouched
    }
    else
    {
        lofd::SkillConfig * skillConfig = lofd::ConfigManager::getInstance()->skillConfig;
        lofd::SkillDef * skillDef;
        if (this->currentFocus->currentWaitSkill != -1)
        {
            skillDef = skillConfig->getSkillDefById(this->currentFocus->currentWaitSkill);
        }
        else
        {
            skillDef = skillConfig->getSkillDefById(this->currentFocus->currentFocusSkill);
        }
        lofd::SkillCoolDownData * skillCoolDownData = this->currentFocus->getSkillCoolDownData(skillDef->skillId);
        if (skillDef->operationContextDef->operationType == SKILL_OPERATION_TYPE_TARGET)
        {
            if (skillCoolDownData->isCoolDown) return;
            this->currentFocus->changeSkillFocus(this->currentFocus->currentWaitSkill);
            this->currentFocus->currentWaitSkill = -1;
            this->currentFocus->targetPoint.x = touchLocalPoint.x;
            this->currentFocus->targetPoint.y = touchLocalPoint.y;
            constellation::BehaviorEvent * event = new constellation::BehaviorEvent(LOFD_BEHAVIOR_EVENT_AI_TARGET);
            lofd::AIBehaviorDynamicData * aiBehaviorData = new lofd::AIBehaviorDynamicData;
            aiBehaviorData->actorData = this->currentFocus;
            aiBehaviorData->mapScene = this;
            aiBehaviorData->mapSceneData = this->mapSceneData;
            aiBehaviorData->targetPoint = this->currentFocus->targetPoint;
            aiBehaviorData->operationType = LOFD_ACTOR_STATE_OPERATION_TYPE_CUSOTM;
            event->behaviorData = aiBehaviorData;
            this->currentFocus->aiBehavior->root->execute(event);
            delete event;
            delete aiBehaviorData;
        }
        else if (skillDef->operationContextDef->operationType == SKILL_OPERATION_TYPE_ACTOR)
        {
            for (int i = 0; i < mapLayerData->mapActorDatas.size(); i++)
            {
                moonsugar::MapActorData * mapActorData = mapLayerData->mapActorDatas.at(i);
                actorData = static_cast<lofd::ActorData *>(mapActorData);
                if (actorData->tagId == this->currentFocus->tagId || actorData->isDead) continue;
                cocos2d::Rect * rect = actorData->getBound();
                if (moonsugar::VectorUtils::calculateIsHitRect(touchLocalPoint, rect)) {
                    isHit = true;
                    break;
                }
            }
            if (isHit)
            {
                if (skillCoolDownData->isCoolDown) return;
                cocos2d::log("touch actor %d", actorData->actorId);
                lofd::CampRelationShipDef * def = this->currentFocus->campContext->getDefByCampId(actorData->campContext->campId);
                if (def->relationShip < 0)
                {
                    this->currentFocus->currentFocus = actorData;
                    cocos2d::log("change the focus");
                    constellation::BehaviorEvent * event = new constellation::BehaviorEvent(LOFD_BEHAVIOR_EVENT_AI_TRACK);
                    lofd::AIBehaviorDynamicData * aiBehaviorData = new lofd::AIBehaviorDynamicData;
                    aiBehaviorData->actorData = this->currentFocus;
                    aiBehaviorData->mapScene = this;
                    aiBehaviorData->operationType = LOFD_ACTOR_STATE_OPERATION_TYPE_CUSOTM;
                    event->behaviorData = aiBehaviorData;
                    this->currentFocus->aiBehavior->root->execute(event);
                    delete event;
                    delete aiBehaviorData;
                }
                else if (def->relationShip > 0)
                {
                    
                }
            }
            else
            {
                int gap = 150;
                if (dx <= gap && dy <= gap)
                {
                    cocos2d::log("localPoint x=%f, y=%f", touchLocalPoint.x, touchLocalPoint.y);
                    
                    bool isAllow = moonsugar::VectorUtils::calculateIsAllow(touchLocalPoint, this->mapSceneData);
                    bool isHitMapItem = moonsugar::VectorUtils::calculateIsHitMapItem(touchLocalPoint, this->mapSceneData);
                    if (isAllow && !isHitMapItem)
                    {
                        constellation::BehaviorEvent * actionBehaviorEvent = new constellation::BehaviorEvent(LOFD_BEHAVIOR_EVENT_AI_MOVE);
                        lofd::AIBehaviorDynamicData * data = new lofd::AIBehaviorDynamicData();
                        data->point = &touchLocalPoint;
                        data->actorData = this->currentFocus;
                        data->mapSceneData = mapSceneData;
                        actionBehaviorEvent->behaviorData = data;
                        this->currentFocus->aiBehavior->root->execute(actionBehaviorEvent);
                        delete actionBehaviorEvent;
                        delete data;
                    }
                }
            }
        }
        else
        {
            
        }
    }
    cocos2d::log("point gap x=%f y=%f", dx, dy);
    startPoint.x = touch->getLocation().x;
    startPoint.y = touch->getLocation().y;
    moonsugar::MapScene::onTouchEnded(touch, unused_event);
}

void MapScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    moonsugar::MapScene::onTouchCancelled(touch, unused_event);
}

void MapScene::onTouchEndedOrCancelledExecute(cocos2d::Touch * touch, cocos2d::Event *unused_event)
{
    moonsugar::MapScene::onTouchEndedOrCancelledExecute(touch, unused_event);
}

void MapScene::addActor(lofd::ActorData *actorValue)
{
    moonsugar::MapActorsLayer * mapActorsLayer = this->getMapActorsLayer();
    if (mapActorsLayer)
    {
        mapActorsLayer->actorLayerData->mapActorDatas.pushBack(actorValue);
        actorValue->mapScene = this;
        mapActorsLayer->addChild(actorValue->actorContainer);
    }
}

void MapScene::removeActor(lofd::ActorData * actorValue)
{
    moonsugar::MapActorsLayer * mapActorsLayer = this->getMapActorsLayer();
    if (mapActorsLayer)
    {
        long n = mapActorsLayer->actorLayerData->mapActorDatas.size();
        for (int i = 0; i < n; i++) {
            lofd::ActorData * checkActorData = static_cast<lofd::ActorData *>(mapActorsLayer->actorLayerData->mapActorDatas.at(i));
            if (checkActorData->tagId == actorValue->tagId)
            {
                mapActorsLayer->actorLayerData->mapActorDatas.erase(i);
                break;
            }
        }
        mapActorsLayer->removeChild(actorValue->actorContainer);
    }
}

void MapScene::addOperationActor(lofd::ActorData *actorValue)
{
    this->addActor(actorValue);
    this->operationActors.pushBack(actorValue);
}

void MapScene::removeOperationActor(lofd::ActorData * actorValue)
{
    long n = this->operationActors.size();
    for (int i = 0; i < n; i ++) {
        lofd::ActorData * checkActorData = static_cast<lofd::ActorData *>(this->operationActors.at(i));
        if (checkActorData->tagId == actorValue->tagId)
        {
            this->operationActors.erase(i);
        }
    }
    this->removeActor(actorValue);
}

void MapScene::draw(cocos2d::Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
{
    moonsugar::MapScene::draw(renderer, transform, transformUpdated);
}

void MapScene::onDraw(const kmMat4 &transform, bool transformUpdated)
{
    moonsugar::MapScene::onDraw(transform, transformUpdated);
    if (!this->isDrawPath) return;
}

moonsugar::MapUILayer * lofd::MapScene::createMapUILayer(moonsugar::UILayerData *uiLayerDataValue)
{
    lofd::MapUILayer * mapUILayer = lofd::MapUILayer::create(uiLayerDataValue);
    mapUILayer->mapScene = this;
    return mapUILayer;
}

moonsugar::MapActorsLayer * lofd::MapScene::createMapActorsLayer(moonsugar::ActorLayerData *actorLayerDataValue)
{
    lofd::MapActorsLayer * mapActorsLayer = lofd::MapActorsLayer::create(actorLayerDataValue);
    mapActorsLayer->mapScene = this;
    return mapActorsLayer;
}

moonsugar::MapEffectLayer * lofd::MapScene::createMapEffectLayer(moonsugar::EffectLayerData *effectLayerDataValue)
{
    lofd::MapEffectLayer * mapEffectLayer = lofd::MapEffectLayer::create(effectLayerDataValue);
    return mapEffectLayer;
}

void lofd::MapScene::changeFocus(lofd::ActorData * actorDataValue)
{
    if (this->currentFocus != nullptr)
    {
        this->currentFocus->isCurrentFocus = false;
        
//        constellation::BehaviorEvent * behaviorEvent = new constellation::BehaviorEvent(LOFD_BEHAVIOR_EVENT_AI_PATROL);
//        lofd::AIBehaviorDynamicData * behaviorDynamicData = new lofd::AIBehaviorDynamicData();
//        behaviorDynamicData->actorData = this->currentFocus;
//        behaviorDynamicData->mapSceneData = this->mapSceneData;
//        behaviorDynamicData->operationType = LOFD_ACTOR_STATE_OPERATION_TYPE_AI;
//        behaviorEvent->behaviorData = behaviorDynamicData;
//        this->currentFocus->aiBehavior->root->execute(behaviorEvent);
//        delete behaviorEvent;
//        delete behaviorDynamicData;
    }
    
    this->currentFocus = actorDataValue;
    this->currentFocus->isCurrentFocus = true;
    
//    constellation::BehaviorEvent * behaviorEvent = new constellation::BehaviorEvent(LOFD_BEHAVIOR_EVENT_AI_CANCEL_PATROL);
//    lofd::AIBehaviorDynamicData * behaviorDynamicData = new lofd::AIBehaviorDynamicData();
//    behaviorDynamicData->actorData = this->currentFocus;
//    behaviorDynamicData->mapSceneData = this->mapSceneData;
//    behaviorDynamicData->operationType = LOFD_ACTOR_STATE_OPERATION_TYPE_AI;
//    behaviorEvent->behaviorData = behaviorDynamicData;
//    this->currentFocus->aiBehavior->root->execute(behaviorEvent);
//    delete behaviorEvent;
//    delete behaviorDynamicData;
    
    lofd::MapUILayer * mapUILayer = dynamic_cast<lofd::MapUILayer *>(this->getMapUILayer());
    mapUILayer->changeFocus(this->currentFocus);
}
/****************MapUILayer*********************/
MapUILayer * MapUILayer::create(moonsugar::UILayerData *uiLayerData)
{
    MapUILayer * uiLayer = new lofd::MapUILayer(uiLayerData);
    if (uiLayer && uiLayer->init())
    {
        uiLayer->autorelease();
        return uiLayer;
    }
    else
    {
        delete uiLayer;
        uiLayer = nullptr;
        return nullptr;
    }
}

void MapUILayer::atk1CallBack(cocos2d::Ref *pSender)
{
    cocos2d::log("atk1 click");
//    if (this->currentFocus == nullptr)
//    {
//        lofd::ActorData * actorData = lofd::ActorControllerUtils::createActorDataById(710040);
//        actorData->isAI = true;
//        actorData->isRange = false;
//        lofd::ActorStateData * actorStateData = new lofd::ActorStateData(LOFD_ACTOR_STATE_IDLE, LOFD_ACTOR_STATE_OPERATION_TYPE_AI);
//        actorData->stateContext->insertStateData(actorStateData);
//        actorData->runAction(LOFD_ACTOR_STATE_IDLE);
//        actorData->actorContainer->setPosition(cocos2d::Point(280, 100));
//        actorData->actorPropertyData->hp = 10000;
//        mapScene->addOperationActor(actorData);
//        mapScene->changeFocus(actorData);
//    }
    //debugTextField->textFieldTTF->get
    std::string str = debugTextField->textFieldTTF->getString();
    int tagId = moonsugar::TypeConver::CharToInt(str.c_str());
    testTagId = tagId;
    cocos2d::log("change tagId %d", testTagId);
}

void MapUILayer::skillCallBack(cocos2d::Ref *pSender)
{
    cocos2d::log("skill click");
//    constellation::BehaviorEvent * event = new constellation::BehaviorEvent(LOFD_ACTOR_BEHAVIOR_EVENT_SKILL1);
//    lofd::ActorBehaviorData * bData = new lofd::ActorBehaviorData();
//    bData->actorData = this->currentFocus;
//    event->behaviorData = bData;
//    this->currentFocus->actorBehavior->root->execute(event);
//    delete bData;
//    delete event;
    
//    lofd::SkillConfig * skillConfig = lofd::ConfigManager::getInstance()->skillConfig;
//    lofd::SkillParamContext * skillParamContext = new lofd::SkillParamContext();
//    skillParamContext->mapScene = this->mapScene;
//    skillParamContext->actorData = this->mapScene->currentFocus;
//    lofd::SkillDef * skillDef = skillConfig->skillDefs.at(0);
//    lofd::SkillData * skillData = lofd::SkillDataUtils::createSkillData(skillDef, skillParamContext);
//    skillData->skillDef = skillDef;
//    currentFocus->skill(skillData);
    //this->currentFocus->currentFocusSkill = 2;
}

bool MapUILayer::hasButtonTouched()
{
    bool hasTouched = false;
    if (this->skillButton1->isTouched)
    {
        hasTouched = true;
    }
    else if (this->skillButton2->isTouched)
    {
        hasTouched = true;
    }
    else if (this->skillButton3->isTouched)
    {
        hasTouched = true;
    }
    else if (this->skillButton4->isTouched)
    {
        hasTouched = true;
    }
    this->skillButton1->isTouched = false;
    this->skillButton2->isTouched = false;
    this->skillButton3->isTouched = false;
    this->skillButton4->isTouched = false;
    return hasTouched;
}

void MapUILayer::addActorCallBack(cocos2d::Ref *pSender)
{
    cocos2d::log("add actor click");
    std::vector<int> ids;
    ids.push_back(710010);
        ids.push_back(710020);
    ids.push_back(710040);
        //ids.push_back(710030);
    ids.push_back(710050);
    ids.push_back(710060);
    int i = rand() % ids.size();
    
    lofd::ActorData * actorData = lofd::ActorControllerUtils::createActorDataById(ids.at(i), this->mapScene->dungeonDef);
    actorData->isAI = true;
    
    //cocos2d::Point * local = new cocos2d::Point(rand() % (int)this->mapLayerData->allowRect->size.width, rand() % (int)this->mapLayerData->allowRect->size.height);
    cocos2d::Point * local = new cocos2d::Point(rand() % 1280, rand() % (int)this->mapLayerData->allowRect->size.height);
    actorData->point = local;
//    lofd::ActorStateData * actorStateData = new lofd::ActorStateData(LOFD_ACTOR_STATE_IDLE, LOFD_ACTOR_STATE_OPERATION_TYPE_AI);
    lofd::ActorStateData * actorStateData = lofd::ActorStateData::create(LOFD_ACTOR_STATE_IDLE, LOFD_ACTOR_STATE_OPERATION_TYPE_AI);
    actorData->stateContext->insertStateData(actorStateData);
    actorData->runAction(LOFD_ACTOR_STATE_IDLE);
    actorData->actorContainer->setPosition(local->x, local->y);
    this->mapScene->addActor(actorData);
    
    constellation::BehaviorEvent * behaviorEvent = new constellation::BehaviorEvent(LOFD_BEHAVIOR_EVENT_AI_PATROL);
    lofd::AIBehaviorDynamicData * behaviorDynamicData = new lofd::AIBehaviorDynamicData();
    behaviorDynamicData->actorData = actorData;
    behaviorDynamicData->mapSceneData = this->mapScene->mapSceneData;
    behaviorDynamicData->operationType = LOFD_ACTOR_STATE_OPERATION_TYPE_AI;
    behaviorEvent->behaviorData = behaviorDynamicData;
    actorData->aiBehavior->root->execute(behaviorEvent);
    delete behaviorEvent;
    delete behaviorDynamicData;
}

void lofd::MapUILayer::changeFocus(lofd::ActorData *actorDataValue)
{
    this->currentFocus = actorDataValue;
    
    lofd::SkillCoolDownData * skillCoolDownData;
    if (this->currentFocus->skillCoolDownDatas.size() > 0)
    {
        skillCoolDownData = dynamic_cast<lofd::SkillCoolDownData *>(this->currentFocus->skillCoolDownDatas.at(0));
        skillButton1->skillDef = skillCoolDownData->skillDef;
    }
    if (this->currentFocus->skillCoolDownDatas.size() > 1)
    {
        skillCoolDownData = dynamic_cast<lofd::SkillCoolDownData *>(this->currentFocus->skillCoolDownDatas.at(1));
        skillButton2->skillDef = skillCoolDownData->skillDef;
    }
    if (this->currentFocus->skillCoolDownDatas.size() > 2)
    {
        skillCoolDownData = dynamic_cast<lofd::SkillCoolDownData *>(this->currentFocus->skillCoolDownDatas.at(2));
        skillButton3->skillDef = skillCoolDownData->skillDef;
    }
    if (this->currentFocus->skillCoolDownDatas.size() > 3)
    {
        skillCoolDownData = dynamic_cast<lofd::SkillCoolDownData *>(this->currentFocus->skillCoolDownDatas.at(3));
        skillButton4->skillDef = skillCoolDownData->skillDef;
    }
}

bool lofd::MapUILayer::init()
{
    if (!moonsugar::MapUILayer::init()) return false;
    auto atk1Item = cocos2d::MenuItemImage::create("up.png", "up.png", CC_CALLBACK_1(MapUILayer::atk1CallBack, this));
    auto skillItem = cocos2d::MenuItemImage::create("down.png", "down.png", CC_CALLBACK_1(MapUILayer::skillCallBack, this));
    auto addActorItem = cocos2d::MenuItemImage::create("continue.png", "continue.png", CC_CALLBACK_1(MapUILayer::addActorCallBack, this));
    atk1Item->setPosition(100, 0);
    skillItem->setPosition(0, 0);
    addActorItem->setPosition(200, 0);
    auto menu = cocos2d::Menu::create(addActorItem, NULL);
    menu->setPosition(cocos2d::Point(200, 50));
    this->addChild(menu);
    
    cocos2d::Point point1(1200, 80);
    this->skillButton1 = lofd::SkillButton::create();
    skillButton1->createButton();
    this->addChild(skillButton1);
    skillButton1->setPosition(point1);
    
    cocos2d::Point point2(1200, 200);
    this->skillButton2 = lofd::SkillButton::create();
    skillButton2->createButton();
    this->addChild(skillButton2);
    skillButton2->setPosition(point2);
    skillButton2->setScale(.7, .7);
    
    cocos2d::Point point3(1100, 160);
    this->skillButton3 = lofd::SkillButton::create();
    this->skillButton3->createButton();
    this->addChild(skillButton3);
    skillButton3->setPosition(point3);
    skillButton3->setScale(.7, .7);
    
    cocos2d::Point point4(1000, 70);
    this->skillButton4 = lofd::SkillButton::create();
    this->skillButton4->createButton();
    this->addChild(skillButton4);
    skillButton4->setPosition(point4);
    skillButton4->setScale(.7, .7);
    
    debugTextField = lofd::DebugTextField::create();
    this->addChild(debugTextField);
    debugTextField->setPosition(cocos2d::Point(1000, 600));
    
    this->addListeners();
    return true;
}

bool lofd::MapUILayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    touchPoint = touch->getLocation();
    return true;
}

void lofd::MapUILayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
//    cocos2d::Point endPos = touch->getLocation();
//    const float gap = 20.0f;
//    if (abs(endPos.x - touchPoint.x) > gap  || abs(endPos.y - touchPoint.y) > gap)
//    {
//        return;
//    }
//    auto point = convertTouchToNodeSpace(touch);
//    cocos2d::Rect rect;
//    rect.size = textFiledTTF->getContentSize();
//    rect.origin.x = textFiledTTF->getPosition().x - textFiledTTF->getContentSize().width * .5;
//    rect.origin.y = textFiledTTF->getPosition().y - textFiledTTF->getContentSize().height * .5;
//    
//    if (rect.containsPoint(point))
//    {
//        //
//        textFiledTTF->attachWithIME();
//    }
}

void lofd::MapUILayer::addListeners()
{
//    this->touchListener = cocos2d::EventListenerTouchOneByOne::create();
//    this->touchListener->onTouchBegan = CC_CALLBACK_2(lofd::MapUILayer::onTouchBegan, this);
//    this->touchListener->onTouchEnded = CC_CALLBACK_2(lofd::MapUILayer::onTouchEnded, this);
//    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(this->touchListener, this);
}

void lofd::MapUILayer::removeListeners()
{
    //cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(this->touchListener);
}

void lofd::MapUILayer::startCD(int skillId)
{
    if (this->skillButton1->skillDef->skillId == skillId)
    {
        this->skillButton1->startCoolDown();
    }
    else if (this->skillButton2->skillDef->skillId == skillId)
    {
        this->skillButton2->startCoolDown();
    }
    else if (this->skillButton3->skillDef->skillId == skillId)
    {
        this->skillButton3->startCoolDown();
    }
    else if (this->skillButton4->skillDef->skillId == skillId)
    {
        this->skillButton4->startCoolDown();
    }
}
/************MapActorsLayer************/
void lofd::MapActorsLayer::draw(cocos2d::Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
{
    cocos2d::Layer::draw(renderer, transform, transformUpdated);
    this->_customCommand.init(_globalZOrder);
    this->_customCommand.func = CC_CALLBACK_0(MapActorsLayer::onDraw, this, transform, transformUpdated);
    renderer->addCommand(&_customCommand);
}

void lofd::MapActorsLayer::onDraw(const kmMat4 &transform, bool transformUpdated)
{
    /*******排序********/
    long n = this->getChildrenCount();
    for (int i = 0; i < n; i ++) {
        cocos2d::Node * node = this->getChildren().at(i);
        this->reorderChild(node, 0 - node->getPositionY());
    }
    
    if (countCheckRange >= 5)
    {
        countCheckRange = 0;
        /*******查找range*******/
        for (int j = 0; j < this->actorLayerData->mapActorDatas.size(); j ++)
        {
            lofd::ActorData * checkActorData = (lofd::ActorData *)this->actorLayerData->mapActorDatas.at(j);
            if (!checkActorData->isRange) continue;
            cocos2d::Vector<lofd::ActorData *> newIntoRange;
            cocos2d::Vector<lofd::ActorData *> newOutRange;
            for (int t = 0; t < this->actorLayerData->mapActorDatas.size(); t++) {
                lofd::ActorData * beCheckActorData = (lofd::ActorData *)this->actorLayerData->mapActorDatas.at(t);
                if (beCheckActorData->isDead) continue;
                if (checkActorData->tagId != beCheckActorData->tagId && !checkActorData->isDead)
                {
                    //checkActorData->actorEntry
                    cocos2d::Point * start = new cocos2d::Point(checkActorData->actorContainer->getPositionX(), checkActorData->actorContainer->getPositionY());
                    cocos2d::Point * end = new cocos2d::Point(beCheckActorData->actorContainer->getPositionX(), beCheckActorData->actorContainer->getPositionY());
                    float distance = moonsugar::VectorUtils::calculateDistance(start, end);
                    delete start;
                    delete end;
                    int getActorIndex = -1;
                    getActorIndex = checkActorData->getActorFromCurrentRangeActors(beCheckActorData->tagId);
                    if (distance <= checkActorData->actorDef->warnRound)
                    {
                        if (getActorIndex == -1) {
                            //cocos2d::log("new actor into range");
                            checkActorData->currentRangeActors.pushBack(beCheckActorData);
                            newIntoRange.pushBack(beCheckActorData);
                            //cocos2d::log("actor reference count %d", beCheckActorData->getReferenceCount());
                        }
                    }
                    else
                    {
                        if (getActorIndex != -1) {
                            //cocos2d::log("new actor out range");
                            checkActorData->currentRangeActors.erase(getActorIndex);
                            newOutRange.pushBack(beCheckActorData);
                            //cocos2d::log("actor reference count %d", beCheckActorData->getReferenceCount());
                        }
                        
                    }
                }
            }
            if (newIntoRange.size() > 0)
            {
                constellation::BehaviorEvent * event = new constellation::BehaviorEvent(LOFD_BEHAVIOR_EVENT_NEW_ACTOR_INTO_RANGE);
                lofd::AIBehaviorDynamicData * data = new lofd::AIBehaviorDynamicData();
                data->actorData = checkActorData;
                //data->rangeChangeActors = newIntoRange;
                data->mapScene = this->mapScene;
                data->operationType = LOFD_ACTOR_STATE_OPERATION_TYPE_AI;
                event->behaviorData = data;
                checkActorData->aiBehavior->root->execute(event);
                delete event;
                delete data;
            }
            if (newOutRange.size() > 0)
            {
                constellation::BehaviorEvent * event = new constellation::BehaviorEvent(LOFD_BEHAVIOR_EVENT_NEW_ACTOR_OUT_RANGE);
                lofd::AIBehaviorDynamicData * data = new lofd::AIBehaviorDynamicData();
                data->actorData = checkActorData;
                //data->rangeChangeActors = newOutRange;
                data->operationType = LOFD_ACTOR_STATE_OPERATION_TYPE_AI;
                event->behaviorData = data;
                checkActorData->aiBehavior->root->execute(event);
                delete event;
                delete data;
            }
            newIntoRange.clear();
            newOutRange.clear();
        }
    }
    countCheckRange ++;
}

bool lofd::MapActorsLayer::init()
{
    if (!moonsugar::MapActorsLayer::init()) return false;
    return true;
}

lofd::MapActorsLayer * lofd::MapActorsLayer::create(moonsugar::ActorLayerData *actorLayerDataValue)
{
    lofd::MapActorsLayer * mapActorsLayer = new lofd::MapActorsLayer(actorLayerDataValue);
    if (mapActorsLayer && mapActorsLayer->init())
    {
        mapActorsLayer->autorelease();
        return mapActorsLayer;
    }
    else
    {
        delete mapActorsLayer;
        mapActorsLayer = nullptr;
        return nullptr;
    }
}

/************MapEffectLayer****************/
MapEffectLayer * MapEffectLayer::create(moonsugar::EffectLayerData * effectLayerDataValue)
{
    MapEffectLayer * mapEffectLayer = new MapEffectLayer(effectLayerDataValue);
    if (mapEffectLayer && mapEffectLayer->init())
    {
        mapEffectLayer->autorelease();
        return mapEffectLayer;
    }
    else
    {
        delete mapEffectLayer;
        mapEffectLayer = nullptr;
        return nullptr;
    }
}

bool MapEffectLayer::init()
{
    if (!moonsugar::MapEffectLayer::init()) return false;
    this->addContextListeners();
    return true;
}

void MapEffectLayer::addContextListeners()
{
    cocos2d::EventDispatcher * eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    this->actorHurtEventListener = cocos2d::EventListenerCustom::create(ACTOR_EVENT_HURT, CC_CALLBACK_1(MapEffectLayer::onActorHurtHandler, this));
    eventDispatcher->addEventListenerWithFixedPriority(this->actorHurtEventListener, 1);
}

void MapEffectLayer::removeContextListeners()
{
    cocos2d::EventDispatcher * eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    eventDispatcher->removeEventListener(this->actorHurtEventListener);
}

void MapEffectLayer::onActorHurtHandler(cocos2d::EventCustom *event)
{
    //cocos2d::log("onActorHurtHandler");
    lofd::ActorHurtEventParam * actorHurtEventParam = static_cast<lofd::ActorHurtEventParam *>(event->getUserData());
    //
    auto hurtCount = lofd::HurtCount::create();
    hurtCount->setCount(actorHurtEventParam->hurt, actorHurtEventParam->location);
    this->addChild(hurtCount);
    hurtCount->startAnimation();
    
    delete actorHurtEventParam;
}
/**********BattleScene*********************/
lofd::BattleScene * BattleScene::create()
{
    lofd::BattleScene * battleScene = new lofd::BattleScene();
    if (battleScene && battleScene->init())
    {
        battleScene->autorelease();
        return battleScene;
    }
    else
    {
        delete battleScene;
        battleScene = nullptr;
        return nullptr;
    }
}

bool lofd::BattleScene::init()
{
    if (!cocos2d::Scene::init()) return false;
    return true;
}

/************UISceneLayer*************/
bool UISceneLayer::init()
{
    if (!cocos2d::Layer::init()) return false;
    
    cocos2d::Point point(1200, 80);
    lofd::SkillButton * skillButton = lofd::SkillButton::create();
    skillButton->createButton();
    this->addChild(skillButton);
    skillButton->setPosition(point);
    
    lofd::SkillDef * skillDef = lofd::ConfigManager::getInstance()->skillConfig->getSkillDefById(1);
    skillButton->skillDef = skillDef;
    return true;
}
NS_LOFD_END;