//
//  BehaviorTree.cpp
//  Constellation
//
//  Created by RockLee on 14-4-2.
//
//

#include "BehaviorTree.h"
#include "editor-support/cocostudio/DictionaryHelper.h"
#include "LOFDActorAi.h"

NS_CSL_BEGIN

/*******BehaviorEvent********/
BehaviorEvent::BehaviorEvent(std::string eventTypeValue)
{
    eventType = eventTypeValue;
}

BehaviorEvent::~BehaviorEvent()
{
    //CC_SAFE_DELETE(behaviorData);
}

/********BehaviorNode************/
BehaviorNode::BehaviorNode(std::string typeValue)
{
    type = typeValue;
}

BehaviorNode::~BehaviorNode()
{
    
}

int BehaviorNode::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    //cocos2d::log("execute %s", this->nodeName.c_str());
    return 0;
}

/********CompositeBehaviorNode*******/
CompositeBehaviorNode::~CompositeBehaviorNode()
{
    
}

int CompositeBehaviorNode::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    return 0;
}

/********SelectorBehaviorNode**********/
SelectorBehaviorNode::~SelectorBehaviorNode()
{
    
}

int SelectorBehaviorNode::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    //BehaviorNode::execute(behaviorEventValue);
    //cocos2d::log("event %s selector node: %s handler", behaviorEventValue->eventType.c_str(), this->nodeName.c_str());
    long n = children.size();
    for (int i = 0; i < n; i++) {
        int result = children.at(i)->execute(behaviorEventValue);
        if (result == BEHAVIOR_RESULT_SUCCESS || result == BEHAVIOR_RESULT_WAIT)
        {
            //cocos2d::log("event %s selector node: %s return success", behaviorEventValue->eventType.c_str(), this->nodeName.c_str());
            return result;
        }
    }
    //cocos2d::log("event %s selector node : %s return failded", behaviorEventValue->eventType.c_str(), this->nodeName.c_str());
    return BEHAVIOR_RESULT_FAILED;
}

/********sequenceBehviorNode***********/
SequenceBehaviorNode::~SequenceBehaviorNode()
{
    
}

int SequenceBehaviorNode::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    //BehaviorNode::execute(behaviorEventValue);
    long n = children.size();
    int result = BEHAVIOR_RESULT_FAILED;
    //cocos2d::log("event %s sequence node: %s handler", behaviorEventValue->eventType.c_str(), this->nodeName.c_str());
    for (int i = 0; i < n;i ++)
    {
        result = children.at(i)->execute(behaviorEventValue);
        if (result == BEHAVIOR_RESULT_FAILED)
        {
            //cocos2d::log("event %s sequence node: %s return failed", behaviorEventValue->eventType.c_str(), this->nodeName.c_str());
            return result;
        }
    }
    //cocos2d::log("event %s sequence node: %s return success", behaviorEventValue->eventType.c_str(), this->nodeName.c_str());
    return result;
}

/*********ParalledBehaviorNode************/
ParallelBehaviorNode::~ParallelBehaviorNode()
{
    
}

int ParallelBehaviorNode::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    //BehaviorNode::execute(behaviorEventValue);
    long n = children.size();
    for (int i = 0; i < n; i ++) {
        children.at(i)->execute(behaviorEventValue);
    }
    return BEHAVIOR_RESULT_SUCCESS;
}

/*********ConditionBehaviorNode**********/
ConditionBehaviorNode::~ConditionBehaviorNode()
{
    
}

int ConditionBehaviorNode::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    //BehaviorNode::execute(behaviorEventValue);
    return 0;
}

/************ActionBehaviorNode**********/
ActionBehaviorNode::~ActionBehaviorNode()
{
    
}

int ActionBehaviorNode::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    //BehaviorNode::execute(behaviorEventValue);
    return 0;
}

/*************DecoratorBehaviorNode*********/
DecoratorBehaviorNode::~DecoratorBehaviorNode()
{
    
}

int DecoratorBehaviorNode::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    return 0;
}

/**************DecoratorBehaviorNodeTurn***********/
DecoratorBehaviorNodeTurn::~DecoratorBehaviorNodeTurn()
{
    
}

int DecoratorBehaviorNodeTurn::execute(constellation::BehaviorEvent *behaviorEventValue)
{
    int result = child->execute(behaviorEventValue);
    if (result == BEHAVIOR_RESULT_SUCCESS || result == BEHAVIOR_RESULT_WAIT)
    {
        return BEHAVIOR_RESULT_FAILED;
    }
    return BEHAVIOR_RESULT_SUCCESS;
}

/************CondtionBehaviorNodeCampareEventType*******/
ConditionBehaviorNodeCampareEventType::~ConditionBehaviorNodeCampareEventType()
{
    
}

int ConditionBehaviorNodeCampareEventType::execute(constellation::BehaviorEvent *behaviorEventvalue)
{
    //cocos2d::log("condition node campareEventType %s eventType=%s handler", this->nodeName.c_str(), this->campareEventType.c_str());
    if (behaviorEventvalue->eventType == this->campareEventType)
    {
        //cocos2d::log("condition node campareEventType %s eventType=%s return success", this->nodeName.c_str(), this->campareEventType.c_str());
        return BEHAVIOR_RESULT_SUCCESS;
    }
    //cocos2d::log("condition node campareEventType %s eventType=%s return failed", this->nodeName.c_str(), this->campareEventType.c_str());
    return BEHAVIOR_RESULT_FAILED;
}

/*****BehaviorTreeParse*************/
BehaviorNode * BehaviorTreeParse::parseNode(const rapidjson::Value &nodeValue)
{
    constellation::BehaviorNode * behaviorNode = nullptr;
    std::string type = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_TYPE);
    std::string nodeName = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_NAME);
    cocos2d::log("parseNode %s", nodeName.c_str());
    if (constellation_for_editor_SEQUENCE_BEHAVIOR_NODE == type)
    {
        behaviorNode = parseSequenceNode(nodeValue, type, nodeName);
    }
    else if (constellation_for_editor_SELECTOR_BEHAVIOR_NODE == type)
    {
        behaviorNode = parseSelectorNode(nodeValue, type, nodeName);
    }
    else if (constellation_for_editor_PARALLEL_BEHAVIOR_NODE == type)
    {
        behaviorNode = parseParallelNode(nodeValue, type, nodeName);
    }
    else if (constellation_for_editor_CONDITION_BEHAVIOR_NODE == type)
    {
        behaviorNode = parseConditionNode(nodeValue, type, nodeName);
    }
    else if (constellation_for_editor_ACTION_BEHAVIOR_NODE == type)
    {
        behaviorNode = parseActionNode(nodeValue, type, nodeName);
    }
    else if (constellation_for_editor_DECORATOR_BEHAVIOR_NODE == type)
    {
        behaviorNode = parseDecoratorNode(nodeValue, type, nodeName);
    }
    return behaviorNode;
}

ActionBehaviorNode * BehaviorTreeParse::parseActionNode(const rapidjson::Value &nodeValue, std::string type, std::string name)
{
    return nullptr;
}

ConditionBehaviorNode * BehaviorTreeParse::parseConditionNode(const rapidjson::Value &nodeValue, std::string type, std::string name)
{
    std::string conditionType = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_CONDITION_TYPE);
    constellation::ConditionBehaviorNode * conditionBehaviorNode = nullptr;
    if (conditionType == CONDITION_CAMPARE_EVENT_TYPE) {
        std::string campareCurrentState = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(nodeValue, CONDITION_CAMPARE_EVENT_TYPE);
        conditionBehaviorNode = new ConditionBehaviorNodeCampareEventType(type, conditionType, campareCurrentState);
    }
    return conditionBehaviorNode;
}

DecoratorBehaviorNode * BehaviorTreeParse::parseDecoratorNode(const rapidjson::Value &nodeValue, std::string type, std::string name)
{
    std::string decoratorType = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_DECORATOR_TYPE);
    DecoratorBehaviorNode * decoratorBehaviorNode = nullptr;
    if (decoratorType == DECORATOR_TURN_BEHAVIOR_NODE)
    {
        decoratorBehaviorNode = new constellation::DecoratorBehaviorNodeTurn(type, decoratorType);
        decoratorBehaviorNode->nodeName = name;
        const rapidjson::Value & childNodeValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_CHILD);
        decoratorBehaviorNode->child = this->parseNode(childNodeValue);
    }
    return decoratorBehaviorNode;
}

SequenceBehaviorNode * BehaviorTreeParse::parseSequenceNode(const rapidjson::Value &nodeValue, std::string type, std::string name)
{
    constellation::SequenceBehaviorNode * sequenceBehaviorNode = new constellation::SequenceBehaviorNode(type);
    sequenceBehaviorNode->nodeName = name;
    int n = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_CHILDREN);
    for (int i = 0; i < n; i++) {
        const rapidjson::Value& childNodeValue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_CHILDREN, i);
        constellation::BehaviorNode * behaviorNode = parseNode(childNodeValue);
        sequenceBehaviorNode->children.pushBack(behaviorNode);
    }
    return sequenceBehaviorNode;
}

SelectorBehaviorNode * BehaviorTreeParse::parseSelectorNode(const rapidjson::Value &nodeValue, std::string type, std::string name)
{
    constellation::SelectorBehaviorNode * selectorBehaviorNode = new constellation::SelectorBehaviorNode(type);
    selectorBehaviorNode->nodeName = name;
    int n = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_CHILDREN);
    for (int i = 0; i < n; i++) {
        const rapidjson::Value& childNodeValue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_CHILDREN, i);
        constellation::BehaviorNode * behaviorNode = parseNode(childNodeValue);
        selectorBehaviorNode->children.pushBack(behaviorNode);
    }
    return selectorBehaviorNode;
}

ParallelBehaviorNode * BehaviorTreeParse::parseParallelNode(const rapidjson::Value &nodeValue, std::string type, std::string name)
{
    constellation::ParallelBehaviorNode * parallelBehaviorNode = new constellation::ParallelBehaviorNode(type);
    int n = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_CHILDREN);
    for (int i = 0; i < n; i++) {
        const rapidjson::Value& childNodeValue = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(nodeValue, BEHAVIOR_TREE_PARSE_TAG_CHILDREN, i);
        constellation::BehaviorNode * behaviorNode = parseNode(childNodeValue);
        parallelBehaviorNode->children.pushBack(behaviorNode);
    }
    parallelBehaviorNode->nodeName = name;
    return parallelBehaviorNode;
}
NS_CSL_END;