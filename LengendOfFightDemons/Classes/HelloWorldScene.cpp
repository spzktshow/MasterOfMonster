#include "HelloWorldScene.h"
#include "VectorAlgorithm.h"
#include "MSVectorMap.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);
    layer->addTouchEvents();

    // return the scene
    return scene;
}
/***
void HelloWorld::draw(cocos2d::Renderer *renderer, const kmMat4& transform, bool transformUpdated)
{
    Layer::draw(renderer, transform, transformUpdated);
    return;
//    if (drawRect != nullptr)
//    {

//        cocos2d::log("x=%f, y=%f, width=%f, height=%f", drawRect->origin.x, drawRect->origin.y, drawRect->size.width, drawRect->size.height);
//        DrawPrimitives::drawRect(cocos2d::Point(drawRect->getMinX(), drawRect->getMinY()), cocos2d::Point(drawRect->getMaxX(), drawRect->getMaxY()));
    ///
//        for (int i = 0; i < lines.size(); i ++) {
//            moonsugar::VectorLine & drawL = *lines.at(i);
//            DrawPrimitives::drawLine(*drawL.p1, *drawL.p2);
//        }

//    }
    ///
    CHECK_GL_ERROR_DEBUG();
    //drawStart
    DrawPrimitives::setDrawColor4F(0, 255, 0, 255);
    DrawPrimitives::drawCircle(*startPoint->point, POINT_RADIUS, 0, 20, true);
    
    //drawMapItem
    DrawPrimitives::setDrawColor4F(0, 70, 140, 255);
    for (int i = 0; i < map->mapItems.size(); i ++)
    {
        moonsugar::VectorMapItem * mapItem = map->mapItems.at(i);
        DrawPrimitives::drawRect(cocos2d::Point(mapItem->points.at(0)->point->x, mapItem->points.at(0)->point->y), cocos2d::Point(mapItem->points.at(2)->point->x, mapItem->points.at(2)->point->y));
    }
    
    //drawEnd
    if (endPoint)
    {
        DrawPrimitives::setDrawColor4F(255, 70, 140, 255);
        DrawPrimitives::drawCircle(*endPoint->point, POINT_RADIUS, 0, 10, true);
        
        moonsugar::VectorLine * totalPath = new moonsugar::VectorLine(new cocos2d::Point(*startPoint->point), new cocos2d::Point(*endPoint->point));
        //drawTotalPath
        //DrawPrimitives::setDrawColor4F(140, 100, 140, 255);
        //DrawPrimitives::drawLine(*totalPath->p1, *totalPath->p2);
        //暂时弄一个
        cocos2d::Vector<moonsugar::VectorPoint *> paths = moonsugar::VectorUtils::findPath(startPoint, endPoint, map);
        if (paths.size() >= 0)
        {
            DrawPrimitives::setDrawColor4F(255, 255, 255, 255);
            for (int i = 0; i < paths.size(); i ++)
            {
                if (i < paths.size() - 1)
                {
                    DrawPrimitives::drawLine(*paths.at(i)->point, *paths.at(i + 1)->point);
                }
            }
        }
        //
        
//        while(currentMapItem)
//        {
//            cocos2d::Vector<moonsugar::VectorLine *> lines = moonsugar::VectorUtils::getLines(currentMapItem);
//            cocos2d::Vector<moonsugar::VectorPoint *> path = moonsugar::VectorUtils::getPath(startPoint, endPoint, currentMapItem, lines);
//            
//        }
//        for (int i = 0; i < 1; i ++) {
//            cocos2d::Vector<moonsugar::VectorLine *> lines = moonsugar::VectorUtils::getLines(map->mapItems.at(i));
//            bool isHit = moonsugar::VectorUtils::checkHit(totalPath, lines);
//            if (isHit)
//            {
//                cocos2d::log("%s", "isHit");
//                //
//                
//                int length = path.size();
//                cocos2d::log("paths length%d", length);
//                if (length > 0)
//                {
//                    DrawPrimitives::setDrawColor4F(255, 255, 255, 255);
//
//                    DrawPrimitives::drawLine(*startPoint->point, *path.at(0)->point);
//                    for (int i = 0; i < path.size(); i ++)
//                    {
//                        if (i < path.size() - 1)
//                        {
//                            DrawPrimitives::drawLine(*path.at(i)->point, *path.at(i + 1)->point);
//                        }
//                    }
//                    DrawPrimitives::drawLine(*path.at(path.size() - 1)->point, *endPoint->point);
//                }
//            }
//            else
//            {
//                cocos2d::log("%s", "notHit");
//            }
//        }
        delete  totalPath;
    }
}
 **/

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);
    
//    cocos2d::Point * point1 = new cocos2d::Point(2, 2);
//    cocos2d::Point * point2 = new cocos2d::Point(300, 280);
//    moonsugar::VectorLine * vectorLine = new moonsugar::VectorLine(point1, point2);
//    cocos2d::Rect * rect = moonsugar::VectorAlgorithm::createRect(*vectorLine);
//    drawRect = rect;
    ///
//    cocos2d::Point * p1 = new cocos2d::Point(200, 200);
//    cocos2d::Point * p2 = new cocos2d::Point(400, 400);
//    cocos2d::Point * q1 = new cocos2d::Point(400, 400);
//    cocos2d::Point * q2 = new cocos2d::Point(600, 400);
//    moonsugar::VectorLine * line1 = new moonsugar::VectorLine(p1, p2);
//    moonsugar::VectorLine * line2 = new moonsugar::VectorLine(q1, q2);
//    lines.pushBack(line1);
//    lines.pushBack(line2);
//    bool isCheck = moonsugar::VectorAlgorithm::fastExclude(*line1, *line2);
//    if (isCheck)
//    {
//        cocos2d::log("%s", "fastExclude successed");
//        bool isDo = moonsugar::VectorAlgorithm::straddle(*line1, *line2, false);
//        if (isDo)
//        {
//            cocos2d::log("%s", "straddle successed");
//        }
//        else
//        {
//            cocos2d::log("%s", "straddle failed");
//        }
//    }
//    else
//    {
//        cocos2d::log("%s", "fastExclude failed");
//    }
    ///
    endPoint = nullptr;
    cocos2d::Rect * rect = new cocos2d::Rect(0, 0, 960, 640);
        cocos2d::Vector<moonsugar::VectorMapItem *> mapItems;
    for (int i = 0; i < 30; i ++)
    {
        cocos2d::Vector<moonsugar::VectorPoint *> points;
        cocos2d::Rect rect(rand() % 960, rand() % 640, rand() % 200, rand() % 200);
        points.pushBack(new moonsugar::VectorPoint(new cocos2d::Point(rect.getMinX(), rect.getMinY())));
        points.pushBack(new moonsugar::VectorPoint(new cocos2d::Point(rect.getMinX(), rect.getMaxY())));
        points.pushBack(new moonsugar::VectorPoint(new cocos2d::Point(rect.getMaxX(), rect.getMaxY())));
        points.pushBack(new moonsugar::VectorPoint(new cocos2d::Point(rect.getMaxX(), rect.getMinY())));
        moonsugar::VectorMapItem * mapItem = new moonsugar::VectorMapItem(points);
        mapItems.pushBack(mapItem);
    }
    map = new moonsugar::VectorMap(rect, mapItems, nullptr);
    startPoint = new moonsugar::VectorPoint(new cocos2d::Point(100, 150));
    //endPoint = new moonsugar::VectorPoint(new cocos2d::Point(590, 187));
    //moonsugar::VectorPoint * endPoint = new moonsugar::VectorPoint(new cocos2d::Point(400, 150));
//    cocos2d::Vector<moonsugar::VectorLine *> lines = moonsugar::VectorUtils::getLines(mapItem);
//    struct timeval now;
//    struct timeval last;
//    gettimeofday(&last, nullptr);
//    cocos2d::Vector<moonsugar::VectorPoint *> path = moonsugar::VectorUtils::getPath(point, endPoint, mapItem, lines);
//    gettimeofday(&now, nullptr);
//    float gap = (now.tv_sec - last.tv_sec) * 1000 + (now.tv_usec - last.tv_usec) * 0.001;
//    cocos2d::log("gap %f", gap);
//    int length = path.size();
//    cocos2d::log("path %d", length);
    
    std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("scene.plist");
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(fullPath);
    cocos2d::SpriteFrame * backgroundSF = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("background.png");
    cocos2d::SpriteFrame * middle = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("middle.png");
    cocos2d::SpriteFrame * front = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("front.png");
    cocos2d::Sprite * rectTotal = Sprite::create();
    cocos2d::Sprite * front1 = Sprite::createWithSpriteFrame(front);
    cocos2d::Sprite * frong2 = Sprite::createWithSpriteFrame(front);
    this->addChild(front1);
    front1->setPositionY(visibleSize.height/2);
    rectTotal->addChild(frong2);
    frong2->setPositionX(front->getTexture()->getContentSize().width);
    frong2->setPositionY(front1->getPositionY());
    this->addChild(rectTotal);
    return true;
}

void HelloWorld::addTouchEvents()
{
    touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
    auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void HelloWorld::removeTouchEvents()
{
    auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    eventDispatcher->removeEventListener(touchListener);
    delete touchListener;
}

bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    //cocos2d::log("%s", "touchBegan");
    endPoint = new moonsugar::VectorPoint(new cocos2d::Point(touch->getLocation()));
    return true;
}

void HelloWorld::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    //cocos2d::log("%s", "touchCancelled");
    cocos2d::Layer::onTouchCancelled(touch, unused_event);
    onTouchEndedOrCancelledExecute(touch, unused_event);
}

void HelloWorld::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    //cocos2d::log("%s", "touchMoved");
    cocos2d::Layer::onTouchMoved(touch, unused_event);
    
    if (endPoint != nullptr)
    {
        cocos2d::Point location = touch->getLocation();
        endPoint->point->setPoint(location.x, location.y);
    }
}

void HelloWorld::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    //cocos2d::log("%s", "touchEnded");
    cocos2d::Layer::onTouchEnded(touch, unused_event);
    onTouchEndedOrCancelledExecute(touch, unused_event);
}

void HelloWorld::onTouchEndedOrCancelledExecute(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    delete  endPoint;
    endPoint = nullptr;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
