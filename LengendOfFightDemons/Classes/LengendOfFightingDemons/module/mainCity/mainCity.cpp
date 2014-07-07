//
//  mainCity.cpp
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-4-18.
//
//

#include "mainCity.h"
#include "cocostudio/CocoStudio.h"
#include "json/document.h"
#include "bag.h"
#include "hero.h"
<<<<<<< .mine
#include "SmartLoadScene.h"
#include "LOFDConfigManager.h"

static char SHOW_BAG[]                 = "show_bag";
static char SHOW_HERO[]                = "show_hero";
static char SHOW_COPY[]                = "show_copy";
static char SHOW_STRENGTH[]            = "show_strength";
static char SHOW_UNION[]               = "show_union";
static char SHOW_SIGN[]                = "show_sign";
static char SHOW_FIRST_RECHARGE[]      = "show_first_recharge";
static char SHOW_LUCKY_TURN[]          = "show_lucky_turn";
static char SHOW_LEVEL_GIFT_PACKAGE[]  = "show_level_gift_package";
static char SHOW_SET[]                 = "show_set";
static char SHOW_SPIRIT[]              = "show_spirit";
static char SHOW_MINE[]                = "show_mine";
static char SHOW_RUNE[]                = "show_rune";
static char SHOW_TRAVEN[]              = "show_traven";
static char SHOW_BLACKMISTY[]          = "show_blacksimty";
static char ADD_SLIVER[]               = "add_sliver";
static char ADD_GOLD[]                 = "add_gold";
static char ADD_FOOD[]                 = "add_food";
=======
#include "mainCommand.h"
#include "MainModel.h"
>>>>>>> .r799

/***********************************************  主场景   ***************************************************************/

bool expend = false;

//cocos2d::Scene* MainCityScene::createScene()
//{
//    auto scene = cocos2d::Scene::create();
//    scene->addChild(MainCityScene::create());
//    return scene;
//}

CREATE_SCENE(MainCityScene);

MainCityScene::MainCityScene()
{

}

void MainCityScene::update(float dt)
{
    cocos2d::log("main city scene running");
}

void MainCityScene::onExit()
{
    cocos2d::Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool MainCityScene::init()
{
    if(!Layer::init())
        return false;
    
    bgLayer = cocos2d::Sprite::create();
    this->addChild(bgLayer);
    cloudLayer = cocos2d::Sprite::create();
    this->addChild(cloudLayer);
    buildLayer = cocos2d::Sprite::create();
    this->addChild(buildLayer);
    effectLayer = cocos2d::Sprite::create();
    this->addChild(effectLayer);
    panelContainer = cocos2d::Sprite::create();
    this->addChild(panelContainer);
    resourceLayer = cocos2d::Sprite::create();
    this->addChild(resourceLayer);
//    cocos2d::Sprite* bg = cocos2d::Sprite::create("mainCity.png");
//    
//    new MainCitySceneMediator(this);
//    
//    bg->setAnchorPoint(cocos2d::Point(0,0));
//    this->addChild(bg);
//    
//    this->addChild(CloudLayer::create());
//    
//    this->addChild(MainUI::create());
//    
//    this->addChild(MainCityEffect::create());
//    
//    
//    this->addChild(Resource::create());
    
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("");
    /*
      模板 test code
     */
//    auto clip = cocos2d::ClippingNode::create();
//    auto layer = cocos2d::LayerColor::create(cocos2d::Color4B(255,0,0,255));
//    clip->addChild(layer);
//    this->addChild(clip);
//    clip->setInverted(true);
   // auto draw = cocos2d::DrawNode::create();
    //draw->drawTriangle(cocos2d::Point(300,300), cocos2d::Point(200,400), cocos2d::Point(400,400), cocos2d::Color4F(0,1,0,1));
//    clip->setStencil(draw);
    //cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("number.plist");
    
//    auto numS = BMNum::getBMNumWithStringAndType("156625553665552773556772555732325677");
//    numS->setPosition(300, 300);
//    numS->setCascadeColorEnabled(true);
//    //numS->setColor(cocos2d::Color3B::BLACK);
//    this->addChild(numS);
//    auto bmLabel = cocos2d::LabelBMFont::create("我爱  北京天安门385763865345736862345723646237462384682364234238746264537457358236482648264826482358326823658265826534752834628", "Hq.fnt");
//    bmLabel->setPosition(400, 400);
//    this->addChild(bmLabel);
<<<<<<< .mine
    //this->scheduleUpdate();
=======
    
    showCity();
    new MainCitySceneMediator(this);
    
    /******************************************* test code *********************************/
//    for (int i=0; i<20; i++)
//    {
//        auto txt = cocos2d::ui::Text::create();
//        auto label = cocos2d::Label::create();
//        label->setString("adadad");
//        txt->setText("adajdhaskjd");
//        this->addChild(label);
//    }
    
    
>>>>>>> .r799
    return true;
}

void MainCityScene::showCity()
{
    cocos2d::Sprite* bg = cocos2d::Sprite::create("mainCity.png");
    bg->setAnchorPoint(cocos2d::Point(0,0));
    bgLayer->addChild(bg);
    cloudLayer->addChild(CloudLayer::create());
    buildLayer->addChild(MainUI::create());
    effectLayer->addChild(MainCityEffect::create());
    resourceLayer->addChild(Resource::create());
}

void MainCityScene::hideCity()
{
    bgLayer->removeAllChildren();
    cloudLayer->removeAllChildren();
    effectLayer->removeAllChildren();
    buildLayer->removeAllChildren();
    effectLayer->removeAllChildren();
}

void MainCityScene::addPanel(mvc::OrionPanel* panel)
{
    panelContainer->addChild(panel);
    
    int childCount = panelContainer->getChildrenCount();
    if(childCount == 1)
    {
        panel->showAddEffect();
    }
    else
    {
        CCLOG("按照设定不可能出现三个或三个以上的呀！");
    }
}


MainCityScene::~MainCityScene()
{
   
}

MainCitySceneMediator::MainCitySceneMediator(MainCityScene* instance)
{
    scene = instance;
    //registerAttentions(2,"showBag","showHero");
    registerAttentions(19,SHOW_HERO_INFO_PANEL,SHOW_BAG,SHOW_BLACKMISTY,SHOW_COPY,SHOW_FIRST_RECHARGE,SHOW_HERO,SHOW_LEVEL_GIFT_PACKAGE,SHOW_LUCKY_TURN,SHOW_MINE,SHOW_RUNE,SHOW_SET,SHOW_SIGN,SHOW_SPIRIT,SHOW_STRENGTH,SHOW_TRAVEN,SHOW_UNION,ADD_GOLD,ADD_FOOD,REMOVE_WONDOW_PANEL);
    ATTENTION_CALLBACK
    {
//        if(eventName == REMOVE_WONDOW_PANEL)
//        {
//            scene->showCity();
//        }
//        else
        if(eventName == SHOW_BAG)
        {
            //scene->addPanel(BagMainPanel::create());
            loading::PreLoader::getInstance()->LoadBag();
        }
        else if(eventName == SHOW_HERO)
        {
            //scene->addPanel(HeroPanel::create());
            //scene->addPanel(HeroLookAtPanel::create());
           // scene->hideCity();
            loading::PreLoader::getInstance()->loadHero();
        }
        else if (eventName == SHOW_TRAVEN)
        {
            loading::PreLoader::getInstance()->loadTraven();
        }
        else
        {
            cocos2d::Vector<lofd::LoadContext *> loadContexts;
            lofd::LoadSceneDataContext * loadSceneDataContext = new lofd::LoadSceneDataContext(TYPE_SCENE_DATA, "300011.json", 300011);
            loadContexts.pushBack(loadSceneDataContext);
            //    loadContexts.pushBack(lll);
            lofd::LoadConfigContext * loadConfigContext = new lofd::LoadConfigContext(TYPE_CONFIG_JSON, "actorConfig.json", CONFIG_TYPE_ACTOR);
            loadContexts.pushBack(loadConfigContext);
            std::vector<int> stateIds;
            stateIds.push_back(71001);
            //        stateIds.push_back(700002);
            //        stateIds.push_back(700003);
            stateIds.push_back(71002);
            stateIds.push_back(71003);
            stateIds.push_back(71004);
            stateIds.push_back(71005);
            stateIds.push_back(71006);
            stateIds.push_back(MY_ACTOR_ID);
            //        stateIds.push_back(700007);
            //        stateIds.push_back(700008);
            //        stateIds.push_back(700009);
            //        stateIds.push_back(700010);
            //        stateIds.push_back(700011);
            //        stateIds.push_back(700012);
            loadConfigContext = new lofd::LoadConfigContext(TYPE_CONFIG_JSON, "actorStateConfig.json", CONFIG_TYPE_ACTOR_STATE);
            loadContexts.pushBack(loadConfigContext);
            loadConfigContext = new lofd::LoadConfigContext(TYPE_CONFIG_JSON, "actionConfig.json", CONFIG_TYPE_ACTOR_ACTION);
            loadContexts.pushBack(loadConfigContext);
            loadConfigContext = new lofd::LoadConfigContext(TYPE_CONFIG_JSON, "campConfig.json", CONFIG_TYPE_CAMP);
            loadContexts.pushBack(loadConfigContext);
            loadConfigContext = new lofd::LoadConfigContext(TYPE_CONFIG_JSON, "aiConfig.json", CONFIG_TYPE_AI);
            loadContexts.pushBack(loadConfigContext);
            loadConfigContext = new lofd::LoadConfigContext(TYPE_CONFIG_JSON, "skillConfig.json", CONFIG_TYPE_SKILL);
            //    loadContexts.pushBack(loadConfigContext);
            lofd::SmartLoadScene * loadScene = lofd::SmartLoadScene::create();
            loadScene->setLoadContexts(loadContexts);
            loadScene->loadActorStateData(stateIds);
            loadScene->loadSkillData(loadConfigContext);
            cocos2d::log("loadScene");
            cocos2d::Director::getInstance()->replaceScene(loadScene);
        }
    };
    
    scene->registerMeditor(this);
    
    DEFINE_COMMAND_PARMA(WEB_MAIN_START);
    SEND_COMMAND(MAIN_COMMAND);
}

/***********************************************  建筑特效层   ***************************************************************/
bool MainCityEffect::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    std::string jsonpath;
	rapidjson::Document jsonDict;
    jsonpath = cocos2d::CCFileUtils::getInstance()->fullPathForFilename("mainCityEffect.json");
    std::string contentStr = cocos2d::FileUtils::getInstance()->getStringFromFile(jsonpath);
	jsonDict.Parse<0>(contentStr.c_str());
    if (jsonDict.HasParseError())
    {
        CCLOG("GetParseError %s\n",jsonDict.GetParseError());
    }

   // cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(jsonDict["texture"].GetString());
    
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(cocostudio::DictionaryHelper::getInstance()->getStringValue_json(jsonDict, "texture"));
    
    int effectSize = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(jsonDict, "effects");
    for(int i = 0;i < effectSize;i++)
    {
       const rapidjson::Value& effectValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(jsonDict, "effects", i);
        
        int fileSize = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(effectValue, "files");
        //cocos2d::Array
        cocos2d::Vector<cocos2d::SpriteFrame*> spriteVec;
      
        std::string animationName = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(effectValue, "files", 0).GetString();
        cocos2d::Animation* animation = cocos2d::AnimationCache::getInstance()->getAnimation(animationName);
        
        if(!animation)
        {
            
            for(int j = 0; j < fileSize;j++)
            {
                const rapidjson::Value& fileValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(effectValue, "files", j);
                
               // CCLOG("now frame name is %s",fileValue.GetString());
                cocos2d::SpriteFrame* frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(fileValue.GetString());
                
                spriteVec.pushBack(frame);
            }
            
            animation = cocos2d::Animation::createWithSpriteFrames(spriteVec,0.1f);
            cocos2d::AnimationCache::getInstance()->addAnimation(animation, animationName);
        }
       
        cocos2d::Sprite* effect = cocos2d::Sprite::create();
        float _x = cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(effectValue, "x");
        float _y = designResolutionSize.height -  cocostudio::DictionaryHelper::getInstance()->getFloatValue_json(effectValue, "y");
        effect->setPosition(_x, _y);
        effect->setAnchorPoint(cocos2d::Point(0,1));
        effect->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animation)));
        this->addChild(effect);
    }
    
    return true;
}

/***********************************************  云   ***************************************************************/
static int currentCloudCount = 0;
static int lastCloudTag = -1;

CloudLayer::~CloudLayer()
{
    cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("cloudDead");
}

bool CloudLayer::init()
{
    if(!Layer::init())
        return false;
    
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("20005.plist");
    cloudCount = 0;
    
    
    cocos2d::Director::getInstance()->getEventDispatcher()->addCustomEventListener("cloudDead",
                                                                                   [this](cocos2d::EventCustom* custom)
    {
        cloudCount--;
        createCloud();
    });
    cocos2d::Action* action = cocos2d::CallFunc::create([this](){createCloud();});
    cocos2d::Sequence* seq = cocos2d::Sequence::create(cocos2d::DelayTime::create(6),action, NULL);
    this->runAction(cocos2d::RepeatForever::create(seq));
    
    createCloud();
    
    return true;
}

void CloudLayer::createCloud()
{
    if(cloudCount > 5)
        return;
    cloudCount++;
    this->addChild(Cloud::create());
}

cocos2d::Sprite* Cloud::getContent()
{
    return content;
}

bool Cloud::init()
{
    if (!cocos2d::Node::init()) {
        return false;
    }

    int randValue = CCRANDOM_0_1() * CLOUD_COUNT;
    while(randValue == lastCloudTag)
    {
        randValue = CCRANDOM_0_1() * CLOUD_COUNT;
    }
    lastCloudTag = randValue;
    char info[15] = {0};
    std::sprintf(info, "20005_00%02d.png",randValue);
    CCLOG("current ranValue is %d",randValue);
    //std::string cloudName =
    
    content = cocos2d::Sprite::createWithSpriteFrameName(info);
    //content->retain()；
    this->addChild(content);
    currentCloudCount++;
    //z3d = DEPTFH * 3;
    dir = currentCloudCount < 3?1:-1;
    if(currentCloudCount == 4)
        currentCloudCount = 0;
    _contentWidth = content->getContentSize().width / 2;
    _width = _contentWidth + cocos2d::Director::getInstance()->getVisibleSize().width;
//    this->setPosition(_width, cocos2d::Director::getInstance()->getVisibleSize().height * .9);
//    this->scheduleUpdate();
//    this->runAction(cocos2d::FadeIn::create(0.3));
//    update(0.2);
    float startX = _width * (CCRANDOM_MINUS1_1() * dir*0.1);
    this->setPosition(_width* 0.5 + startX, cocos2d::Director::getInstance()->getVisibleSize().height * 0.7);
    //float offsetx = CCRANDOM_0_1() * cocos2d::Director::getInstance()->getVisibleSize().width*0.2;
    //float aimx = dir==1?(cocos2d::Director::getInstance()->getVisibleSize().width + _contentWidth - offsetx):(offsetx - _contentWidth);
    //float aimy = cocos2d::Director::getInstance()->getVisibleSize().height + (CCRANDOM_0_1()) *　content->getContentSize().height;
    float time = 40 + CCRANDOM_0_1()*20;
   // cocos2d::MoveTo* move = cocos2d::MoveTo::create(time,cocos2d::Point(aimx,aimy));
    cocos2d::MoveTo* move = cocos2d::MoveTo::create(time,cocos2d::Point(_width*0.5 + startX*6,cocos2d::Director::getInstance()->getVisibleSize().height + content->getContentSize().height / 2));
    //this->setScale(0.2 + CCRANDOM_0_1()* 0.3);
    this->setScale(0);
    //this->set
    //cocos2d::EaseOut::create(move,1.0f);
    cocos2d::ScaleTo* scaleA = cocos2d::ScaleTo::create(time, 1.5 + CCRANDOM_MINUS1_1()*0.2);
    cocos2d::Sequence* seq = cocos2d::Sequence::create(cocos2d::Spawn::create(cocos2d::EaseOut::create(move,1.0f), cocos2d::EaseOut::create(scaleA,1.0f),NULL),cocos2d::CallFunc::create(
                                                                                      [this]()
    {
        this->removeFromParent();
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("cloudDead");
    }),NULL);
    this->runAction(seq);
    return true;
}

Cloud* Cloud::initWithZ3D(int value)
{
    //z3d = value;
    Cloud* cloud = create();
    cloud->z3d = value;
    return cloud;
}

void Cloud::update(float delta)
{
    delayCount++;
    if (delayCount < 3) {
        return;
    }
    delayCount = 0;
    z3d--;
    double total = DEPTFH + z3d;
    float scale = DEPTFH / total;
    
    this->setScale(scale);
    //float _x = dir == 1?(_width + scale*_width):(_width - scale*_width);
    float _x = _width - (scale - 0.25)*_width;
    this->setPositionX(_x);
    
    if (_x <  -_contentWidth|| _x > (_width * 2 + _contentWidth))
    {
        //content->release();
        this->unscheduleUpdate();
        this->removeFromParent();
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("cloudDead");
    }
    
}

/***********************************************  主UI   ***************************************************************/
bool MainUI::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    new MainUIMediator(this);
    
    showFunBar = false;
    isMoving = false;
    
    auto widget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(MAIN_CITY);
    this->addChild(widget);
    
    bottomLayout = dynamic_cast<cocos2d::ui::Layout*>(widget->getChildByName("functionBar")->getChildByName("bottom"));
    heroBtn = getButtonFromeWidget(bottomLayout,"hero");
    bagBtn = getButtonFromeWidget(bottomLayout,"bag");
    copyBtn = getButtonFromeWidget(bottomLayout,"copy");
    strengthBtn = getButtonFromeWidget(bottomLayout,"strength");
    unionBtn = getButtonFromeWidget(bottomLayout,"union");
    
    rightLayout = dynamic_cast<cocos2d::ui::Layout*>(widget->getChildByName("functionBar")->getChildByName("right"));
    signBtn = getButtonFromeWidget(rightLayout,"sign");
    rechargeBtn1 = getButtonFromeWidget(rightLayout,"recharge");
    luckTurnBtn = getButtonFromeWidget(rightLayout,"luckTurn");
    levelGiftPackageBtn = getButtonFromeWidget(rightLayout,"levelGiftPackage");
    
    controlBtn = getButtonFromeWidget(dynamic_cast<cocos2d::ui::Layout*>(widget->getChildByName("functionBar")), "fubBtn");
    showLabel = dynamic_cast<cocos2d::ui::ImageView*>(widget->getChildByName("functionBar")->getChildByName("showLabel"));
    hideLabel = dynamic_cast<cocos2d::ui::ImageView*>(widget->getChildByName("functionBar")->getChildByName("hideLabel"));
    
    updateFunBarStatus();
    showFunBar = expend;
    if(!showFunBar)
    {
        rightLayout->setPositionY(rightLayout->getPositionY() - rightLayout->getContentSize().height);
        bottomLayout->setPositionX(bottomLayout->getPositionX() + bottomLayout->getContentSize().width);
    }
    
//    resourceLayout = dynamic_cast<cocos2d::ui::Layout*>(widget->getChildByName("top"));
//    sliverAddBtn = getButtonFromeWidget(resourceLayout,"sliverAddBtn");
//    goldAddBtn = getButtonFromeWidget(resourceLayout,"goldAddBtn");
//    foodAddBtn = getButtonFromeWidget(resourceLayout,"foodAddBtn");
//    sliverLabel = dynamic_cast<cocos2d::ui::Text*>(resourceLayout->getChildByName("sliverLabel"));
//    goldLabel = dynamic_cast<cocos2d::ui::Text*>(resourceLayout->getChildByName("goldLabel"));
//    foodLabel = dynamic_cast<cocos2d::ui::Text*>(resourceLayout->getChildByName("foodLabel"));
    
    userinfoLayout = dynamic_cast<cocos2d::ui::Layout*>(widget->getChildByName("userInfo"));
    rechargeBtn2 = getButtonFromeWidget(userinfoLayout,"recharge");
    nameLabel = dynamic_cast<cocos2d::ui::Text*>(userinfoLayout->getChildByName("nameLabel"));
    levelLabel = dynamic_cast<cocos2d::ui::Text*>(userinfoLayout->getChildByName("levelLabel"));
    heroAvatar = dynamic_cast<cocos2d::ui::ImageView*>(userinfoLayout->getChildByName("heroAvatar"));
    heroAvatar->loadTexture("testHeroHead.png");
    
//    auto setLayout = dynamic_cast<cocos2d::ui::Layout*>(widget->getChildByName("set"));
//    setBtn = getButtonFromeWidget(setLayout,"set");
    
    auto buildLayout = dynamic_cast<cocos2d::ui::Layout*>(widget->getChildByName("scene"));
    spiritBtn = getImageViewFromeWidget(buildLayout,"spirit",true);
    mineBtn = getImageViewFromeWidget(buildLayout,"mine",true);
    tarvenBtn = getImageViewFromeWidget(buildLayout,"traven",true);
    runeBtn = getImageViewFromeWidget(buildLayout,"rune",true);
    blacksmithyBtn = getImageViewFromeWidget(buildLayout,"blacksmity",true);
    
    auto maskLayout = widget->getChildByName("functionBar")->getChildByName("mask");
    
    auto mask = cocos2d::Sprite::create("mainCity.png", cocos2d::Rect(designResolutionSize.width - maskLayout->getContentSize().width,designResolutionSize.height-maskLayout->getContentSize().height ,maskLayout->getContentSize().width,maskLayout->getContentSize().height));
    maskLayout->addChild(mask);
    mask->setAnchorPoint(cocos2d::Point(0,0));
    
    
//    CHILD_CLICK_CALLBACK
//    {
//      IF_CHILD_TOUCH_ENDED
//      {
//          if(ref == tarvenBtn)
//              publishEvent(SHOW_TRAVEN);
//      }
//    };
    return true;
}

cocos2d::ui::Button* MainUI::getButtonFromeWidget(cocos2d::ui::Layout* layout,std::string name)
{
    auto btn = dynamic_cast<cocos2d::ui::Button*>(layout->getChildByName(name.c_str()));
    if(btn)
        btn->addTouchEventListener(this,cocos2d::ui::SEL_TouchEvent(&MainUI::onChildClick));
    return btn;
}


cocos2d::ui::ImageView* MainUI::getImageViewFromeWidget(cocos2d::ui::Layout *layout, std::string name,bool addListener)
{
    auto image = dynamic_cast<cocos2d::ui::ImageView*>(layout->getChildByName(name.c_str()));
    if(image && addListener)
        image->addTouchEventListener(this,cocos2d::ui::SEL_TouchEvent(&MainUI::onChildClick));
    return image;

}

void MainUI::updateFunBarStatus()
{
    hideLabel->setVisible(showFunBar);
    showLabel->setVisible(!showFunBar);
}

/*
 合并
 */
void MainUI::foldFunBar()
{
    isMoving = true;
    bottomLayout->runAction(cocos2d::Sequence::create(cocos2d::MoveBy::create(FUNBAR_MOVE_TIME, cocos2d::Point(bottomLayout->getContentSize().width,0)),
                                                      cocos2d::CallFunc::create(
                                                                                [this]()
    {
        isMoving = false;
        updateFunBarStatus();
    }),
                                                      NULL));
    
    rightLayout->runAction(cocos2d::Sequence::create(cocos2d::MoveBy::create(FUNBAR_MOVE_TIME, cocos2d::Point(0,-rightLayout->getContentSize().height)),NULL));

}

/*
 展开
 */
void MainUI::unFlodBar()
{
    isMoving = true;
    
    bottomLayout->runAction(cocos2d::Sequence::create(cocos2d::MoveBy::create(FUNBAR_MOVE_TIME, cocos2d::Point(-bottomLayout->getContentSize().width,0)),
                                                      cocos2d::CallFunc::create(
                                                                                [this]()
                                                                                {
                                                                                    isMoving = false;
                                                                                    updateFunBarStatus();
                                                                                }),
                                                      NULL));
    
    rightLayout->runAction(cocos2d::Sequence::create(cocos2d::MoveBy::create(FUNBAR_MOVE_TIME, cocos2d::Point(0,rightLayout->getContentSize().height)),NULL));

}

void MainUI::onChildClick(cocos2d::Ref* target, cocos2d::ui::TouchEventType type)
{
    
    if(type != cocos2d::ui::TouchEventType::TOUCH_EVENT_ENDED)return;
    
    if(target == controlBtn)
    {
        if(isMoving)return;
        showFunBar = !showFunBar;
        expend = showFunBar;
        if(showFunBar)
            unFlodBar();
        else
            foldFunBar();
    }
    else if(target == rechargeBtn1)//首充礼包
    {
        
    }
    else if(target == rechargeBtn2)//充值
    {
        
    }
    else if(target == heroBtn)
    {
        //sendEvent("showHero");
        publishEvent(SHOW_HERO);
    }
    else if(target == bagBtn)
    {
       // sendEvent("showBag");
        publishEvent(SHOW_BAG);
    }
    else if(target == copyBtn)
    {
        
    }
    else if(target == strengthBtn)
    {
        
    }
    else if(target == unionBtn)
    {
        publishEvent(SHOW_UNION);
    }
    else if(target == signBtn)
    {
        
    }
    else if(target == luckTurnBtn)
    {
        
    }
    else if(target == levelGiftPackageBtn)
    {
        
    }
    else if(target == spiritBtn)
    {
       publishEvent("gotoBattle");
    }
    else if(target == mineBtn)
    {
        
    }
    else if(target == blacksmithyBtn)
    {
        
    }
    else if(target == runeBtn)
    {
        
    }
    else if(target == tarvenBtn)
    {
        publishEvent(SHOW_TRAVEN);
    }
}

MainUIMediator::MainUIMediator(MainUI* instance)
{
    ui = instance;
    registerAttentions(1,MAIN_START_RESPONSE);
    ATTENTION_CALLBACK
    {
        if(eventName == MAIN_START_RESPONSE)
        {
            showMainUIInfo();
        }

    };
    ui->registerMeditor(this);
    //showMainUIInfo();
}

void MainUIMediator::showMainUIInfo()
{
    ui->nameLabel->setText(main::MainModel::getInstance()->userInfo->user_name);
    std::string lvlStr = "Lv."+ main::MainModel::getInstance()->userInfo->level;
    ui->levelLabel->setText(lvlStr);
}

/***********************************************  资源条   ***************************************************************/
bool Resource::init()
{
    if(!Layer::init())return false;
    showWidget(MAIN_CITY_RESOURCE,false,false);
    auto resourceLayout = dynamic_cast<cocos2d::ui::Layout*>(rootWidget->getChildByName("top"));
    sliverAddBtn = getButtonFromeWidget(resourceLayout,"sliverAddBtn");
    goldAddBtn = getButtonFromeWidget(resourceLayout,"goldAddBtn");
    foodAddBtn = getButtonFromeWidget(resourceLayout,"foodAddBtn");
    sliverLabel = dynamic_cast<cocos2d::ui::Text*>(resourceLayout->getChildByName("sliverLabel"));
    sliverLabel->setAnchorPoint(cocos2d::Point(0.0f,0.0f));
    goldLabel = dynamic_cast<cocos2d::ui::Text*>(resourceLayout->getChildByName("goldLabel"));
    goldLabel->setAnchorPoint(cocos2d::Point(0.0f,0.0f));
    foodLabel = dynamic_cast<cocos2d::ui::Text*>(resourceLayout->getChildByName("foodLabel"));
    foodLabel->setAnchorPoint(cocos2d::Point(0.0f,0.0f));
    
    auto setLayout = dynamic_cast<cocos2d::ui::Layout*>(rootWidget->getChildByName("set"));
    setBtn = getButtonFromeWidget(setLayout,"set");
    
    CHILD_CLICK_CALLBACK
    {
        if(type == cocos2d::ui::TouchEventType::TOUCH_EVENT_ENDED)
        {
                if(ref == setBtn)
                {
            
                }
                else if(ref == sliverAddBtn)
                {
                    publishEvent(ADD_SLIVER);
                }
                else if(ref == goldAddBtn)
                {
                    
                }
                else if(ref == foodAddBtn)
                {
                    
                }

        }
    };
    new ResourseMediator(this);
    return true;
}


ResourseMediator::~ResourseMediator()
{
    CCLOG("ResourseMediator::~ResourseMediator()");
}

ResourseMediator::ResourseMediator(Resource* instance)
{
    CCLOG("---------------------------ResourseMediator::ResourseMediator()");
    resource = instance;
    registerAttentions(1,MAIN_START_RESPONSE);
    ATTENTION_CALLBACK
    {
        if(eventName == MAIN_START_RESPONSE)
        {
             showResourceInfo();
        }
    };
    resource->registerMeditor(this);
    
    showResourceInfo();
}

void ResourseMediator::showResourceInfo()
{
    resource->sliverLabel->setText(tool::NumberFormat::getMoneyFormatWithNumber(StringToInt(main::MainModel::getInstance()->userRes->silver)));
    resource->goldLabel->setText(tool::NumberFormat::getMoneyFormatWithNumber(StringToInt(main::MainModel::getInstance()->userRes->diamond)));
    resource->foodLabel->setText(tool::NumberFormat::getMoneyFormatWithNumber(StringToInt(main::MainModel::getInstance()->userRes->energy)));
    
}
