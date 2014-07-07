//
//  MSGeometry.h
//  MoonSugarAStar
//
//  Created by RockLee on 14-3-21.
//
//

#ifndef __MoonSugarAStar__MSGeometry__
#define __MoonSugarAStar__MSGeometry__

#include "moonSugar.h"
#include "cocos2d.h"

NS_MS_BEGIN
/******Matrix******/
class Matrix
{
public:
    Matrix(int colValue, int rowValue);
    ~Matrix();
    
    int col;
    int row;
};

/*****MatrixIndex*******/
class MatrixIndex
{
public:
    MatrixIndex(int colIndexValue, int rowIndexValue);
    ~MatrixIndex();
    
    int colIndex;
    int rowIndex;
};

/******node*******/
class Node : public cocos2d::Ref
{
public:
    moonsugar::Matrix * matrixData;
    moonsugar::MatrixIndex * matrixIndexData;
    
    Node(moonsugar::Matrix * matrixValue, moonsugar::MatrixIndex * matrixIndexValue);
    ~Node();
};


/************coordinateUitls**********************/
class CoordinateUtils
{
public:
	static float calculateCoordinateSystem(float y, const cocos2d::Size & size);
};

NS_MS_END;
#endif /* defined(__MoonSugarAStar__MSGeometry__) */
