//
//  MSMap.h
//  MoonSugarAStar
//
//  Created by RockLee on 14-3-21.
//
//

#ifndef __MoonSugarAStar__MSMap__
#define __MoonSugarAStar__MSMap__

#include "moonSugar.h"
#include "MSGeometry.h"

NS_MS_BEGIN

#define MAP_TILE_NORMAL_CONSUME        10
#define MAP_TILE_ANGEL_CONSUME         14

/***********最小单位地图节点***************/
class MapNode : public moonsugar::Node
{
public:
    MapNode(moonsugar::Matrix * matrixValue, moonsugar::MatrixIndex * matrixIndexValue):Node(matrixValue, matrixIndexValue){};
    ~MapNode();
    
};

/*********地图块*************************/
class MapTile : public moonsugar::MapNode
{
public:
    MapTile(moonsugar::Matrix * matrixValue, moonsugar::MatrixIndex * matrixIndexValue, cocos2d::Size * tileSizeValue):MapNode(matrixValue, matrixIndexValue){
        tileSize = tileSizeValue;
    };
    ~MapTile();
    
    //暂定，走到这格的消费
    /***行动权重，地图节点本质上不是地图上的物体，而是描述地图的最小单位**/
	/**暂时只有是否能行走，所以当actionWeight>=1时为不能行走<1时能行走**/
    float consume = 0;
    //寻路用，父节点
    moonsugar::MapTile * father;
    /**是否在开放列表中**/
    bool isOpen;
    /**是否在关闭列表中**/
	bool isClose;
    
    /**h+g**/
	int f;
	/**当前点到终点的消费**/
	int g;
	/**起点到当前点的消费**/
	int h;
    
    /*****mapTile的size，这个size由map传进来，考虑到素有size都一样，所以释放工作交由Map处理******/
    cocos2d::Size * tileSize;
};

/*********地图上的物体********************/
class MapItem : public moonsugar::MapNode
{
public:
    MapItem(moonsugar::Matrix * matrixValue, moonsugar::MatrixIndex * matrixIndexValue):MapNode(matrixValue, matrixIndexValue){};
    ~MapItem();
    
    /****is moving***/
    bool isMoving;
    /********current move tile*********/
    //moonsugar::MapTile * currentMoveTile;
    int currentMoveTileIndex;
    cocos2d::Vector<MapTile *> path;
    void move(cocos2d::Vector<MapTile *> path);
};

/***********地图************************/
class Map : public moonsugar::MapNode
{
public:
    Map(moonsugar::Matrix * matrixValue, moonsugar::MatrixIndex * matrixIndexValue, cocos2d::Size * blockSizeValue, cocos2d::Point * pointValue):MapNode(matrixValue, matrixIndexValue){
        blockSize = blockSizeValue;
        point = pointValue;
    };
    ~Map();
    
    cocos2d::Vector<moonsugar::MapItem *> mapItems;
    //simulator 2d Array, getMapItem(colIndex, rowIndex), colIndex * rowIndex
    cocos2d::Vector<moonsugar::MapTile *> mapTiles;
    //MapTile * 缓存，所有的MapTile的MatrixData数值相同,在create中初始化
    moonsugar::Matrix * mapTileMatrixData;
    void create();
    int getIndex(int colIndex, int rowIndex);
    moonsugar::MapTile * getTile(int colIndex, int rowIndex);
    
    /****单个块的Size****/
    cocos2d::Size * blockSize;
    /*****地图Size******/
    cocos2d::Size * mapSize;
    /****地图的坐标******/
    cocos2d::Point * point;
    
    void clearOpenAndCloseList();
};

/**************MapUtils****************/
class MapUtils
{
public:
    //寻路
    static moonsugar::MapTile * searchPath(moonsugar::MatrixIndex &startMatrixIndex, moonsugar::MatrixIndex &endMatrixIndex, moonsugar::Map * mapValue);
    //检测MapNode是否容许通过
    static bool checkAllow(moonsugar::MapTile * mapTile);
    //检测两个Tile是否相等
    static bool checkEqual(moonsugar::MapTile * aMapTile, moonsugar::MapTile * bMapTile);
    //计算当前tile到endTile的H值
    static int calculateH(moonsugar::MapTile * currentTile, moonsugar::MapTile * endTile);
    //得到开放列表中F值最小得Tile
    static moonsugar::MapTile * getMinF(cocos2d::Vector<moonsugar::MapTile *> openList);
    //得导开发列表中f值最小得Tile的索引
    static int getIndexMinF(cocos2d::Vector<moonsugar::MapTile *> openList);
    //
    static void clearCloseList(cocos2d::Vector<moonsugar::MapTile *> closeList);
    //
    static void clearOpenList(cocos2d::Vector<moonsugar::MapTile *> openList);
    /*******序列化寻路完成的MapTile********/
    static cocos2d::Vector<moonsugar::MapTile *> sequencePath(moonsugar::MapTile * mapTileValue);
    
};

NS_MS_END;

#endif /* defined(__MoonSugarAStar__MSMap__) */
