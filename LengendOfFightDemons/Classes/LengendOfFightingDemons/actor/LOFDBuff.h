//
//  LOFDBuff.h
//  LengendOfFightDemons
//
//  Created by RockLee on 14-6-5.
//
//

#ifndef __LengendOfFightDemons__LOFDBuff__
#define __LengendOfFightDemons__LOFDBuff__

#include "lengendOfFightDemons.h"
#include "moonSugar.h"

/***************BuffLifeCircle***************/
#define BUFF_LIFE_CIRCLE_TYPE_NONE          "none"/*****无生命周期********/
#define BUFF_LIFE_CIRCLE_TYPE_TIME          "time"/*****有效时间内结束*****/
#define BUFF_LIFE_CIRCLE_TYPE_PRODUCE_COUNT "produceCount"/********产出次数********/

class BuffLifeCircleDef : public cocos2d::Ref
{
public:
    BuffLifeCircleDef(){};
    ~BuffLifeCircleDef(){};
    
    std::string type;
    
    CREATE_REF_FUNC(BuffLifeCircleDef);
};

class BuffLifeCircleNoneDef : public cocos2d::Ref
{
public:
    BuffLifeCircleNoneDef(){};
    ~BuffLifeCircleNoneDef(){};
    
    CREATE_REF_FUNC(BuffLifeCircleNoneDef);
};

class BuffLifeCircleTimeDef : public cocos2d::Ref
{
public:
    BuffLifeCircleTimeDef(){};
    ~BuffLifeCircleTimeDef(){};
    
    /********持续时间*******/
    float time;
    
    CREATE_REF_FUNC(BuffLifeCircleTimeDef);
};

class BuffLifeCircleProduceCount : public cocos2d::Ref
{
public:
    BuffLifeCircleProduceCount(){};
    ~BuffLifeCircleProduceCount(){};
    
    /*********产出次数********/
    int produceCount;
    
    CREATE_REF_FUNC(BuffLifeCircleProduceCount);
};

/*************BuffProduceType********************/
#define BUFF_PRODUCE_TYPE_GAP_TIME          "gapTime"
#define BUFF_PRODUCE_TYPE_IMMEDIATELY_ONCE  "immediatelyOnce"

class BuffProduceType : public cocos2d::Ref
{
public:
    BuffProduceType(){};
    ~BuffProduceType(){};
    
    std::string type;
    
    CREATE_REF_FUNC(BuffProduceType);
};

class BuffProduceTypeGapTime : public cocos2d::Ref
{
public:
    BuffProduceTypeGapTime():gapTime(-1.0f){};
    ~BuffProduceTypeGapTime(){};
    
    float gapTime;
    
    CREATE_REF_FUNC(BuffProduceTypeGapTime);
};

class BuffProduceTypeImmediatelyOnce : public cocos2d::Ref
{
public:
    BuffProduceTypeImmediatelyOnce(){};
    ~BuffProduceTypeImmediatelyOnce(){};
    
    CREATE_REF_FUNC(BuffProduceTypeImmediatelyOnce);
};

/***********BuffProduceValueType***************/
class BuffProduceValueType : public cocos2d::Ref
{
public:
    BuffProduceValueType():base(0), coefficient(0){};
    ~BuffProduceValueType(){};
    
    CREATE_REF_FUNC(BuffProduceValueType);
    
    float base;
    float coefficient;
};

/*******Buff定义*******/
#define BUFF_TYPE_ADDTION_TYPE      "buffTypeAddtionType"/******增加type类型********/
#define BUFF_TYPE_STOP_EVENT        "buffTypeStopEvent"/********阻断事件类型********/
#define BUFF_TYPE_DAMAGE_TYPE       "buffTypeDamageType"/*********伤害类型**********/
#define BUFF_TYPE_CURE_TYPE         "buffTypeCureType"/************治疗类型*********/

class BuffDef : public cocos2d::Ref
{
public:
    BuffDef():buffId(-1){};
    ~BuffDef(){};
    
    CREATE_REF_FUNC(BuffDef);
    
    int buffId;
    std::string type;
};

/**********BuffData********/
class BuffData : public cocos2d::Node
{
public:
    BuffData():buffDef(nullptr){};
    ~BuffData(){};
    
    BuffDef * buffDef;
    
    CREATE_FUNC(BuffData);
};


#endif /* defined(__LengendOfFightDemons__LOFDBuff__) */
