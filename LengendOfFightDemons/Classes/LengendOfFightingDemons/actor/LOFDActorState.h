//
//  LOFDActorState.h
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-8.
//
//

#ifndef __LengendOfFightDemons__LOFDActorState__
#define __LengendOfFightDemons__LOFDActorState__

#include "lengendOfFightDemons.h"
#include "MSDataStructure.h"
#include "BehaviorTree.h"

#define LOFD_ACTOR_STATE_CONFIG_TAG_ACTORS          "datas"
#define LOFD_ACTOR_STATE_CONFIG_TAG_NAME            "name"
#define LOFD_ACTOR_STATE_CONFIG_TAG_ID              "id"
#define LOFD_ACTOR_STATE_CONFIG_TAG_STATES          "states"
#define LOFD_ACTOR_STATE_CONFIG_TAG_STATE_GROUPS    "stateGroups"
#define LOFD_ACTOR_STATE_CONFIG_TAG_FRAMES          "frames"
#define LOFD_ACTOR_STATE_CONFIG_TAG_RELATIVE_POINT  "relativePoint"
#define LOFD_ACTOR_STATE_CONFIG_TAG_TAG             "tag"
#define LOFD_ACTOR_STATE_CONFIG_TAG_RECT            "rect"
#define LOFD_ACTOR_STATE_CONFIG_TAG_X               "x"
#define LOFD_ACTOR_STATE_CONFIG_TAG_Y               "y"
#define LOFD_ACTOR_STATE_CONFIG_TAG_WIDTH           "width"
#define LOFD_ACTOR_STATE_CONFIG_TAG_HEIGHT          "height"
#define LOFD_ACTOR_STATE_CONFIG_TAG_BHAVIOR_ID      "behaviorId"
#define LOFD_ACTOR_STATE_CONFIG_TAG_AI_ID           "aiId"
#define LOFD_ACTOR_STATE_CONFIG_TAG_ROUND           "round"
#define LOFD_ACTOR_STATE_CONFIG_TAG_CAMP_ID         "campId"
#define LOFD_ACTOR_STATE_CONFIG_TAG_STATE_ID        "stateId"
#define LOFD_ACTOR_STATE_CONFIG_TAG_WARN_ROUND      "warnRound"
#define LOFD_ACTOR_STATE_CONFIG_TAG_ATK             "atk"
#define LOFD_ACTOR_STATE_CONFIG_TAG_DEF             "def"
#define LOFD_ACTOR_STATE_CONFIG_TAG_HP              "hp"
#define LOFD_ACTOR_STATE_CONFIG_TAG_SKILLIDS        "skillIds"
#define LOFD_ACTOR_STATE_CONFIG_TAG_STATE_MATCH     "stateMatch"
#define LOFD_ACTOR_STATE_CONFIG_TAG_MATCH_TAG       "matchTag"

/******new hero config*********/
#define LOFD_ACTOR_STATE_CONFIG_TAG_HERO_ID         "hero_id"
#define LOFD_ACTOR_STATE_CONFIG_TAG_HERO_NAME       "hero_name"
#define LOFD_ACTOR_STATE_CONFIG_TAG_HALF_BODY       "half_body"
#define LOFD_ACTOR_STATE_CONFIG_TAG_ALL_BODY        "all_body"
#define LOFD_ACTOR_STATE_CONFIG_TAG_HEAD_ICON       "head_icon"
#define LOFD_ACTOR_STATE_CONFIG_TAG_JOB_ID          "job_id"
#define LOFD_ACTOR_STATE_CONFIG_TAG_JOB_NAME        "job_name"
#define LOFD_ACTOR_STATE_CONFIG_TAG_BEHAVIOR_ID     "behavior_id"
#define LOFD_ACTOR_STATE_CONFIG_TAG_AI__ID           "ai_id"
#define LOFD_ACTOR_STATE_CONFIG_TAG_CAMP__ID         "camp_id"
#define LOFD_ACTOR_STATE_CONFIG_TAG_TYPE            "type"
#define LOFD_ACTOR_STATE_CONFIG_TAG_SEX             "sex"
#define LOFD_ACTOR_STATE_CONFIG_TAG_COLOUR          "colour"
#define LOFD_ACTOR_STATE_CONFIG_TAG_CATE            "cate"
#define LOFD_ACTOR_STATE_CONFIG_TAG_NEXT_ID         "next_id"
#define LOFD_ACTOR_STATE_CONFIG_TAG_LAST_ID         "last_id"
#define LOFD_ACTOR_STATE_CONFIG_TAG_NEED_RES        "need_res"
#define LOFD_ACTOR_STATE_CONFIG_TAG_NEED_ITEM       "need_item"
#define LOFD_ACTOR_STATE_CONFIG_TAG_UP_STR          "up_str"
#define LOFD_ACTOR_STATE_CONFIG_TAG_UP_INT          "up_int"
#define LOFD_ACTOR_STATE_CONFIG_TAG_UP_AGI          "up_agi"
#define LOFD_ACTOR_STATE_CONFIG_TAG_UP_CON          "up_con"
#define LOFD_ACTOR_STATE_CONFIG_TAG_MOVE_SPEED      "move_speed"
#define LOFD_ACTOR_STATE_CONFIG_TAG_ATK_SPEED       "atk_speed"
#define LOFD_ACTOR_STATE_CONFIG_TAG_SCOUT_RANGE     "scout_range"
#define LOFD_ACTOR_STATE_CONFIG_TAG_ATK_RANGE       "atk_range"
#define LOFD_ACTOR_STATE_CONFIG_TAG_BASE_CH         "base_ch"
#define LOFD_ACTOR_STATE_CONFIG_TAG_BASE_CRI        "base_cri"
#define LOFD_ACTOR_STATE_CONFIG_TAG_BASE_BR         "base_br"
#define LOFD_ACTOR_STATE_CONFIG_TAG_BASE_DR         "base_dr"
#define LOFD_ACTOR_STATE_CONFIG_TAG_BASE_ER         "base_er"
#define LOFD_ACTOR_STATE_CONFIG_TAG_BASE_TR         "base_tr"
#define LOFD_ACTOR_STATE_CONFIG_TAG_ARMS            "arms"
#define LOFD_ACTOR_STATE_CONFIG_TAG_ARMOR           "armor"
#define LOFD_ACTOR_STATE_CONFIG_TAG_INTRO           "intro"
#define LOFD_ACTOR_STATE_CONFIG_TAG_INTRO_FEATURE   "intro_feature"
#define LOFD_ACTOR_STATE_CONFIG_TAG_SKILL_ID        "skill_id"

NS_LOFD_BEGIN

class StateDef : public cocos2d::Ref
{
public:
    StateDef(){};
    ~StateDef(){};
    
    std::string type;
    
    CREATE_REF_FUNC(StateDef);
};

class ActorStateDef : public cocos2d::Ref
{
public:
    ActorStateDef();
    ~ActorStateDef();
    
    std::string tag;
    int frames;
    cocos2d::Point * relativePoint;
    cocos2d::Rect * rect;
    int round;
    
    CREATE_REF_FUNC(ActorStateDef);
};

class ActorStateMatch : public cocos2d::Ref
{
public:
    ActorStateMatch(){};
    ~ActorStateMatch(){};
    
    std::string tag;
    std::string matchTag;
    
    CREATE_REF_FUNC(ActorStateMatch);
};

class ActorStateGroupDef : public cocos2d::Ref
{
public:
    ActorStateGroupDef();
    ~ActorStateGroupDef();
    
    int stateId;
    cocos2d::Vector<lofd::ActorStateDef *> states;
    cocos2d::Vector<lofd::ActorStateMatch *> stateMatch;
    
    lofd::ActorStateDef * getActorStateDefByTag(std::string actionTag);
    /********通过match表取到ActorStateDef,如果match中找不到就直接去states中找*********/
    lofd::ActorStateDef * getActorStateMatchByTag(std::string actionTag);
    
    CREATE_REF_FUNC(ActorStateGroupDef);
};

class ActorStateConfig : public cocos2d::Ref
{
public:
    ActorStateConfig();
    ~ActorStateConfig();
    
    cocos2d::Vector<lofd::ActorStateGroupDef *> stateGroups;
    
    void parse(std::string dataValue);
    
    lofd::ActorStateGroupDef * getActorStateGroupDefByStateId(int stateId);
};

/*********随机技能数据结构*******/
class ActorRandomSkillGroupDef : public cocos2d::Ref
{
public:
    ActorRandomSkillGroupDef(){};
    ~ActorRandomSkillGroupDef(){
    };
    
    std::vector<int> skillIds;
};

class ActorDef : public cocos2d::Ref
{
public:
    ActorDef();
    ~ActorDef();
    
    int actorId;/****hero_id(人物id)******/
    std::string name;/****hero_name(人物名称)****/
    int half_body;/*****(半身id)******/
    int all_body;/*****(美术资源id)******/
    int head_icon;/******(头像ID)*****/
    int stateId;/*****job_id(动作资源id)*****/
    std::string job_name;/*****(职业名称)*******/
    int behaviorId;/****behavior_id(动作 id)*****/
    int aiId;/*****ai_id(ai id)******/
    int campId;/*****camp_id(阵营id)******/
    int type;/*******(英雄类型)********/
    int sex;/*****(性别)******/
    int color;/*****colour*(英雄品质)*******/
    int cate;/******(职业系)********/
    int next_id;/*******(下一级品质ID)*******/
    int last_id;/*******(最终品质id)*********/
    int need_res;/*****(品质提升所需要的银币)**********/
    int need_item;/******(品质提升需要碎片数量)********/
    
    int up_str;/******(力量)*****/
    int up_int;/******(智力)*****/
    int up_agi;/******(敏捷)*****/
    int up_con;/******(体质)*****/
    
    int speed;/*****move_speed(移动速度)*******/
    int atk_speed;/*****(攻击速度)********/
    int warnRound;/*****scout_range(警戒范围)******/
    int atk_range;/*****(攻击范围)******/
    
    int base_ch;/*****(基础命中)******/
    int base_cri;/*****(基础爆击)******/
    int base_br;/*****(基础格挡)*******/
    int base_dr;/*****(基础闪避)*******/
    int base_er;/*****(基础精准)*******/
    int base_tr;/*****(基础韧性)*******/
    
    float arms;/*******(武器)********/
    float armors;/*******(防具)*******/
    std::string intro;/********(英雄简介)*******/
    std::string intro_feature;
    std::vector<int> skillIds;/***skill_id(技能id)****/
//    cocos2d::Vector<ActorRandomSkillGroupDef *> skills;
    
    CREATE_REF_FUNC(ActorDef);
};

class ActorConfig : public cocos2d::Ref
{
public:
    ActorConfig();
    ~ActorConfig();
    
    cocos2d::Vector<lofd::ActorDef *> actors;
    
    void parse(std::string dataValue);
    
    lofd::ActorDef * getActorDefById(int actorId);
};

/***********ActorPropertyData*************/
class ActorPropertyData : public cocos2d::Ref
{
public:
    ActorPropertyData(){};
    ~ActorPropertyData(){};
    
    float physicalAtt;
    float physicalDef;
    float magicAtt;
    float magicDef;
    /***Immune injury(免伤)***/
    float physicalImmuneInjury;/****物理免伤*****/
    float magicImmuneInjury;/*****魔法免伤******/
    
    /*****总血量******/
    float hp;
    /*****当前血量*****/
    float currentHp;
    /****命中******/
    float hit;
    /****闪避******/
    float dodge;
    /****精准******/
    float expertise;
    /****格挡******/
    float block;
    /****韧性******/
    float toughness;
    /****爆击******/
    float crit;
};

/********标示状态的数据结构**********/
class StateData : public moonsugar::QueueItem
{
public:
    StateData(std::string itemNameValue):QueueItem(itemNameValue) {};
    ~StateData();
    
    static StateData * create(std::string itemNameValue)
    {
        StateData * stateData = new StateData(itemNameValue);
        if (stateData)
        {
            stateData->autorelease();
            return stateData;
        }
        else
        {
            delete stateData;
            stateData = nullptr;
            return nullptr;
        }
    };
};

/********状态上下文数据结构，以栈形式保存状态*********/
class StateContext
{
public:
    moonsugar::Stack * stateList;
    StateData * currentState;
    
    StateContext();
    ~StateContext();
    
    /***插入状态数据***/
    void insertStateData(lofd::StateData * stateData);
    /****以特定索引插入数据*****/
    void insertStateDataIndex(lofd::StateData * stateData, int index);
    /****按索引插入，且判断是否自动转换到当前状态******/
    bool insertStateDataIndexAutoChange(lofd::StateData * stateData, int index);
    
    bool insertStateDataBitAutoChange(lofd::StateData * stateData, int bit);
    
    void removeStateDataChangeNext(int bit);
    
    /***弹出当前状态，转换成栈内下一个状态***/
    void popStateDataChangeNext();
    
    /***cancel出当前状态，转换成参数的状态***/
    void cancelStateDataChange(lofd::StateData * stateData);
};

/***************ActorStateData*****************/
class ActorStateData : public lofd::StateData
{
public:
    ActorStateData(std::string itemNameValue, std::string operationTypeValue):StateData(itemNameValue) {
        operationType = operationTypeValue;
    };
    ~ActorStateData(){
    };
    
    std::string operationType;
    
    static ActorStateData * create(std::string itemNameValue, std::string operationTypeValue)
    {
        ActorStateData * actorStateData = new ActorStateData(itemNameValue, operationTypeValue);
        if (actorStateData)
        {
            actorStateData->autorelease();
            return actorStateData;
        }
        else
        {
            delete actorStateData;
            actorStateData = nullptr;
            return nullptr;
        }
    };
};
NS_LOFD_END;

#endif /* defined(__LengendOfFightDemons__LOFDActorState__) */
