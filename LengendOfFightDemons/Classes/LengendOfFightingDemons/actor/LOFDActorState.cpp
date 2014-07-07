//
//  LOFDActorState.cpp
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-8.
//
//

#include "LOFDActorState.h"
#include "external/json/rapidjson.h"
#include "external/json/document.h"
#include "editor-support/cocostudio/DictionaryHelper.h"
#include "FileFormat.h"
#include "TypeConver.h"

NS_LOFD_BEGIN

/**********ActorStateGroupDef**********/
ActorStateGroupDef::ActorStateGroupDef()
{
    
}

ActorStateGroupDef::~ActorStateGroupDef()
{
    
}

lofd::ActorStateDef * ActorStateGroupDef::getActorStateDefByTag(std::string actionTag)
{
    for (int i = 0; i < states.size(); i ++)
    {
        lofd::ActorStateDef * actorStateDef = states.at(i);
        if (actionTag == actorStateDef->tag) return actorStateDef;
    }
    return nullptr;
}

lofd::ActorStateDef * ActorStateGroupDef::getActorStateMatchByTag(std::string actionTag)
{
    for (int i = 0; i < stateMatch.size(); i++) {
        lofd::ActorStateMatch * actorStateMatch = stateMatch.at(i);
        if (actionTag == actorStateMatch->tag)
        {
            return getActorStateDefByTag(actorStateMatch->matchTag);
        }
    }
    return getActorStateDefByTag(actionTag);
}

/*************ActorStateConfig***********/
ActorStateConfig::ActorStateConfig()
{
    
}

ActorStateConfig::~ActorStateConfig()
{
    
}

void ActorStateConfig::parse(std::string dataValue)
{
    rapidjson::Document dt;
    dt.Parse<0>(dataValue.c_str());
    if (dt.HasParseError())
    {
        cocos2d::log("%s", "ActorStateConfig error");
    }
    else
    {
        int stateGroups = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(dt, LOFD_ACTOR_STATE_CONFIG_TAG_STATE_GROUPS);
        for (int i = 0; i < stateGroups; i ++) {
            const rapidjson::Value& stateGroupValue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(dt, LOFD_ACTOR_STATE_CONFIG_TAG_STATE_GROUPS, i);
            lofd::ActorStateGroupDef * stateGroup = lofd::ActorStateGroupDef::create();
            stateGroup->stateId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(stateGroupValue, LOFD_ACTOR_STATE_CONFIG_TAG_STATE_ID);
            
            int states = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(stateGroupValue, LOFD_ACTOR_STATE_CONFIG_TAG_STATES);
            cocos2d::Vector<ActorStateDef *> actorStates;
            for (int j = 0; j < states; j ++)
            {
                lofd::ActorStateDef * actorStateDef = lofd::ActorStateDef::create();
                const rapidjson::Value& stateValue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(stateGroupValue, LOFD_ACTOR_STATE_CONFIG_TAG_STATES, j);
                actorStateDef->tag = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(stateValue, LOFD_ACTOR_STATE_CONFIG_TAG_TAG);
                actorStateDef->frames = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(stateValue, LOFD_ACTOR_STATE_CONFIG_TAG_FRAMES);
                actorStateDef->round = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(stateValue, LOFD_ACTOR_STATE_CONFIG_TAG_ROUND);
                const rapidjson::Value& relativePointValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(stateValue, LOFD_ACTOR_STATE_CONFIG_TAG_RELATIVE_POINT);
                const rapidjson::Value& rectValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(stateValue, LOFD_ACTOR_STATE_CONFIG_TAG_RECT);
                float rx = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(relativePointValue, LOFD_ACTOR_STATE_CONFIG_TAG_X);
                float ry = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(relativePointValue, LOFD_ACTOR_STATE_CONFIG_TAG_Y);
                cocos2d::Point * relativePoint = new cocos2d::Point(rx, ry);
                actorStateDef->relativePoint = relativePoint;
                rx = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(rectValue, LOFD_ACTOR_STATE_CONFIG_TAG_X);
                ry = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(rectValue, LOFD_ACTOR_STATE_CONFIG_TAG_Y);
                float width = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(rectValue, LOFD_ACTOR_STATE_CONFIG_TAG_WIDTH);
                float height = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(rectValue, LOFD_ACTOR_STATE_CONFIG_TAG_HEIGHT);
                cocos2d::Rect * rect = new cocos2d::Rect(rx, ry, width, height);
                actorStateDef->rect = rect;
                actorStates.pushBack(actorStateDef);
            }
            int stateMatchs = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(stateGroupValue, LOFD_ACTOR_STATE_CONFIG_TAG_STATE_MATCH);
            cocos2d::Vector<lofd::ActorStateMatch *> actorStateMatchs;
            for (int k = 0; k < stateMatchs; k ++)
            {
                lofd::ActorStateMatch * actorStateMatchDef = lofd::ActorStateMatch::create();
                const rapidjson::Value& stateMatchValue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(stateGroupValue, LOFD_ACTOR_STATE_CONFIG_TAG_STATE_MATCH, k);
                actorStateMatchDef->tag = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(stateMatchValue, LOFD_ACTOR_STATE_CONFIG_TAG_TAG);
                actorStateMatchDef->matchTag = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(stateMatchValue, LOFD_ACTOR_STATE_CONFIG_TAG_MATCH_TAG);
                actorStateMatchs.pushBack(actorStateMatchDef);
            }
            stateGroup->stateMatch = actorStateMatchs;
            stateGroup->states = actorStates;
            this->stateGroups.pushBack(stateGroup);
        }
    }
}

lofd::ActorStateGroupDef * ActorStateConfig::getActorStateGroupDefByStateId(int stateId)
{
    long n = stateGroups.size();
    for (int i = 0; i < n; i ++) {
        lofd::ActorStateGroupDef * groupDef = stateGroups.at(i);
        if (groupDef->stateId == stateId)
        {
            return groupDef;
        }
    }
    return nullptr;
}

/************ActorStateDef*************/
ActorStateDef::ActorStateDef()
{
    tag = "";
    frames = 0;
    relativePoint = nullptr;
    rect = nullptr;
}

ActorStateDef::~ActorStateDef()
{
    delete relativePoint;
    delete rect;
}

/************ActorDef************/
ActorDef::ActorDef()
{
    speed = 5;
}

ActorDef::~ActorDef()
{
    
}

/************ActorStateConfig************/
ActorConfig::ActorConfig()
{
    
}

ActorConfig::~ActorConfig()
{
    
}

void ActorConfig::parse(std::string dataValue)
{
    rapidjson::Document dt;
    dt.Parse<0>(dataValue.c_str());
    if (dt.HasParseError())
    {
        cocos2d::log("%s", "ActorConfig error");
    }
    else
    {
        long n = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(dt, LOFD_ACTOR_STATE_CONFIG_TAG_ACTORS);
        for (int i = 0; i < n; i ++)
        {
            lofd::ActorDef * actorDef = lofd::ActorDef::create();
            const rapidjson::Value& actorValue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(dt, LOFD_ACTOR_STATE_CONFIG_TAG_ACTORS, i);
            actorDef->actorId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_HERO_ID);
            actorDef->name = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_HERO_NAME);
            actorDef->half_body = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_HALF_BODY);
            actorDef->all_body = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_ALL_BODY);
            actorDef->head_icon = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_HEAD_ICON);
            actorDef->stateId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_JOB_ID);
            actorDef->job_name = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_JOB_NAME);
            actorDef->behaviorId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_BEHAVIOR_ID);
            actorDef->aiId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_AI__ID);
            actorDef->campId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_CAMP__ID);
            actorDef->type = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_TYPE);
            actorDef->sex = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_SEX);
            actorDef->color = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_COLOUR);
            actorDef->cate = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_CATE);
            actorDef->next_id = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_NEXT_ID);
            actorDef->last_id = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_LAST_ID);
            actorDef->need_res = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_NEED_RES);
            actorDef->need_item = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_NEED_ITEM);
            actorDef->up_str = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_UP_STR);
            actorDef->up_int = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_UP_INT);
            actorDef->up_agi = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_UP_AGI);
            actorDef->up_con = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_UP_CON);
            actorDef->speed = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_MOVE_SPEED);
            actorDef->atk_speed = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_ATK_SPEED);
            actorDef->warnRound = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_SCOUT_RANGE);
            actorDef->atk_range = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_ATK_RANGE);
            actorDef->base_ch = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_BASE_CH);
            actorDef->base_cri = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_BASE_CRI);
            actorDef->base_br = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_BASE_BR);
            actorDef->base_dr = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_BASE_DR);
            actorDef->base_er = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_BASE_ER);
            actorDef->base_tr = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_BASE_TR);
            actorDef->arms = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_ARMS);
            actorDef->armors = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_ARMOR);
            actorDef->intro = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_INTRO);
            actorDef->warnRound = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_SCOUT_RANGE);
            actorDef->intro_feature = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_INTRO_FEATURE);
            actorDef->warnRound = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_SCOUT_RANGE);
            std::string skillIds = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(actorValue, LOFD_ACTOR_STATE_CONFIG_TAG_SKILL_ID);
            std::vector<std::string> skillIdsVector = moonsugar::TypeConver::splitStr(skillIds, STRING_SPLIT_GANG);
            long m = skillIdsVector.size();
            for (int j = 0; j < m; j ++) {
//                std::string skillIdStr = skillIdsVector.at(j);
//                lofd::ActorRandomSkillGroupDef * actorRandomSkillGroupDef = new lofd::ActorRandomSkillGroupDef;
//                std::vector<std::string> randomSkillsVector = moonsugar::TypeConver::splitStr(skillIdStr, STRING_SPLIT_MAOHAO);
//                long b = randomSkillsVector.size();
//                for (int k = 0; k < b; k ++) {
//                    std::string randomSkillId = randomSkillsVector.at(k);
//                    int skillId = moonsugar::TypeConver::CharToInt(randomSkillId.c_str());
//                    actorRandomSkillGroupDef->skillIds.push_back(skillId);
//                }
//                actorDef->skills.pushBack(actorRandomSkillGroupDef);
                std::string skillIdStr = skillIdsVector.at(j);
                int skillId = moonsugar::TypeConver::CharToInt(skillIdStr.c_str());
                actorDef->skillIds.push_back(skillId);
            }
            this->actors.pushBack(actorDef);
        }
    }
}

lofd::ActorDef * ActorConfig::getActorDefById(int actorId)
{
    for (int i = 0; i < actors.size(); i++) {
        lofd::ActorDef * actorDef = actors.at(i);
        if (actorDef->actorId == actorId)
        {
            return actorDef;
        }
    }
    return nullptr;
}

/*********StateData*************/
StateData::~StateData()
{
    
}

/*********StateContext*************/
StateContext::StateContext()
{
    stateList = new moonsugar::Stack();
}

StateContext::~StateContext()
{
    delete stateList;
}

void StateContext::insertStateData(lofd::StateData *stateData)
{
    stateList->addItem(stateData);
    currentState = stateData;
}

void StateContext::popStateDataChangeNext()
{
    stateList->popItem();
    //delete queueItem;
    if (stateList->checkNext()) {
        currentState = (lofd::StateData *)stateList->getCurrentItem();
    }
}

void StateContext::cancelStateDataChange(lofd::StateData *stateData)
{
    stateList->popItem();
    //delete queueItem;
    insertStateData(stateData);
}

void StateContext::insertStateDataIndex(lofd::StateData *stateData, int index)
{
    stateList->addItemIndex(stateData, index);
}

bool StateContext::insertStateDataIndexAutoChange(lofd::StateData *stateData, int index)
{
    stateList->addItemIndex(stateData, index);
    if (index == 0)
    {
        currentState = stateData;
        return true;
    }
    return false;
}

bool StateContext::insertStateDataBitAutoChange(lofd::StateData *stateData, int bit)
{
    int index = (int)stateList->queueList.size() - bit;
    if (index < 0) index = 0;
    stateList->addItemIndex(stateData, index);
    if (index == 0)
    {
        currentState = stateData;
        return true;
    }
    return false;
}

void StateContext::removeStateDataChangeNext(int bit)
{
    int index = (int)stateList->queueList.size() - bit - 1;
    if (index < 0) index = 0;
    stateList->removeItem(index);
    if (stateList->checkNext()) {
        currentState = (lofd::StateData *)stateList->getCurrentItem();
    }
}
NS_LOFD_END;