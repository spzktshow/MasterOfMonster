//
//  LOFDConfigManager.cpp
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-8.
//
//

#include "LOFDConfigManager.h"

NS_LOFD_BEGIN
static lofd::ConfigManager * _configManagerInstance = nullptr;

lofd::ConfigManager * ConfigManager::getInstance()
{
    if (nullptr == _configManagerInstance)
    {
        _configManagerInstance = new lofd::ConfigManager();
    }
    return _configManagerInstance;
}

void ConfigManager::destoryInstance()
{
    CC_SAFE_DELETE(_configManagerInstance);
}

void ConfigManager::createActorConfig()
{
    std::string actorConfigFullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("actorConfig.json");
    std::string actorConfigData = cocos2d::FileUtils::getInstance()->getStringFromFile(actorConfigFullPath);
    lofd::ActorConfig * config = new lofd::ActorConfig();
    config->parse(actorConfigData);
    this->actorConfig = config;
}

void ConfigManager::createActorStateConfig()
{
    std::string actorStateConfigFullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("actorStateConfig.json");
    std::string actorStateConfigData = cocos2d::FileUtils::getInstance()->getStringFromFile(actorStateConfigData);
    lofd::ActorStateConfig * config = new lofd::ActorStateConfig;
    config->parse(actorStateConfigData);
    this->actorStateConfig = config;
}

void ConfigManager::createActorActionConfig()
{
    std::string actionConfig = cocos2d::FileUtils::getInstance()->fullPathForFilename("actionConfig.json");
    std::string actionConfigValue = cocos2d::FileUtils::getInstance()->getStringFromFile(actionConfig);
    lofd::ActorActionConfig * config = new lofd::ActorActionConfig();
    config->parse(actionConfigValue);
    this->actorActionConfig = config;
}

void ConfigManager::createCampConfig()
{
    std::string config = cocos2d::FileUtils::getInstance()->fullPathForFilename("campConfig.json");
    std::string configValue = cocos2d::FileUtils::getInstance()->getStringFromFile(config);
    campConfig = new lofd::CampConfig();
    campConfig->parse(configValue);
}

void ConfigManager::createAIConfig()
{
    std::string config = cocos2d::FileUtils::getInstance()->fullPathForFilename("aiConfig.json");
    std::string configValue = cocos2d::FileUtils::getInstance()->getStringFromFile(config);
    aiConfig = new lofd::AIConfig();
    aiConfig->parse(configValue);
}

void ConfigManager::createSkillBeforeConfig()
{
    //if(skillBeforeConfig)return;
    std::string config = cocos2d::FileUtils::getInstance()->fullPathForFilename("skill_before.json");
    std::string configValue = cocos2d::FileUtils::getInstance()->getStringFromFile(config);
    skillBeforeConfig = new lofd::SkillBeforeConfig();
    skillBeforeConfig->parse(configValue);
}

void ConfigManager::createHeroBeforeConfig()
{
    //if(heroBeforeConfig && heroBeforeConfig->hasParse)return;
    
    std::string config = cocos2d::FileUtils::getInstance()->fullPathForFilename("hero_before.json");
    std::string configValue = cocos2d::FileUtils::getInstance()->getStringFromFile(config);
    heroBeforeConfig = new lofd::HeroBeforeConfig();
    heroBeforeConfig->parse(configValue);

}

void ConfigManager::createPubBeforeConfig()
{
    std::string config = cocos2d::FileUtils::getInstance()->fullPathForFilename("pub_before.json");
    std::string configValue = cocos2d::FileUtils::getInstance()->getStringFromFile(config);
    pubBeforeConfig = new lofd::PubBeforeConfig();
    pubBeforeConfig->parse(configValue);
}

void ConfigManager::createConfig(std::string configType, std::string configData)
{
    if (configType == CONFIG_TYPE_ACTOR_STATE)
    {
        lofd::ActorStateConfig * config = new lofd::ActorStateConfig();
        config->parse(configData);
        this->actorStateConfig = config;
    }
    else if (configType == CONFIG_TYPE_ACTOR)
    {
        lofd::ActorConfig * config = new lofd::ActorConfig();
        config->parse(configData);
        this->actorConfig = config;
    }
    else if (configType == CONFIG_TYPE_ACTOR_ACTION)
    {
        lofd::ActorActionConfig * config = new lofd::ActorActionConfig();
        config->parse(configData);
        this->actorActionConfig = config;
    }
    else if (configType == CONFIG_TYPE_CAMP)
    {
        campConfig = new lofd::CampConfig();
        campConfig->parse(configData);
    }
    else if (configType == CONFIG_TYPE_AI)
    {
        aiConfig = new lofd::AIConfig();
        aiConfig->parse(configData);
    }
    else if (configType == CONFIG_TYPE_SKILL)
    {
        //skillConfig = new lofd::SkillConfig;
        skillConfig->parse(configData);
    }
    else if (configType == CONFIG_TYPE_SKILL_EFFECTS)
    {
        skillConfig = new lofd::SkillConfig;
        skillConfig->parseEffects(configData);
    }
    else if (configType == CONFIG_TYPE_DUNGEONS)
    {
        dungeonConfig = new lofd::DungeonConfig;
        dungeonConfig->parse(configData);
    }
}

ConfigManager::ConfigManager()
{
    
}

ConfigManager::~ConfigManager()
{
    
}

void ConfigManager::disposeAllConfig()
{
    delete actorConfig;
    delete actorStateConfig;
    delete actorActionConfig;
    delete campConfig;
    delete aiConfig;
    delete skillConfig;
}
NS_LOFD_END