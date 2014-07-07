//
//  MSSource.h
//  Vector
//
//  Created by RockLee on 14-3-31.
//
//

#ifndef __Vector__MSSource__
#define __Vector__MSSource__

#include "moonSugar.h"

#define SOURCE_ENUM_FILE_FORMAT_PNG         ".png"
#define SOURCE_ENUM_FILE_FORMAT_PLIST       ".plist"
#define SOURCE_ENUM_FILE_FORMAT_JSON        ".json"

NS_MS_BEGIN
/****抽象资源类*****/
class Source : public cocos2d::Ref
{
public:
    Source(std::string nameValue);
    ~Source();
    /***资源name****/
    std::string sourceName;
};

NS_MS_END;
#endif /* defined(__Vector__MSSource__) */
