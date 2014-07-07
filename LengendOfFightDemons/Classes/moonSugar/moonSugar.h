//
//  moonSugar.h
//  NowGo
//
//  Created by RockLee on 14-2-10.
//
//

#ifndef NowGo_moonSugar_h
#define NowGo_moonSugar_h

#define NS_MS_BEGIN namespace moonsugar {
#define NS_MS_END   }
#define USING_NS_MS using namespace moonsugar

#ifndef MS_DEBUG_DRAW
#define MS_DEBUG_DRAW 2
#endif

#define CREATE_REF_FUNC(__TYPE__) \
static __TYPE__* create() \
{ \
__TYPE__ *pRet = new __TYPE__(); \
if (pRet) \
{ \
pRet->autorelease(); \
return pRet; \
} \
else \
{ \
delete pRet; \
pRet = NULL; \
return NULL; \
} \
}

#include "cocos2d.h"

#endif
