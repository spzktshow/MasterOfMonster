//
//  ActorEvent.h
//  LengendOfFightDemons
//
//  Created by RockLee on 14-5-6.
//
//

#ifndef __LengendOfFightDemons__ActorEvent__
#define __LengendOfFightDemons__ActorEvent__
#include "lengendOfFightDemons.h"

#define ACTOR_EVENT_DEAD                "actorEventDead"/****角色死亡事件*****/
#define ACTOR_EVENT_DEAD_COMPLETE       "actorEventDeadComplete"/******角色死亡完成事件*****/
#define ACTOR_EVENT_HURT                "actorEventHurt"/*****角色受伤*****/

NS_LOFD_BEGIN

class ActorEventParam : public cocos2d::Ref
{
public:
    ActorEventParam(){};
    ~ActorEventParam(){};
    
    int tagId;
};

class ActorHurtEventParam
{
public:
    ActorHurtEventParam(){};
    ~ActorHurtEventParam(){};
    
    /******伤害*****/
    int hurt;
    /******位置*****/
    cocos2d::Point location;
};
NS_LOFD_END;
#endif /* defined(__LengendOfFightDemons__ActorEvent__) */
