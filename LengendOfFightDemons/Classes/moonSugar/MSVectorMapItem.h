//
//  MSVectorMapItem.h
//  Vector
//
//  Created by RockLee on 14-4-3.
//
//

#ifndef __Vector__MSVectorMapItem__
#define __Vector__MSVectorMapItem__

#include "moonsugar.h"
#include "MSVectorMap.h"

#define MAP_ITEM_TYPE_STATIC            "static"
#define MAP_ITEM_TYPE_BUILDING          "building"
#define MAP_ITEM_TYPE_ACTOR             "actor"

NS_MS_BEGIN

class VectorMapItemDef : public cocos2d::Ref
{
public:
    VectorMapItemDef() {};
    ~VectorMapItemDef();
    int itemId;
    std::string itemName;
    /*****VectorMapItem type****/
    std::string type;
};

class VectorMapItemsDef : public cocos2d::Ref
{
public:
    static moonsugar::VectorMapItemsDef * getInstance();
    
    VectorMapItemsDef() {};
    ~VectorMapItemsDef();
    
    cocos2d::Vector<moonsugar::VectorMapItemDef *> items;
    
    moonsugar::VectorMapItemDef * getMapItemByItemId(int itemIdValue);
    
    void parse(std::string data);
};

NS_MS_END;
#endif /* defined(__Vector__MSVectorMapItem__) */
