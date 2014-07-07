//
//  VectorAlgorithm.cpp
//  Vector
//
//  Created by RockLee on 14-3-25.
//
//

#include "VectorAlgorithm.h"

NS_MS_BEGIN
/****VectorLine******/
VectorLine::VectorLine(cocos2d::Point * p1Value, cocos2d::Point * p2Value)
{
    p1 = p1Value;
    p2 = p2Value;
}

VectorLine::~VectorLine()
{
    delete p1;
    delete p2;
    p1 = nullptr;
    p2 = nullptr;
}

/****VectorAlgorithm********/
cocos2d::Point * VectorAlgorithm::vectorAddition(cocos2d::Point &a, cocos2d::Point &b)
{
    cocos2d::Point * addition = new cocos2d::Point(a.x + b.x, a.y + b.y);
    return addition;
}

cocos2d::Point * VectorAlgorithm::vectorSubtraction(cocos2d::Point &a, cocos2d::Point &b)
{
    cocos2d::Point * substraction = new cocos2d::Point(a.x - b.x, a.y - b.y);
    return substraction;
}

float VectorAlgorithm::vectorCrossproduct(cocos2d::Point & a, cocos2d::Point & b)
{
    //lineA x lineB
    return a.x * b.y - b.x * a.y;
}

cocos2d::Rect * VectorAlgorithm::createRect(moonsugar::VectorLine & lineValue)
{
    float minX = MIN(lineValue.p1->x, lineValue.p2->x);
    float minY = MIN(lineValue.p1->y, lineValue.p2->y);
    cocos2d::Rect * rect = new cocos2d::Rect(minX, minY, abs(lineValue.p2->x - lineValue.p1->x), abs(lineValue.p2->y - lineValue.p1->y));
    return rect;
}

bool VectorAlgorithm::fastExclude(moonsugar::VectorLine &lineA, moonsugar::VectorLine &lineB)
{
    cocos2d::Rect * rect1 = VectorAlgorithm::createRect(lineA);
    cocos2d::Rect * rect2 = VectorAlgorithm::createRect(lineB);
    bool returnBool = rect1->intersectsRect(*rect2);
    delete rect1;
    delete rect2;
    return returnBool;
}

bool VectorAlgorithm::straddle(moonsugar::VectorLine &lineA, moonsugar::VectorLine &lineB, bool unCrossButHit)
{
    /***
     (The X means crossProduct!!!)
                        cross1                                          cross2
            subPointA               subPointB               subPointB               subPointC
     (lineA.p1 - lineB.p1) X (lineB.p2 - lineB.p1) * (lineB.p2 - lineB.p1) X (lineA.p2 - lineB.p1)
     (lineB.p1 - lineA.p1) X (lineA.p2 - lineA.p1) * (lineA.p2 - lineA.p1) X (lineB.p2 - lineA.p1)
     **/
    cocos2d::Point * subPointA = VectorAlgorithm::vectorSubtraction(*lineA.p1, *lineB.p1);
    cocos2d::Point * subPointB = VectorAlgorithm::vectorSubtraction(*lineB.p2, *lineB.p1);
    float cross1 = VectorAlgorithm::vectorCrossproduct(*subPointA, *subPointB);
    cocos2d::Point * subPointC = VectorAlgorithm::vectorSubtraction(*lineA.p2, *lineB.p1);
    float cross2 = VectorAlgorithm::vectorCrossproduct(*subPointB, *subPointC);
    if (cross1 * cross2 < 0)
    {
        delete subPointA;
        delete subPointB;
        delete subPointC;
        return false;
    }
    else if (!unCrossButHit)
    {
        if (cross1 * cross2 == 0)
        {
            delete subPointA;
            delete subPointB;
            delete subPointC;
            return false;
        }
    }
    subPointA = VectorAlgorithm::vectorSubtraction(*lineB.p1, *lineA.p1);
    subPointB = VectorAlgorithm::vectorSubtraction(*lineA.p2, *lineA.p1);
    subPointC = VectorAlgorithm::vectorSubtraction(*lineB.p2, *lineA.p1);
    cross1 = VectorAlgorithm::vectorCrossproduct(*subPointA, *subPointB);
    cross2 = VectorAlgorithm::vectorCrossproduct(*subPointB, *subPointC);
    delete subPointA;
    delete subPointB;
    delete subPointC;
    if (cross1 * cross2 < 0)
    {
        return false;
    }
    else if (!unCrossButHit && cross1 * cross2 == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

NS_MS_END;