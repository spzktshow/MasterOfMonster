//
//  SmartLoadScene.cpp
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-15.
//
//

#include "SmartLoadScene.h"
#include "MSVectorMap.h"
#include "LOFDConfigManager.h"
#include "TypeConver.h"

NS_LOFD_BEGIN
/*************LoadSceneDataContext*************/
LoadSceneDataContext * LoadSceneDataContext::create(std::string typeValue, std::string pathValue, int idValue)
{
    LoadSceneDataContext * context = new LoadSceneDataContext(typeValue, pathValue, idValue);
    if (context != nullptr)
    {
        context->autorelease();
        return context;
    }
    else
    {
        delete context;
        context = nullptr;
        return nullptr;
    }
}

/*************SmartLoadScene*******************/
void SmartLoadScene::calculatePrecent()
{
    float last = this->maxProcessLength - this->processRect.size.width;
    this->processRect.size.width += this->currentIndex * last / this->loadContexts.size();
}

void SmartLoadScene::update(float dt)
{
    cocos2d::log("smartloadScene is running");
}

bool SmartLoadScene::init()
{
    if (!lofd::LoadScene::init()) return false;
    return true;
}

void SmartLoadScene::load(lofd::LoadContext *loadContextValue)
{
    if (loadContextValue->type == TYPE_CONFIG_JSON)
    {
        lofd::LoadConfigContext * loadConfigContext = static_cast<lofd::LoadConfigContext *>(loadContextValue);
        if (loadConfigContext->configType == CONFIG_TYPE_SKILL)
        {
            loadSkillData(loadContextValue);
        }
        else
        {
            loadConfigJson(loadContextValue);
        }
    }
    else if (loadContextValue->type == TYPE_TEXTURE_PLIST)
    {
        loadTextureAndPlist(loadContextValue);
    }
    else if (loadContextValue->type == TYPE_TEXTURE)
    {
        loadTexture(loadContextValue);
    }
    else if (loadContextValue->type == TYPE_SCENE_DATA)
    {
        loadSceneData(loadContextValue);
    }
}

void SmartLoadScene::loadActorStateData(std::vector<int> stateIds)
{
    this->label->setString("load actor state");
    
    for (int i = 0; i < stateIds.size(); i++) {
        std::string sourceName = moonsugar::TypeConver::intToString(stateIds.at(i));
        lofd::LoadTextureAndPlistContext * loadTextureContext = new lofd::LoadTextureAndPlistContext(TYPE_TEXTURE_PLIST, sourceName, FILE_PVR_CCZ);
        this->loadContexts.pushBack(loadTextureContext);
    }
    
    lofd::LoadTextureAndPlistContext * loadTextureContext = new lofd::LoadTextureAndPlistContext(TYPE_TEXTURE_PLIST, "actorGeneral", FILE_PVR_CCZ);
    this->loadContexts.pushBack(loadTextureContext);
    
    lofd::LoadTextureAndPlistContext * loadDead0TextureContext = new lofd::LoadTextureAndPlistContext(TYPE_TEXTURE_PLIST, "dead0", FILE_PVR_CCZ);
    lofd::LoadTextureAndPlistContext * loadDead1TextureContext = new lofd::LoadTextureAndPlistContext(TYPE_TEXTURE_PLIST, "dead1", FILE_PVR_CCZ);
    this->loadContexts.pushBack(loadDead0TextureContext);
    this->loadContexts.pushBack(loadDead1TextureContext);
}

void SmartLoadScene::loadSceneData(lofd::LoadContext * loadContextValue)
{
    this->label->setString(loadContextValue->path);
    
    lofd::LoadSceneDataContext * loadSceneDataContext = dynamic_cast<lofd::LoadSceneDataContext *>(loadContextValue);
    
    std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(loadContextValue->path);
    std::string sceneData = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath);
    
    moonsugar::MapSceneData * mapSceneData = moonsugar::MapSceneDataParse::parse(sceneData);
    
    lofd::ConfigManager::getInstance()->currentSceneDataConfig = mapSceneData;
    
    //back;
//    for (int i = 0; i < mapSceneData->layers.size(); i ++) {
//        moonsugar::MapLayerData * mapLayerData = mapSceneData->layers.at(i);
//        if (mapLayerData->layerType == LAYER_TYPE_ROLL)
//        {
//            moonsugar::RollLayerData * rollLayerData = (moonsugar::RollLayerData *) mapLayerData;
//            for (int j = 0; j < rollLayerData->mapRenderRectData->sourceList.size(); j ++)
//            {
//                lofd::LoadTextureContext * loadTextureContext = new lofd::LoadTextureContext(TYPE_TEXTURE, rollLayerData->mapRenderRectData->sourceList.at(j)->sourceName);
//                this->loadContexts.pushBack(loadTextureContext);
//            }
//        }
//    }
    std::string idName = moonsugar::TypeConver::intToString(loadSceneDataContext->sceneId);
    std::string backName = idName + "background";
    lofd::LoadTextureAndPlistContext * back = new lofd::LoadTextureAndPlistContext(TYPE_TEXTURE_PLIST, backName, FILE_PVR_CCZ);
    this->loadContexts.pushBack(back);
    
    std::string frontName = idName + "front";
    lofd::LoadTextureAndPlistContext * front = new lofd::LoadTextureAndPlistContext(TYPE_TEXTURE_PLIST, frontName, FILE_PVR_CCZ);
    this->loadContexts.pushBack(front);
    
    std::string middleName = idName + "middle";
    lofd::LoadTextureAndPlistContext * middle = new lofd::LoadTextureAndPlistContext(TYPE_TEXTURE_PLIST, middleName, FILE_PVR_CCZ);
    this->loadContexts.pushBack(middle);
    
    //lofd::LoadTextureAndPlistContext * loadTextureAndPlistContext = new lofd::LoadTextureAndPlistContext(TYPE_TEXTURE_PLIST, "barrier_source", FILE_PVR_CCZ);
    //this->loadContexts.pushBack(loadTextureAndPlistContext);
    
    lofd::LoadConfigContext * loadConfigContext = new lofd::LoadConfigContext(TYPE_CONFIG_JSON, "barrier_config", CONFIG_MAP_ITEM);
    this->loadContexts.pushBack(loadConfigContext);
    
    lofd::LoadTextureAndPlistContext * hurt = new lofd::LoadTextureAndPlistContext(TYPE_TEXTURE_PLIST, "hurtCount", FILE_PVR_CCZ);
    this->loadContexts.pushBack(hurt);
    
    this->loadComplete();
}

void SmartLoadScene::loadSkillData(lofd::LoadContext *loadContextValue)
{
    this->label->setString(loadContextValue->path);
    
    lofd::LoadConfigContext * loadConfigContext = (lofd::LoadConfigContext *) loadContextValue;
    std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(loadContextValue->path);
    std::string configData = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath);
    lofd::ConfigManager::getInstance()->createConfig(loadConfigContext->configType, configData);
    
    lofd::SkillConfig * skillConfig = lofd::ConfigManager::getInstance()->skillConfig;
    long n = skillConfig->skillDefs.size();
    for (int i = 0; i < n; i ++) {
        lofd::SkillDef * skillDef = skillConfig->skillDefs.at(i);
        long m = skillDef->skillRandomContexts.size();
        for (int j = 0; j < m; j ++) {
            lofd::SkillRandomContext * skillRandomContext = skillDef->skillRandomContexts.at(j);
            long  b = skillRandomContext->effectContextDefs.size();
            for (int k = 0; k < b; k ++) {
                lofd::SkillEffectContextDef * skillEffectContextDef = skillRandomContext->effectContextDefs.at(k);
                if (skillEffectContextDef->skillEffectDef->viewDef->animationType == EFFECT_VIEW_TYPE_NONE) continue;
                lofd::LoadTextureAndPlistContext * loadTextureAndPlistContext = new lofd::LoadTextureAndPlistContext(TYPE_TEXTURE_PLIST, moonsugar::TypeConver::intToString(skillEffectContextDef->skillEffectDef->effectId), FILE_PVR_CCZ);
                this->loadContexts.pushBack(loadTextureAndPlistContext);
            }
        }
    }
    
    this->loadComplete();
}

void SmartLoadScene::totalLoadComplete()
{
    lofd::MapScene * mapScene = lofd::MapScene::create(lofd::ConfigManager::getInstance()->currentSceneDataConfig);
    
//    lofd::ActorData * actorData = lofd::ActorControllerUtils::createActorDataById(MY_ACTOR_PINGZHI_ID);
//    actorData->isRange = false;
//    cocos2d::Point * local = new cocos2d::Point(200, 200);
//    actorData->isAI = true;
//    actorData->point = local;
////    actorData->currentFocusSkill = 3;
//    
//    actorData->runAction(LOFD_ACTOR_STATE_IDLE);
//    actorData->actorContainer->setPosition(200, 200);
//    mapScene->changeFocus(actorData);
//    lofd::MapUILayer * mapUILayer = dynamic_cast<lofd::MapUILayer *>(mapScene->getMapUILayer());
//    mapUILayer->changeFocus(actorData);
//    mapScene->addActor(actorData);
    
    lofd::ActorData * actorData = lofd::ActorControllerUtils::createActorDataById(710040);
    actorData->isAI = false;
    actorData->isRange = false;
//    lofd::ActorStateData * actorStateData = new lofd::ActorStateData(LOFD_ACTOR_STATE_IDLE, LOFD_ACTOR_STATE_OPERATION_TYPE_AI);
    lofd::ActorStateData * actorStateData = lofd::ActorStateData::create(LOFD_ACTOR_STATE_IDLE, LOFD_ACTOR_STATE_OPERATION_TYPE_AI);
    actorData->stateContext->insertStateData(actorStateData);
    actorData->runAction(LOFD_ACTOR_STATE_IDLE);
    
    actorData->actorContainer->setPosition(cocos2d::Point(280, 100));
    actorData->actorPropertyData->hp = 10000;
    actorData->actorPropertyData->currentHp = 10000;
    mapScene->addOperationActor(actorData);
    mapScene->changeFocus(actorData);
    
    cocos2d::Director::getInstance()->replaceScene(mapScene);
    
    constellation::BehaviorEvent * behaviorEvent = new constellation::BehaviorEvent(LOFD_BEHAVIOR_EVENT_AI_PATROL);
    lofd::AIBehaviorDynamicData * behaviorDynamicData = new lofd::AIBehaviorDynamicData();
    behaviorDynamicData->actorData = actorData;
    behaviorDynamicData->mapSceneData = mapScene->mapSceneData;
    behaviorDynamicData->operationType = LOFD_ACTOR_STATE_OPERATION_TYPE_AI;
    behaviorEvent->behaviorData = behaviorDynamicData;
    actorData->aiBehavior->root->execute(behaviorEvent);
    delete behaviorEvent;
    delete behaviorDynamicData;
}
NS_LOFD_END;