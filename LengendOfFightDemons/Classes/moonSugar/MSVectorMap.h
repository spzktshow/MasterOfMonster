//
//  MSVectorMap.h
//  Vector
//
//  Created by RockLee on 14-3-25.
//
//

#ifndef __Vector__MSVectorMap__
#define __Vector__MSVectorMap__

#include "cocos2d.h"
#include "moonSugar.h"
#include "MSGeometry.h"
#include "VectorAlgorithm.h"
#include <vector>
#include "MSSource.h"
#include "external/json/rapidjson.h"
#include "external/json/document.h"
#include "editor-support/cocostudio/DictionaryHelper.h"

#define MAP_SCENE_TAG_ITEMS            "items"

#define MAP_SCENE_TAG_NAME              "name"
#define MAP_SCENE_TAG_ID                "id"
#define MAP_SCENE_TAG_RECT              "rect"
#define MAP_SCENE_TAG_WIDTH             "width"
#define MAP_SCENE_TAG_X                 "x"
#define MAP_SCENE_TAG_HEIGHT            "height"
#define MAP_SCENE_TAG_Y                 "y"

#define MAP_SCENE_TAG_LAYERS                "layers"

#define MAP_SCENE_TAG_SPEED                 "speed"
#define MAP_SCENE_TAG_RECT                  "rect"
#define MAP_SCENE_TAG_IS_MAIN               "isMain"
#define MAP_SCENE_TAG_ROUND_COUNT           "roundCount"
#define MAP_SCENE_TAG_LAYER_SOURCE_DATA_LIST "layerSourceDataList"
#define MAP_SCENE_TAG_RECT_VISIBLE          "rectVisible"
#define MAP_SCENE_TAG_ALLOW_RECT            "allowRect"
#define MAP_SCENE_TAG_LAYER_TYPE            "layerType"

//layerTypeEnum
#define LAYER_TYPE_ROLL             "rollType"
#define LAYER_TYPE_ITEM             "itemType"
#define LAYER_TYPE_ACTOR            "actorType"
#define LAYER_TYPE_UI               "uiType"
#define LAYER_TYPE_EFFECT           "effectType"

NS_MS_BEGIN

class VectorPoint : public cocos2d::Ref
{
public:
    VectorPoint(cocos2d::Point * pointValue);
    ~VectorPoint();
    
    cocos2d::Point * point;
    
    moonsugar::VectorPoint * father;
    
    /******在描述多边形时，顺时针描述的矢量点*****/
    moonsugar::VectorPoint * last;
    moonsugar::VectorPoint * next;
    
    float f;//h + g
    float g;//father to this
    float h;//this to end
    bool isHit;//is h cross
    
    bool isClose;
    bool isOpen;
};

class VectorPointToRectUtils
{
public:
    static cocos2d::Rect * create(cocos2d::Vector<moonsugar::VectorPoint *> pointsValue);
    /*****假设高层用户顺序传入矢量点，且为顺时针，尝试对数组中的VectorPoint进行描述，生成一个顺时针方向的多边形****/
    static void describe(cocos2d::Vector<moonsugar::VectorPoint *> pointsValue);
    /************通过矩阵创建点列表**********/
    static cocos2d::Vector<moonsugar::VectorPoint *> createPoints(cocos2d::Rect * rectValue);
};

class VectorMapItem : public cocos2d::Ref
{
public:
    /*****传入顶点信息,顶点信息必须是连续的,且顺时针******/
    VectorMapItem(cocos2d::Vector<moonsugar::VectorPoint *> pointsValue);
    ~VectorMapItem();
    
    cocos2d::Vector<moonsugar::VectorPoint *> points;
    /*****刷新isClose isOpen*****/
    void refreshData();
};

class VectorRectMapItem : public moonsugar::VectorMapItem
{
public:
    VectorRectMapItem(cocos2d::Vector<moonsugar::VectorPoint *> pointsValue):VectorMapItem(pointsValue)
    {
        mapItemRect = VectorPointToRectUtils::create(pointsValue);
    };
    ~VectorRectMapItem();
    
    cocos2d::Rect * mapItemRect;
};

class VectorMap : public cocos2d::Ref
{
public:
    /********矢量地图*******/
    VectorMap(cocos2d::Rect * mapRectValue, cocos2d::Vector<moonsugar::VectorMapItem *> mapItemsValue, cocos2d::Rect * allowRectValue);
    ~VectorMap();
    
    cocos2d::Rect * mapRect;
    cocos2d::Vector<moonsugar::VectorMapItem *> mapItems;
    cocos2d::Rect * allowRect;
};

class VectorUtils
{
public:
    /***获取多边形的线段**/
    static cocos2d::Vector<moonsugar::VectorLine *> getLines(moonsugar::VectorMapItem * mapItemValue);
    /***检测碰撞，noCross isn't Hit********/
    static bool checkHit(moonsugar::VectorLine * pathValue, cocos2d::Vector<moonsugar::VectorLine *> lines);
    /*****计算距离****************/
    static float getDistance(cocos2d::Point * point1, cocos2d::Point * point2);
    /*************计算矩形得中心点,如果以后改为多边形检测，则这个方法要替换成计算多边形中心点得方法******************/
    static cocos2d::Point * calculateMiddle(cocos2d::Point * point1, cocos2d::Point * point2);
    /**************计算G*************/
    static float calculateG(moonsugar::VectorPoint * startValue, moonsugar::VectorPoint * currentValue);
    /**************计算H*************/
    static float calculateH(moonsugar::VectorPoint * endValue, moonsugar::VectorPoint * currentValue);
    /*****获取带有向量信息的路径****/
    static cocos2d::Vector<moonsugar::VectorPoint *> getPath(moonsugar::VectorPoint * startValue, moonsugar::VectorPoint * endValue, moonsugar::VectorMapItem * mapItemValue, cocos2d::Vector<moonsugar::VectorLine *> lines);
    /*********检测VectorPoint equal****************/
    static bool checkEqual(moonsugar::VectorPoint * a, moonsugar::VectorPoint * b);
    /*********检测地图上所有碰撞到得VectorMapItem中离startValue最近得MapItem***********/
    static moonsugar::VectorMapItem * getCross(moonsugar::VectorPoint * startValue, moonsugar::VectorPoint * endValue, moonsugar::VectorMap * mapValue);
    /*********搜索全路径*************/
    static cocos2d::Vector<moonsugar::VectorPoint *> findPath(moonsugar::VectorPoint * startValue, moonsugar::VectorPoint * endValue, moonsugar::VectorMap * mapValue);
    /***********计算距离**********/
    static float calculateDistance(moonsugar::VectorPoint * startValue, moonsugar::VectorPoint * endValue);
    static float calculateDistance(cocos2d::Point * startValue, cocos2d::Point * endValue);
    /**********计算向量**********/
    static bool calculateRowVector(cocos2d::Point * startValue, cocos2d::Point * endValue);
    /***********检测点是否在allow中***********/
    static bool calculateIsAllow(cocos2d::Point checkPoint, moonsugar::VectorMap * mapValue);
    /************计算是否碰撞MapItem*************/
    static bool calculateIsHitMapItem(cocos2d::Point checkPoint, moonsugar::VectorMap * mapValue);
    /************计算碰撞rect********************/
    static bool calculateIsHitRect(cocos2d::Point checkPoint, cocos2d::Rect * rect);
};

/*****************Map*****************************/
class MapRenderRectData : public cocos2d::Ref
{
public:
    MapRenderRectData();
    ~MapRenderRectData();
    
    cocos2d::Vector<moonsugar::Source *> sourceList;
    /******渲染块的大小******/
    cocos2d::Size * size;
};

class MapLayerData : public cocos2d::Ref
{
public:
    MapLayerData();
    ~MapLayerData();
    
    std::string layerType;
    //ref
    cocos2d::Rect * allowRect;
    //ref
    cocos2d::Rect * mapRect;
};

class RollLayerData : public moonsugar::MapLayerData
{
public:
    RollLayerData();
    ~RollLayerData();
    
    /***循环次数**********/
    int roundCount;
    /***rect****/
    cocos2d::Rect * rect;
    /****资源列表组成的渲染块，每个渲染块的大小一样，且就是rect.size.width*****/
    //cocos2d::Vector<moonsugar::Source *> sourceList;
    moonsugar::MapRenderRectData * mapRenderRectData;
    /****是否是主layer****/
    bool isMainLayer;
};

class MapItemData : public moonsugar::VectorRectMapItem
{
public:
    MapItemData(cocos2d::Vector<moonsugar::VectorPoint *> pointsValue):VectorRectMapItem(pointsValue){};
    ~MapItemData();
    /*******point*******/
    cocos2d::Point * point;
    /*******id********/
    int itemId;
};

class ItemLayerData : public moonsugar::MapLayerData
{
public:
    ItemLayerData();
    ~ItemLayerData();
    
    cocos2d::Vector<moonsugar::VectorMapItem *> mapItemsValue;
};

class MapActorData : public cocos2d::Node
{
public:
    MapActorData(){};
    ~MapActorData(){
        delete point;
    };
    
    /******point*******/
    cocos2d::Point * point;
    /******actorId******/
    int actorId;
};

class ActorLayerData : public moonsugar::MapLayerData
{
public:
    ActorLayerData(){};
    ~ActorLayerData(){};
    
    cocos2d::Vector<moonsugar::MapActorData *> mapActorDatas;
};

class UILayerData : public moonsugar::MapLayerData
{
public:
    UILayerData(){};
    ~UILayerData(){};
};

/**************特效层数据**************/
class EffectLayerData : public moonsugar::MapLayerData
{
public:
    EffectLayerData(){};
    ~EffectLayerData(){};
};

class MapSceneData : public moonsugar::VectorMap
{
public:
    MapSceneData(cocos2d::Rect * mapRectValue, moonsugar::ItemLayerData * itemsLayer, cocos2d::Vector<moonsugar::MapLayerData *> layersValue, cocos2d::Rect * allowRectValue):VectorMap(mapRectValue, itemsLayer->mapItemsValue, allowRectValue)
    {
        layers = layersValue;
    };
    ~MapSceneData();
    /*****layers****/
    cocos2d::Vector<moonsugar::MapLayerData *> layers;
};

class MapSceneDataParse
{
public:
    static moonsugar::MapSceneData * parse(std::string data);
    
    static cocos2d::Rect * createRect(const rapidjson::Value &rectValue);
};

class MapRenderUtils
{
public:
    /*********计算需求的渲染块***********/
    static int calculateReqRect(cocos2d::Rect * rectValue, moonsugar::RollLayerData * mapLayerData);
    /************计算总长度*************/
    static float calculateTotalDistance(moonsugar::RollLayerData * mapLayerData);
};

class MapLayer : public cocos2d::Layer
{
public:
    MapLayer(moonsugar::MapLayerData * mapLayerDataValue);
    ~MapLayer();
    
    moonsugar::MapLayerData * mapLayerData;
    
    float hasDistance;
    virtual bool init();
};

class MapRollLayer : public moonsugar::MapLayer
{
public:
    MapRollLayer(moonsugar::RollLayerData * mapLayerDataValue):MapLayer(mapLayerDataValue)
    {
        rollLayerData = mapLayerDataValue;
    };
    ~MapRollLayer();
    
    /*******MapRenderRects*******/
    cocos2d::Vector<cocos2d::Sprite *> mapRenderRects;
    
    moonsugar::RollLayerData * rollLayerData;
    
    virtual bool init();
    
    virtual void initRenderRects();
    
    static MapRollLayer * create(moonsugar::RollLayerData * rollLayerDataValue);
};

class MapRectItem : public cocos2d::Sprite
{
public:
    MapRectItem(moonsugar::MapItemData * itemDataValue);
    ~MapRectItem();
    
    moonsugar::MapItemData * itemData;
    static moonsugar::MapRectItem * createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame, moonsugar::MapItemData * itemDatavalue);
    
    virtual bool initWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame);
protected:
};

class MapItemsLayer : public moonsugar::MapLayer
{
public:
    MapItemsLayer(moonsugar::ItemLayerData * itemLayerDataValue):MapLayer(itemLayerDataValue)
    {
        itemLayerData = itemLayerDataValue;
    };
    ~MapItemsLayer();
    
    static MapItemsLayer * create(moonsugar::ItemLayerData * itemLayerDataValue);
    
    virtual void draw(cocos2d::Renderer *renderer, const kmMat4& transform, bool transformUpdated);
    virtual bool init();
    
    moonsugar::ItemLayerData * itemLayerData;
protected:
    virtual void onDraw(const kmMat4& transform, bool transformUpdated);
    cocos2d::CustomCommand _customCommand;
};

class MapActorsLayer : public moonsugar::MapLayer
{
public:
    MapActorsLayer(moonsugar::ActorLayerData * actorLayerDataValue):MapLayer(actorLayerDataValue){
        actorLayerData = actorLayerDataValue;
    };
    ~MapActorsLayer();
    
    moonsugar::ActorLayerData * actorLayerData;
    
    static MapActorsLayer * create(moonsugar::ActorLayerData * actorLayerDataValue);
    
    virtual bool init();
};

class MapUILayer : public moonsugar::MapLayer
{
public:
    MapUILayer(moonsugar::UILayerData * uiLayerDataValue):MapLayer(uiLayerDataValue){};
    ~MapUILayer();
    
    static MapUILayer * create(moonsugar::UILayerData * uiLayerDataValue);
    
    virtual bool init();
};

class MapEffectLayer : public moonsugar::MapLayer
{
public:
    MapEffectLayer(moonsugar::EffectLayerData * effectLayerDataValue):MapLayer(effectLayerDataValue){};
    ~MapEffectLayer(){};
    
    static MapEffectLayer * create(moonsugar::EffectLayerData * effectLayerDataValue);
    
    virtual bool init();
};

class MapScene : public cocos2d::Scene
{
public:
    MapScene(moonsugar::MapSceneData * mapSceneDataValue);
    ~MapScene();
    
    /****mapSceneData*****/
    moonsugar::MapSceneData * mapSceneData;
    
    /****MapLayers********/
    cocos2d::Vector<moonsugar::MapLayer *> mapLayers;
    
    float hasDistance;
    
    /****绘制********/
    virtual void draw(cocos2d::Renderer *renderer, const kmMat4& transform, bool transformUpdated);
    
    static moonsugar::MapScene * create(moonsugar::MapSceneData * mapSceneDataValue);
    virtual bool init();
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    void onTouchEndedOrCancelledExecute(cocos2d::Touch * touch, cocos2d::Event *unused_event);
    
    /***touch listener***/
    cocos2d::EventListenerTouchOneByOne * touchListener;
    cocos2d::Point * touchPoint;
    
    void cameraMove(float dx);
    void localCamera(float distance);
    
    virtual void addListeners();
    virtual void removeListeners();
    
    moonsugar::MapActorsLayer * getMapActorsLayer();
    moonsugar::MapUILayer * getMapUILayer();
    moonsugar::MapEffectLayer * getMapEffectLayer();
protected:
#if MS_DEBUG_DRAW
    virtual void onDraw(const kmMat4& transform, bool transformUpdated);
    cocos2d::CustomCommand _customCommand;
#endif //MS_DEBUG_DRAW
    
    void layeroutOtherLayer(float distance, moonsugar::RollLayerData * mainRollLayerData);
    
    virtual MapUILayer * createMapUILayer(moonsugar::UILayerData * uiLayerDataValue);
    virtual MapActorsLayer * createMapActorsLayer(moonsugar::ActorLayerData * actorLayerDataValue);
    virtual MapEffectLayer * createMapEffectLayer(moonsugar::EffectLayerData * effectLayerDataValue);
};
NS_MS_END;

#endif /* defined(__Vector__MSVectorMap__) */
