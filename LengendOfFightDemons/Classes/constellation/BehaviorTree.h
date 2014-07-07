//
//  BehaviorTree.h
//  Constellation
//
//  Created by RockLee on 14-4-2.
//
//

#ifndef __Constellation__BehaviorTree__
#define __Constellation__BehaviorTree__

#include "constellation.h"
#include "external/json/rapidjson.h"
#include "external/json/document.h"

#define BEHAVIOR_RESULT_FAILED     0
#define BEHAVIOR_RESULT_SUCCESS    1
#define BEHAVIOR_RESULT_WAIT       2

///
#define constellation_for_editor_SEQUENCE_BEHAVIOR_NODE     /**/"sequence"//顺序节点@node///
#define constellation_for_editor_SELECTOR_BEHAVIOR_NODE     /**/"selector"//选择节点@node///
#define constellation_for_editor_PARALLEL_BEHAVIOR_NODE     /**/"parallel"//并行节点@node///
#define constellation_for_editor_CONDITION_BEHAVIOR_NODE    /**/"condition"//抽象条件节点@node///
#define constellation_for_editor_ACTION_BEHAVIOR_NODE       /**/"action"//抽象动作节点@node///
#define constellation_for_editor_DECORATOR_BEHAVIOR_NODE    /**/"decorator"//抽象装饰节点@node
///

#define BEHAVIOR_TREE_PARSE_TAG_TYPE             "type"
#define BEHAVIOR_TREE_PARSE_TAG_NAME             "name"
#define BEHAVIOR_TREE_PARSE_TAG_CHILDREN         "children"
#define BEHAVIOR_TREE_PARSE_TAG_ACTION_TYPE      "actionType"
#define BEHAVIOR_TREE_PARSE_TAG_CONDITION_TYPE   "conditionType"
#define BEHAVIOR_TREE_PARSE_TAG_DECORATOR_TYPE   "decoratorType"
#define BEHAVIOR_TREE_PARSE_TAG_CHILD            "child"

NS_CSL_BEGIN
/*********BehaviorData**********/
class BehaviorData
{
public:
    BehaviorData(){};
    ~BehaviorData(){};
};

/*********抽象的行为事件*********/
class BehaviorEvent
{
public:
    BehaviorEvent(std::string eventTypeValue);
    ~BehaviorEvent();
    
    std::string eventType;
    
    BehaviorData * behaviorData;
};
/**********条件节点行为事件*********/
class ConditionBehaviorEvent : public constellation::BehaviorEvent
{
public:
    ConditionBehaviorEvent(std::string eventTypeValue, std::string conditionTypeValue):BehaviorEvent(eventTypeValue)
    {
        conditionType = conditionTypeValue;
    };
    
    ~ConditionBehaviorEvent();
    
    std::string conditionType;
};

/***********action行为事件**********/
class ActionBehaviorEvent : public constellation::BehaviorEvent
{
public:
    ActionBehaviorEvent(std::string eventTypeValue, std::string actionTypeValue):BehaviorEvent(eventTypeValue)
    {
        actionType = actionTypeValue;
    };
    
    ~ActionBehaviorEvent();
    
    std::string actionType;
};

/***********抽象节点************/
class BehaviorNode : public cocos2d::Ref
{
public:
    BehaviorNode(std::string typeValue);
    ~BehaviorNode();
    
    std::string type;
    std::string nodeName;
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
};

/************组合节点*************/
class CompositeBehaviorNode : public constellation::BehaviorNode
{
public:
    CompositeBehaviorNode(std::string typeValue):BehaviorNode(typeValue){};
    ~CompositeBehaviorNode();
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
    
    cocos2d::Vector<constellation::BehaviorNode *> children;
};

/*********顺序节点*********/
class SequenceBehaviorNode : public constellation::CompositeBehaviorNode
{
public:
    SequenceBehaviorNode(std::string typeValue):CompositeBehaviorNode(typeValue){};
    ~SequenceBehaviorNode();
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
};

/*********选择节点**********/
class SelectorBehaviorNode : public constellation::CompositeBehaviorNode
{
public:
    SelectorBehaviorNode(std::string typeValue):CompositeBehaviorNode(typeValue){};
    ~SelectorBehaviorNode();
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
};

/*********并行节点**********/
class ParallelBehaviorNode : public constellation::CompositeBehaviorNode
{
public:
    ParallelBehaviorNode(std::string typeValue):CompositeBehaviorNode(typeValue){};
    ~ParallelBehaviorNode();
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
};

/********条件节点***********/
class ConditionBehaviorNode : public constellation::BehaviorNode
{
public:
    ConditionBehaviorNode(std::string typeValue, std::string conditionTypeValue):BehaviorNode(typeValue)
    {
        conditionType = conditionTypeValue;
    };
    ~ConditionBehaviorNode();
    
    std::string conditionType;
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
};

/********动作节点***********/
class ActionBehaviorNode : public constellation::BehaviorNode
{
public:
    ActionBehaviorNode(std::string typeValue, std::string actionTypeValue):BehaviorNode(typeValue)
    {
        actionType = actionTypeValue;
    };
    ~ActionBehaviorNode();
    
    std::string actionType;
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
};

/********装饰节点***********/
class DecoratorBehaviorNode : public constellation::BehaviorNode
{
public:
    DecoratorBehaviorNode(std::string typeValue, std::string decoratorTypeValue):BehaviorNode(typeValue)
    {
        decoratorType = decoratorTypeValue;
    };
    ~DecoratorBehaviorNode();
    
    std::string decoratorType;
    /*****子节点****/
    constellation::BehaviorNode * child;
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
};

//================decorator====================================/
/*********翻转节点********/
//==========decorator=========/
/****decorator type*****/
///
#define DECORATOR_TURN_BEHAVIOR_NODE          /**/"turn"//翻转节点@decorator///
class DecoratorBehaviorNodeTurn : public constellation::DecoratorBehaviorNode
{
public:
    DecoratorBehaviorNodeTurn(std::string typeValue, std::string decoratorTypeValue):DecoratorBehaviorNode(typeValue, decoratorTypeValue){};
    ~DecoratorBehaviorNodeTurn();
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventValue);
};

//================condition====================================/
/**********campare event type********/
//============================/
//==========condition=========/
/****campare current event******/
///
#define CONDITION_CAMPARE_EVENT_TYPE          /**/"campareEventType"//比较事件类型@condition|campareEventTypeValue-string*event///
class ConditionBehaviorNodeCampareEventType : public constellation::ConditionBehaviorNode
{
public:
    ConditionBehaviorNodeCampareEventType(std::string typeValue, std::string conditionTypeValue, std::string campareEventTypeValue):ConditionBehaviorNode(typeValue, conditionTypeValue){
        campareEventType = campareEventTypeValue;
    };
    ~ConditionBehaviorNodeCampareEventType();
    
    virtual int execute(constellation::BehaviorEvent * behaviorEventvalue);
    
    std::string campareEventType;
};

/*********抽象的行为树解析器*********/
class BehaviorTreeParse : public cocos2d::Ref
{
public:
    virtual BehaviorNode * parseNode(const rapidjson::Value &nodeValue);
    
    virtual ActionBehaviorNode * parseActionNode(const rapidjson::Value &nodeValue, std::string type, std::string name);
    virtual ConditionBehaviorNode * parseConditionNode(const rapidjson::Value &nodeValue, std::string type, std::string name);
    virtual DecoratorBehaviorNode * parseDecoratorNode(const rapidjson::Value &nodeValue, std::string type, std::string name);
    virtual SequenceBehaviorNode * parseSequenceNode(const rapidjson::Value &nodeValue, std::string type, std::string name);
    virtual SelectorBehaviorNode * parseSelectorNode(const rapidjson::Value &nodeValue, std::string type, std::string name);
    virtual ParallelBehaviorNode * parseParallelNode(const rapidjson::Value &nodeValue, std::string type, std::string name);
};
NS_CSL_END;


#endif /* defined(__Constellation__BehaviorTree__) */
