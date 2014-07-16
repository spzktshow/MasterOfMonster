//
//  ApplicationMainScene.cpp
//  Vector
//
//  Created by RockLee on 14-4-1.
//
//

#include "ApplicationMainScene.h"
#include "ApplicationContext.h"

#include "moonSugar/MSVectorMap.h"
#include "moonSugar/MSVectorMapItem.h"
#include "LengendOfFightingDemons/actor/LOFDActorBehavior.h"
#include "constellation/BehaviorTree.h"
#include "LengendOfFightingDemons/actor/LOFDActorState.h"
#include "LengendOfFightingDemons/config/LOFDConfigManager.h"
#include "LengendOfFightingDemons/actor/LOFDActor.h"
#include "moonSugar/TypeConver.h"
#include "LengendOfFightingDemons/scene/LOFDScene.h"
#include "LengendOfFightingDemons/fileFormat/FileFormat.h"
#include "SmartLoadScene.h"
#include "LOFDConfigManager.h"

bool ApplicationMainScene::init()
{
    if (!cocos2d::Scene::init())
    {
        return false;
    }
    
    std::string jokeFullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("joke.png");
    auto goSceneItem = cocos2d::MenuItemImage::create(jokeFullPath, jokeFullPath, CC_CALLBACK_1(ApplicationMainScene::onGoSceneHandler, this));
    auto menu = cocos2d::Menu::create(goSceneItem, NULL);
    menu->setPosition(cocos2d::Point::ZERO);
    this->addChild(menu, 1);
    goSceneItem->setPosition(designResolutionSize.width * .5, designResolutionSize.height * .5);
    
//    cocos2d::Director::getInstance()->getEventDispatcher()->addCustomEventListener("gotoBattle",
//                                                                                   [this](cocos2d::EventCustom* custom)
//    {
//        onGoSceneHandler(nullptr);
//    });
//    
////    auto mainCity = MainCityScene::create();
////    this->addChild(mainCity);
//    this->addChild(SelectCharacterPanel::create());
//    // init register
//    GameRegister::getInstance();
    return true;
}

void ApplicationMainScene::onGoSceneHandler(cocos2d::Ref *pSender)
{
    cocos2d::Vector<lofd::LoadContext *> loadContexts;
    lofd::LoadSceneDataContext * loadSceneDataContext = lofd::LoadSceneDataContext::create(TYPE_SCENE_DATA, "300011.json", 300011);
    loadContexts.pushBack(loadSceneDataContext);
    lofd::LoadConfigContext * loadConfigContext = lofd::LoadConfigContext::create(TYPE_CONFIG_JSON, "hero_before.json", CONFIG_TYPE_ACTOR);
    loadContexts.pushBack(loadConfigContext);
    std::vector<int> stateIds;
    stateIds.push_back(71001);
    stateIds.push_back(71002);
    stateIds.push_back(71003);
    stateIds.push_back(71004);
    stateIds.push_back(71005);
    stateIds.push_back(71006);
    stateIds.push_back(74013);
    loadConfigContext = lofd::LoadConfigContext::create(TYPE_CONFIG_JSON, "actorStateConfig.json", CONFIG_TYPE_ACTOR_STATE);
    loadContexts.pushBack(loadConfigContext);
    loadConfigContext = lofd::LoadConfigContext::create(TYPE_CONFIG_JSON, "actionConfig.json", CONFIG_TYPE_ACTOR_ACTION);
    loadContexts.pushBack(loadConfigContext);
    loadConfigContext = lofd::LoadConfigContext::create(TYPE_CONFIG_JSON, "campConfig.json", CONFIG_TYPE_CAMP);
    loadContexts.pushBack(loadConfigContext);
    loadConfigContext = lofd::LoadConfigContext::create(TYPE_CONFIG_JSON, "aiConfig.json", CONFIG_TYPE_AI);
    loadContexts.pushBack(loadConfigContext);
    loadConfigContext = lofd::LoadConfigContext::create(TYPE_CONFIG_JSON, "skillEffectConfig.json", CONFIG_TYPE_SKILL_EFFECTS);
    loadContexts.pushBack(loadConfigContext);
    loadConfigContext = lofd::LoadConfigContext::create(TYPE_CONFIG_JSON, "skillConfig.json", CONFIG_TYPE_SKILL);
    loadContexts.pushBack(loadConfigContext);
    loadConfigContext = lofd::LoadConfigContext::create(TYPE_CONFIG_JSON, "dungeonConfig.json", CONFIG_TYPE_DUNGEONS);
    loadContexts.pushBack(loadConfigContext);
    lofd::SmartLoadScene * loadScene = lofd::SmartLoadScene::create();
    loadScene->setLoadContexts(loadContexts);
    loadScene->loadActorStateData(stateIds);
    cocos2d::Director::getInstance()->replaceScene(loadScene);
}