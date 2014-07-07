//
//  LOFDMapItem.h
//  LengendOfFightDemons
//
//  Created by RockLee on 14-6-27.
//
//

#ifndef __LengendOfFightDemons__LOFDMapItem__
#define __LengendOfFightDemons__LOFDMapItem__

#include "lengendOfFightDemons.h"
#include "MSVectorMapItem.h"

NS_LOFD_BEGIN

class StaticMapItemDef : public moonsugar::VectorMapItemDef
{
public:
    StaticMapItemDef(){};
    ~StaticMapItemDef(){};
    
    CREATE_REF_FUNC(StaticMapItemDef);
};

class BuildingMapItemDef : public moonsugar::VectorMapItemDef
{
public:
    BuildingMapItemDef(){};
    ~BuildingMapItemDef(){};
    
    CREATE_REF_FUNC(BuildingMapItemDef);
};

NS_LOFD_END;

#endif /* defined(__LengendOfFightDemons__LOFDMapItem__) */
