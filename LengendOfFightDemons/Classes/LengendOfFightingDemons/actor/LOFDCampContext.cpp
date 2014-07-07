//
//  LOFDCampContext.cpp
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-10.
//
//

#include "LOFDCampContext.h"
#include "external/json/rapidjson.h"
#include "external/json/document.h"
#include "editor-support/cocostudio/DictionaryHelper.h"

NS_LOFD_BEGIN

/********CampConfig***********/
void CampConfig::parse(std::string data)
{
    rapidjson::Document dt;
    dt.Parse<0>(data.c_str());
    if (dt.HasParseError())
    {
        cocos2d::log("camp config parse error");
    }
    else
    {
        int n = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(dt, CAMP_CONFIG_PARSE_TAG_CAMPS);
        for (int i = 0; i < n; i++) {
            const rapidjson::Value& campDefValue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(dt, CAMP_CONFIG_PARSE_TAG_CAMPS, i);
            lofd::CampDef * campDef = new lofd::CampDef;
            campDef->campId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(campDefValue, CAMP_CONFIG_PARSE_TAG_ID);
            campDef->campName = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(campDefValue, CAMP_CONFIG_PARSE_TAG_NAME);
            int m = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(campDefValue, CAMP_CONFIG_PARSE_TAG_RELATIONSHIP_DEFS);
            for (int j = 0; j < m; j ++) {
                lofd::CampRelationShipDef * campRelationShipDef = new lofd::CampRelationShipDef;
                const rapidjson::Value & campRelationShipDefValue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(campDefValue, CAMP_CONFIG_PARSE_TAG_RELATIONSHIP_DEFS, j);
                campRelationShipDef->campId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(campRelationShipDefValue, CAMP_CONFIG_PARSE_TAG_ID);
                campRelationShipDef->relationShip = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(campRelationShipDefValue, CAMP_CONFIG_PARSE_TAG_RELATIONSHIP);
                campDef->relationShips.pushBack(campRelationShipDef);
            }
            this->camps.pushBack(campDef);
        }
    }
}

CampDef * CampConfig::getCampDefById(int campId)
{
    long n = this->camps.size();
    for (int i = 0; i < n; i ++) {
        CampDef * def = this->camps.at(i);
        if (def->campId == campId) return def;
    }
    return nullptr;
}

/**********CampDef*************/
lofd::CampRelationShipDef * CampDef::getDefByCampId(int campId)
{
    for (int i = 0; i < relationShips.size(); i ++)
    {
        if (campId == relationShips.at(i)->campId) return relationShips.at(i);
    }
    return nullptr;
}
NS_LOFD_END;