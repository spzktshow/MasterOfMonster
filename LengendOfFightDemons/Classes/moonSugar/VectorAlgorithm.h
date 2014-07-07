//
//  VectorAlgorithm.h
//  Vector
//
//  Created by RockLee on 14-3-25.
//
//

#ifndef __Vector__VectorAlgorithm__
#define __Vector__VectorAlgorithm__

#include "moonSugar.h"
#include "cocos2d.h"

NS_MS_BEGIN

class VectorLine : public cocos2d::Ref
{
public:
    VectorLine(cocos2d::Point * p1Value, cocos2d::Point * p2Value);
    ~VectorLine();
    
    cocos2d::Point * p1;
    cocos2d::Point * p2;
};

class VectorAlgorithm
{
public:
    /*****矢量加法******/
    static cocos2d::Point * vectorAddition(cocos2d::Point & a, cocos2d::Point & b);
    /*****矢量减法******/
    static cocos2d::Point * vectorSubtraction(cocos2d::Point & a, cocos2d::Point & b);
    /*****矢量叉积******/
    static float vectorCrossproduct(cocos2d::Point & a, cocos2d::Point & b);
    /*****根据矢量生成矩形*****/
    static cocos2d::Rect * createRect(moonsugar::VectorLine & lineValue);
    /*****快速排斥************/
    static bool fastExclude(moonsugar::VectorLine & lineA, moonsugar::VectorLine & lineB);
    /*****跨立实验(unCrossButHit:未交叉但是碰撞的线是否跨立，true则视为跨立情况，false则视为不跨立)************/
    static bool straddle(moonsugar::VectorLine & lineA, moonsugar::VectorLine & lineB, bool unCrossButHit);
};

NS_MS_END;

#endif /* defined(__Vector__VectorAlgorithm__) */
