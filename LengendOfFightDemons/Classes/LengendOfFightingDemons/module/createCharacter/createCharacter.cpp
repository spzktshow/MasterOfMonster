//
//  createCharacter.cpp
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-7.
//
//

#include "createCharacter.h"
#include "TypeConver.h"
#include "LOFDConfigManager.h"
#include "mainCity.h"
#include "LoginModel.h"

Character::Character(cocos2d::ui::ImageView* instacne,CharacterType type)
{
    image = instacne;
    _type = type;
    
    lastStatus = false;
}

void Character::select(bool status)
{
    if(status == lastStatus)return;
    char path[50] = {0};
    
    if(status)
    {
        sprintf(path,SELECT_CHARACTER_FILE,_type);
    }
    else
    {
         sprintf(path,NORMAL_CHARACTER_FILE,_type);
    }
    
    //loadTexture(const char* fileName,TextureResType texType = UI_TEX_TYPE_LOCAL);
    image->loadTexture(path,cocos2d::ui::TextureResType::UI_TEX_TYPE_PLIST);
    lastStatus = status;
}

//cocos2d::Scene* SelectCharacterPanel::createScene()
//{
//    auto scene = cocos2d::Scene::create();
//    scene->addChild(SelectCharacterPanel::create());
//    return scene;
//}

CREATE_SCENE(SelectCharacterPanel);

bool SelectCharacterPanel::init()
{
    if(!Layer::init())return false;
    
    showWidget(CREATE_CHARCATER_SCENE);
    
    typeBMF = mvc::UITool::getTextBMFFromWidget(rootWidget,"typeBMF");
    desc1Text = mvc::UITool::getTextFromWidget(rootWidget,"desc1Text");
    desc2Text = mvc::UITool::getTextFromWidget(rootWidget,"desc2Text");
    nameBg = getImageViewFromeWidget(rootWidget, "nameBg");
    nameText = mvc::UITool::getTextFieldFromWidget(nameBg,"nameText");
    
    diceBtn = getButtonFromeWidget(rootWidget,"diceBtn");
    enterGameBtn = getButtonFromeWidget(rootWidget,"enterGameBtn");
    
    heroLayout = getLayoutFromWidget(rootWidget,"heroLayout",true);
    //heroLayout->setAnchorPoint(cocos2d::Point(-0.5f,0));
    heroLayout->setTouchEnabled(true);
    skillLayout = getLayoutFromWidget(rootWidget,"skillLayout");
    
    p1 = getImageViewFromeWidget(rootWidget,"p1",true);
    p2 = getImageViewFromeWidget(rootWidget,"p2",true);
    p3 = getImageViewFromeWidget(rootWidget,"p3",true);
    p4 = getImageViewFromeWidget(rootWidget,"p4",true);
    p5 = getImageViewFromeWidget(rootWidget,"p5",true);
    p6 = getImageViewFromeWidget(rootWidget,"p6",true);
    
    
    
    hero = cocos2d::Sprite::create();
    hero->setAnchorPoint(cocos2d::Point(0.5f,0));
    hero->setPositionX(heroLayout->getSize().width /2);
    heroLayout->addChild(hero);
    
    characterMap[CharacterType::ONE]   = new Character(p1,CharacterType::ONE);
    characterMap[CharacterType::TWO]   = new Character(p2,CharacterType::TWO);
    characterMap[CharacterType::THREE] = new Character(p3,CharacterType::THREE);
    characterMap[CharacterType::FOUR]  = new Character(p4,CharacterType::FOUR);
    characterMap[CharacterType::FIVE]  = new Character(p5,CharacterType::FIVE);
    characterMap[CharacterType::SIX]   = new Character(p6,CharacterType::SIX);
    
    desc2Text->setAnchorPoint(cocos2d::Point(0.0f,1.0f));
    desc2Text->setText("");
    desc2Text->setFontSize(30);
    desc1Text->setAnchorPoint(cocos2d::Point(0.0f,1.0f));
    desc1Text->setText("");
    nameText->setText(HeroNameConfig::getInstance()->getHeroNameBySex(getSlectHeroSex()));
    nameText->addEventListenerTextField(this, cocos2d::ui::SEL_TextFieldEvent(&SelectCharacterPanel::onTextFiledHandler));
    //desc1Text->setColor(cocos2d::Color3B(255,255,0));
    //dynamic_cast<cocos2d::Label*>(desc1Text->getVirtualRenderer())->enableShadow();
   // dynamic_cast<cocos2d::Label*>(desc1Text->getVirtualRenderer())->enableGlow(cocos2d::Color4B(255,0,0,200));
    CHILD_CLICK_CALLBACK
    {
        if(type == cocos2d::ui::TouchEventType::TOUCH_EVENT_ENDED)
        {
            if(ref == p1)
            {
                updateSelectHeroSex(1);
                updateSelectHero(CharacterType::ONE);
            }
            else if(ref == p2)
            {
                updateSelectHeroSex(2);
                updateSelectHero(CharacterType::TWO);
            }
            else if(ref == p3)
            {
                updateSelectHeroSex(1);
                updateSelectHero(CharacterType::THREE);
            }
            else if(ref == p4)
            {
               updateSelectHeroSex(2);
                updateSelectHero(CharacterType::FOUR);
            }
            else if(ref == p5)
            {
                updateSelectHeroSex(1);
                updateSelectHero(CharacterType::FIVE);
            }
            else if(ref == p6)
            {
                updateSelectHeroSex(2);
                updateSelectHero(CharacterType::SIX);
            }
            else if(ref == heroLayout)
            {
                if(lastHeroActor == HeroActor::STAND)
                    showHeroAction(lastSelectCharacterType,HeroActor::ATTACK);
            }
            else if(ref == enterGameBtn)
            {
                publishEvent(REQUIRE_ENTER_GAME);
            }
            else if(ref == diceBtn)
            {
                publishEvent(GET_HERO_NAME);
            }
        }
    };
  
    new SelectCharacterMediator(this);
    lofd::ConfigManager::getInstance()->createHeroBeforeConfig();
    //loading::PreLoader::getInstance()->loadCreateCharcater([this](){loadComplete();});
    loadComplete();
    return true;
}

void SelectCharacterPanel::onTextFiledHandler(cocos2d::Ref* ref,cocos2d::ui::TextFiledEventType type)
{
    if(type == cocos2d::ui::TextFiledEventType::TEXTFIELD_EVENT_ATTACH_WITH_IME)
    {
        if(!attackIME)
        {
            //nameText->setText("");
            attackIME = true;
            auto move = cocos2d::MoveBy::create(0.1, cocos2d::Point(0,moveOffset));
            nameBg->runAction(move);
        }
    }
    else if(type == cocos2d::ui::TextFiledEventType::TEXTFIELD_EVENT_DETACH_WITH_IME)
    {
        if(attackIME)
        {
            attackIME = false;
            auto move = cocos2d::MoveBy::create(0.1, cocos2d::Point(0,-moveOffset));
            nameBg->runAction(move);
            
        }
    }

}

void SelectCharacterPanel::loadComplete()
{
    updateSelectHero();
}

void SelectCharacterPanel::updateSelectHeroSex(int sex)
{
    if(sex == lastSelectHeroSex)return;
    lastSelectHeroSex = sex;
    nameText->setText(HeroNameConfig::getInstance()->getHeroNameBySex(lastSelectHeroSex));
}

int SelectCharacterPanel::getSlectHeroSex()
{
    return lastSelectHeroSex;
}

std::string HERO_ACTOR_STAND           =         "stand";
std::string HERO_ACTOR_ATTACK          =         "attack";
std::string HERO_RESOURCE_HEAD         =         "7100";
std::string HERO_RESOURCE_TAIL         =         "00%02d.png";

void SelectCharacterPanel::showHeroAction(CharacterType heroType,HeroActor actorType)
{
   
    lastHeroActor = actorType;
    
    hero->stopAllActions();
    mvc::AnimatInfo info;
    info.duration = 0.1f;
    info.totalFrames = heroActorFrame[heroType - 1][actorType - 18];
    info.animationName = HERO_RESOURCE_HEAD + cocos2d::StringUtils::toString(heroType) + cocos2d::StringUtils::toString(actorType);
    std::string line = "_";
    info.frameName = info.animationName + line + HERO_RESOURCE_TAIL;
    if(actorType == HeroActor::ATTACK)
    {
        hero->runAction(cocos2d::Sequence::create(mvc::UITool::getAnimateWithInfo(info),
                                                  cocos2d::CallFunc::create([this]()
        {
            showHeroAction(lastSelectCharacterType,HeroActor::STAND);
        }),
                                                  NULL));
    }
    else
    {
        hero->runAction(cocos2d::RepeatForever::create(mvc::UITool::getAnimateWithInfo(info)));
    }
}

void SelectCharacterPanel::updateSelectHero(CharacterType type)
{
    if(lastSelectCharacterType == type)return;
    lastSelectCharacterType = type;
    if(curentSelectCharacter)
        curentSelectCharacter->select(false);
    
    curentSelectCharacter = characterMap[type];
    curentSelectCharacter->select(true);
    showHeroAction(type,HeroActor::STAND);
    
    if(type == CharacterType::ONE || type == CharacterType::TWO)
    {
        typeBMF->setText("剑士");
    }
    else if(type == CharacterType::THREE || type == CharacterType::FOUR)
    {
        typeBMF->setText("法师");
    }
    else if(type == CharacterType::FIVE || type == CharacterType::SIX)
    {
        typeBMF->setText("弓箭手");
    }
    
    lofd::HeroBeforeDef* def = lofd::ConfigManager::getInstance()->heroBeforeConfig->getHeroBeforeDefById(heroIds[type - 1]);
    if(def)
    {
        desc2Text->setText(def->intro);
        desc1Text->setText(def->intro_feature);
        showSkill(def->skillIds);
    }
    else
    {
        desc2Text->setText("没有数据呀！");
        desc1Text->setText("没有数据呀");
        std::vector<int> skillList;
        showSkill(skillList);

    }
}

int SelectCharacterPanel::getCurrentSelectHeroID()
{
    return heroIds[lastSelectCharacterType - 1];
}

void SelectCharacterPanel::showSkill(std::vector<int> skillList)
{
    skillLayout->removeAllChildren();
    
    std::vector<int>::iterator it = skillList.begin();
    float totalWidth = 0;
    while (it != skillList.end())
    {
        std::string ID = IntToString(*it);
        ID += ".png";
        auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(ID);
        if(frame)
        {
            auto skillSpr = cocos2d::Sprite::createWithSpriteFrame(frame);
            skillSpr->setAnchorPoint(cocos2d::Point(0.0f,0.0f));
            skillSpr->setPositionX(totalWidth);
            totalWidth += skillSpr->getContentSize().width + 20;
            skillLayout->addChild(skillSpr);
        }
        else
        {
            auto skillSpr = cocos2d::Sprite::create("jump.png");
            skillSpr->setAnchorPoint(cocos2d::Point(0.0f,0.0f));
            skillSpr->setPositionX(totalWidth);
            totalWidth += skillSpr->getContentSize().width + 20;
            skillLayout->addChild(skillSpr);
        }
        
        it++;
    }

}

SelectCharacterMediator::SelectCharacterMediator(SelectCharacterPanel* instance)
{
    panel = instance;
    
    panel->registerMeditor(this);
    registerAttentions(5,ASYNC_LOAD_COMPLETE,REQUIRE_ENTER_GAME,REQUIRE_ENTER_GAME_RESPONSE,GET_HERO_NAME,GET_HERO_NAME_RESPONSE);
    ATTENTION_CALLBACK
    {
        if(eventName == ASYNC_LOAD_COMPLETE)
        {
            CCLOG("resource loadCompelet................................");
            //panel->updateSelectHero(CharacterType::ONE);
        }
        else if (eventName == REQUIRE_ENTER_GAME)
        {
            cocos2d::ValueMap* param = new cocos2d::ValueMap();
            param->insert(std::make_pair(HTTP_ACTION, cocos2d::Value(REQUIRE_ENTER_GAME)));
            param->insert(std::make_pair(LOGIN_ROLE_NAME, cocos2d::Value(panel->nameText->getStringValue())));
            param->insert(std::make_pair(LOGIN_BASE_ID, cocos2d::Value(panel->getCurrentSelectHeroID())));
            sendCommand(SELECT_CHARCTER_COMMAND,param);
            //cocos2d::Director::getInstance()->replaceScene(MainCityScene::createScene());
            //loading::PreLoader::getInstance()->loadMainCity();
        }
        else if(eventName == REQUIRE_ENTER_GAME_RESPONSE)
        {
            //cocos2d::Director::getInstance()->replaceScene(MainCityScene::createScene());
            loading::PreLoader::getInstance()->loadMainCity();
        }
        else if (eventName == GET_HERO_NAME)
        {
//            cocos2d::ValueMap* param = new cocos2d::ValueMap();
//            param->insert(std::make_pair(HTTP_ACTION, cocos2d::Value(GET_HERO_NAME)));
//            sendCommand(SELECT_CHARCTER_COMMAND,param);
              panel->nameText->setText(HeroNameConfig::getInstance()->getHeroNameBySex(panel->getSlectHeroSex()));
        }
        else if(eventName == GET_HERO_NAME_RESPONSE)
        {
            //panel->nameText->setText("");
        }
        
        
    };
}

SelectCharacterServer::SelectCharacterServer()
{
//    service->registerHttpCallBack(WEB_LOGIN, WEB_DO_ACTIVE,
//                                  [this](const char* response)
//    {
//        
//    });
}

void SelectCharacterServer::getHeroName()
{
//    char sendInfo[256] = {0};
//    
//    rapidjson::InsituStringStream stream(sendInfo);
//    stream.PutBegin();
//    
//    rapidjson::PrettyWriter<rapidjson::InsituStringStream> write(stream);
//    write.StartObject();
//    write.String("userId");
//    write.String("8");
//    write.String("c");
//    write.String("Main");
//    write.String("a");
//    write.String("index");
//    
//    write.EndObject();
//    //cocos2d::ValueMap* map;
//    //map->
//    service->sendHttp(sendInfo,strlen(sendInfo));
}

void SelectCharacterServer::enterGame(int heroID,std::string userName)
{
//    char sendInfo[1024] = {0};
//    
//    rapidjson::InsituStringStream stream(sendInfo);
//    stream.PutBegin();
//    
//    rapidjson::PrettyWriter<rapidjson::InsituStringStream> write(stream);
//    write.StartObject();
    START_SEVER_INFO(WEB_LOGIN,WEB_DO_ACTIVE);
//    write.String("c");
//    write.String(WEB_LOGIN);
//    write.String("a");
//    write.String(WEB_DO_ACTIVE);
    write.String(LOGIN_BASE_ID);
    write.Int(heroID);
    write.String(LOGIN_ROLE_NAME);
    write.String(userName.c_str());
    write.String("token");
    write.String(LoginModel::getInstance()->ret->token.c_str());
    END_SERVER_INFO;
    
    //map->
   // service->sendHttp(sendInfo,strlen(sendInfo));
}

SelectCharacterCommand::SelectCharacterCommand()
{
    server = SelectCharacterServer::create();
}

void SelectCharacterCommand::execute(cocos2d::ValueMap *map)
{
    std::string action = map->at(HTTP_ACTION).asString();
    if(action == REQUIRE_ENTER_GAME)
    {
        server->enterGame(map->at(LOGIN_BASE_ID).asInt(),map->at(LOGIN_ROLE_NAME).asString());
    }
    else if(action == GET_HERO_NAME)
    {
        server->getHeroName();
    }
}

CREATE_SIGNAL_INSTANCE(HeroNameConfig, heroNameConfig);

HeroNameConfig::HeroNameConfig()
{
    std::string path = cocos2d::FileUtils::getInstance()->fullPathForFilename("hero_name_before.json");
    std::string data = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
    rapidjson::Document doc;
    
	doc.Parse<0>(data.c_str());
	if(doc.HasParseError())
	{
		CCLOG("GetParserError %s\n",doc.GetParseError());
	}
    else
    {
        int n = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(doc, "datas");
        for (int i = 0; i < n; i ++)
        {
            const rapidjson::Value & heroDef = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(doc, "datas", i);
            int sex = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(heroDef, "sex");
            std::string heroName = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(heroDef, "name");
            HeroNameNode* node = new HeroNameNode();
            node->sex = sex;
            node->name = heroName;
            if(sex == 1)
                manNameMap.push_back(node);
            else
                womenNameMap.push_back(node);
        }
    }
}

std::string HeroNameConfig::getHeroNameBySex(int sex)
{
    int index;
    
    if(sex == 1)
    {
        index = CCRANDOM_0_1() * manNameMap.size();
        return manNameMap[index]->name;
    }
    
    index = CCRANDOM_0_1() * womenNameMap.size();
    return womenNameMap[index]->name;
}