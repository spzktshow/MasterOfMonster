//
//  LOFDConfigManager.h
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-8.
//
//

#ifndef __LengendOfFightDemons__LOFDConfigManager__
#define __LengendOfFightDemons__LOFDConfigManager__

#include "lengendOfFightDemons.h"
#include "LOFDActorState.h"
#include "LOFDActorBehavior.h"
#include "LOFDCampContext.h"
#include "MSVectorMap.h"
#include "LOFDSkill.h"
#include "LOFDSkillBeforeConfig.h"
#include "LOFDHeroBeforeConfig.h"
#include "LOFDPubBeforeConfig.h"
#include "LOFDDungeons.h"

NS_LOFD_BEGIN

#define CONFIG_TYPE_ACTOR_STATE         "actorState"
#define CONFIG_TYPE_ACTOR_ACTION        "actorAction"
#define CONFIG_TYPE_CAMP                "camp"
#define CONFIG_TYPE_AI                  "ai"
#define CONFIG_MAP_ITEM                 "mapItem"
#define CONFIG_TYPE_ACTOR               "actor"
#define CONFIG_TYPE_SKILL               "skill"
#define CONFIG_TYPE_SKILL_EFFECTS       "skillEffects"
#define CONFIG_TYPE_DUNGEONS            "dungeons"

class ConfigManager
{
public:
    static lofd::ConfigManager * getInstance();
    static void destoryInstance();
    
    ConfigManager();
    ~ConfigManager();
    
    lofd::ActorConfig * actorConfig;
    void createActorConfig();
    
    lofd::ActorStateConfig * actorStateConfig;
    void createActorStateConfig();
    
    lofd::ActorActionConfig * actorActionConfig;
    void createActorActionConfig();
    
    lofd::CampConfig * campConfig;
    void createCampConfig();
    
    lofd::AIConfig * aiConfig;
    void createAIConfig();
    
    lofd::SkillConfig * skillConfig=0;
    
    lofd::SkillBeforeConfig* skillBeforeConfig=0;
    void createSkillBeforeConfig();
    
    lofd::HeroBeforeConfig* heroBeforeConfig;
    void createHeroBeforeConfig();
    
    lofd::PubBeforeConfig* pubBeforeConfig;
    void createPubBeforeConfig();
    
    lofd::DungeonConfig * dungeonConfig;
    /*******当前场景数据配置******/
    moonsugar::MapSceneData * currentSceneDataConfig;
    /********create config by type*******/
    virtual void createConfig(std::string configType, std::string configData);
    
    virtual void disposeAllConfig();
};

NS_LOFD_END;

#endif /* defined(__LengendOfFightDemons__LOFDConfigManager__) */
