#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "VectorAlgorithm.h"
#include "MSVectorMap.h"

#define POINT_RADIUS        10

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    cocos2d::Rect * drawRect;
    
    cocos2d::Vector<moonsugar::VectorLine *> lines;
    
    moonsugar::VectorMap * map;
    
    moonsugar::VectorPoint * startPoint;
    moonsugar::VectorPoint * endPoint;
    
    //virtual void draw(cocos2d::Renderer *renderer, const kmMat4& transform, bool transformUpdated);
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void addTouchEvents();
    void removeTouchEvents();
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    void onTouchEndedOrCancelledExecute(cocos2d::Touch * touch, cocos2d::Event *unused_event);
    
    /***touch listener***/
    cocos2d::EventListenerTouchOneByOne * touchListener;
    cocos2d::Point * touchPoint;
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
