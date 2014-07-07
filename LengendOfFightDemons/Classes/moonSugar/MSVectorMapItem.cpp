//
//  MSVectorMapItem.cpp
//  Vector
//
//  Created by RockLee on 14-4-3.
//
//

#include "MSVectorMapItem.h"

#define BARRIER_ITEMS_CONFIG_TAG_BARRIER_ITEMS_CONFIG           "barrierItemsConfig"
#define BARRIER_ITEMS_CONFIG_ID                                 "id"
#define BARRIER_ITEMS_CONFIG_NAME                               "name"

NS_MS_BEGIN

/*****VectorMapItemDef******/
VectorMapItemDef::~VectorMapItemDef()
{
    
}

/******VectorMapItemsDef********/
VectorMapItemsDef::~VectorMapItemsDef()
{
    
}
static moonsugar::VectorMapItemsDef * s_vectorMapItemsDef;
VectorMapItemsDef * VectorMapItemsDef::getInstance()
{
    if (s_vectorMapItemsDef == nullptr)
    {
        s_vectorMapItemsDef = new moonsugar::VectorMapItemsDef;
        if (!s_vectorMapItemsDef)
        {
            delete s_vectorMapItemsDef;
            s_vectorMapItemsDef = nullptr;
            CCLOG("ERROR: Could not init VectorMapItemsDef");
        }
    }
    return s_vectorMapItemsDef;
}

VectorMapItemDef * VectorMapItemsDef::getMapItemByItemId(int itemIdValue)
{
    for (int i = 0; i < items.size(); i++) {
        moonsugar::VectorMapItemDef * itemDef = items.at(i);
        if (itemDef->itemId == itemIdValue)
        {
            return itemDef;
        }
    }
    return nullptr;
}

void VectorMapItemsDef::parse(std::string data)
{
    rapidjson::Document dt;
    dt.Parse<0>(data.c_str());
    if (dt.HasParseError())
    {
        cocos2d::log("%s", "VectorMapitemsDef Parse error");
    }
    else
    {
        int count = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(dt, BARRIER_ITEMS_CONFIG_TAG_BARRIER_ITEMS_CONFIG);
        for (int i = 0; i < count; i++) {
            const rapidjson::Value & mapItemDef = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(dt, BARRIER_ITEMS_CONFIG_TAG_BARRIER_ITEMS_CONFIG, i);
            moonsugar::VectorMapItemDef * itemDef = new moonsugar::VectorMapItemDef();
            itemDef->itemId =  cocostudio::DictionaryHelper::getInstance()->getIntValue_json(mapItemDef, BARRIER_ITEMS_CONFIG_ID);
            itemDef->itemName = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(mapItemDef, BARRIER_ITEMS_CONFIG_NAME);
            items.pushBack(itemDef);
        }
    }
}

NS_MS_END;