//
//  MSMap.cpp
//  MoonSugarAStar
//
//  Created by RockLee on 14-3-21.
//
//

#include "MSMap.h"
#include <math.h>

NS_MS_BEGIN
/*************MapNode**********/
MapNode::~MapNode()
{
    
}

/***********MapItem***********/
MapItem::~MapItem()
{
    delete matrixData;
    delete matrixIndexData;
}

void MapItem::move(cocos2d::Vector<MapTile *> pathValue)
{
    path = pathValue;
    isMoving = true;
    currentMoveTileIndex = 0;
}

/***********MapTile***********/
MapTile::~MapTile()
{
    //delete matrixData;交给创建MapTile的高层类释放
    //delete tileRect;//交给创建Maptile的高层类释放
}

/***********Map***************/
Map::~Map()
{
    delete mapTileMatrixData;
    delete blockSize;
    delete point;
    delete mapSize;
}

void Map::create()
{
    //
    mapTileMatrixData = new moonsugar::Matrix(1, 1);
    for (int i = 0; i < matrixData->col; i ++) {
        for (int j = 0; j < matrixData->row; j ++)
        {
            moonsugar::MatrixIndex * mapTileMatrixIndex = new moonsugar::MatrixIndex(i, j);
            moonsugar::MapTile * tile = new moonsugar::MapTile(mapTileMatrixData, mapTileMatrixIndex, blockSize);
            mapTiles.pushBack(tile);
        }
    }
    
    mapSize = new cocos2d::Size(matrixData->col * blockSize->width, matrixData->row * blockSize->height);
}

int Map::getIndex(int colIndex, int rowIndex)
{
    return colIndex * matrixData->row + rowIndex;
}

moonsugar::MapTile * Map::getTile(int colIndex, int rowIndex)
{
    int index = getIndex(colIndex, rowIndex);
    if (index < mapTiles.size())
    {
        return mapTiles.at(index);
    }
    return nullptr;
}

void Map::clearOpenAndCloseList()
{
    for (int i = 0; i < mapTiles.size(); i ++) {
        moonsugar::MapTile * check = mapTiles.at(i);
        check->isClose = false;
        check->isOpen = false;
    }
}

/**************MapUtils*************/
moonsugar::MapTile * MapUtils::searchPath(moonsugar::MatrixIndex &startMatrixIndex, moonsugar::MatrixIndex &endMatrixIndex, moonsugar::Map *mapValue)
{
    cocos2d::Vector<moonsugar::MapTile *> openList;
    cocos2d::Vector<moonsugar::MapTile *> closeList;
    moonsugar::MapTile * endNode = mapValue->getTile(endMatrixIndex.colIndex, endMatrixIndex.rowIndex);
    moonsugar::MapTile * searchNode = mapValue->getTile(startMatrixIndex.colIndex, startMatrixIndex.rowIndex);
    searchNode->g = 0;
    searchNode->h = MapUtils::calculateH(searchNode, endNode);
    searchNode->f = searchNode->g + searchNode->h;
    int searchNodeIndex;
    int g, f, h;
    while(searchNode->matrixIndexData->rowIndex != endMatrixIndex.rowIndex || searchNode->matrixIndexData->colIndex != endMatrixIndex.colIndex)
    {
        int colIndex, rowIndex, col, row, i, j;
        row = std::min(searchNode->matrixIndexData->rowIndex + 2, mapValue->matrixData->row);
        col = std::min(searchNode->matrixIndexData->colIndex + 2, mapValue->matrixData->col);
        rowIndex = std::max(searchNode->matrixIndexData->rowIndex - 1, 0);
        colIndex = std::max(searchNode->matrixIndexData->colIndex - 1, 0);
        for (i = rowIndex; i < row; i ++) {
            for (j = colIndex; j < col; j ++)
            {
                moonsugar::MapTile * checkNode = mapValue->getTile(j, i);
                bool b1 = moonsugar::MapUtils::checkEqual(searchNode, checkNode);
                if (b1 == true)
                {
                    continue;
                }
                else
                {
                    bool b2 = moonsugar::MapUtils::checkAllow(checkNode);
                    if (b2 == false)
                    {
                        continue;
                    }
                    else
                    {
                        bool b5 = checkNode->isClose;
                        if (b5 == true)
                        {
                            continue;
                        }
                        else
                        {
                            bool b3 = moonsugar::MapUtils::checkAllow(mapValue->getTile(checkNode->matrixIndexData->colIndex, searchNode->matrixIndexData->rowIndex));
                            if (b3 == false)
                            {
                                continue;
                            }
                            else
                            {
                                bool b4 = moonsugar::MapUtils::checkAllow(mapValue->getTile(searchNode->matrixIndexData->colIndex, checkNode->matrixIndexData->rowIndex));
                                if (b4 == false)
                                {
                                    continue;
                                }
                            }
                        }
                    }
                }
                if (searchNode->matrixIndexData->rowIndex != checkNode->matrixIndexData->rowIndex
                    && searchNode->matrixIndexData->colIndex != checkNode->matrixIndexData->colIndex
                    )
                {
                    g = searchNode->g + MAP_TILE_ANGEL_CONSUME;
                }
                else
                {
                    g = searchNode->g + MAP_TILE_NORMAL_CONSUME;
                }
                h = MapUtils::calculateH(checkNode, endNode);
                f = g + h;
                if (checkNode->isOpen == true)
                {
                    if (checkNode->g > g)
                    {
                        checkNode->f = f;
                        checkNode->h = h;
                        checkNode->g = g;
                        checkNode->father = searchNode;
                    }
                }
                else
                {
                    checkNode->f = f;
                    checkNode->g = g;
                    checkNode->h = h;
                    checkNode->father = searchNode;
                    
                    checkNode->isOpen = true;
                    openList.pushBack(checkNode);
                }
            }
        }
        searchNode->isClose = true;
        closeList.pushBack(searchNode);
        if (openList.size() <= 0) return nullptr;
        searchNode->isOpen = false;
        //searchNode = moonsugar::MapUtils::getMinF(openList);
        searchNodeIndex = moonsugar::MapUtils::getIndexMinF(openList);
        searchNode = openList.at(searchNodeIndex);
        openList.erase(searchNodeIndex);
    }
    //clearOpenList(openList);
    //clearCloseList(closeList);
    return searchNode;
}

bool MapUtils::checkAllow(moonsugar::MapTile *mapTile)
{
    if (mapTile->consume >= 1)
    {
        return false;
    }
    return true;
}

bool MapUtils::checkEqual(moonsugar::MapTile *aMapTile, moonsugar::MapTile *bMapTile)
{
    if (aMapTile->matrixIndexData->colIndex == bMapTile->matrixIndexData->colIndex
        && aMapTile->matrixIndexData->rowIndex == bMapTile->matrixIndexData->rowIndex
        )
    {
        return true;
    }
    return false;
}

int MapUtils::calculateH(moonsugar::MapTile *currentTile, moonsugar::MapTile *endTile)
{
    return (abs(endTile->matrixIndexData->colIndex - currentTile->matrixIndexData->colIndex)
            + abs(endTile->matrixIndexData->rowIndex - currentTile->matrixIndexData->rowIndex)) * MAP_TILE_NORMAL_CONSUME;
}

moonsugar::MapTile * MapUtils::getMinF(cocos2d::Vector<moonsugar::MapTile *> openList)
{
    moonsugar::MapTile * mapTile = nullptr;
    int n = openList.size();
    int index;
    for (int i = 0; i < n; i ++) {
        moonsugar::MapTile * checkTile = (moonsugar::MapTile*)openList.at(i);
        if (!mapTile || checkTile->f < mapTile->f)
        {
            mapTile = checkTile;
            index = i;
        }
    }
    openList.erase(index);
    return mapTile;
}

int MapUtils::getIndexMinF(cocos2d::Vector<moonsugar::MapTile *> openList)
{
    int n = openList.size();
    int index;
    int minFValue = -1;
    for (int i = 0; i < n; i ++)
    {
        moonsugar::MapTile * checkTile = (moonsugar::MapTile*)openList.at(i);
        if (minFValue == -1 || checkTile->f < minFValue)
        {
            minFValue = checkTile->f;
            index = i;
        }
    }
    return index;
}

void MapUtils::clearCloseList(cocos2d::Vector<moonsugar::MapTile *> closeList)
{
    int n = closeList.size();
    for (int i = 0; i < n; i ++)
    {
        moonsugar::MapTile * mapTile = closeList.at(i);
        mapTile->isClose = false;
    }
}

void MapUtils::clearOpenList(cocos2d::Vector<moonsugar::MapTile *> openList)
{
    int n = openList.size();
    for (int i = 0; i < n; i ++)
    {
        moonsugar::MapTile * mapTile = openList.at(i);
        mapTile->isOpen = false;
    }
}

cocos2d::Vector<moonsugar::MapTile *> MapUtils::sequencePath(moonsugar::MapTile *mapTileValue)
{
    cocos2d::Vector<moonsugar::MapTile *> tiles;
    //cocos2d::log("%s", "sequenceStart");
    moonsugar::MapTile * mapTile = mapTileValue;
    while(mapTile != nullptr)
    {
        //cocos2d::log("showSequece col%d row%d", mapTile->matrixIndexData->colIndex, mapTile->matrixIndexData->rowIndex);
        tiles.pushBack(mapTile);
        moonsugar::MapTile & tempTile = *mapTile;
        mapTile = mapTile->father;
        tempTile.father = nullptr;
    }
    //cocos2d::log("%s", "sequenceEnd");
    tiles.reverse();
    return tiles;
}
NS_MS_END;