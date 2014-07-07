//
//  MSDataStructure.cpp
//  Run
//
//  Created by RockLee on 14-3-10.
//
//

#include "MSDataStructure.h"

NS_MS_BEGIN
/****************QueueItem*****************/
QueueItem::QueueItem(std::string itemNameValue)
{
    itemName = itemNameValue;
}

QueueItem::~QueueItem()
{
    
}

/***************Queue*********************/
Queue::Queue(std::string nameValue)
{
    name = nameValue;
}

Queue::~Queue()
{
    
}

int Queue::addItem(moonsugar::QueueItem *itemValue)
{
    queueList.pushBack(itemValue);
    return queueList.size() - 1;
}

int Queue::addItemIndex(moonsugar::QueueItem *itemValue, int indexValue)
{
    queueList.insert(indexValue, itemValue);
    return indexValue;
}

moonsugar::QueueItem * Queue::removeItem(std::string itemName)
{
    int index = checkItem(itemName);
    return removeIndex(index);
}

moonsugar::QueueItem * Queue::removeIndex(int indexValue)
{
    QueueItem * tempItem = (QueueItem*)queueList.at(indexValue);
    queueList.erase(indexValue);
    return tempItem;
}

moonsugar::QueueItem * Queue::getItem(std::string itemNameValue)
{
    int index = checkItem(itemNameValue);
    return getIndex(index);
}

moonsugar::QueueItem * Queue::getIndex(int indexValue)
{
    return (QueueItem *)queueList.at(indexValue);
}

int Queue::checkItem(std::string itemNameValue)
{
    int n = queueList.size();
    for (int i = 0; i < n; i ++)
    {
        QueueItem * tempItem = (QueueItem *)queueList.at(i);
        if (tempItem->itemName == itemNameValue)
        {
            return i;
        }
    }
    return -1;
}

/**************Stack***************/
Stack::Stack()
{
}

Stack::~Stack()
{
    
}

int Stack::addItem(moonsugar::QueueItem *itemValue)
{
    queueList.insert(0, itemValue);
    return 0;
}

int Stack::addItemIndex(moonsugar::QueueItem *itemValue, int index)
{
    queueList.insert(index, itemValue);
    return index;
}

moonsugar::QueueItem * Stack::popItem()
{
    if (queueList.size() <= 0) return nullptr;
    QueueItem * tempItem = (QueueItem *)queueList.at(0);
    queueList.erase(0);
    return tempItem;
}

moonsugar::QueueItem * Stack::removeItem(int index)
{
    if (index >= queueList.size() - 1)
    {
        QueueItem * tempItem = (QueueItem *)queueList.at(index);
        queueList.erase(index);
        return tempItem;
    }
    return nullptr;
}

moonsugar::QueueItem * Stack::getCurrentItem()
{
    if (checkNext() == false) return nullptr;
    return (QueueItem *)queueList.at(0);
}

bool Stack::checkNext()
{
    if (queueList.size() <= 0) return false;
    return true;
}
NS_MS_END;