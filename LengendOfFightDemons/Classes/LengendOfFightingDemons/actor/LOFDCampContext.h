//
//  LOFDCampContext.h
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-10.
//
//

#ifndef __LengendOfFightDemons__LOFDCampContext__
#define __LengendOfFightDemons__LOFDCampContext__

#include "lengendOfFightDemons.h"
#include "MSDataStructure.h"

NS_LOFD_BEGIN

#define CAMP_CONFIG_PARSE_TAG_CAMPS                     "camps"
#define CAMP_CONFIG_PARSE_TAG_NAME                      "name"
#define CAMP_CONFIG_PARSE_TAG_ID                        "id"
#define CAMP_CONFIG_PARSE_TAG_RELATIONSHIP_DEFS         "relationShipDefs"
#define CAMP_CONFIG_PARSE_TAG_RELATIONSHIP              "relationShip"

/**********阵营关系定义********/
class CampRelationShipDef : public cocos2d::Ref
{
public:
    CampRelationShipDef(){};
    ~CampRelationShipDef(){};
    
    /******阵营id******/
    int campId;
    /*******关系值********/
    int relationShip;
};

/***********阵营定义**********/
class CampDef : public cocos2d::Ref
{
public:
    CampDef(){};
    ~CampDef(){};
    
    /*****阵营id*******/
    int campId;
    /******阵营名称********/
    std::string campName;
    
    cocos2d::Vector<lofd::CampRelationShipDef *> relationShips;
    
    lofd::CampRelationShipDef * getDefByCampId(int campId);
};

/*************阵营配置**********/
class CampConfig : public cocos2d::Ref
{
public:
    CampConfig(){};
    ~CampConfig(){};
    
    cocos2d::Vector<lofd::CampDef *> camps;
    
    void parse(std::string data);
    
    lofd::CampDef * getCampDefById(int campId);
};

NS_LOFD_END;

#endif /* defined(__LengendOfFightDemons__LOFDCampContext__) */
