//
//  MSVectorMap.cpp
//  Vector
//
//  Created by RockLee on 14-3-25.
//
//

#include "MSVectorMap.h"
#include "MSVectorMapItem.h"
#include "TypeConver.h"
NS_MS_BEGIN

/**************VectorPoint********************/
VectorPoint::VectorPoint(cocos2d::Point * pointValue)
{
    point = pointValue;
    
    f = 0;
    g = 0;
    h = 0;
    
    isHit = false;
    isClose = false;
    isOpen = false;
    father = nullptr;
    last = nullptr;
    next = nullptr;
}

VectorPoint::~VectorPoint()
{
    delete point;
}

/***************VectorMapItem*******************/
VectorMapItem::VectorMapItem(cocos2d::Vector<moonsugar::VectorPoint *> pointsValue)
{
    points = pointsValue;
    VectorPointToRectUtils::describe(pointsValue);
}

VectorMapItem::~VectorMapItem()
{
    
}

void VectorMapItem::refreshData()
{
    for (int i = 0; i < points.size(); i ++) {
        points.at(i)->isClose = false;
        points.at(i)->isOpen = false;
        points.at(i)->father = nullptr;
    }
}

/***************VectorPointToRectUtils**********/
cocos2d::Rect * VectorPointToRectUtils::create(cocos2d::Vector<moonsugar::VectorPoint *> pointsValue)
{
    moonsugar::VectorPoint * minVectorPoint = nullptr;
    moonsugar::VectorPoint * maxVectorPoint = nullptr;
    for (int i = 0; i < pointsValue.size(); i ++)
    {
        moonsugar::VectorPoint * checkPoint = pointsValue.at(i);
        if (!minVectorPoint || checkPoint->point->x < minVectorPoint->point->x || checkPoint->point->y < minVectorPoint->point->y)
        {
            minVectorPoint = checkPoint;
        }
        if (!maxVectorPoint || checkPoint->point->x > maxVectorPoint->point->x || checkPoint->point->y > maxVectorPoint->point->y)
        {
            maxVectorPoint = checkPoint;
        }
    }
    cocos2d::Rect * rect = new cocos2d::Rect(minVectorPoint->point->x, minVectorPoint->point->y, maxVectorPoint->point->x - minVectorPoint->point->x, maxVectorPoint->point->y - minVectorPoint->point->y);
    return rect;
}

void VectorPointToRectUtils::describe(cocos2d::Vector<moonsugar::VectorPoint *> pointsValue)
{
    for (int i = 0; i < pointsValue.size(); i ++)
    {
        moonsugar::VectorPoint * currentVectorPoint = pointsValue.at(i);
        if (i < pointsValue.size() - 1)
        {
            currentVectorPoint->next = pointsValue.at(i + 1);
        }
        else
        {
            currentVectorPoint->next = pointsValue.at(0);
        }
        if (i == 0)
        {
            currentVectorPoint->last = pointsValue.at(pointsValue.size() - 1);
        }
        else
        {
            currentVectorPoint->last = pointsValue.at(i - 1);
        }
    }
}

cocos2d::Vector<moonsugar::VectorPoint *> VectorPointToRectUtils::createPoints(cocos2d::Rect *rectValue)
{
    cocos2d::Vector<moonsugar::VectorPoint *> points;
    points.pushBack(new moonsugar::VectorPoint(new cocos2d::Point(rectValue->getMinX(), rectValue->getMinY())));
    points.pushBack(new moonsugar::VectorPoint(new cocos2d::Point(rectValue->getMinX(), rectValue->getMaxY())));
    points.pushBack(new moonsugar::VectorPoint(new cocos2d::Point(rectValue->getMaxX(), rectValue->getMaxY())));
    points.pushBack(new moonsugar::VectorPoint(new cocos2d::Point(rectValue->getMaxX(), rectValue->getMinY())));
    return points;
}

/***************VectorRectMapItem***************/
VectorRectMapItem::~VectorRectMapItem()
{
    
}

/***************VectorMap***********************/
VectorMap::VectorMap(cocos2d::Rect * mapRectValue, cocos2d::Vector<moonsugar::VectorMapItem *> mapItemsValue, cocos2d::Rect * allowRectValue)
{
    mapRect = mapRectValue;
    mapItems = mapItemsValue;
    allowRect = allowRectValue;
}

VectorMap::~VectorMap()
{
    delete mapRect;
    delete allowRect;
}

/***********VectorUtils***********************/
cocos2d::Vector<moonsugar::VectorLine *> VectorUtils::getLines(moonsugar::VectorMapItem *mapItemValue)
{
    cocos2d::Vector<moonsugar::VectorLine *> lines;
    moonsugar::VectorLine * tempLine;
    for (int i = 0; i < mapItemValue->points.size(); i ++)
    {
        if (i < mapItemValue->points.size() - 1)
        {
            tempLine = new moonsugar::VectorLine(new cocos2d::Point(*mapItemValue->points.at(i)->point), new cocos2d::Point(*mapItemValue->points.at(i + 1)->point));
        }
        else
        {
            tempLine = new moonsugar::VectorLine(new cocos2d::Point(*mapItemValue->points.at(i)->point), new cocos2d::Point(*mapItemValue->points.at(0)->point));
        }
        lines.pushBack(tempLine);
    }
    return lines;
}

bool VectorUtils::checkHit(moonsugar::VectorLine * pathValue, cocos2d::Vector<moonsugar::VectorLine *> lines)
{
    for (int i = 0; i < lines.size(); i ++)
    {
        bool fastExclude = moonsugar::VectorAlgorithm::fastExclude(*pathValue, *lines.at(i));
        if (fastExclude)
        {
            bool straddle = moonsugar::VectorAlgorithm::straddle(*pathValue, *lines.at(i), false);
            if (straddle) return true;
        }
    }
    return false;
}

float VectorUtils::getDistance(cocos2d::Point *point1, cocos2d::Point *point2)
{
    float dx = point1->x - point2->x;
    float dy = point1->y - point2->y;
    float distance = sqrtf(dx * dx + dy * dy);
    return distance;
}

float VectorUtils::calculateG(moonsugar::VectorPoint *startValue, moonsugar::VectorPoint *currentValue)
{
    return VectorUtils::getDistance(startValue->point, currentValue->point);
}

float VectorUtils::calculateH(moonsugar::VectorPoint *endValue, moonsugar::VectorPoint *currentValue)
{
    return VectorUtils::getDistance(endValue->point, currentValue->point);
}

cocos2d::Vector<moonsugar::VectorPoint *> VectorUtils::getPath(moonsugar::VectorPoint * startValue, moonsugar::VectorPoint * endValue, moonsugar::VectorMapItem *mapItemValue, cocos2d::Vector<moonsugar::VectorLine *> lines)
{
    cocos2d::Vector<moonsugar::VectorPoint *> path;
    cocos2d::Vector<moonsugar::VectorPoint*> openList;
    cocos2d::Vector<moonsugar::VectorPoint*> closeList;
    bool hasUnHitThing = false;
    for (int i = 0; i < mapItemValue->points.size(); i++) {
        moonsugar::VectorPoint * vectorPoint = mapItemValue->points.at(i);
        moonsugar::VectorLine * vectorLine = new moonsugar::VectorLine(new cocos2d::Point(*startValue->point), new cocos2d::Point(*vectorPoint->point));
        bool isHit = VectorUtils::checkHit(vectorLine, lines);
        delete vectorLine;
        if (!isHit)
        {
            vectorPoint->g = VectorUtils::calculateG(startValue, vectorPoint);
            vectorPoint->h = VectorUtils::calculateH(endValue, vectorPoint);
            vectorPoint->f = vectorPoint->g + vectorPoint->h;
            //vectorPoint->father = startValue;
            moonsugar::VectorLine * toEndVectorLine = new moonsugar::VectorLine(new cocos2d::Point(*vectorPoint->point), new cocos2d::Point(*endValue->point));
            vectorPoint->isHit = VectorUtils::checkHit(toEndVectorLine, lines);
            delete toEndVectorLine;
            if (!hasUnHitThing) {
                hasUnHitThing = !vectorPoint->isHit;
            }
            vectorPoint->isOpen = true;
            openList.pushBack(vectorPoint);
        }
        else
        {
            vectorPoint->father = nullptr;
        }
    }
    if (openList.size() <= 0) return path;
    
    moonsugar::VectorPoint * searchVectorPoint;
    while (!hasUnHitThing) {
        cocos2d::Vector<moonsugar::VectorPoint *> temp;
        for (int i = 0; i < openList.size(); i ++)
        {
            searchVectorPoint = openList.at(i);
            moonsugar::VectorPoint * vectorPoint = searchVectorPoint->last;
            /*******三角定律,2条边和一定大于第3边，所以在open列表中的不更新g,h,f*****/
            if (!vectorPoint->isOpen && !vectorPoint->isClose)
            {
                vectorPoint->g = VectorUtils::calculateG(searchVectorPoint, vectorPoint) + searchVectorPoint->g;
                vectorPoint->h = VectorUtils::calculateH(endValue, vectorPoint);
                vectorPoint->f = vectorPoint->g + vectorPoint->h;
                vectorPoint->father = searchVectorPoint;
                moonsugar::VectorLine * toEndVectorLine = new moonsugar::VectorLine(new cocos2d::Point(*vectorPoint->point), new cocos2d::Point(*endValue->point));
                vectorPoint->isHit = VectorUtils::checkHit(toEndVectorLine, lines);
                if (!hasUnHitThing) {
                    hasUnHitThing = !vectorPoint->isHit;
                }
                //vectorPoint->isOpen = true;
                //openList.pushBack(vectorPoint);
                temp.pushBack(vectorPoint);
            }
            vectorPoint = searchVectorPoint->next;
            /*******三角定律,2条边和一定大于第3边，所以在open列表中的不更新g,h,f*****/
            if (!vectorPoint->isOpen && !vectorPoint->isClose)
            {
                vectorPoint->g = VectorUtils::calculateG(searchVectorPoint, vectorPoint) + searchVectorPoint->g;
                vectorPoint->h = VectorUtils::calculateH(endValue, vectorPoint);
                vectorPoint->f = vectorPoint->g + vectorPoint->h;
                vectorPoint->father = searchVectorPoint;
                moonsugar::VectorLine * toEndVectorLine = new moonsugar::VectorLine(new cocos2d::Point(*vectorPoint->point), new cocos2d::Point(*endValue->point));
                vectorPoint->isHit = VectorUtils::checkHit(toEndVectorLine, lines);
                if (!hasUnHitThing) {
                    hasUnHitThing = !vectorPoint->isHit;
                }
                //vectorPoint->isOpen = true;
                //openList.pushBack(vectorPoint);
                temp.pushBack(vectorPoint);
            }
        }
        searchVectorPoint->isClose = true;
        closeList.pushBack(searchVectorPoint);
        openList.eraseObject(searchVectorPoint);
        searchVectorPoint->isOpen = false;
        for (int j = 0; j < temp.size(); j ++) {
            openList.pushBack(temp.at(j));
        }
    }
    moonsugar::VectorPoint * min = nullptr;
    /***找到h值最小的***/
    for (int i = 0; i < openList.size(); i ++)
    {
        moonsugar::VectorPoint * vectorPoint = openList.at(i);
        if (!vectorPoint->isHit)
        {
            if (min == nullptr || min->f > vectorPoint->f)
            {
                 min = vectorPoint;
            }
        }
    }
    if (min)
    {
        moonsugar::VectorPoint * vectorPoint = min;
        while (vectorPoint) {
            path.pushBack(vectorPoint);
            vectorPoint = vectorPoint->father;
        }
        path.reverse();
    }
    else
    {
        cocos2d::log("%s", "矢量寻路逻辑错误，openList中没有找到有效的结束点");
    }
    return path;
}

moonsugar::VectorMapItem * VectorUtils::getCross(moonsugar::VectorPoint *startValue, moonsugar::VectorPoint *endValue, moonsugar::VectorMap *mapValue)
{
    moonsugar::VectorMapItem * minMapItem = nullptr;
    float minDistance = -1;
    moonsugar::VectorLine * totalPath = new moonsugar::VectorLine(new cocos2d::Point(*startValue->point), new cocos2d::Point(*endValue->point));
    for (int i = 0; i < mapValue->mapItems.size(); i ++)
    {
        moonsugar::VectorMapItem * mapItem = mapValue->mapItems.at(i);
        cocos2d::Vector<moonsugar::VectorLine *> lines = VectorUtils::getLines(mapItem);
        bool isHit = VectorUtils::checkHit(totalPath, lines);
        if (isHit)
        {
            cocos2d::Point * point = VectorUtils::calculateMiddle(mapItem->points.at(0)->point, mapItem->points.at(2)->point);
            float distance = moonsugar::VectorUtils::getDistance(startValue->point, point);
            if (minDistance == -1 || minDistance > distance)
            {
                minMapItem = mapItem;
                minDistance = distance;
            }
            delete point;
        }
    }
    delete totalPath;
    return minMapItem;
}

cocos2d::Vector<moonsugar::VectorPoint *> VectorUtils::findPath(moonsugar::VectorPoint * startValue, moonsugar::VectorPoint * endValue, moonsugar::VectorMap * map)
{
    //
    //cocos2d::log("findPathBegin");
    cocos2d::Vector<moonsugar::VectorPoint *> path;
    path.pushBack(startValue);
    moonsugar::VectorMapItem * currentMapItem = moonsugar::VectorUtils::getCross(startValue, endValue, map);
    moonsugar::VectorPoint * currentStartValue = startValue;
    while (currentMapItem) {
        //cocos2d::log("currentMapItem x=%f y=%f", currentMapItem->points.at(0)->point->x, currentMapItem->points.at(2)->point->y);
        cocos2d::Vector<moonsugar::VectorLine *> lines = moonsugar::VectorUtils::getLines(currentMapItem);
        currentMapItem->refreshData();
        cocos2d::Vector<moonsugar::VectorPoint*> temp = VectorUtils::getPath(currentStartValue, endValue, currentMapItem, lines);
        if (temp.size() > 0)
        {
            for (int i = 0; i < temp.size(); i ++) {
                path.pushBack(temp.at(i));
            }
            currentStartValue = temp.at(temp.size() - 1);
            currentMapItem = VectorUtils::getCross(currentStartValue, endValue, map);
        }
        else
        {
            currentMapItem = nullptr;
            return path;
        }
    }
    path.pushBack(endValue);
    //cocos2d::log("findPathEnd");
    return path;
}

cocos2d::Point * VectorUtils::calculateMiddle(cocos2d::Point *point1, cocos2d::Point *point2)
{
    float xDistance = (point2->x - point1->x) * .5;
    float yDistance = (point2->y - point1->x) * .5;
    cocos2d::Point * middle = new cocos2d::Point(point1->x + xDistance, point1->y + yDistance);
    return middle;
}

float VectorUtils::calculateDistance(moonsugar::VectorPoint *startValue, moonsugar::VectorPoint *endValue)
{
    float dx = endValue->point->x - startValue->point->x;
    float dy = endValue->point->y - startValue->point->y;
    return sqrtf(dx * dx + dy * dy);
}

float VectorUtils::calculateDistance(cocos2d::Point *startValue, cocos2d::Point *endValue)
{
    float dx = endValue->x - startValue->x;
    float dy = endValue->y - startValue->y;
    float ddx = dx * dx;
    float ddy = dy * dy;
    float total = ddx + ddy;
    return sqrtf(total);
}

bool VectorUtils::calculateRowVector(cocos2d::Point *startValue, cocos2d::Point *endValue)
{
    float direct = endValue->x - startValue->x;
    if (direct >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool VectorUtils::calculateIsAllow(cocos2d::Point checkPoint, moonsugar::VectorMap *mapValue)
{
    if (checkPoint.x >= mapValue->allowRect->origin.x
        && checkPoint.x <= mapValue->allowRect->size.width + mapValue->allowRect->origin.x
        && checkPoint.y >= mapValue->allowRect->origin.y - mapValue->allowRect->size.height
        && checkPoint.y <= mapValue->allowRect->origin.y
        )
    {
        return true;
    }
    return false;
}

bool VectorUtils::calculateIsHitMapItem(cocos2d::Point checkPoint, moonsugar::VectorMap *mapValue)
{
    long n = mapValue->mapItems.size();

    for (int i = 0; i < n; i ++) {
        moonsugar::VectorMapItem * vectorMapItem = mapValue->mapItems.at(i);
        cocos2d::Rect * rect = VectorPointToRectUtils::create(vectorMapItem->points);
        if (checkPoint.x >= rect->origin.x
            && checkPoint.x <= rect->size.width + rect->origin.x
            && checkPoint.y >= rect->origin.y
            && checkPoint.y <= rect->origin.y + rect->size.height
            ) {
            return true;
        }
    }
    return false;
}

bool VectorUtils::calculateIsHitRect(cocos2d::Point checkPoint, cocos2d::Rect * rect)
{
    if (checkPoint.x >= rect->origin.x
        && checkPoint.x <= rect->size.width + rect->origin.x
        && checkPoint.y >= rect->origin.y
        && checkPoint.y <= rect->origin.y + rect->size.height
        )
    {
        return true;
    }
    return false;
}
/**********MapRenderRect************/
MapRenderRectData::MapRenderRectData()
{
    
}

MapRenderRectData::~MapRenderRectData()
{
    delete  size;
}
/***********MapLayerData************/
MapLayerData::MapLayerData()
{
    //
}

MapLayerData::~MapLayerData()
{
    allowRect = nullptr;
    mapRect = nullptr;
}

/*********RollLayerData********/
RollLayerData::RollLayerData()
{
    
}

RollLayerData::~RollLayerData()
{
    delete rect;
    delete mapRenderRectData;
}

/*********ItemLayerData*******/
ItemLayerData::ItemLayerData()
{
    
}

ItemLayerData::~ItemLayerData()
{
    
}

/*********MapItemData*********/
MapItemData::~MapItemData()
{
    delete point;
}

/*********MapActorData*******/
/*********ActorLayerData*******/

/*********MapSceneData*******/
MapSceneData::~MapSceneData()
{
    
}

/********MapSceneDataParse*********/
moonsugar::MapSceneData * moonsugar::MapSceneDataParse::parse(std::string data)
{
    rapidjson::Document dt;
    dt.Parse<0>(data.c_str());
    if (dt.HasParseError())
    {
        cocos2d::log("%s", "MapSceneDataParse error");
    }
    else
    {
        const rapidjson::Value& rectVisible = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(dt, MAP_SCENE_TAG_RECT_VISIBLE);
        const rapidjson::Value& allowRectValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(dt, MAP_SCENE_TAG_ALLOW_RECT);
        cocos2d::Rect * rect = MapSceneDataParse::createRect(rectVisible);
        cocos2d::Rect * allowRect = MapSceneDataParse::createRect(allowRectValue);
        cocos2d::Vector<moonsugar::MapLayerData *> layers;
        int layerCount = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(dt, MAP_SCENE_TAG_LAYERS);
        moonsugar::ItemLayerData * itemLayerData;
        for (int j = 0; j < layerCount; j ++) {
            const rapidjson::Value& mapLayer = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(dt, MAP_SCENE_TAG_LAYERS, j);
            std::string layerType = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(mapLayer, MAP_SCENE_TAG_LAYER_TYPE);
            if (layerType == LAYER_TYPE_ROLL)
            {
                moonsugar::RollLayerData * mapLayerData = new moonsugar::RollLayerData();
                const rapidjson::Value& mapLayerRect = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(mapLayer, MAP_SCENE_TAG_RECT);
                cocos2d::Rect * rect = MapSceneDataParse::createRect(mapLayerRect);
                mapLayerData->rect = rect;
                mapLayerData->roundCount = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(mapLayer, MAP_SCENE_TAG_ROUND_COUNT);
                mapLayerData->isMainLayer = cocostudio::DictionaryHelper::getInstance()->getBooleanValue_json(mapLayer, MAP_SCENE_TAG_IS_MAIN);
                //sourceDataList
                moonsugar::MapRenderRectData * mapRenderRectData = new moonsugar::MapRenderRectData();
                cocos2d::Vector<moonsugar::Source *> sourceList;
                int sourceCount = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(mapLayer, MAP_SCENE_TAG_LAYER_SOURCE_DATA_LIST);
                for (int d = 0; d < sourceCount; d++)
                {
                    const rapidjson::Value& sourceData = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(mapLayer, MAP_SCENE_TAG_LAYER_SOURCE_DATA_LIST, d);
                    std::string sourceName = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(sourceData, MAP_SCENE_TAG_NAME);
                    moonsugar::Source * source = new moonsugar::Source(sourceName);
                    sourceList.pushBack(source);
                }
                mapRenderRectData->sourceList = sourceList;
                cocos2d::Size * size = new cocos2d::Size(mapLayerData->rect->size.width, mapLayerData->rect->size.height);
                mapRenderRectData->size = size;
                mapLayerData->mapRenderRectData = mapRenderRectData;
                mapLayerData->layerType = layerType;
                mapLayerData->allowRect = allowRect;
                mapLayerData->mapRect = rect;
                layers.pushBack(mapLayerData);
            }
            else
            {
                cocos2d::Vector<moonsugar::VectorMapItem *> mapItems;
                itemLayerData = new moonsugar::ItemLayerData();
                int itemsCount = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(mapLayer, MAP_SCENE_TAG_ITEMS);
                for (int i = 0; i < itemsCount; i ++) {
                    const rapidjson::Value& mapItemValue = cocostudio::DICTOOL->getDictionaryFromArray_json(mapLayer, MAP_SCENE_TAG_ITEMS, i);
                    //这部分到时候改称读配置表
                    const rapidjson::Value& mapItemRect = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(mapItemValue, MAP_SCENE_TAG_RECT);
                    cocos2d::Rect * rect = MapSceneDataParse::createRect(mapItemRect);
                    //
                    cocos2d::Vector<VectorPoint *> points = moonsugar::VectorPointToRectUtils::createPoints(rect);
                    moonsugar::MapItemData * mapItem = new moonsugar::MapItemData(points);
                    mapItem->itemId = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(mapItemValue, MAP_SCENE_TAG_ID);
                    cocos2d::Point * mapItemPoint = new cocos2d::Point(rect->origin.x, rect->origin.y);
                    mapItem->point = mapItemPoint;
                    delete  rect;
                    mapItems.pushBack(mapItem);
                }
                itemLayerData->allowRect = allowRect;
                itemLayerData->mapRect = rect;
                itemLayerData->layerType = layerType;
                itemLayerData->mapItemsValue = mapItems;
                layers.pushBack(itemLayerData);
            }
        }
        moonsugar::MapSceneData * mapSceneData = new moonsugar::MapSceneData(rect, itemLayerData, layers, allowRect);
        return mapSceneData;
    }
    return nullptr;
}

cocos2d::Rect * MapSceneDataParse::createRect(const rapidjson::Value &rectValue)
{
    float x = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(rectValue, MAP_SCENE_TAG_X);
    float y = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(rectValue, MAP_SCENE_TAG_Y);
    float width = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(rectValue, MAP_SCENE_TAG_WIDTH);
    float height = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(rectValue, MAP_SCENE_TAG_HEIGHT);
    cocos2d::Rect * rect = new cocos2d::Rect(x, y, width, height);
    return rect;
}
/********MapRenderUtils********/
int MapRenderUtils::calculateReqRect(cocos2d::Rect *rectValue, moonsugar::RollLayerData *mapLayerData)
{
    int count = mapLayerData->roundCount;
    return count;
}

float MapRenderUtils::calculateTotalDistance(moonsugar::RollLayerData *mapLayerData)
{
    return mapLayerData->rect->size.width * mapLayerData->roundCount;
}
/**********MapLayer**************/
bool MapLayer::init()
{
    if (!cocos2d::Layer::init())
    {
        return false;
    }
    return true;
}

MapLayer::MapLayer(moonsugar::MapLayerData * mapLayerDataValue)
{
    mapLayerData = mapLayerDataValue;
    this->hasDistance = 0;
}

MapLayer::~MapLayer()
{
    delete mapLayerData;
    mapLayerData = nullptr;
}
/***********RollMapLayer*************/
MapRollLayer::~MapRollLayer()
{
    
}

void MapRollLayer::initRenderRects()
{
    //cocos2d::log("%s", "------------");
    for (int i = 0; i < mapRenderRects.size(); i ++) {
        cocos2d::Sprite * mapRenderRect = mapRenderRects.at(i);
        mapRenderRect->setPosition(i * mapRenderRect->getSpriteFrame()->getRect().size.width, moonsugar::CoordinateUtils::calculateCoordinateSystem(this->rollLayerData->rect->origin.y, this->mapLayerData->mapRect->size));
        mapRenderRect->setPosition(i * mapRenderRect->getSpriteFrame()->getRect().size.width, this->rollLayerData->rect->origin.y);
        //cocos2d::log("local x=%f, y=%f", mapRenderRect->getPositionX(), mapRenderRect->getPositionY());
        this->addChild(mapRenderRect);
    }
    //cocos2d::log("%s", "------------");
}

bool MapRollLayer::init()
{
    if (!MapLayer::init()) return false;
    return true;
}

MapRollLayer * MapRollLayer::create(moonsugar::RollLayerData *rollLayerDataValue)
{
    MapRollLayer * rollLayer = new moonsugar::MapRollLayer(rollLayerDataValue);
    if (rollLayer && rollLayer->init())
    {
        rollLayer->autorelease();
        return rollLayer;
    }
    else
    {
        delete rollLayer;
        rollLayer = nullptr;
        return nullptr;
    }
}

/************MapScene*************/
MapScene * MapScene::create(moonsugar::MapSceneData *mapSceneDataValue)
{
    MapScene * mapScene = new moonsugar::MapScene(mapSceneDataValue);
    if (mapScene && mapScene->init())
    {
        mapScene->autorelease();
        return mapScene;
    }
    else
    {
        delete mapScene;
        mapScene = NULL;
        return NULL;
    }
}

MapScene::MapScene(moonsugar::MapSceneData * mapSceneDataValue)
{
    hasDistance = 0;
    mapSceneData = mapSceneDataValue;
}

MapScene::~MapScene()
{
    this->removeListeners();
    
    mapSceneData->release();
}

void MapScene::draw(cocos2d::Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
{
    cocos2d::Scene::draw(renderer, transform, transformUpdated);
    //
#if MS_DEBUG_DRAW
    this->_customCommand.init(_globalZOrder);
    this->_customCommand.func = CC_CALLBACK_0(moonsugar::MapScene::onDraw, this, transform, transformUpdated);
    renderer->addCommand(&this->_customCommand);
#endif //MS_DEBUG_DRAW

}
#if MS_DEBUG_DRAW
void MapScene::onDraw(const kmMat4 &transform, bool transformUpdated)
{
    kmGLPushMatrix();
    kmGLLoadMatrix(&transform);
    CHECK_GL_ERROR_DEBUG();

    cocos2d::Point points[4] = {
        cocos2d::Point(this->mapSceneData->allowRect->getMinX(), moonsugar::CoordinateUtils::calculateCoordinateSystem(this->mapSceneData->allowRect->getMinY(), this->mapSceneData->mapRect->size)),
        cocos2d::Point(this->mapSceneData->allowRect->getMinX(), moonsugar::CoordinateUtils::calculateCoordinateSystem(this->mapSceneData->allowRect->getMaxY(), this->mapSceneData->mapRect->size)),
        cocos2d::Point(this->mapSceneData->allowRect->getMaxX(), moonsugar::CoordinateUtils::calculateCoordinateSystem(this->mapSceneData->allowRect->getMaxY(), this->mapSceneData->mapRect->size)),
        cocos2d::Point(this->mapSceneData->allowRect->getMaxX(), moonsugar::CoordinateUtils::calculateCoordinateSystem(this->mapSceneData->allowRect->getMinY(), this->mapSceneData->mapRect->size))
    };
    cocos2d::DrawPrimitives::drawPoly(points, 4, true);
    CHECK_GL_ERROR_DEBUG();
    kmGLPopMatrix();
}
#endif //MS_DEBUG_DRAW
bool MapScene::init()
{
    if (!cocos2d::Scene::init())
    {
        return false;
    }
    for (int i = 0; i < mapSceneData->layers.size(); i ++)
    {
        moonsugar::MapLayerData * mapLayerData = mapSceneData->layers.at(i);
        if (mapLayerData->layerType == LAYER_TYPE_ROLL)
        {
            moonsugar::RollLayerData * rollLayerData = (moonsugar::RollLayerData *)mapLayerData;
            moonsugar::MapRollLayer * mapLayer = MapRollLayer::create(rollLayerData);
            int count = moonsugar::MapRenderUtils::calculateReqRect(mapSceneData->mapRect, (moonsugar::RollLayerData *)mapLayerData);
            cocos2d::Vector<cocos2d::Sprite *> list;
            int tempIndex = 0;
            for (int i = 0; i < count; i ++)
            {
                if (tempIndex >= rollLayerData->mapRenderRectData->sourceList.size()) tempIndex = 0;
                //
                std::string sourceName = rollLayerData->mapRenderRectData->sourceList.at(tempIndex)->sourceName;
                cocos2d::SpriteFrame * sf = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(sourceName);
                cocos2d::Sprite * mapRenderRect = cocos2d::Sprite::createWithSpriteFrame(sf);
                //cocos2d::Sprite * mapRenderRect = cocos2d::Sprite::create(sourceName);
                mapRenderRect->setAnchorPoint(cocos2d::Point(0, 1));
                list.pushBack(mapRenderRect);
                tempIndex ++;
            }
            mapLayer->mapRenderRects = list;
            mapLayer->initRenderRects();
            mapLayers.pushBack(mapLayer);
            this->addChild(mapLayer);
        }
        else if (mapLayerData->layerType == LAYER_TYPE_ITEM)
        {
            moonsugar::ItemLayerData * itemLayerData = (moonsugar::ItemLayerData *)mapLayerData;
            moonsugar::MapItemsLayer * itemsLayer = MapItemsLayer::create(itemLayerData);
            mapLayers.pushBack(itemsLayer);
            this->addChild(itemsLayer);
        }
        if (i == mapSceneData->layers.size() - 2)
        {
            moonsugar::ActorLayerData * actorLayer = new moonsugar::ActorLayerData;
            actorLayer->layerType = LAYER_TYPE_ACTOR;
            actorLayer->allowRect = this->mapSceneData->allowRect;
            actorLayer->mapRect = this->mapSceneData->mapRect;
            moonsugar::MapActorsLayer * mapActorsLayer = this->createMapActorsLayer(actorLayer);
            mapLayers.pushBack(mapActorsLayer);
            this->addChild(mapActorsLayer);
        }
    }
    
    moonsugar::EffectLayerData * effectLayerData = new moonsugar::EffectLayerData;
    effectLayerData->layerType = LAYER_TYPE_EFFECT;
    effectLayerData->allowRect = this->mapSceneData->allowRect;
    effectLayerData->mapRect = this->mapSceneData->mapRect;
    moonsugar::MapEffectLayer * mapEffectLayer = this->createMapEffectLayer(effectLayerData);
    mapLayers.pushBack(mapEffectLayer);
    this->addChild(mapEffectLayer);
    
    moonsugar::UILayerData * uiLayerData = new moonsugar::UILayerData;
    uiLayerData->layerType = LAYER_TYPE_UI;
    uiLayerData->allowRect = this->mapSceneData->allowRect;
    uiLayerData->mapRect = this->mapSceneData->mapRect;
    moonsugar::MapUILayer * mapUILayer = this->createMapUILayer(uiLayerData);
    mapLayers.pushBack(mapUILayer);
    this->addChild(mapUILayer);
    
    this->touchPoint = new cocos2d::Point(0, 0);
    
    this->addListeners();
    return true;
}

moonsugar::MapUILayer * MapScene::createMapUILayer(moonsugar::UILayerData *uiLayerDataValue)
{
    return moonsugar::MapUILayer::create(uiLayerDataValue);
}

moonsugar::MapActorsLayer * MapScene::createMapActorsLayer(moonsugar::ActorLayerData *actorLayerDataValue)
{
    return moonsugar::MapActorsLayer::create(actorLayerDataValue);
}

moonsugar::MapEffectLayer * MapScene::createMapEffectLayer(moonsugar::EffectLayerData *effectLayerDataValue)
{
    return moonsugar::MapEffectLayer::create(effectLayerDataValue);
}

moonsugar::MapActorsLayer * MapScene::getMapActorsLayer()
{
    long n = mapLayers.size();
    for (int i = 0; i < n; i ++) {
        moonsugar::MapLayer * mapLayer = mapLayers.at(i);
        if (mapLayer->mapLayerData->layerType == LAYER_TYPE_ACTOR) return (moonsugar::MapActorsLayer *)mapLayer;
    }
    return nullptr;
}

moonsugar::MapUILayer * MapScene::getMapUILayer()
{
    long n = mapLayers.size();
    for (int i = 0; i < n; i ++) {
        moonsugar::MapLayer * mapLayer = mapLayers.at(i);
        if (mapLayer->mapLayerData->layerType == LAYER_TYPE_UI) return (moonsugar::MapUILayer *)mapLayer;
    }
    return nullptr;
}

moonsugar::MapEffectLayer * MapScene::getMapEffectLayer()
{
    long n = mapLayers.size();
    for (int i = 0; i < n; i++) {
        moonsugar::MapLayer * mapLayer = mapLayers.at(i);
        if (mapLayer->mapLayerData->layerType == LAYER_TYPE_EFFECT) return (moonsugar::MapEffectLayer *)mapLayer;
    }
    return nullptr;
}

void MapScene::addListeners()
{
    touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(MapScene::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(MapScene::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(MapScene::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(MapScene::onTouchCancelled, this);
    auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void MapScene::removeListeners()
{
    auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    eventDispatcher->removeEventListener(touchListener);
    delete touchListener;
}

bool MapScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    this->touchPoint->setPoint(touch->getLocation().x, touch->getLocation().y);
    return true;
}

void MapScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    float dx = touch->getLocation().x - this->touchPoint->x;
    this->cameraMove(dx);
    this->touchPoint->x = touch->getLocation().x;
    this->touchPoint->y = touch->getLocation().y;
}

void MapScene::layeroutOtherLayer(float distance, moonsugar::RollLayerData * mainRollLayerData)
{
//    cocos2d::log("%s", "---------");
    for (int j = 0; j < this->mapLayers.size(); j ++) {
        moonsugar::MapLayer * temp = this->mapLayers.at(j);
        if (temp->mapLayerData->layerType == LAYER_TYPE_ROLL)
        {
            moonsugar::MapRollLayer * tempRollLayer = (moonsugar::MapRollLayer *)temp;
            if (tempRollLayer->rollLayerData->isMainLayer)
            {
                temp->hasDistance = distance;
                temp->setPositionX(0 - temp->hasDistance);
            }
            else
            {
                float tempTotal = moonsugar::MapRenderUtils::calculateTotalDistance(tempRollLayer->rollLayerData) - this->mapSceneData->mapRect->size.width;
                float mainTotal = moonsugar::MapRenderUtils::calculateTotalDistance(mainRollLayerData) - this->mapSceneData->mapRect->size.width;
                temp->hasDistance = distance * (tempTotal / mainTotal);
                temp->setPositionX(0 - temp->hasDistance);
            }
        }
        else if(temp->mapLayerData->layerType != LAYER_TYPE_UI)
        {
            temp->hasDistance = distance;
            temp->setPositionX(0 - temp->hasDistance);
        }
    }
}

void MapScene::localCamera(float distance)
{
    moonsugar::MapLayer * layer = nullptr;
    for (int i = 0; i < this->mapLayers.size(); i ++) {
        layer = mapLayers.at(i);
        if (layer->mapLayerData->layerType == LAYER_TYPE_ROLL)
        {
            moonsugar::MapRollLayer * mapRollLayer = (moonsugar::MapRollLayer *) layer;
            if (mapRollLayer->rollLayerData->isMainLayer)
            {
                if (distance < 0) {
                    distance = 0;
                }
                else
                {
                    float total = MapRenderUtils::calculateTotalDistance(mapRollLayer->rollLayerData) - this->mapSceneData->mapRect->size.width;
                    if (distance > total)
                    {
                        distance = total;
                    }
                }
                layeroutOtherLayer(distance, mapRollLayer->rollLayerData);
                break;
            }
        }
    }
}

void MapScene::cameraMove(float dx)
{
    moonsugar::MapLayer * layer = nullptr;
    float mainDistance = 0;
    for (int i = 0; i < this->mapLayers.size(); i ++) {
        layer = mapLayers.at(i);
        if (layer->mapLayerData->layerType == LAYER_TYPE_ROLL)
        {
            moonsugar::MapRollLayer * mapRollLayer = (moonsugar::MapRollLayer *) layer;
            if (mapRollLayer->rollLayerData->isMainLayer)
            {
                mainDistance = layer->hasDistance - dx;
                if (mainDistance < 0)
                {
                    mainDistance = 0;
                }
                else
                {
                    float total = MapRenderUtils::calculateTotalDistance(mapRollLayer->rollLayerData) - this->mapSceneData->mapRect->size.width;
                    if (mainDistance > total)
                    {
                        mainDistance = total;
                    }
                }
                layeroutOtherLayer(mainDistance, mapRollLayer->rollLayerData);
                return;
            }
        }
    }
}

void MapScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
}


void MapScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
}

void MapScene::onTouchEndedOrCancelledExecute(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
}
/***********MapActorsLayer**********/
MapActorsLayer::~MapActorsLayer()
{
    actorLayerData = nullptr;
}

MapActorsLayer * MapActorsLayer::create(moonsugar::ActorLayerData *actorLayerDataValue)
{
    MapActorsLayer * actorsLayer = new moonsugar::MapActorsLayer(actorLayerDataValue);
    if (actorsLayer && actorsLayer->init())
    {
        actorsLayer->autorelease();
        return actorsLayer;
    }
    else
    {
        delete  actorsLayer;
        actorsLayer = nullptr;
        return nullptr;
    }
}

bool MapActorsLayer::init()
{
    if (!cocos2d::Layer::init()) return false;
    return true;
}

/**********MapUILayer***************/
MapUILayer::~MapUILayer()
{
    
}

MapUILayer * MapUILayer::create(moonsugar::UILayerData *uiLayerDataValue)
{
    MapUILayer * uiLayer = new moonsugar::MapUILayer(uiLayerDataValue);
    if (uiLayer && uiLayer->init())
    {
        uiLayer->autorelease();
        return uiLayer;
    }
    else
    {
        delete uiLayer;
        uiLayer = nullptr;
        return nullptr;
    }
}

bool MapUILayer::init()
{
    if (!cocos2d::Layer::init()) return false;
    return true;
}

/***********MapEffectLayer*************/
MapEffectLayer * MapEffectLayer::create(moonsugar::EffectLayerData *effectLayerDataValue)
{
    MapEffectLayer * effectLayer = new moonsugar::MapEffectLayer(effectLayerDataValue);
    if (effectLayer && effectLayer->init())
    {
        effectLayer->autorelease();
        return effectLayer;
    }
    else
    {
        delete effectLayer;
        effectLayer = nullptr;
        return nullptr;
    }
}

bool MapEffectLayer::init()
{
    if (!cocos2d::Layer::init()) return false;
    return true;
}

/***********MapItemsLayer***********/
MapItemsLayer::~MapItemsLayer()
{
    itemLayerData = nullptr;
}

MapItemsLayer * MapItemsLayer::create(moonsugar::ItemLayerData * itemLayerDataValue)
{
    MapItemsLayer * itemsLayer = new moonsugar::MapItemsLayer(itemLayerDataValue);
    if (itemsLayer && itemsLayer->init())
    {
        itemsLayer->autorelease();
        return itemsLayer;
    }
    else
    {
        delete itemsLayer;
        itemsLayer = nullptr;
        return nullptr;
    }
}

bool MapItemsLayer::init()
{
    if (!cocos2d::Layer::init()) return false;
    for (int i = 0; i < this->itemLayerData->mapItemsValue.size(); i ++)
    {
        moonsugar::MapItemData * mapItemData = (moonsugar::MapItemData *)this->itemLayerData->mapItemsValue.at(i);
        //moonsugar::VectorMapItemDef * def = moonsugar::VectorMapItemsDef::getInstance()->getMapItemByItemId(mapItemData->itemId);
        std::string sourceName = moonsugar::TypeConver::intToString(mapItemData->itemId);
        sourceName += SOURCE_ENUM_FILE_FORMAT_PNG;
        cocos2d::SpriteFrame * sf = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(sourceName);
        moonsugar::MapRectItem * sp = moonsugar::MapRectItem::createWithSpriteFrame(sf, mapItemData);
        sp->cocos2d::Node::setAnchorPoint(cocos2d::Point(0, 0));
        this->addChild(sp);
        sp->setPosition(mapItemData->point->x, mapItemData->point->y);
    }
    return true;
}

void MapItemsLayer::draw(cocos2d::Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
{
    MapLayer::draw(renderer, transform, transformUpdated);
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(MapItemsLayer::onDraw, this, transform, transformUpdated);
    renderer->addCommand(&_customCommand);
}

void MapItemsLayer::onDraw(const kmMat4 &transform, bool transformUpdated)
{
    kmGLPushMatrix();
    kmGLLoadMatrix(&transform);
    CHECK_GL_ERROR_DEBUG();
    for (int i = 0; i < this->itemLayerData->mapItemsValue.size(); i ++)
    {
        moonsugar::MapItemData * mapItemData = (moonsugar::MapItemData *)this->itemLayerData->mapItemsValue.at(i);
        cocos2d::DrawPrimitives::drawRect(mapItemData->mapItemRect->origin, cocos2d::Point(mapItemData
                                                                                        ->mapItemRect->origin.x + mapItemData->mapItemRect->size.width, mapItemData->mapItemRect->origin.y + mapItemData->mapItemRect->size.height));
    }
    CHECK_GL_ERROR_DEBUG();
    cocos2d::Point points[4] = {
        cocos2d::Point(this->mapLayerData->allowRect->getMinX(), this->mapLayerData->allowRect->getMinY()),
        cocos2d::Point(this->mapLayerData->allowRect->getMinX(), this->mapLayerData->allowRect->getMaxY()),
        cocos2d::Point(this->mapLayerData->allowRect->getMaxX(), this->mapLayerData->allowRect->getMaxY()),
        cocos2d::Point(this->mapLayerData->allowRect->getMaxX(), this->mapLayerData->allowRect->getMinY())
    };
    CHECK_GL_ERROR_DEBUG();
    cocos2d::DrawPrimitives::drawPoly(points, 4, true);
    kmGLPopMatrix();
}

/**************MapRectItem************/
MapRectItem::MapRectItem(moonsugar::MapItemData * mapItemDataValue)
{
    itemData = mapItemDataValue;
}

MapRectItem::~MapRectItem()
{
    
}

bool MapRectItem::initWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame)
{
    return cocos2d::Sprite::initWithSpriteFrame(spriteFrame);
}

MapRectItem * MapRectItem::createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame, moonsugar::MapItemData * itemDatavalue)
{
    MapRectItem * mapRectItem = new MapRectItem(itemDatavalue);
    if (mapRectItem && mapRectItem->initWithSpriteFrame(spriteFrame))
    {
        mapRectItem->autorelease();
        return mapRectItem;
    }
    else
    {
        delete mapRectItem;
        mapRectItem = nullptr;
        return nullptr;
    }
}
NS_MS_END;