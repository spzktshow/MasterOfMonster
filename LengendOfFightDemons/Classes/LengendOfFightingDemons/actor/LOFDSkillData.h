//
//  LOFDSkillData.h
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-19.
//
//

#ifndef __LengendOfFightDemons__LOFDSkillData__
#define __LengendOfFightDemons__LOFDSkillData__

#include "LOFDSkill.h"
#include "LOFDScene.h"
#include "LOFDSkillController.h"
#include "moonsugar.h"

NS_LOFD_BEGIN

/***************SkillCoolDownData***************/
class SkillCoolDownData : public cocos2d::Node
{
public:
    SkillCoolDownData():isCoolDown(false), currentTime(0){};
    ~SkillCoolDownData(){
        skillDef = nullptr;
        actorDataCallBackPoint = nullptr;
        skillCoolDownCallBack = nullptr;
    };
    
    /******标记是否在cd中*******/
    bool isCoolDown;
    /*******开始冷却***********/
    void startCoolDown();
    /*******结束冷却***********/
//    void coolDownComplete();
    
    virtual void update(float dt);
    
    lofd::SkillDef * skillDef;
    
//    lofd::ActorRandomSkillGroupDef * actorRandSkillGroupDef;
    
    /********技能coolDown回调**********/
    lofd::ActorData * actorDataCallBackPoint;
    cocos2d::SEL_CallFuncO skillCoolDownCallBack;
    float currentTime;
    
    CREATE_FUNC(SkillCoolDownData);
};
/***********************************************/
/**********SkillStateData****************/
class SkillStateData : public cocos2d::Ref
{
public:
    SkillStateData(){};
    ~SkillStateData(){
        actorData = nullptr;
        skillStateDef = nullptr;
    };
    
//    /*******释放者id*******/
    lofd::ActorData * actorData;
    lofd::SkillStateDef * skillStateDef;
    
    CREATE_REF_FUNC(SkillStateData);
};

/********EffectLocationData**************/
class EffectLocationData : public cocos2d::Ref
{
public:
    EffectLocationData(lofd::EffectLocationDef * locationDefValue){
        locationDef = locationDefValue;
    };
    ~EffectLocationData(){};
    
    lofd::EffectLocationDef * locationDef;
    
    static EffectLocationData * create(lofd::EffectLocationDef * locationDefValue);
};

class EffectRelativeLocationData : public lofd::EffectLocationData
{
public:
    EffectRelativeLocationData(lofd::EffectRelativeLocationDef * relativeLocationDefValue):EffectLocationData(relativeLocationDefValue){
        relativeLocationDef = relativeLocationDefValue;
    };
    ~EffectRelativeLocationData(){
        relativeLocationDef = nullptr;
    };
    
    lofd::EffectRelativeLocationDef * relativeLocationDef;
    
    static EffectRelativeLocationData * create(lofd::EffectRelativeLocationDef * relativeLocationDefValue)
    {
        EffectRelativeLocationData * effectRelativeLocationData = new EffectRelativeLocationData(relativeLocationDefValue);
        if (effectRelativeLocationData)
        {
            effectRelativeLocationData->autorelease();
            return effectRelativeLocationData;
        }
        else
        {
            delete effectRelativeLocationData;
            effectRelativeLocationData = nullptr;
            return nullptr;
        }
    };
};

class EffectTargetLocationData : public lofd::EffectLocationData
{
public:
    EffectTargetLocationData(lofd::EffectTargetLocationDef * targetLocationDefValue):EffectLocationData(targetLocationDefValue)
    {
        targetLocationDef = targetLocationDefValue;
    };
    ~EffectTargetLocationData(){
        targetLocationDef = nullptr;
        CC_SAFE_DELETE(targetPoint);
    };
    lofd::EffectTargetLocationDef * targetLocationDef;
    
    cocos2d::Point * targetPoint;
    
    static EffectTargetLocationData * create(lofd::EffectTargetLocationDef * targetLocationDefValue)
    {
        EffectTargetLocationData * effectTargetLocationData = new EffectTargetLocationData(targetLocationDefValue);
        if (effectTargetLocationData)
        {
            effectTargetLocationData->autorelease();
            return effectTargetLocationData;
        }
        else
        {
            delete effectTargetLocationData;
            effectTargetLocationData = nullptr;
            return nullptr;
        }
    };
};
/*****************************************/

/***************EffectMovementData**********/
class EffectMovementData : public cocos2d::Ref
{
public:
    EffectMovementData(){
    };
    ~EffectMovementData(){
        movementDef = nullptr;
    };
    
    lofd::EffectMovementDef * movementDef;
    
    CREATE_REF_FUNC(EffectMovementData);
};

class EffectMovementStillData : public EffectMovementData
{
public:
    EffectMovementStillData(){};
    ~EffectMovementStillData(){};
    
    CREATE_REF_FUNC(EffectMovementStillData);
};

class EffectMovementTargetData : public EffectMovementData
{
public:
    EffectMovementTargetData(){};
    ~EffectMovementTargetData(){
//        CC_SAFE_DELETE(targetPoint);
    };
    
    cocos2d::Point targetPoint;
    
    CREATE_REF_FUNC(EffectMovementTargetData);
};

class EffectMovementTrackData : public EffectMovementData
{
public:
    EffectMovementTrackData(){};
    ~EffectMovementTrackData(){
        actorData = nullptr;
        selfActorData = nullptr;
    };
    
    lofd::ActorData * selfActorData;
    lofd::ActorData * actorData;
    
    CREATE_REF_FUNC(EffectMovementTrackData);
};

/******************************************/

/*************EffectCoordinate*************/
class EffectCoordinateData : public cocos2d::Ref
{
public:
    EffectCoordinateData(){};
    ~EffectCoordinateData(){
        coordinateDef = nullptr;
    };
    
    lofd::EffectCoordinateDef * coordinateDef;
    
    CREATE_REF_FUNC(EffectCoordinateData);
};

class EffectActorCoordinateData : public EffectCoordinateData
{
public:
    EffectActorCoordinateData(){};
    ~EffectActorCoordinateData(){
        actorData = nullptr;
    };
    
    lofd::ActorData * actorData;
    
    CREATE_REF_FUNC(EffectActorCoordinateData);
};

class EffectSelfActorCoordinateData : public EffectCoordinateData
{
public:
    EffectSelfActorCoordinateData(){};
    ~EffectSelfActorCoordinateData(){
        selfActorData = nullptr;
    };
    
    lofd::ActorData * selfActorData;
    
    CREATE_REF_FUNC(EffectSelfActorCoordinateData);
};

class EffectSceneCoordinateData : public EffectCoordinateData
{
public:
    EffectSceneCoordinateData(){
        mapScene = nullptr;
        actorData = nullptr;
    };
    ~EffectSceneCoordinateData(){
        mapScene = nullptr;
        actorData = nullptr;
    };
    
    lofd::MapScene * mapScene;
    lofd::ActorData * actorData;
    
    CREATE_REF_FUNC(EffectSceneCoordinateData);
};
/******************************************/

/**************EffectAnimationType**********/
class EffectViewData : public cocos2d::Ref
{
public:
    EffectViewData(lofd::EffectViewDef * effectViewDefValue){
        this->effectViewDef = effectViewDefValue;
    };
    ~EffectViewData(){
        this->effectViewDef = nullptr;
    };
    
    lofd::EffectViewDef * effectViewDef;
    
    cocos2d::Sprite * entry;
    
    static EffectViewData * create(lofd::EffectViewDef * effectViewDefValue)
    {
        EffectViewData * effectViewData = new EffectViewData(effectViewDefValue);
        if (effectViewData)
        {
            effectViewData->autorelease();
            return effectViewData;
        }
        else
        {
            delete effectViewData;
            effectViewData = nullptr;
            return nullptr;
        }
    };
};

class EffectFrameViewData : public EffectViewData
{
public:
    EffectFrameViewData(lofd::EffectFrameViewDef * effectFrameViewDefValue):EffectViewData(effectFrameViewDefValue){
        this->effectFrameViewDef = effectFrameViewDefValue;
    };
    ~EffectFrameViewData(){
        effectFrameViewDef = nullptr;
        spriteFrame = nullptr;
        entry->release();
    };
    
    lofd::EffectFrameViewDef * effectFrameViewDef;
    
    cocos2d::SpriteFrame * spriteFrame;
    
    static EffectFrameViewData * create(lofd::EffectFrameViewDef * effectFrameViewDefValue)
    {
        EffectFrameViewData * effectFrameViewData = new EffectFrameViewData(effectFrameViewDefValue);
        if (effectFrameViewData)
        {
            effectFrameViewData->autorelease();
            return effectFrameViewData;
        }
        else
        {
            delete effectFrameViewData;
            effectFrameViewData = nullptr;
            return nullptr;
        }
    };
};

class EffectFramesViewData : public EffectViewData
{
public:
    EffectFramesViewData(lofd::EffectFramesViewDef * effectFramesViewDefValue):EffectViewData(effectFramesViewDefValue){
        effectFramesViewDef = effectFramesViewDefValue;
    };
    ~EffectFramesViewData(){
        effectFramesViewDef = nullptr;
        entry->release();
        animate->release();
    };
    
    lofd::EffectFramesViewDef * effectFramesViewDef;
    
    cocos2d::Animate * animate;
    
    static EffectFramesViewData * create(lofd::EffectFramesViewDef * effectFramesViewDefValue)
    {
        EffectFramesViewData * effectFramesViewData = new EffectFramesViewData(effectFramesViewDefValue);
        if (effectFramesViewData)
        {
            effectFramesViewData->autorelease();
            return effectFramesViewData;
        }
        else
        {
            delete effectFramesViewData;
            effectFramesViewData = nullptr;
            return nullptr;
        }
    };
};
/*******************************************/

/**********EffectHitData********************/
class EffectHitData : public cocos2d::Ref
{
public:
    EffectHitData(lofd::EffectHitDef * effectHitDefValue){
        effectHitDef = effectHitDefValue;
    };
    ~EffectHitData(){
        effectHitDef = nullptr;
    };
    
    lofd::EffectHitDef * effectHitDef;
    
    static EffectHitData * create(lofd::EffectHitDef * effectHitDefValue)
    {
        EffectHitData * effectHitData = new EffectHitData(effectHitDefValue);
        if (effectHitData)
        {
            effectHitData->autorelease();
            return effectHitData;
        }
        else
        {
            delete effectHitData;
            effectHitData = nullptr;
            return nullptr;
        }
    };
};

class EffectFrameHitData : public EffectHitData
{
public:
    EffectFrameHitData(lofd::EffectFrameHitDef * effectFrameHitDefValue):EffectHitData(effectFrameHitDefValue){
        effectFrameHitDef = effectFrameHitDefValue;
        hasTrigger = false;
    };
    ~EffectFrameHitData(){
        effectFrameHitDef = nullptr;
    };
    
    lofd::EffectFrameHitDef * effectFrameHitDef;
    
    bool hasTrigger;
    
    static EffectFrameHitData * create(lofd::EffectFrameHitDef * effectFrameHitDefValue)
    {
        EffectFrameHitData * effectFrameHitData = new EffectFrameHitData(effectFrameHitDefValue);
        if (effectFrameHitData)
        {
            effectFrameHitData->autorelease();
            return effectFrameHitData;
        }
        else
        {
            delete effectFrameHitData;
            effectFrameHitData = nullptr;
            return nullptr;
        }
    };
};

class EffectFramesHitData : public EffectHitData
{
public:
    EffectFramesHitData(lofd::EffectFramesHitDef * effectFramesHitDefValue):EffectHitData(effectFramesHitDefValue){
        effectFramesHitDef = effectFramesHitDefValue;
        hasTriggerIndex = -1;
    };
    ~EffectFramesHitData(){
        effectFramesHitDef = nullptr;
    };
    
    lofd::EffectFramesHitDef * effectFramesHitDef;
    
    int hasTriggerIndex;
    
    static EffectFramesHitData * create(lofd::EffectFramesHitDef * effectFramesHitDefValue)
    {
        EffectFramesHitData * effectFramesHitData = new EffectFramesHitData(effectFramesHitDefValue);
        if (effectFramesHitData)
        {
            effectFramesHitData->autorelease();
            return effectFramesHitData;
        }
        else
        {
            delete effectFramesHitData;
            effectFramesHitData = nullptr;
            return nullptr;
        }
    };
};
/*******************************************/
/***********EffectHarmData*******************/
class EffectOutputData : public cocos2d::Ref
{
public:
    EffectOutputData(){};
    ~EffectOutputData(){
        effectOutputDef = nullptr;
    };
    
    lofd::EffectOutputDef * effectOutputDef;
    
    CREATE_REF_FUNC(EffectOutputData);
};
/******************************************/
/***********EffectStartLifeCycleData*********/
class EffectStartLifeCycleData : public cocos2d::Ref
{
public:
    EffectStartLifeCycleData(lofd::EffectStartLifeCycleDef * effectStartLifeCycleDefValue){
        effectStartLifeCycleDef = effectStartLifeCycleDefValue;
    };
    ~EffectStartLifeCycleData(){
        effectStartLifeCycleDef = nullptr;
        //CC_SAFE_DELETE(effectOutputData);
        effectOutputData->retain();
    };
    
    lofd::EffectStartLifeCycleDef * effectStartLifeCycleDef;
    
    lofd::EffectOutputData * effectOutputData;
    
    static EffectStartLifeCycleData * create(lofd::EffectStartLifeCycleDef * effectStartLifeCycleDefValue)
    {
        EffectStartLifeCycleData * effectStartLifeCycleData = new EffectStartLifeCycleData(effectStartLifeCycleDefValue);
        if (effectStartLifeCycleData)
        {
            effectStartLifeCycleData->autorelease();
            return effectStartLifeCycleData;
        }
        else
        {
            delete effectStartLifeCycleData;
            effectStartLifeCycleData = nullptr;
            return nullptr;
        }
    };
};

class EffectStartLifeCycleDependData : public EffectStartLifeCycleData
{
public:
    EffectStartLifeCycleDependData(lofd::EffectStartLifeCycleDependDef * effectStartLifeCycleDependDefValue):EffectStartLifeCycleData(effectStartLifeCycleDependDefValue){
        effectStartLifeCycleDependDef = effectStartLifeCycleDependDefValue;
    };
    ~EffectStartLifeCycleDependData(){
        effectStartLifeCycleDependDef = nullptr;
    };
    
    lofd::EffectStartLifeCycleDependDef * effectStartLifeCycleDependDef;
    
    static EffectStartLifeCycleDependData * create(lofd::EffectStartLifeCycleDependDef * effectStartLifeCycleDependDefValue)
    {
        EffectStartLifeCycleDependData * effectStartLifeCycleData = new EffectStartLifeCycleDependData(effectStartLifeCycleDependDefValue);
        if (effectStartLifeCycleData)
        {
            effectStartLifeCycleData->autorelease();
            return effectStartLifeCycleData;
        }
        else
        {
            delete effectStartLifeCycleData;
            effectStartLifeCycleData = nullptr;
            return nullptr;
        }
    };
};

class EffectStartLifeCycleIntervalData : public EffectStartLifeCycleData
{
public:
    EffectStartLifeCycleIntervalData(lofd::EffectStartLifeCycleIntervalDef * effectStartLifeCycleIntervalDefValue):EffectStartLifeCycleData(effectStartLifeCycleIntervalDefValue){
        effectStartLifeCycleIntervalDef = effectStartLifeCycleIntervalDefValue;
    };
    ~EffectStartLifeCycleIntervalData(){
        effectStartLifeCycleIntervalDef = nullptr;
    };
    
    lofd::EffectStartLifeCycleIntervalDef * effectStartLifeCycleIntervalDef;
    
    static EffectStartLifeCycleIntervalData * create(lofd::EffectStartLifeCycleIntervalDef * effectStartLifeCycleIntervalDefValue)
    {
        EffectStartLifeCycleIntervalData * effectStartLifeCycleIntervalData = new EffectStartLifeCycleIntervalData(effectStartLifeCycleIntervalDefValue);
        if (effectStartLifeCycleIntervalData)
        {
            effectStartLifeCycleIntervalData->autorelease();
            return effectStartLifeCycleIntervalData;
        }
        else
        {
            delete effectStartLifeCycleIntervalData;
            effectStartLifeCycleIntervalData = nullptr;
            return nullptr;
        }
    };
};
/********************************************/
/***********EffectEndLifeCycleData***********/
class EffectEndLifeCycleData : public cocos2d::Ref
{
public:
    EffectEndLifeCycleData(lofd::EffectEndLifeCycleDef * effectEndLifeCycleDefValue){
        effectEndLifeCycleDef = effectEndLifeCycleDefValue;
        effectOutputData = nullptr;
    };
    ~EffectEndLifeCycleData(){
        effectEndLifeCycleDef = nullptr;
        //CC_SAFE_DELETE(effectOutputData);
        effectOutputData->autorelease();
    };
    
    lofd::EffectEndLifeCycleDef * effectEndLifeCycleDef;
    
    lofd::EffectOutputData * effectOutputData;
    
    static EffectEndLifeCycleData * create(lofd::EffectEndLifeCycleDef * effectEndLifeCycleDefValue)
    {
        EffectEndLifeCycleData * effectEndLifeCycleData = new EffectEndLifeCycleData(effectEndLifeCycleDefValue);
        if (effectEndLifeCycleData)
        {
            effectEndLifeCycleData->autorelease();
            return effectEndLifeCycleData;
        }
        else
        {
            delete effectEndLifeCycleData;
            effectEndLifeCycleData = nullptr;
            return nullptr;
        }
    };
};

class EffectEndLifeCycleHitData : public EffectEndLifeCycleData
{
public:
    EffectEndLifeCycleHitData(lofd::EffectEndLifeCycleHitDef * effectEndLifeCycleHitDefValue):EffectEndLifeCycleData(effectEndLifeCycleHitDefValue), currentHit(0)
    {
        effectEndLifeCycleHitDef = effectEndLifeCycleHitDefValue;
    };
    ~EffectEndLifeCycleHitData(){
        effectEndLifeCycleHitDef = nullptr;
    };
    
    lofd::EffectEndLifeCycleHitDef * effectEndLifeCycleHitDef;
    
    int currentHit;
    
    static EffectEndLifeCycleHitData * create(lofd::EffectEndLifeCycleHitDef * effectEndLifeCycleHitDefValue)
    {
        EffectEndLifeCycleHitData * effectEndLifeCycleHitData = new EffectEndLifeCycleHitData(effectEndLifeCycleHitDefValue);
        if (effectEndLifeCycleHitData)
        {
            effectEndLifeCycleHitData->autorelease();
            return effectEndLifeCycleHitData;
        }
        else
        {
            delete effectEndLifeCycleHitData;
            effectEndLifeCycleHitData = nullptr;
            return nullptr;
        }
    };
};

class EffectEndLifeCycleTimeData : public EffectEndLifeCycleData
{
public:
    EffectEndLifeCycleTimeData(lofd::EffectEndLifeCycleTimeDef * effectEndLifeCycleTimeDefValue):EffectEndLifeCycleData(effectEndLifeCycleTimeDefValue), currentTime(0){
        effectEndLifeCycleTimeDef = effectEndLifeCycleTimeDefValue;
    };
    ~EffectEndLifeCycleTimeData(){
        effectEndLifeCycleTimeDef = nullptr;
    };
    
    lofd::EffectEndLifeCycleTimeDef * effectEndLifeCycleTimeDef;
    
    float currentTime;
    
    static EffectEndLifeCycleTimeData * create(lofd::EffectEndLifeCycleTimeDef * effectEndLifeCycleTimeDefValue)
    {
        EffectEndLifeCycleTimeData * effectEndLifeCycleTimeData = new EffectEndLifeCycleTimeData(effectEndLifeCycleTimeDefValue);
        if (effectEndLifeCycleTimeData)
        {
            effectEndLifeCycleTimeData->autorelease();
            return effectEndLifeCycleTimeData;
        }
        else
        {
            delete effectEndLifeCycleTimeData;
            effectEndLifeCycleTimeData = nullptr;
            return nullptr;
        }
    };
};

class EffectEndLifeCycleDistanceData : public EffectEndLifeCycleData
{
public:
    EffectEndLifeCycleDistanceData(lofd::EffectEndLifeCycleDistanceDef * effectEndLifeCycleDistanceDefValue):EffectEndLifeCycleData(effectEndLifeCycleDistanceDefValue), currentDistance(0){};
    ~EffectEndLifeCycleDistanceData(){
        effectEndLifeCycleDistanceDef = nullptr;
    };
    
    lofd::EffectEndLifeCycleDistanceDef * effectEndLifeCycleDistanceDef;
    
    float currentDistance;
    
    static EffectEndLifeCycleDistanceData * create(lofd::EffectEndLifeCycleDistanceDef * effectEndLifeCycleDistanceDefValue)
    {
        EffectEndLifeCycleDistanceData * effectEndLifeCycleDistanceData = new EffectEndLifeCycleDistanceData(effectEndLifeCycleDistanceDefValue);
        if (effectEndLifeCycleDistanceData)
        {
            effectEndLifeCycleDistanceData->autorelease();
            return effectEndLifeCycleDistanceData;
        }
        else
        {
            delete effectEndLifeCycleDistanceData;
            effectEndLifeCycleDistanceData = nullptr;
            return nullptr;
        }
    };
};

class EffectEndLifeCycleTargetData : public EffectEndLifeCycleData
{
public:
    EffectEndLifeCycleTargetData(lofd::EffectEndLifeCycleTargetDef * effectEndLifeCycleTargetDefValue):
    EffectEndLifeCycleData(effectEndLifeCycleTargetDefValue)
    {
        effectEndLifeCycleTargetDef = effectEndLifeCycleTargetDefValue;
    };
    ~EffectEndLifeCycleTargetData(){
        effectEndLifeCycleTargetDef = nullptr;
        CC_SAFE_DELETE(target);
    };
    
    lofd::EffectEndLifeCycleTargetDef * effectEndLifeCycleTargetDef;
    
    cocos2d::Point * target;
    
    static EffectEndLifeCycleTargetData * create(lofd::EffectEndLifeCycleTargetDef * effectEndLifeCycleTargetDefValue)
    {
        EffectEndLifeCycleTargetData * effectEndLifeCycleTargetData = new EffectEndLifeCycleTargetData(effectEndLifeCycleTargetDefValue);
        if (effectEndLifeCycleTargetData)
        {
            effectEndLifeCycleTargetData->autorelease();
            return effectEndLifeCycleTargetData;
        }
        else
        {
            delete effectEndLifeCycleTargetData;
            effectEndLifeCycleTargetData = nullptr;
            return nullptr;
        }
    };
};

class EffectEndLifeCycleTrackData : public EffectEndLifeCycleData
{
public:
    EffectEndLifeCycleTrackData(lofd::EffectEndLifeCycleTrackDef * effectEndLifeCycleTrackDefValue):EffectEndLifeCycleData(effectEndLifeCycleTrackDefValue)
    {
        effectEndLifeCycleTrackDef = effectEndLifeCycleTrackDefValue;
    };
    ~EffectEndLifeCycleTrackData(){
        effectEndLifeCycleTrackDef = nullptr;
    };
    
    lofd::EffectEndLifeCycleTrackDef * effectEndLifeCycleTrackDef;
    
    lofd::ActorData * actorData;
    
    static EffectEndLifeCycleTrackData * create(lofd::EffectEndLifeCycleTrackDef * effectEndLifeCycleTrackDefValue)
    {
        EffectEndLifeCycleTrackData * effectEndLifeCycleDTrackData = new EffectEndLifeCycleTrackData(effectEndLifeCycleTrackDefValue);
        if (effectEndLifeCycleDTrackData)
        {
            effectEndLifeCycleDTrackData->autorelease();
            return effectEndLifeCycleDTrackData;
        }
        else
        {
            delete effectEndLifeCycleDTrackData;
            effectEndLifeCycleDTrackData = nullptr;
            return nullptr;
        }
    };
};
/*effectEndLifeCycleTrackDefValue***********/

/************EffectLifeCycleData*************/
class EffectLifeCycleData : public cocos2d::Ref
{
public:
    EffectLifeCycleData(){};
    ~EffectLifeCycleData(){
        //CC_SAFE_DELETE(startLifeCycleData);
        startLifeCycleData->release();
        endLifeCycleDatas.clear();
//        long n = endLifeCycleDatas.size();
//        for (int i = 0; i < n; i ++)
//        {
//            delete endLifeCycleDatas.at(i);
//        }
        
        effectLifeCycleDef = nullptr;
    };
    
    lofd::EffectLifeCycleDef * effectLifeCycleDef;
    
    lofd::EffectStartLifeCycleData * startLifeCycleData;
    cocos2d::Vector<lofd::EffectEndLifeCycleData *> endLifeCycleDatas;
    
    CREATE_REF_FUNC(EffectLifeCycleData);
};
/********************************************/

/**************SkillEffectData****************/
class SkillEffectData : public cocos2d::Ref
{
public:
    SkillEffectData(){};
    ~SkillEffectData(){
        skillEffectDef = nullptr;
        
//        CC_SAFE_DELETE(effectLocationData);
//        CC_SAFE_DELETE(effectMovementData);
//        CC_SAFE_DELETE(effectCoordinateData);
//        CC_SAFE_DELETE(effectViewData);
//        CC_SAFE_DELETE(effectHitData);
        effectLocationData->release();
        effectMovementData->release();
        effectCoordinateData->release();
        effectViewData->release();
        effectHitData->release();
    };
    
    lofd::SkillEffectDef * skillEffectDef;
    
    lofd::EffectLocationData * effectLocationData;
    lofd::EffectMovementData * effectMovementData;
    lofd::EffectCoordinateData * effectCoordinateData;
    
    lofd::EffectViewData * effectViewData;
    lofd::EffectHitData * effectHitData;
    
    CREATE_REF_FUNC(SkillEffectData);
};
/*********************************************/

/**************SkillEffectContextData**********/
class SkillData;
class SkillEffectContextData : public cocos2d::Node
{
public:
    SkillEffectContextData():currentLifeCycle(0), isRunning(false), isComplete(false), isCompleteTag(false){};
    ~SkillEffectContextData(){
        //CC_SAFE_DELETE(skillEffectData);
        //CC_SAFE_DELETE(effectLifeCycleData);
        skillEffectData->release();
        effectLifeCycleData->release();
        
        skillEffectContextDef = nullptr;
        
        skillData = nullptr;
    };
    
    lofd::SkillEffectContextDef * skillEffectContextDef;
    
    lofd::SkillEffectData * skillEffectData;
    lofd::EffectLifeCycleData * effectLifeCycleData;
    
    /*********当前已运行的生命周期时间*********/
    float currentLifeCycle;
    bool isRunning;
    bool isComplete;
    /*********这个函数用来标记当前是否已经完成状态，如果为true，则update下的子函数都不执行********/
    bool isCompleteTag;
    
    void startLifeCycle();
    void endLifeCycle();
    
    virtual void update(float dt);
    
    lofd::SkillData * skillData;
    
    CREATE_FUNC(SkillEffectContextData);
protected:
    /*******locate********/
    virtual void locateInit();
    virtual void locateDispose();
    
    /*******update*********/
    virtual void endLifeCycleTimeUpdate(float dt);//endLifeCycleTime
    virtual void endLifeCyleDistanceUpdate(float dt);//endLifeCycleDistance
    //rocklee:lofd:TODO: endLifeCycleHit
    //virtual void endLifeCycleHitUpdate(float dt);//endLifeCycleHit
    virtual void endLifeCycleTargetUpdate(float dt);//endLifeCycleTarget
    virtual void endLifeCycleTrackUpdate(float dt);//endLifeCycleTrack
    /*******modify hit count(width function *frameHitUpdate* or *framesHitUpdate*)********/
    virtual void modifyEndLifeCycleHit(lofd::ActorData * actorDataValue);
    
    virtual void movementActor(float dt);
    
    virtual void frameHitUpdate(float dt);
    virtual void framesHitUpdate(float dt);
    
    //rocklee:lofd:TODO: effectFrameHit
    //rocklee:lofd:TODO: effectFramesHit
    /*******view************/
    virtual void effectViewFrameInit();
    virtual void effectViewFrameDispose();
    virtual void effectViewFramesInit();
    virtual void effectViewFramesRunAnimation();
    virtual void effectViewFramesDispose();
    virtual void effectViewFrameStopAnimation();
    
};
/**********************************************/

/***************SkillOperationContextData**********/
class SkillOperationContextData : public cocos2d::Ref
{
public:
    SkillOperationContextData(){};
    ~SkillOperationContextData(){
        operationContextDef = nullptr;
    };
    
    lofd::SkillOperationContextDef * operationContextDef;
    
    CREATE_REF_FUNC(SkillOperationContextData);
};

class SkillOperationContextActorData : public SkillOperationContextData
{
public:
    SkillOperationContextActorData(){};
    ~SkillOperationContextActorData(){
        actorData = nullptr;
    };
    
    lofd::ActorData * actorData;
    
    CREATE_REF_FUNC(SkillOperationContextActorData);
};

class SkillOperationContextTargetData : public SkillOperationContextData
{
public:
    SkillOperationContextTargetData(){};
    ~SkillOperationContextTargetData(){
        //CC_SAFE_DELETE(target);
    };
    
    cocos2d::Point target;
    
    CREATE_REF_FUNC(SkillOperationContextTargetData);
};
/**************************************************/

/****************SkillData**********************/
class SkillData : public cocos2d::Node
{
public:
    SkillData():currentLifeCycle(0), isRunning(false), isComplete(false), targetActor(nullptr), actor(nullptr){};
    ~SkillData(){
        skillDef = nullptr;
        
        effectContextDatas.clear();
        
        //CC_SAFE_DELETE(skillOperationContextData);
        skillOperationContextData->release();
        //CC_SAFE_DELETE(skillStateData);
        skillStateData->release();
        
        if (targetActor) targetActor->release();
        actor->release();
    };
    /*******目标******/
    lofd::ActorData * targetActor;
    void setTargetActor(lofd::ActorData * targetActorData);
    /*******释放者*******/
    lofd::ActorData * actor;
    void setActor(lofd::ActorData * actor);
    
    lofd::SkillDef * skillDef;
    lofd::SkillStateData * skillStateData;
    
    cocos2d::Vector<lofd::SkillEffectContextData *> effectContextDatas;
    lofd::SkillOperationContextData * skillOperationContextData;
    
    /*********当前已运行的生命周期时间*********/
    float currentLifeCycle;
    bool isRunning;
    bool isComplete;
    
    virtual void startLifeCycle();
    virtual void endLifeCycle();
    
    /*******检测生命周期的开启*******/
    virtual void update(float dt);
    
    virtual void onSkillEffectContextDataComplete(lofd::SkillEffectContextData * skillEffectContextDataValue);
    
    CREATE_FUNC(SkillData);
};
/***********************************************/
/****************SkillParamContext**************/
class SkillParamContext : public cocos2d::Ref
{
public:
    SkillParamContext(){
        actorData = nullptr;
        mapScene = nullptr;
        targetActorData = nullptr;
    };
    ~SkillParamContext(){
        actorData = nullptr;
        mapScene = nullptr;
        targetActorData = nullptr;
    };
    
    lofd::ActorData * actorData;
    lofd::MapScene * mapScene;
    lofd::ActorData * targetActorData;
    /******目标点*****/
    cocos2d::Point targetPoint;
    
    CREATE_REF_FUNC(SkillParamContext);
};
/***********************************************/
/****************SkillDataUitls*****************/
class SkillDataUtils
{
public:
    static lofd::SkillData * createSkillData(lofd::SkillDef * skillDefValue, lofd::SkillParamContext * skillParamContext);
    
    static lofd::SkillOperationContextData * createSkillOperationContextData(lofd::SkillOperationContextDef * skillOperationContextDefValue, lofd::SkillParamContext * skillParamContext);
    
    static lofd::SkillEffectContextData * createSkillEffectContextData(lofd::SkillEffectContextDef * skillEffectContextDefValue, lofd::SkillParamContext * skillParamContext);
                                                                       
    static lofd::SkillEffectData * createSkillEffectData(lofd::SkillEffectDef * skillEffectDefValue, lofd::SkillParamContext * skillParamContext);
                                                                       
    static lofd::EffectLifeCycleData * createEffectLifeCycleData(lofd::EffectLifeCycleDef * effectLifeCycleDefValue, lofd::SkillParamContext * skillParamContext);
                                                                       
    static lofd::EffectEndLifeCycleData * createEffectEndLifeCycleData(lofd::EffectEndLifeCycleDef * effectEndLifeCycleDefValue, lofd::SkillParamContext * skillParamContext);
    
    static lofd::EffectStartLifeCycleData * createEffectStartLifeCycleData(lofd::EffectStartLifeCycleDef * effectStartLifeCycleDefValue, lofd::SkillParamContext * skillParamContext);
    
    static lofd::EffectHitData * createEffectHitData(lofd::EffectHitDef * effectHitDefValue, lofd::SkillParamContext * skillParamContext);
    
    static lofd::EffectViewData * createEffectViewData(lofd::EffectViewDef * effectViewDefValue, lofd::SkillParamContext * skillParamContext);
    
    static lofd::EffectCoordinateData * createEffectCoordinateData(lofd::EffectCoordinateDef * effectCoordinateDefValue, lofd::SkillParamContext * skillParamContext);
    
    static lofd::EffectMovementData * createEffectMovementData(lofd::EffectMovementDef * effectMovementDefValue, lofd::SkillParamContext * skillParamContext);
    
    static lofd::EffectLocationData * createEffectLocationData(lofd::EffectLocationDef * effectLocationDefValue, lofd::SkillParamContext * skillParamContext);
    
    static lofd::SkillStateData * createSkillStateData(lofd::SkillStateDef * skillStateDefValue, lofd::SkillParamContext * skillParamContext);
    
    static lofd::EffectOutputData * createEffectOutputData(lofd::EffectOutputDef * effectOutputDefValue, lofd::SkillParamContext * skillParamContext);
    
    static int checkHasEndLifeCycletype(lofd::EffectLifeCycleDef * effectLifeCycleDefValue, std::string checkType);
};
/***********************************************/
NS_LOFD_END;
#endif /* defined(__LengendOfFightDemons__LOFDSkillData__) */
