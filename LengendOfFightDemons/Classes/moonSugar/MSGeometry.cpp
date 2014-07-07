//
//  MSGeometry.cpp
//  MoonSugarAStar
//
//  Created by RockLee on 14-3-21.
//
//

#include "MSGeometry.h"

NS_MS_BEGIN
/*********Matrix**********/
Matrix::Matrix(int colValue, int rowValue)
{
    col = colValue;
    row = rowValue;
}

Matrix::~Matrix()
{
    
}

/*********MatrixIndex***********/
MatrixIndex::MatrixIndex(int colIndexValue, int rowIndexValue)
{
    colIndex = colIndexValue;
    rowIndex = rowIndexValue;
}

MatrixIndex::~MatrixIndex()
{
    
}

/*********Node************/
Node::Node(moonsugar::Matrix * matrixValue, moonsugar::MatrixIndex * matrixIndexValue)
{
    matrixData = matrixValue;
    matrixIndexData = matrixIndexValue;
}

Node::~Node()
{
    //考虑到移动端内存问题，而node又属于可能大量创建但matrix相同的
    //所以matrixData 不进行释放，交由子类实现释放，可以将次数据结构缓存在创建node的高层类
    //由高层类进行释放
    //delete matrixData;交给子类或创建node的高层类释放
    delete matrixIndexData;
}

/************CoordinateUtils************/
float CoordinateUtils::calculateCoordinateSystem(float y, const cocos2d::Size & size)
{
	return (size.height - y);
}
NS_MS_END;