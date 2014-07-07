//
//  MSDataStructure.h
//  Run
//
//  Created by RockLee on 14-3-10.
//
//

#ifndef __Run__MSDataStructure__
#define __Run__MSDataStructure__

#include <iostream>
#include "moonSugar.h"
#include <vector>

NS_MS_BEGIN

/*********************Queue*******************/
class QueueItem : public cocos2d::Ref
{
public:
    /***队列的item名称，在队列中必须保持唯一性**/
    std::string itemName;
    
    QueueItem(std::string itemNameValue);
    ~QueueItem();
};

class Queue
{
public:
    /***队列的名称，标示队列的唯一性**/
    std::string name;
    
    cocos2d::Vector<moonsugar::QueueItem *> queueList;
    
    Queue(std::string nameValue);
    ~Queue();
    
    /***向列队末尾添加item**/
    virtual int addItem(moonsugar::QueueItem * itemValue);
    /***向队列的索引中添加item，如果索引值大于等于当前队列的长度，则直接添加到末尾**/
    virtual int addItemIndex(moonsugar::QueueItem * itemValue, int indexValue);
    
    /***从队列中删除名称相同的item**/
    virtual moonsugar::QueueItem * removeItem(std::string itemName);
    /***从队列的相应位置删除item，如果给定的缩影超出队列长度则返回null**/
    virtual moonsugar::QueueItem * removeIndex(int indexValue);
    
    /***返回相应名称的item**/
    virtual moonsugar::QueueItem * getItem(std::string itemNameValue);
    /***返回给定位置的item**/
    virtual moonsugar::QueueItem * getIndex(int indexValue);
    /***检测给定名称的item的索引值**/
    virtual int checkItem(std::string itemNameValue);
};
/**********************************************/

/***************Stack**************************/
class Stack
{
public:
    Stack();
    ~Stack();
    
    cocos2d::Vector<QueueItem *> queueList;
    
    /***向队列中添加item，item的索引值为0**/
    virtual int addItem(moonsugar::QueueItem * itemValue);
    
    /***向队列中添加item，item的索引值为index**/
    virtual int addItemIndex(moonsugar::QueueItem * itemValue, int index);
    
    /***弹出item，栈特有的获取item的方式，直接从0索引处删除并返回item**/
    virtual moonsugar::QueueItem * popItem();
    
    virtual moonsugar::QueueItem * removeItem(int index);
    
    /***获取当前item，从0索引处返回item*****/
    virtual moonsugar::QueueItem * getCurrentItem();
    
    /***检测下一个item**/
    virtual bool checkNext();
};
/**********************************************/
NS_MS_END;

#endif /* defined(__Run__MSDataStructure__) */
