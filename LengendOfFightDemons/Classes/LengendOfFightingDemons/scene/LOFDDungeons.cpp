//
//  LOFDDungeons.cpp
//  LengendOfFightDemons
//
//  Created by RockLee on 14-5-19.
//
//

#include "LOFDDungeons.h"
#include "external/json/rapidjson.h"
#include "external/json/document.h"
#include "editor-support/cocostudio/DictionaryHelper.h"

NS_LOFD_BEGIN

void DungeonConfig::parse(std::string dataValue)
{
    rapidjson::Document dt;
    dt.Parse<0>(dataValue.c_str());
    if (dt.HasParseError())
    {
        cocos2d::log("%s", "DungeonConfig error");
    }
    else
    {
        int dungeonsCount = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(dt, DUNGEON_CONFIG_TAG_DUNGEONS);
        for (int i = 0; i < dungeonsCount; i ++) {
            const rapidjson::Value & dungeonDefValue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(dt, DUNGEON_CONFIG_TAG_DUNGEONS, i);
            lofd::DungeonDef * def = lofd::DungeonDef::create();
            def->dungeonId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(dungeonDefValue, DUNGEON_CONFIG_TAG_DUNGEON_ID);
            def->sceneId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(dungeonDefValue, DUNGEON_CONFIG_TAG_SCENE_ID);
            def->type = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(dungeonDefValue, DUNGEON_CONFIG_TAG_TYPE);
            int lifeCirclesCount = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(dungeonDefValue, DUNGEON_CONFIG_TAG_LIFE_CIRCLES);
            for (int j = 0; j < lifeCirclesCount; j ++) {
                const rapidjson::Value & lifeCircleDefValue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(dungeonDefValue, DUNGEON_CONFIG_TAG_LIFE_CIRCLES, j);
                std::string type = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(lifeCircleDefValue, DUNGEON_CONFIG_TAG_TYPE);
                if (type == DUNGEON_LIFE_CIRCLE_TYPE_TIME)
                {
                    lofd::DungeonTimeLifeCircle * lifeCircleDef = lofd::DungeonTimeLifeCircle::create();
                    lifeCircleDef->time = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(lifeCircleDefValue, DUNGEON_CONFIG_TAG_TIME);
                    lifeCircleDef->type = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(lifeCircleDefValue, DUNGEON_CONFIG_TAG_TYPE);
                    lifeCircleDef->result = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(lifeCircleDefValue, DUNGEON_CONFIG_TAG_RESULT);
                    def->lifeCircles.pushBack(lifeCircleDef);
                }
                else if (type == DUNGEON_LIFE_CIRCLE_TYPE_GUARD_DESTORY)
                {
                    lofd::DungeonGuardDestoryLifeCircle * lifeCircleDef = lofd::DungeonGuardDestoryLifeCircle::create();
                    lifeCircleDef->campId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(lifeCircleDefValue, DUNGEON_CONFIG_TAG_CAMP_ID);
                    lifeCircleDef->type = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(lifeCircleDefValue, DUNGEON_CONFIG_TAG_TYPE);
                    lifeCircleDef->result = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(lifeCircleDefValue, DUNGEON_CONFIG_TAG_RESULT);
                    def->lifeCircles.pushBack(lifeCircleDef);
                }
                else
                {
                    cocos2d::log("dungeonConfig dungeonLifeCircle unknow type");
                    continue;
                }
            }
            this->dungeons.pushBack(def);
        }
        cocos2d::log("dungeonConfig parse complete");
    }
}

lofd::DungeonDef * DungeonConfig::getDungeonDefById(int dungeonId)
{
    long n = this->dungeons.size();
    for (int i = 0; i < n;i ++)
    {
        lofd::DungeonDef * def = this->dungeons.at(i);
        if (def->dungeonId == dungeonId)
        {
            return def;
        }
    }
    return nullptr;
}

NS_LOFD_END;