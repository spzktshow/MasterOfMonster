//
//  LOFDSkill.h
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-17.
//
//

#ifndef __LengendOfFightDemons__LOFDSkill__
#define __LengendOfFightDemons__LOFDSkill__

#include "lengendOfFightDemons.h"
#include <vector>
#include "LOFDScene.h"
#include "external/json/rapidjson.h"
#include "external/json/document.h"

NS_LOFD_BEGIN
#define SKILL_NORMAL            0
#define SKILL_1                 1
#define SKILL_2                 2
#define SKILL_3                 3
#define SKILL_4                 1

#define SKILL_CONFIG_TAG_LOCATION_TYPE          "locationType"
#define SKILL_CONFIG_TAG_RELATIVE_POINT         "relativePoint"
#define SKILL_CONFIG_TAG_MOVEMENT_TYPE          "movementType"
#define SKILL_CONFIG_TAG_SPEED                  "speed"
#define SKILL_CONFIG_TAG_COORDINATE_TYPE        "coordinateType"
#define SKILL_CONFIG_TAG_ANIMATION_TYPE         "animationType"
#define SKILL_CONFIG_TAG_FRAME_NAME             "frameName"
#define SKILL_CONFIG_TAG_ANCHOR_POINT           "anchorPoint"
#define SKILL_CONFIG_TAG_FRAMES                 "frames"
#define SKILL_CONFIG_TAG_DELAY                  "delay"
#define SKILL_CONFIG_TAG_FRAME_INDEX            "frameIndex"
#define SKILL_CONFIG_TAG_HIT_RECTS              "hitRects"
#define SKILL_CONFIG_TAG_HIT_TYPE               "hitType"
#define SKILL_CONFIG_TAG_EFFECT_HIT_FRAME_DEF   "effectHitFrameDef"
#define SKILL_CONFIG_TAG_TYPE                   "type"
#define SKILL_CONFIG_TAG_DEPEND_ID              "dependId"
#define SKILL_CONFIG_TAG_START_INTERVAL         "startInterval"
#define SKILL_CONFIG_TAG_HIT_LIMIT              "hitLimit"
#define SKILL_CONFIG_TAG_TIME                   "time"
#define SKILL_CONFIG_TAG_DISTANCE               "distance"
#define SKILL_CONFIG_TAG_EFFECT_START_LIFE_CYCLE_DEF    "effectStartLifeCycleDef"
#define SKILL_CONFIG_TAG_EFFECT_END_LIFE_CYCLE_DEFS     "effectEndLifeCycelDefs"
#define SKILL_CONFIG_TAG_EFFECT_ID              "effectId"
#define SKILL_CONFIG_TAG_EFFECT_NAME            "effectName"
#define SKILL_CONFIG_TAG_LOCATION_DEF           "locationDef"
#define SKILL_CONFIG_TAG_MOVEMENT_DEF           "movementDef"
#define SKILL_CONFIG_TAG_COORDINATE_DEF         "coordinateDef"
#define SKILL_CONFIG_TAG_VIEW_DEF               "viewDef"
#define SKILL_CONFIG_TAG_HIT_DEF                "hitDef"
#define SKILL_CONFIG_TAG_SKILL_EFFECT_DEF       "skillEffectDef"
#define SKILL_CONFIG_TAG_EFFECT_LICE_CYCLE_DEF  "effectLifeCycleDef"
#define SKILL_CONFIG_TAG_OPERATION_TYPE         "operationType"
#define SKILL_CONFIG_TAG_R                      "r"
#define SKILL_CONFIG_TAG_RECT                   "rect"
#define SKILL_CONFIG_TAG_SKILL_DEFS             "skillDefs"
#define SKILL_CONFIG_TAG_SKILL_ID               "skillId"
#define SKILL_CONFIG_TAG_STATE_DEF              "stateDef"
#define SKILL_CONFIG_TAG_EFFECT_CONTEXT_DEFS    "effectContextDefs"
#define SKILL_CONFIG_TAG_OPERATION_CONTEXT_DEF  "operationContextDef"
#define SKILL_CONFIG_TAG_SKILL_RANG_DEF       "skillRangeDef"
#define SKILL_CONFIG_TAG_SKILL_STATE_TAG        "tag"
#define SKILL_CONFIG_TAG_X                      "x"
#define SKILL_CONFIG_TAG_Y                      "y"
#define SKILL_CONFIG_TAG_WIDTH                  "width"
#define SKILL_CONFIG_TAG_HEIGHT                 "height"
#define SKILL_CONFIG_TAG_COOL_DOWN              "coolDown"
#define SKILL_CONFIG_TAG_FORMULA_ID             "formulaId"
#define SKILL_CONFIG_TAG_EFFECT_OUTPUT_DEF      "effectOutputDef"
#define SKILL_CONFIG_TAG_IS_AUTO                "isAuto"
#define SKILL_CONFIG_TAG_OUTPUT_TARGET_TYPE     "outputTargetType"
#define SKILL_CONFIG_TAG_SKILL_EFFECT_DEFS      "skillEffectDefs"
#define SKILL_CONFIG_TAG_SKILL_RANDOM_CONTEXTS  "skillRandomContexts"

/***********SkillConfig start**************/
class MapScene;
class SkillUtils;

/******
 **SkillStateDef**
 **这个数据结构控制技能附带的人物tag状态
 ****/
class SkillStateDef : public cocos2d::Ref
{
public:
    SkillStateDef(){};
    ~SkillStateDef(){};
    /*******state tag*******/
    //std::string tag;
    /*****随机tags*******/
    std::vector<std::string> tags;
};

/********技能位置(LOCATION_DEF)********/
/************location type**********/
#define EFFECT_LOCATION_TYPE_RELATIVE      "relative"/****相对位置***/
#define EFFECT_LOCATION_TYPE_TARGET        "target"/****目标点***/
/***********************************/
class EffectLocationDef : public cocos2d::Ref
{
public:
    EffectLocationDef(){};
    ~EffectLocationDef(){
    };
    
    /*******位置类型*******/
    std::string locationType;
};
/********特效相对位置的定义********/
class EffectRelativeLocationDef : public EffectLocationDef
{
public:
    EffectRelativeLocationDef(){};
    ~EffectRelativeLocationDef(){
        delete relativePoint;
        relativePoint = nullptr;
    };
    
    cocos2d::Point * relativePoint;
};
/********特效目标点位置定义**********/
class EffectTargetLocationDef : public EffectLocationDef
{
public:
    EffectTargetLocationDef(){};
    ~EffectTargetLocationDef(){
    };
};
/***********************/

/********(MOVEMENT_DEF)************/
/************movement type**********/
#define EFFECT_MOVEMENT_TYPE_STILL         "still"/******静止******/
#define EFFECT_MOVEMENT_TYPE_TARGET        "target"/******目标点(控制生命周期)******/
#define EFFECT_MOVEMENT_TYPE_TRACK         "track"/****跟踪(控制生命周期)*****/
#define EFFECT_MOVEMENT_TYPE_VECTOR        "vector"/****矢量(控制生命周期)****/
/***********************************/
class EffectMovementDef : public cocos2d::Ref
{
public:
    EffectMovementDef(){};
    ~EffectMovementDef(){};
    
    /*******移动类型*******/
    std::string movementType;
};

class EffectMovementTrackDef : public EffectMovementDef
{
public:
    EffectMovementTrackDef(){};
    ~EffectMovementTrackDef(){};
    
    float speed;
};

class EffectMovementVectorDef : public EffectMovementDef
{
public:
    EffectMovementVectorDef(){};
    ~EffectMovementVectorDef(){};
    
    float speed;
};

/**********************************/

/**************(EFFECT_COORDINATE_DEF)******************/
/********coordinate type********/
#define EFFECT_COORDINATE_TYPE_ACTOR        "actor"/*****坐标系为人物容器******/
#define EFFECT_COORDINATE_TYPE_SCENE        "scene"/*****坐标系为场景*********/
#define EFFECT_COORDINATE_TYPE_SELF_ACTOR   "selfActor"/******自身人物容器*****/
/******************************/
class EffectCoordinateDef : public cocos2d::Ref
{
public:
    EffectCoordinateDef(){};
    ~EffectCoordinateDef(){};
    
    /******坐标系类型********/
    std::string coordinateType;
};

/******(VIEW_DEF)************/
/*************animation type************/
#define EFFECT_VIEW_TYPE_NONE          "none"/*******无动画********/
#define EFFECT_VIEW_TYPE_FRAME         "frame"/********单帧********/
#define EFFECT_VIEW_TYPE_FRAMES        "frames"/*******帧动画********/
/***************************************/
class EffectViewDef : public cocos2d::Ref
{
public:
    EffectViewDef(){};
    ~EffectViewDef(){};
    
    std::string animationType;
};

class EffectFrameViewDef : public EffectViewDef
{
public:
    EffectFrameViewDef(){};
    ~EffectFrameViewDef(){};
    
    std::string frameName;
    
    cocos2d::Point * anchorPoint;
};

class EffectFramesViewDef : public EffectFrameViewDef
{
public:
    EffectFramesViewDef():frames(0){};
    ~EffectFramesViewDef(){};
    
    int frames;
    /*****动画播放间隔*****/
    float delay;
};
/************************************/

/*********技能特效单帧数据结构********/
class EffectHitFrameDef : public cocos2d::Ref
{
public:
    EffectHitFrameDef():frameIndex(0){};
    ~EffectHitFrameDef(){
        for (int i = 0; i < hitRects.size(); i++) {
            delete hitRects.at(i);
        }
        hitRects.clear();
    };
    /******帧数索引*******/
    float frameIndex;
    /******单帧技能特效的碰撞矩形组******/
    std::vector<cocos2d::Rect *> hitRects;
};

/*********(HIT_DEF)****************/
/**********hit type*************/
#define EFFECT_HIT_TYPE_NONE             "none"
#define EFFECT_HIT_TYPE_FRAME            "frame"
#define EFFECT_HIT_TYPE_FRAMES           "frames"
/*******************************/
class EffectHitDef : public cocos2d::Ref
{
public:
    EffectHitDef(){};
    ~EffectHitDef(){};
    
    std::string hitType;
};

class EffectFrameHitDef : public EffectHitDef
{
public:
    EffectFrameHitDef(){};
    ~EffectFrameHitDef(){
        delete effectHitFrameDef;
    };
    
    lofd::EffectHitFrameDef * effectHitFrameDef;
};

class EffectFramesHitDef : public EffectHitDef
{
public:
    EffectFramesHitDef(){};
    ~EffectFramesHitDef(){
        frames.clear();
    };
    
    cocos2d::Vector<lofd::EffectHitFrameDef *> frames;
    /*****帧计算间隔*****/
    float delay;
};
/********************************/
/*************
 ***特效产出定义***
 ****/
/*******EffectHarmDef*********************/
//type
#define EFFECT_OUTPUT_REDUCE_BLOOD                "reduceBlood"
#define EFFECT_OUTPUT_ADDTION_BLOOD               "addtionBlood"
#define EFFECT_OUTPUT_NONE                        "none"
//outputTargetType
#define EFFECT_OUTPUT_TARGET_TYPE_ENEMY                 "enemy"//敌方campValue<0
#define EFFECT_OUTPUT_TARGET_TYPE_FRIENDLY              "friendly"//友方campValue>0
#define EFFECT_OUTPUT_TARGET_TYPE_ALL                   "all"//所有人，不判断campValue值
class EffectOutputDef : public cocos2d::Ref
{
public:
    EffectOutputDef(){};
    ~EffectOutputDef(){};
    
    /********伤害类型*******/
    std::string type;
    /********作用目标类型********/
    std::string outputTargetType;
    /******公式id*****/
    int formulaId;
};
/*****************************************/
/**********SkillLifeCycleDef*********/
#define EFFECT_START_LIFE_CYCLE_DEPEND         "depend"/****依赖其他技能的生命周期****/
#define EFFECT_START_LIFE_CYCLE_INTERVAL       "interval"/****区间****/
class EffectStartLifeCycleDef : public cocos2d::Ref
{
public:
    EffectStartLifeCycleDef(){};
    ~EffectStartLifeCycleDef(){};
    
    std::string type;
    
    /******特效结束伤害检测******/
    lofd::EffectOutputDef * effectOutputDef;
};
class EffectStartLifeCycleDependDef : public lofd::EffectStartLifeCycleDef
{
public:
    EffectStartLifeCycleDependDef(){};
    ~EffectStartLifeCycleDependDef(){};
    /******dependId*******/
    int dependId;
};

class EffectStartLifeCycleIntervalDef : public lofd::EffectStartLifeCycleDef
{
public:
    EffectStartLifeCycleIntervalDef(){};
    ~EffectStartLifeCycleIntervalDef(){};
    
//    /******单个特效的生命周期
//     , 该值可以为负
//     ，表示技能特效无正常生命周期
//     ，只有外部条件能够结束周期******/
//    float lifeCycle;
    /******区间开始的值, 该值是必填的, 表示特效开始生命周期的时间*********/
    float startInterval;
};
/******************/
/*******EffectEndLifeCycleDef**********/
#define EFFECT_END_LIFE_CYCLE_HIT               "hit"
#define EFFECT_END_LIFE_CYCLE_TIME              "time"
#define EFFECT_END_LIFE_CYCLE_DISTANCE          "distance"
#define EFFECT_END_LIFE_CYCLE_TARGET            "target"
#define EFFECT_END_LIFE_CYCLE_TRACK             "track"
class EffectEndLifeCycleDef : public cocos2d::Ref
{
public:
    EffectEndLifeCycleDef(){};
    ~EffectEndLifeCycleDef(){};
    
    std::string type;
    
    /******特效结束伤害检测******/
    lofd::EffectOutputDef * effectOutputDef;
};

class EffectEndLifeCycleTrackDef : public EffectEndLifeCycleDef
{
public:
    EffectEndLifeCycleTrackDef(){};
    ~EffectEndLifeCycleTrackDef(){};
};

class EffectEndLifeCycleHitDef : public EffectEndLifeCycleDef
{
public:
    EffectEndLifeCycleHitDef(){};
    ~EffectEndLifeCycleHitDef(){};
    
    /******碰撞限制次数******/
    int hitLimit;
};

class EffectEndLifeCycleTimeDef : public EffectEndLifeCycleDef
{
public:
    EffectEndLifeCycleTimeDef(){};
    ~EffectEndLifeCycleTimeDef(){};
    
    /*****持续时间(秒)***********/
    float time;
};

class EffectEndLifeCycleDistanceDef : public EffectEndLifeCycleDef
{
public:
    EffectEndLifeCycleDistanceDef(){};
    ~EffectEndLifeCycleDistanceDef(){};
    
    /*****移动的距离*****/
    float distance;
};

class EffectEndLifeCycleTargetDef : public EffectEndLifeCycleDef
{
public:
    EffectEndLifeCycleTargetDef(){};
    ~EffectEndLifeCycleTargetDef(){};
};
/*************************************/
/*********EffectLifeCycle*************/
class EffectLifeCycleDef : public cocos2d::Ref
{
public:
    EffectLifeCycleDef(){};
    ~EffectLifeCycleDef(){
        CC_SAFE_DELETE(effectStartLifeCycleDef);
        effectEndLifeCycelDefs.clear();
    };
    
    lofd::EffectStartLifeCycleDef * effectStartLifeCycleDef;
    cocos2d::Vector<lofd::EffectEndLifeCycleDef *> effectEndLifeCycelDefs;
};
/*************************************/
/****SkillEffectDef***
 **这个数据结构控制技能特效的定义**
 ******/
class SkillEffectDef : public cocos2d::Ref
{
public:
    SkillEffectDef(){};
    ~SkillEffectDef(){
        CC_SAFE_DELETE(locationDef);
        CC_SAFE_DELETE(movementDef);
        CC_SAFE_DELETE(coordinateDef);
        
        CC_SAFE_DELETE(viewDef);
        CC_SAFE_DELETE(hitDef);
    };
    
    int effectId;
    std::string effectName;
    
    /*****位置定义*****/
    lofd::EffectLocationDef * locationDef;
    /*****特效移动模式****/
    lofd::EffectMovementDef * movementDef;
    /*****特效坐标系*****/
    lofd::EffectCoordinateDef * coordinateDef;
    
    
    /*****特效view定义*****/
    lofd::EffectViewDef * viewDef;
    /*****特效碰撞区域定义*****/
    lofd::EffectHitDef * hitDef;
};

class SkillEffectContextDef : public cocos2d::Ref
{
public:
    SkillEffectContextDef(){};
    ~SkillEffectContextDef(){
        CC_SAFE_DELETE(skillEffectDef);
        CC_SAFE_DELETE(effectLifeCycleDef);
    };
    
    lofd::SkillEffectDef * skillEffectDef;
    lofd::EffectLifeCycleDef * effectLifeCycleDef;
};

/**************技能操作定义*******************/
#define SKILL_OPERATION_TYPE_NONE             "none"/****瞬时技能，不需要目标选择****/
#define SKILL_OPERATION_TYPE_ACTOR            "actor"/****人物目标技能*************/
#define SKILL_OPERATION_TYPE_TARGET           "target"/*****目标点技能*************/
class SkillOperationContextDef : public cocos2d::Ref
{
public:
    SkillOperationContextDef(){};
    ~SkillOperationContextDef(){};
    
    std::string operationType;
};
/*********************************************/

/**************技能攻击范围********************/
#define SKILL_RANGE_TYPE_R                  "r"
#define SKILL_RANGE_TYPE_RECT               "rect"
class SkillRangeDef : public cocos2d::Ref
{
public:
    SkillRangeDef(){};
    ~SkillRangeDef(){};
    
    std::string type;
};

class SkillRangeRDef : public SkillRangeDef
{
public:
    SkillRangeRDef(){};
    ~SkillRangeRDef(){};
    
    float r;
};

class SkillRangeRectDef : public SkillRangeDef
{
public:
    SkillRangeRectDef(){};
    ~SkillRangeRectDef(){
        CC_SAFE_DELETE(rect);
    };
    
    cocos2d::Rect * rect;
};
/********************************************/

/*************技能随机上下文*************/
class SkillRandomContext : public cocos2d::Ref
{
public:
    SkillRandomContext(){};
    ~SkillRandomContext(){
        CC_SAFE_DELETE(stateDef);
    };
    
    /******释放技能时人物状态的定义******/
    lofd::SkillStateDef * stateDef;
    /******释放技能时触发的特效定义列表, 特效未必只有一个******/
    cocos2d::Vector<lofd::SkillEffectContextDef *> effectContextDefs;
};

/********SkillDef***
 **单个技能的配置数据**
 *******/
class SkillDef : public cocos2d::Ref
{
public:
    SkillDef(){};
    ~SkillDef(){
        //CC_SAFE_DELETE(stateDef);
        CC_SAFE_DELETE(operationContextDef);
    };
    
    int skillId;
    
    /******释放技能的操作模式***********/
    lofd::SkillOperationContextDef * operationContextDef;
    /******技能范围定义****************/
    lofd::SkillRangeDef * skillRangeDef;
    /******技能cd********/
    float coolDown;
    /******is auto(技能自动)*******/
    bool isAuto;
    /*******随机不同特效********/
    cocos2d::Vector<lofd::SkillRandomContext *> skillRandomContexts;
};

class SkillConfig : public cocos2d::Ref
{
public:
    SkillConfig(){};
    ~SkillConfig(){
    };
    
    cocos2d::Vector<lofd::SkillDef *> skillDefs;
    cocos2d::Vector<lofd::SkillEffectContextDef *> skillEffectContextDefs;
    
    lofd::SkillDef * getSkillDefById(int skillId);
    lofd::SkillEffectContextDef * getSkillEffectContextDefById(int effectId);
    /******解析*******/
    void parse(std::string data);
    void parseEffects(std::string data);
    lofd::SkillStateDef * parseSkillStateDef(const rapidjson::Value & skillStateDefValue);
    
    lofd::SkillEffectContextDef * parseSkillEffectContextDef(const rapidjson::Value & skillEffectContextDefValue);
    lofd::SkillEffectDef * parseSkillEffectDef(const rapidjson::Value & skillEffectDefValue);
    
    lofd::EffectLifeCycleDef * parseEffectLifeCycleDef(const rapidjson::Value & effectLifeCycleDefValue);
    lofd::EffectStartLifeCycleDef * parseEffectStartLifeCycleDef(const rapidjson::Value & effectStartLifeCycleDefValue);
    lofd::EffectEndLifeCycleDef * parseEffectEndLifeCycleDef(const rapidjson::Value & effectEndLifeCycleDefValue);
    
    lofd::EffectHitDef * parseEffectHitDef(const rapidjson::Value & effectHitDefValue);
    //lofd::EffectFrameHitDef * parseEffectFrameHitDef(const rapidjson::Value & EffectFrameHitDefValue);
    lofd::EffectHitFrameDef * parseEffectHitFrameDef(const rapidjson::Value & effectHitFrameDefValue);
    
    lofd::EffectViewDef * parseEffectViewDef(const rapidjson::Value & effectViewDefValue);
    lofd::EffectCoordinateDef * parseEffectCoordinateDef(const rapidjson::Value & effectCoordinateDefValue);
    lofd::EffectMovementDef * parseEffectMovementDef(const rapidjson::Value & effectMovementDefValue);
    lofd::EffectLocationDef * parseEffectLocationDef(const rapidjson::Value & effectLocationDefValue);
    
    lofd::SkillOperationContextDef * parseSkillOperationContextDef(const rapidjson::Value & skillOperationContextDefValue);
    lofd::SkillRangeDef * parseSkillRangeDef(const rapidjson::Value & skillRangeDefValue);
    
    lofd::EffectOutputDef * parseEffectOutputDef(const rapidjson::Value & effectOutputDefValue);
};
/*****SkillConfig End************/
NS_LOFD_END;

#endif /* defined(__LengendOfFightDemons__LOFDSkill__) */
