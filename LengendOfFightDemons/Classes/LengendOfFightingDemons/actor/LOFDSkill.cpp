//
//  LOFDSkill.cpp
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-17.
//
//

#include "LOFDSkill.h"
#include "TypeConver.h"
#include "FileFormat.h"
#include "editor-support/cocostudio/DictionaryHelper.h"

NS_LOFD_BEGIN
/********SkillConfig************/
lofd::SkillDef * lofd::SkillConfig::getSkillDefById(int skillId)
{
    for (int i = 0; i < skillDefs.size(); i ++) {
        lofd::SkillDef * skillDef = skillDefs.at(i);
        if (skillDef->skillId == skillId)
        {
            return skillDef;
        }
    }
    return nullptr;
}

void SkillConfig::parse(std::string data)
{
    rapidjson::Document dt;
    dt.Parse<0>(data.c_str());
    if (dt.HasParseError())
    {
        cocos2d::log("%s", "skill Config error");
    }
    else
    {
        int n = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(dt, SKILL_CONFIG_TAG_SKILL_DEFS);
        for (int i = 0; i < n; i ++)
        {
            const rapidjson::Value & skillDefValue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(dt, SKILL_CONFIG_TAG_SKILL_DEFS, i);
            lofd::SkillDef * skillDef = new lofd::SkillDef();
            skillDef->skillId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(skillDefValue, SKILL_CONFIG_TAG_SKILL_ID);
            const rapidjson::Value & operationContextDefValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(skillDefValue, SKILL_CONFIG_TAG_OPERATION_CONTEXT_DEF);
            skillDef->operationContextDef = parseSkillOperationContextDef(operationContextDefValue);
            const rapidjson::Value & skillRangeDef = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(skillDefValue, SKILL_CONFIG_TAG_SKILL_RANG_DEF);
            skillDef->skillRangeDef = this->parseSkillRangeDef(skillRangeDef);
            skillDef->coolDown = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(skillDefValue, SKILL_CONFIG_TAG_COOL_DOWN);
            skillDef->isAuto = cocostudio::DictionaryHelper::getInstance()->getBooleanValue_json(skillDefValue, SKILL_CONFIG_TAG_IS_AUTO);
            int m = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(skillDefValue, SKILL_CONFIG_TAG_SKILL_RANDOM_CONTEXTS);
            for (int j = 0; j < m; j++) {
                const rapidjson::Value & skillRandomContextValue  = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(skillDefValue, SKILL_CONFIG_TAG_SKILL_RANDOM_CONTEXTS, j);
                lofd::SkillRandomContext * skillRandomContext = new lofd::SkillRandomContext;
                int b = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(skillRandomContextValue, SKILL_CONFIG_TAG_EFFECT_CONTEXT_DEFS);
                for (int k = 0; k < b; k ++) {
                    int effectId = cocostudio::DictionaryHelper::getInstance()->getIntValueFromArray_json(skillRandomContextValue, SKILL_CONFIG_TAG_EFFECT_CONTEXT_DEFS, k);
                    lofd::SkillEffectContextDef * skillEffectContextDef = this->getSkillEffectContextDefById(effectId);
                    skillRandomContext->effectContextDefs.pushBack(skillEffectContextDef);
                }
                const rapidjson::Value & skillStateDef = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(skillRandomContextValue, SKILL_CONFIG_TAG_STATE_DEF);
                skillRandomContext->stateDef = this->parseSkillStateDef(skillStateDef);
                skillDef->skillRandomContexts.pushBack(skillRandomContext);
            }
            this->skillDefs.pushBack(skillDef);
        }
    }
}

void SkillConfig::parseEffects(std::string data)
{
    rapidjson::Document dt;
    dt.Parse<0>(data.c_str());
    if (dt.HasParseError())
    {
        cocos2d::log("%s", "skill effects Config error");
    }
    else
    {
        int n = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(dt, SKILL_CONFIG_TAG_SKILL_EFFECT_DEFS);
        for (int i = 0; i < n; i ++) {
            const rapidjson::Value & effectContextDef = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(dt, SKILL_CONFIG_TAG_SKILL_EFFECT_DEFS, i);
            lofd::SkillEffectContextDef * skillEffectContextDef = this->parseSkillEffectContextDef(effectContextDef);
            this->skillEffectContextDefs.pushBack(skillEffectContextDef);
        }
    }
}

lofd::SkillEffectContextDef * SkillConfig::getSkillEffectContextDefById(int effectId)
{
    int n = this->skillEffectContextDefs.size();
    for (int i = 0; i< n; i++) {
        lofd::SkillEffectContextDef * def = this->skillEffectContextDefs.at(i);
        if (def->skillEffectDef->effectId == effectId) return def;
    }
    return nullptr;
}

lofd::SkillStateDef * SkillConfig::parseSkillStateDef(const rapidjson::Value & skillStateDefValue)
{
    lofd::SkillStateDef * skillStateDef = new lofd::SkillStateDef();
    std::string tags = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(skillStateDefValue, SKILL_CONFIG_TAG_SKILL_STATE_TAG);
    std::vector<std::string> tagsVector = moonsugar::TypeConver::splitStr(tags, STRING_SPLIT_GANG);
    skillStateDef->tags = tagsVector;
    return skillStateDef;
}

lofd::SkillEffectContextDef * SkillConfig::parseSkillEffectContextDef(const rapidjson::Value &skillEffectContextDefValue)
{
    lofd::SkillEffectContextDef * skillEffectContextDef = new lofd::SkillEffectContextDef;
    const rapidjson::Value & skillEffectDefValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(skillEffectContextDefValue, SKILL_CONFIG_TAG_SKILL_EFFECT_DEF);
    const rapidjson::Value & effectLifeCycleDefValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(skillEffectContextDefValue, SKILL_CONFIG_TAG_EFFECT_LICE_CYCLE_DEF);
    skillEffectContextDef->skillEffectDef = this->parseSkillEffectDef(skillEffectDefValue);
    skillEffectContextDef->effectLifeCycleDef = this->parseEffectLifeCycleDef(effectLifeCycleDefValue);
    return skillEffectContextDef;
}

lofd::EffectLifeCycleDef * SkillConfig::parseEffectLifeCycleDef(const rapidjson::Value &effectLifeCycleDefValue)
{
    lofd::EffectLifeCycleDef * effectLifeCycleDef = new lofd::EffectLifeCycleDef;
    const rapidjson::Value & effectStartLifeCycleDefValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(effectLifeCycleDefValue, SKILL_CONFIG_TAG_EFFECT_START_LIFE_CYCLE_DEF);
    effectLifeCycleDef->effectStartLifeCycleDef = parseEffectStartLifeCycleDef(effectStartLifeCycleDefValue);
    int n = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(effectLifeCycleDefValue, SKILL_CONFIG_TAG_EFFECT_END_LIFE_CYCLE_DEFS); //(effectLifeCycleDefValue, SKILL_CONFIG_TAG_EFFECT_END_LIFE_CYCLE_DEFS);
    for (int i = 0; i < n; i ++)
    {
        const rapidjson::Value & effectEndLifeCycleDefvalue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(effectLifeCycleDefValue, SKILL_CONFIG_TAG_EFFECT_END_LIFE_CYCLE_DEFS, i);
        lofd::EffectEndLifeCycleDef * effectEndLifeCycleDef = this->parseEffectEndLifeCycleDef(effectEndLifeCycleDefvalue);
        effectLifeCycleDef->effectEndLifeCycelDefs.pushBack(effectEndLifeCycleDef);
    }
    return effectLifeCycleDef;
}

lofd::EffectStartLifeCycleDef * SkillConfig::parseEffectStartLifeCycleDef(const rapidjson::Value &effectStartLifeCycleDefValue)
{
    std::string type = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(effectStartLifeCycleDefValue, SKILL_CONFIG_TAG_TYPE);
    const rapidjson::Value & effectOutputDefValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(effectStartLifeCycleDefValue, SKILL_CONFIG_TAG_EFFECT_OUTPUT_DEF);
    lofd::EffectOutputDef * effectOutputDef = lofd::SkillConfig::parseEffectOutputDef(effectOutputDefValue);
    if (type == EFFECT_START_LIFE_CYCLE_DEPEND)
    {
        lofd::EffectStartLifeCycleDependDef * effectStartLifeCycleDependDef = new lofd::EffectStartLifeCycleDependDef;
        effectStartLifeCycleDependDef->dependId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(effectStartLifeCycleDefValue, SKILL_CONFIG_TAG_DEPEND_ID);
        effectStartLifeCycleDependDef->type = type;
        effectStartLifeCycleDependDef->effectOutputDef = effectOutputDef;
        return effectStartLifeCycleDependDef;
    }
    else if (type == EFFECT_START_LIFE_CYCLE_INTERVAL)
    {
        lofd::EffectStartLifeCycleIntervalDef * effectStartLifeCycleIntervalDef = new lofd::EffectStartLifeCycleIntervalDef;
        effectStartLifeCycleIntervalDef->startInterval = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(effectStartLifeCycleDefValue, SKILL_CONFIG_TAG_START_INTERVAL);
        effectStartLifeCycleIntervalDef->type = type;
        effectStartLifeCycleIntervalDef->effectOutputDef = effectOutputDef;
        return effectStartLifeCycleIntervalDef;
    }
    else
    {
        cocos2d::log("parse effect start life cycle def undefine type");
        return nullptr;
    }
}

lofd::EffectEndLifeCycleDef * SkillConfig::parseEffectEndLifeCycleDef(const rapidjson::Value &effectEndLifeCycleDefValue)
{
    std::string type = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(effectEndLifeCycleDefValue, SKILL_CONFIG_TAG_TYPE);
    const rapidjson::Value & effectOutputDefValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(effectEndLifeCycleDefValue, SKILL_CONFIG_TAG_EFFECT_OUTPUT_DEF);
    lofd::EffectOutputDef * effectOutputDef = lofd::SkillConfig::parseEffectOutputDef(effectOutputDefValue);
    if (type == EFFECT_END_LIFE_CYCLE_HIT)
    {
        lofd::EffectEndLifeCycleHitDef * effectEndlifeCycleHitDef = new lofd::EffectEndLifeCycleHitDef;
        effectEndlifeCycleHitDef->type = EFFECT_END_LIFE_CYCLE_HIT;
        effectEndlifeCycleHitDef->hitLimit = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(effectEndLifeCycleDefValue, SKILL_CONFIG_TAG_HIT_LIMIT);
        effectEndlifeCycleHitDef->effectOutputDef = effectOutputDef;
        return effectEndlifeCycleHitDef;
    }
    else if (type == EFFECT_END_LIFE_CYCLE_DISTANCE)
    {
        lofd::EffectEndLifeCycleDistanceDef * effectEndLifeCycleDistanceDef = new lofd::EffectEndLifeCycleDistanceDef;
        effectEndLifeCycleDistanceDef->type = EFFECT_END_LIFE_CYCLE_DISTANCE;
        effectEndLifeCycleDistanceDef->distance = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(effectEndLifeCycleDefValue, SKILL_CONFIG_TAG_DISTANCE);
        effectEndLifeCycleDistanceDef->effectOutputDef = effectOutputDef;
        return effectEndLifeCycleDistanceDef;
    }
    else if (type == EFFECT_END_LIFE_CYCLE_TARGET)
    {
        lofd::EffectEndLifeCycleTargetDef * effectEndLifeCycleTargetDef = new lofd::EffectEndLifeCycleTargetDef();
        effectEndLifeCycleTargetDef->type = EFFECT_END_LIFE_CYCLE_TARGET;
        effectEndLifeCycleTargetDef->effectOutputDef = effectOutputDef;
        return effectEndLifeCycleTargetDef;
    }
    else if (type == EFFECT_END_LIFE_CYCLE_TIME)
    {
        lofd::EffectEndLifeCycleTimeDef * effectEndLifeCycleTimeDef = new lofd::EffectEndLifeCycleTimeDef;
        effectEndLifeCycleTimeDef->type = EFFECT_END_LIFE_CYCLE_TIME;
        effectEndLifeCycleTimeDef->time = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(effectEndLifeCycleDefValue, SKILL_CONFIG_TAG_TIME);
        effectEndLifeCycleTimeDef->effectOutputDef = effectOutputDef;
        return effectEndLifeCycleTimeDef;
    }
    else if (type == EFFECT_END_LIFE_CYCLE_TRACK)
    {
        lofd::EffectEndLifeCycleTrackDef * effectEndLifeCycleTrackDef = new lofd::EffectEndLifeCycleTrackDef();
        effectEndLifeCycleTrackDef->type = EFFECT_END_LIFE_CYCLE_TRACK;
        effectEndLifeCycleTrackDef->effectOutputDef = effectOutputDef;
        return effectEndLifeCycleTrackDef;
    }
    cocos2d::log("skill config parse effect end life cycle def undefine type");
    return nullptr;
}

lofd::SkillEffectDef * SkillConfig::parseSkillEffectDef(const rapidjson::Value &skillEffectDefValue)
{
    lofd::SkillEffectDef * skillEffectDef = new lofd::SkillEffectDef;
    skillEffectDef->effectId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(skillEffectDefValue, SKILL_CONFIG_TAG_EFFECT_ID);
    skillEffectDef->effectName = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(skillEffectDefValue, SKILL_CONFIG_TAG_EFFECT_NAME);
    const rapidjson::Value & locationDefValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(skillEffectDefValue, SKILL_CONFIG_TAG_LOCATION_DEF);
    skillEffectDef->locationDef = this->parseEffectLocationDef(locationDefValue);
    const rapidjson::Value & movementDef = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(skillEffectDefValue, SKILL_CONFIG_TAG_MOVEMENT_DEF);
    skillEffectDef->movementDef = this->parseEffectMovementDef(movementDef);
    const rapidjson::Value & coordinateDef = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(skillEffectDefValue, SKILL_CONFIG_TAG_COORDINATE_DEF);
    skillEffectDef->coordinateDef = this->parseEffectCoordinateDef(coordinateDef);
    const rapidjson::Value & viewDef = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(skillEffectDefValue, SKILL_CONFIG_TAG_VIEW_DEF);
    skillEffectDef->viewDef = this->parseEffectViewDef(viewDef);
    const rapidjson::Value & hitDef = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(skillEffectDefValue, SKILL_CONFIG_TAG_HIT_DEF);
    skillEffectDef->hitDef = this->parseEffectHitDef(hitDef);
    return skillEffectDef;
}

lofd::EffectHitDef * SkillConfig::parseEffectHitDef(const rapidjson::Value & effectHitDefValue)
{
    std::string hitType = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(effectHitDefValue, SKILL_CONFIG_TAG_HIT_TYPE);
    if (hitType == EFFECT_HIT_TYPE_NONE)
    {
        lofd::EffectHitDef * effectHitDef = new lofd::EffectHitDef;
        effectHitDef->hitType = EFFECT_HIT_TYPE_NONE;
        return effectHitDef;
    }
    else if (hitType == EFFECT_HIT_TYPE_FRAME)
    {
        lofd::EffectFrameHitDef * effectFrameHitDef = new lofd::EffectFrameHitDef;
        effectFrameHitDef->hitType = EFFECT_HIT_TYPE_FRAME;
        const rapidjson::Value & effectFrameHitDefValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(effectHitDefValue, SKILL_CONFIG_TAG_EFFECT_HIT_FRAME_DEF);
        effectFrameHitDef->effectHitFrameDef = this->parseEffectHitFrameDef(effectFrameHitDefValue);
        return effectFrameHitDef;
    }
    else if (hitType == EFFECT_HIT_TYPE_FRAMES)
    {
        lofd::EffectFramesHitDef * effectFramesHitDef = new lofd::EffectFramesHitDef;
        effectFramesHitDef->hitType = EFFECT_HIT_TYPE_FRAMES;
        int n = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(effectHitDefValue, SKILL_CONFIG_TAG_FRAMES);
        for (int i = 0; i < n; i ++) {
            const rapidjson::Value & effectHitFrameDefValue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(effectHitDefValue, SKILL_CONFIG_TAG_FRAMES, i);
            lofd::EffectHitFrameDef * hitFrameDef = this->parseEffectHitFrameDef(effectHitFrameDefValue);
            effectFramesHitDef->frames.pushBack(hitFrameDef);
        }
        return effectFramesHitDef;
    }
    cocos2d::log("skill config parse effect hit def undefine type");
    return nullptr;
}

lofd::EffectHitFrameDef * SkillConfig::parseEffectHitFrameDef(const rapidjson::Value &effectHitFrameDefValue)
{
    lofd::EffectHitFrameDef * effectHitFrameDef = new lofd::EffectHitFrameDef;
    effectHitFrameDef->frameIndex = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(effectHitFrameDefValue, SKILL_CONFIG_TAG_FRAME_INDEX);
    int n = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(effectHitFrameDefValue, SKILL_CONFIG_TAG_HIT_RECTS);
    for (int i = 0; i < n; i ++) {
        const rapidjson::Value & hitRect = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(effectHitFrameDefValue, SKILL_CONFIG_TAG_HIT_RECTS, i);
        float x = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(hitRect, SKILL_CONFIG_TAG_X);
        float y = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(hitRect, SKILL_CONFIG_TAG_Y);
        float width = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(hitRect, SKILL_CONFIG_TAG_WIDTH);
        float height = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(hitRect, SKILL_CONFIG_TAG_HEIGHT);
        cocos2d::Rect * rect = new cocos2d::Rect(x, y, width, height);
        effectHitFrameDef->hitRects.push_back(rect);
    }
    return effectHitFrameDef;
}

lofd::EffectViewDef * SkillConfig::parseEffectViewDef(const rapidjson::Value &effectViewDefValue)
{
    std::string animationType = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(effectViewDefValue, SKILL_CONFIG_TAG_ANIMATION_TYPE);
    if (animationType == EFFECT_VIEW_TYPE_NONE)
    {
        lofd::EffectViewDef * effectViewDef = new lofd::EffectViewDef();
        effectViewDef->animationType = EFFECT_VIEW_TYPE_NONE;
        return effectViewDef;
    }
    else if (animationType == EFFECT_VIEW_TYPE_FRAME)
    {
        lofd::EffectFrameViewDef * effectFrameViewDef = new lofd::EffectFrameViewDef;
        effectFrameViewDef->animationType = EFFECT_VIEW_TYPE_FRAME;
        const rapidjson::Value & anchorPointValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(effectViewDefValue, SKILL_CONFIG_TAG_ANCHOR_POINT);
        float x = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(anchorPointValue, SKILL_CONFIG_TAG_X);
        float y = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(anchorPointValue, SKILL_CONFIG_TAG_Y);
        cocos2d::Point * point = new cocos2d::Point(x , y);
        effectFrameViewDef->anchorPoint = point;
        effectFrameViewDef->frameName = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(effectViewDefValue, SKILL_CONFIG_TAG_FRAME_NAME);
        return effectFrameViewDef;
    }
    else if (animationType == EFFECT_VIEW_TYPE_FRAMES)
    {
        lofd::EffectFramesViewDef * effectFramesViewDef = new lofd::EffectFramesViewDef;
        effectFramesViewDef->animationType = EFFECT_VIEW_TYPE_FRAMES;
        effectFramesViewDef->frameName = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(effectViewDefValue, SKILL_CONFIG_TAG_FRAME_NAME);
        const rapidjson::Value & anchorPointValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(effectViewDefValue, SKILL_CONFIG_TAG_ANCHOR_POINT);
        float x = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(anchorPointValue, SKILL_CONFIG_TAG_X);
        float y = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(anchorPointValue, SKILL_CONFIG_TAG_Y);
        cocos2d::Point * point = new cocos2d::Point(x , y);
        effectFramesViewDef->anchorPoint = point;
        effectFramesViewDef->delay = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(effectViewDefValue, SKILL_CONFIG_TAG_DELAY);
        effectFramesViewDef->frames = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(effectViewDefValue, SKILL_CONFIG_TAG_FRAMES);
        return effectFramesViewDef;
    }
    cocos2d::log("skill config parse effect view def undefine type");
    return nullptr;
}

lofd::EffectCoordinateDef * SkillConfig::parseEffectCoordinateDef(const rapidjson::Value &effectCoordinateDefValue)
{
    lofd::EffectCoordinateDef * effectCoordinateDef = new lofd::EffectCoordinateDef;
    effectCoordinateDef->coordinateType = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(effectCoordinateDefValue, SKILL_CONFIG_TAG_COORDINATE_TYPE);
    return effectCoordinateDef;
}

lofd::EffectMovementDef * SkillConfig::parseEffectMovementDef(const rapidjson::Value &effectMovementDefValue)
{
    std::string movementType = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(effectMovementDefValue, SKILL_CONFIG_TAG_MOVEMENT_TYPE);
    if (movementType == EFFECT_MOVEMENT_TYPE_STILL)
    {
        lofd::EffectMovementDef * effectMovementDef = new lofd::EffectMovementDef();
        effectMovementDef->movementType = EFFECT_MOVEMENT_TYPE_STILL;
        return effectMovementDef;
    }
    else if (movementType == EFFECT_MOVEMENT_TYPE_TARGET)
    {
        lofd::EffectMovementDef * effectMovementDef = new lofd::EffectMovementDef;
        effectMovementDef->movementType = EFFECT_MOVEMENT_TYPE_TARGET;
        return effectMovementDef;
    }
    else if (movementType == EFFECT_MOVEMENT_TYPE_TRACK)
    {
        lofd::EffectMovementTrackDef * effectMovementTrackDef = new lofd::EffectMovementTrackDef();
        effectMovementTrackDef->movementType = EFFECT_MOVEMENT_TYPE_TRACK;
        effectMovementTrackDef->speed = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(effectMovementDefValue, SKILL_CONFIG_TAG_SPEED);
        return effectMovementTrackDef;
    }
    else if (movementType == EFFECT_MOVEMENT_TYPE_VECTOR)
    {
        lofd::EffectMovementVectorDef * effectMovementVectorDef = new lofd::EffectMovementVectorDef;
        effectMovementVectorDef->movementType = EFFECT_MOVEMENT_TYPE_VECTOR;
        effectMovementVectorDef->speed = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(effectMovementDefValue, SKILL_CONFIG_TAG_SPEED);
        return effectMovementVectorDef;
    }
    cocos2d::log("skill config parse effect movement def undefine type");
    return nullptr;
}

lofd::EffectLocationDef * SkillConfig::parseEffectLocationDef(const rapidjson::Value &effectLocationDefValue)
{
    std::string locationType = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(effectLocationDefValue, SKILL_CONFIG_TAG_LOCATION_TYPE);
    if (locationType == EFFECT_LOCATION_TYPE_RELATIVE)
    {
        lofd::EffectRelativeLocationDef * effectRelativeLocationDef = new lofd::EffectRelativeLocationDef;
        effectRelativeLocationDef->locationType = EFFECT_LOCATION_TYPE_RELATIVE;
        const rapidjson::Value & relativePointValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(effectLocationDefValue, SKILL_CONFIG_TAG_RELATIVE_POINT);
        cocos2d::Point * point = new cocos2d::Point();
        point->x = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(relativePointValue, SKILL_CONFIG_TAG_X);
        point->y = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(relativePointValue, SKILL_CONFIG_TAG_Y);
        effectRelativeLocationDef->relativePoint = point;
        return effectRelativeLocationDef;
    }
    else if (locationType == EFFECT_LOCATION_TYPE_TARGET)
    {
        lofd::EffectTargetLocationDef * effectTargetLocationDef = new lofd::EffectTargetLocationDef;
        effectTargetLocationDef->locationType = EFFECT_LOCATION_TYPE_TARGET;
        return effectTargetLocationDef;
    }
    cocos2d::log("skill config parse effect location def undefine type");
    return nullptr;
}

lofd::SkillOperationContextDef * SkillConfig::parseSkillOperationContextDef(const rapidjson::Value & skillOperationContextDefValue)
{
    std::string operationType = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(skillOperationContextDefValue, SKILL_CONFIG_TAG_OPERATION_TYPE);
    lofd::SkillOperationContextDef * skillOperationContextDef = new lofd::SkillOperationContextDef;
    skillOperationContextDef->operationType = operationType;
    return skillOperationContextDef;
}

lofd::SkillRangeDef * SkillConfig::parseSkillRangeDef(const rapidjson::Value & skillRangeDefValue)
{
    std::string type = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(skillRangeDefValue, SKILL_CONFIG_TAG_TYPE);
    if (type == SKILL_RANGE_TYPE_R)
    {
        lofd::SkillRangeRDef * skillRangeRDef = new lofd::SkillRangeRDef;
        skillRangeRDef->type = SKILL_RANGE_TYPE_R;
        skillRangeRDef->r = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(skillRangeDefValue, SKILL_CONFIG_TAG_R);
        return skillRangeRDef;
    }
    else if (type == SKILL_RANGE_TYPE_RECT)
    {
        lofd::SkillRangeRectDef * skillRangeRectDef = new lofd::SkillRangeRectDef;
        skillRangeRectDef->type = SKILL_RANGE_TYPE_RECT;
        const rapidjson::Value & rect = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(skillRangeDefValue, SKILL_CONFIG_TAG_RECT);
        float x = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(rect, SKILL_CONFIG_TAG_X);
        float y = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(rect, SKILL_CONFIG_TAG_Y);
        float width = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(rect, SKILL_CONFIG_TAG_WIDTH);
        float height = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(rect, SKILL_CONFIG_TAG_HEIGHT);
        cocos2d::Rect * rectD = new cocos2d::Rect(x, y, width, height);
        skillRangeRectDef->rect = rectD;
        return skillRangeRectDef;
    }
    cocos2d::log("skill config parse skill range def undefine type");
    return nullptr;
}

lofd::EffectOutputDef * SkillConfig::parseEffectOutputDef(const rapidjson::Value &effectOutputDefValue)
{
    lofd::EffectOutputDef * effectOutputDef = new lofd::EffectOutputDef;
    effectOutputDef->type = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(effectOutputDefValue, SKILL_CONFIG_TAG_TYPE);
    if (effectOutputDef->type != EFFECT_OUTPUT_NONE)
    {
        effectOutputDef->formulaId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(effectOutputDefValue, SKILL_CONFIG_TAG_FORMULA_ID);
        effectOutputDef->outputTargetType = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(effectOutputDefValue, SKILL_CONFIG_TAG_OUTPUT_TARGET_TYPE);
    }
    else
    {
        effectOutputDef->formulaId = -1;
    }
    return effectOutputDef;
}
NS_LOFD_END;