//
//  ApplicationMainScene.h
//  Vector
//
//  Created by RockLee on 14-4-1.
//
//

#ifndef __Vector__ApplicationMainScene__
#define __Vector__ApplicationMainScene__

#include "cocos2d.h"

class ApplicationMainScene : public cocos2d::Scene
{
public:
    /****实际初始化调用函数*****/
    virtual bool init();
    /******切换场景回调*********/
    void onGoSceneHandler(cocos2d::Ref * pSender);
    /****静态初始化函数****/
    CREATE_FUNC(ApplicationMainScene);
};

#endif /* defined(__Vector__ApplicationMainScene__) */
