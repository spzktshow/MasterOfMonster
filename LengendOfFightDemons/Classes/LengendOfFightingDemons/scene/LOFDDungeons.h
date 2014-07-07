//
//  LOFDDungeons.h
//  LengendOfFightDemons
//
//  Created by RockLee on 14-5-19.
//
//

#ifndef __LengendOfFightDemons__LOFDDungeons__
#define __LengendOfFightDemons__LOFDDungeons__

#include "moonSugar.h"
#include "lengendOfFightDemons.h"
NS_LOFD_BEGIN

#define DUNGEON_CONFIG_TAG_DUNGEONS              "dungeons"
#define DUNGEON_CONFIG_TAG_LIFE_CIRCLES          "lifeCircles"
#define DUNGEON_CONFIG_TAG_CAMP_ID               "campId"
#define DUNGEON_CONFIG_TAG_RESULT               "result"
#define DUNGEON_CONFIG_TAG_TYPE                 "type"
#define DUNGEON_CONFIG_TAG_DUNGEON_ID           "dungeonId"
#define DUNGEON_CONFIG_TAG_SCENE_ID             "sceneId"
#define DUNGEON_CONFIG_TAG_TIME                 "time"

#define DUNGEON_LIFE_CIRCLE_TYPE_TIME             "time"/********控制生命周期的时间********/
#define DUNGEON_LIFE_CIRCLE_TYPE_GUARD_DESTORY    "guardDestory"/********目标达成*************/

#define DUNGEON_LIFE_CIRCLE_RESULT_SUCCESS        "success"
#define DUNGEON_LIFE_CIRCLE_RESULT_FAILED         "failed"
/************地下城生命周期********/
class DungeonLifeCircle : public cocos2d::Ref
{
public:
    DungeonLifeCircle(){};
    ~DungeonLifeCircle(){};
    
    CREATE_REF_FUNC(DungeonLifeCircle);
    
    /******生命周期类型*******/
    std::string type;
    /******生命周期结束结果******/
    std::string result;
};

class DungeonTimeLifeCircle : public lofd::DungeonLifeCircle
{
public:
    DungeonTimeLifeCircle():time(0){};
    ~DungeonTimeLifeCircle(){};
    
    CREATE_REF_FUNC(DungeonTimeLifeCircle);
    
    /*****生命周期时长*****/
    float time;
};

class DungeonGuardDestoryLifeCircle : public lofd::DungeonLifeCircle
{
public:
    DungeonGuardDestoryLifeCircle():campId(0){};
    ~DungeonGuardDestoryLifeCircle(){};
    
    CREATE_REF_FUNC(DungeonGuardDestoryLifeCircle);
    
    /*****阵营Id***/
    int campId;
};

#define DUNGEON_TYPE_SIEGE          "siege"/****攻城***/
#define DUNGEON_TYPE_GROUP          "group"/****团战***/
/*******地下城配置*******/
class DungeonDef : public cocos2d::Ref
{
public:
    DungeonDef():dungeonId(0), sceneId(0){};
    ~DungeonDef(){};
    CREATE_REF_FUNC(DungeonDef);
    /***关卡id*****/
    int dungeonId;
    /****binding sceneId*****/
    int sceneId;
    /****关卡类型*****/
    std::string type;
    /*****控制生命周期的列表********/
    cocos2d::Vector<lofd::DungeonLifeCircle *> lifeCircles;
    /********操作的阵营id*********/
    int operationCampId;
};

class DungeonConfig : public cocos2d::Ref
{
public:
    DungeonConfig(){};
    ~DungeonConfig(){};
    
    cocos2d::Vector<lofd::DungeonDef *> dungeons;
    void parse(std::string dataValue);
    
    lofd::DungeonDef * getDungeonDefById(int dungeonId);
};

NS_LOFD_END;

#endif /* defined(__LengendOfFightDemons__LOFDDungeons__) */
