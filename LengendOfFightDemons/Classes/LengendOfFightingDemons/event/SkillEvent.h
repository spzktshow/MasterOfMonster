//
//  SkillEvent.h
//  LengendOfFightDemons
//
//  Created by RockLee on 14-5-4.
//
//

#ifndef __LengendOfFightDemons__SkillEvent__
#define __LengendOfFightDemons__SkillEvent__

#include "lengendOfFightDemons.h"
#include "LOFDSkill.h"

#define SKILL_HANDLER           "skillHandler"/*****技能释放*****/
#define SKILL_COOLDOWN_START    "skillCoolDownStart"/****技能coolDownStart*****/
#define SKILL_COOLDOWN_END      "skillCoolDownEnd"/****技能coolDownEnd*****/

NS_LOFD_BEGIN
/******SkillButton handler********/
class SkillEventParam : public cocos2d::Ref
{
public:
    SkillEventParam(){};
    ~SkillEventParam(){};
    
    /*****标示人物唯一性******/
    int actorTag;
    /*****技能定义*****/
    lofd::SkillDef * skillDef;
};
NS_LOFD_END;
#endif /* defined(__LengendOfFightDemons__SkillEvent__) */
