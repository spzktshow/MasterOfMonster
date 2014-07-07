//
//  traven.cpp
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-20.
//
//

#include "traven.h"
#include "mainCity.h"
#include "hero.h"
#include "HeroModel.h"
#include "MainModel.h"

NS_TRAVEN_BEGIN
/*************************************************************************************/

static bool normalOpen = true;
//int select_traven_hero_id;

CREATE_SCENE(TravenPanel);

TravenPanel::~TravenPanel()
{
    RELEASE_ITEM_GROUP;
}

bool TravenPanel::init()
{
    LAYER_INIT;
    
    closeEventTage = REMOVE_WONDOW_PANEL;
    showWidget(TRAVEN_MAIN_PANEL,true,true,false,true,"酒馆",TitleStyle::BEAUTIFUL,-10);
    
    heroImage       =   getImageViewFromeWidgetWithName(rootWidget,HERO_LAYOUT,"heroImage");
    heroNameBMF     =   getTextBMFFromWidgetWithName(rootWidget, BMF_LAYOUT, "heroNameBMF");
    jobBMF          =   getTextFromWidgetWithName(rootWidget, BMF_LAYOUT, "jobBMF");
    locationBMF     =   getTextFromWidgetWithName(rootWidget, BMF_LAYOUT, "locationBMF");
    favorLabel      =   getTextFromWidgetWithName(rootWidget, BMF_LAYOUT, "favorLabel");
    powerBar        =   getLoadingBarFromWidgetWithName(rootWidget, HERO_LAYOUT, "powerBar");
    quickBar        =   getLoadingBarFromWidgetWithName(rootWidget, HERO_LAYOUT, "quickBar");
    phyBar          =   getLoadingBarFromWidgetWithName(rootWidget, HERO_LAYOUT, "phyBar");
    intelBar        =   getLoadingBarFromWidgetWithName(rootWidget, HERO_LAYOUT, "intelBar");
    recruitBtn      =   getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "recruitBtn");
    favorBtn        =   getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "favorBtn");
    cardLayout      =   getLayoutFromWidget(rootWidget, "cardLayout",true);
    cardLayout->setClippingEnabled(true);
    skillLayout     =   getLayoutFromWidget(rootWidget, "skillLayout");
    
    CHILD_CLICK_CALLBACK
    {
        IF_CHILD_TOUCH_MOVED
        {
            if(ref == cardLayout)
            {
                
                moveCardContainer(cardLayout->getTouchMovePos().x - startX);
                startX = cardLayout->getTouchMovePos().x;
            }
        }
        else IF_CHILD_TOUCH_BEGAN
        {
            if(ref == cardLayout)
            {
                startX = cardLayout->getTouchStartPos().x;
                //CCLOG("startX is %f",startX);
                //startX = 0.0f;
            }
        }
        else IF_CHILD_TOUCH_ENDED
        {
            if(ref == cardLayout)
            {
                modifyCardPosition();
            }
            else if(ref == recruitBtn)
            {
                publishEvent(WEB_HERO_RECRUIT);
            }
            else if (ref == favorBtn)
            {
                normalOpen = false;
                publishEvent(TRAVEN_GET_FAVOR);
                this->removeFromParent();
            }
            else if(ref == closeBtn)
            {
                normalOpen = true;
                HeroCommmon::getInstance()->currentSelectPubHeroModel = nullptr;
            }
        }
        else IF_CHILD_TOUCH_CANCELED
        {
            if(ref == cardLayout)
            {
                modifyCardPosition();
            }
        }
    };
    
    CREATE_ITEM_GROUP
    {
        HeroCard* card = TRANSLATE_TYPE(HeroCard, item);
        //select_traven_hero_id = card->def->hero_id;
        HeroCommmon::getInstance()->currentSelectPubHeroModel = card->model;
        this->onSelectPubHero(card->model);
        CCLOG("------------------ curren select card is %s",card->model->hero_id.c_str());
    };
    
    new TravenMediator(this);
    addResourceBar(false,normalOpen);
    if(!normalOpen)
        showEffect = true;
    RETURN_TRUE;
}

void TravenPanel::modifyCardPosition()
{
    int len = cardVec.size();
    for(int i=0; i < len;i++)
    {
        HeroCard* card = cardVec.at(i);
        card->modifyPosition();
    }
}

void TravenPanel::onSelectPubHero(hero::pubInfoModel* model)
{
    lofd::PubBeforeNode* def = lofd::ConfigManager::getInstance()->pubBeforeConfig->getPubBeforeNodeById(model->hero_id);
    lofd::HeroBeforeDef* heroDef = lofd::ConfigManager::getInstance()->heroBeforeConfig->getHeroBeforeDefById(StringToInt(model->hero_id));
    
    jobBMF->setText(heroDef->job_name);
    
    heroImage->removeAllChildren();
    LOAD_TEXTURE_FROM_PLIST_WITH_ID(heroImage, def->hero_bg);
    
    auto image = cocos2d::ui::ImageView::create();
    LOAD_TEXTURE_FROM_PLIST_WITH_ID(image, def->all_body);
    image->setAnchorPoint(cocos2d::Point(0.0f,0.0f));
    if(model->state == "0")
    {
        image->setColor(cocos2d::Color3B::BLACK);
    }
    
    heroImage->addChild(image);
}

void TravenPanel::moveCardContainer(float offset)
{
    //auto moveBy = cocos2d::MoveBy::create(0.1f, cocos2d::Point(offset,0));
    //CCLOG("------------ offset is %f",offset);
    int len = cardVec.size();
    for (int i=0; i<len; i++)
    {
        HeroCard* card = cardVec.at(i);
        card->moveOffset(offset);
    }
}

void TravenPanel::showHeroCardList()
{
    float totalWidth = 0.0f;
    cardLayout->removeAllChildren();
    cardVec.clear();
    
    CLEAR_ITEM_GROUP;
    
    //int len = lofd::ConfigManager::getInstance()->pubBeforeConfig->defVec.size();
    int len = hero::HeroModel::getInstance()->pubInfo->size();
    //int len = 12;
    for(int i=0;i<len;i++)
    {
        HeroCard* item = HeroCard::create();
        float itemWidth = item->getSize().width;
        item->setPosition(cocos2d::Point(totalWidth + itemWidth/2,cardLayout->getSize().height / 2));
        totalWidth += itemWidth;
        cardLayout->addChild(item);
        item->openCards = len;
        item->setLayoutWidth(len * itemWidth);
        cardVec.push_back(item);
        //item->setItemModel(lofd::ConfigManager::getInstance()->pubBeforeConfig->defVec.at(i));
        item->setItemModel(hero::HeroModel::getInstance()->pubInfo->at(i));
        ITEM_SET_GROUP;
        item->modifyPosition();
    }
    //moveCardContainer(0.0f);
}

TravenMediator::TravenMediator(TravenPanel* instance)
{
    panel = instance;
    
    lofd::ConfigManager::getInstance()->createHeroBeforeConfig();
    lofd::ConfigManager::getInstance()->createPubBeforeConfig();
    
    registerAttentions(4,TRAVEN_GET_FAVOR,WEB_HERO_RECRUIT,GET_HERO_RECRUIT_RESPONSE,GET_HERO_PUB_INFO_RESPONSE);
    ATTENTION_CALLBACK
    {
        if(eventName == TRAVEN_GET_FAVOR)
        {
            cocos2d::Director::getInstance()->getRunningScene()->addChild(TravenGuessPanel::create());
        }
        else if(eventName == WEB_HERO_RECRUIT)
        {
            if(HeroCommmon::getInstance()->currentSelectPubHeroModel == nullptr)
            {
                return;
            }
            DEFINE_COMMAND_PARMA(WEB_HERO_RECRUIT);
            CREATE_COMMAND_PARMA(WEB_HERO_ID, HeroCommmon::getInstance()->currentSelectPubHeroModel->hero_id);
            SEND_COMMAND(HERO_COMMAND);
            
        }
        else if(eventName == GET_HERO_RECRUIT_RESPONSE)
        {
            
        }
        else if (eventName == GET_HERO_PUB_INFO_RESPONSE)
        {
            panel->showHeroCardList();
        }

    };
    //panel->showHeroCardList();
    DEFINE_COMMAND_PARMA(WEB_HERO_GET_PUB);
    SEND_COMMAND(HERO_COMMAND);
    panel->registerMeditor(this);
}

/*************************************************************************************/


/*************************************************************************************/
bool HeroCard::init()
{
    NODE_INIT;
    showWidget(TRAVEN_HERO_CARD);
    cocos2d::Size size = rootWidget->getSize();
    size.width = (int)(size.width / 2) * 2;
    this->setSize(size);
    cardImage = mvc::UITool::getImageViewFromeWidget(rootWidget, "cardImage");
    cardImage->setTouchEnabled(false);
    this->setTouchEnabled(false);
    
    frameImage = mvc::UITool::getImageViewFromeWidget(rootWidget, "ImageView_175");
    
    rootWidget->setAnchorPoint(cocos2d::Point(0.5f,0.5f));
//    registerEvent(cardImage);
//    CHILD_CLICK_CALLBACK
//    {
//        if(!isMoving)
//        {
//            IF_CHILD_TOUCH_ENDED
//            {
//                if(ref == cardImage)
//                {
//                    this->group->selectItem(this);
//                }
//            }
//
//        }
//    };
    
    halfWidth = getSize().width/2;
    
    cardImage->loadTexture(FRAME_HERO_LOCK_LABEL_FILE_NAME,cocos2d::ui::TextureResType::UI_TEX_TYPE_PLIST);
    
    RETURN_TRUE;
}

void HeroCard::setItemModel(hero::pubInfoModel* modelValue)
{
    model = modelValue;
    lofd::PubBeforeNode* def = lofd::ConfigManager::getInstance()->pubBeforeConfig->getPubBeforeNodeById(model->hero_id);
    if(model->state == "1" || true)
    {
        LOAD_TEXTURE_FROM_PLIST_WITH_ID(cardImage,def->hero_card);
        setRecruitStatus();
    }
}

void HeroCard::modifyPosition()
{
    float offsetX = this->getPositionX();
    int index = 0;
    if(offsetX > 0.0f)
        index = offsetX / this->getSize().width;
    else if(offsetX < 0.0f)
        index = openCards - 1;
    else index = 0;
    float aimX = index * this->getSize().width + halfWidth;
    this->setPositionX(aimX);
    float scale = MIN_SCALE;
    switch (index)
    {
        case 1:
            scale = MIDDLE_SCALE;
            break;
        case 2:
            scale = MAX_SCALE;
            this->group->selectItem(this);
            break;
        case 3:
            scale = MIDDLE_SCALE;
        default:
            break;
    }
    this->setScale(scale);
    this->setLocalZOrder(scale*10);
}

void HeroCard::select(bool sel)
{
    if(sel)
    {
        frameImage->loadTexture(FRAME_HERO_CARD_SELECT_NAME,cocos2d::ui::TextureResType::UI_TEX_TYPE_PLIST);
    }
    else
    {
        frameImage->loadTexture(FRAME_HERO_CARD_NAME,cocos2d::ui::TextureResType::UI_TEX_TYPE_PLIST);
    }
}

void HeroCard::setLayoutWidth(float width)
{
    layoutWidth = width;
}

void HeroCard::moveOffset(float offset)
{
    float aimX = offset + this->getPositionX();
    if(aimX < -halfWidth)
        aimX = layoutWidth + aimX;
    else if(aimX > layoutWidth - halfWidth)
        //aimX = aimX - (layoutWidth - halfWidth) - halfWidth;
        aimX = aimX - layoutWidth;
    this->setPositionX(aimX);
    
    float scale = MIN_SCALE;
    bool select = false;
    if(aimX > halfWidth && aimX < 9* halfWidth)
    {
        float half = 5*halfWidth;
        
        if(aimX > half)/**right**/
        {
//            float absValue = std::fabs(aimX - half);
//            if(absValue < halfWidth)
//            {
//                scale = 1.4f;
//                select = true;
//            }
//            else
//            {
//                float absValue = std::fabs(aimX - 7*halfWidth);
//                if(absValue < halfWidth)
//                    scale = 1.2f;
//            }
            
            float offsetX = 9*halfWidth - aimX;
            scale = MIN_SCALE + (MAX_SCALE - MIN_SCALE)*(offsetX / (4 * halfWidth));
        }
        else if(aimX < half)/**left**/
        {
//            float absValue = std::fabs(aimX - half);
//            if(absValue < halfWidth)
//            {
//                scale = 1.4f;
//                select = true;
//            }
//            else
//            {
//                float absValue = std::fabs(aimX - 3*halfWidth);
//                if(absValue < halfWidth)
//                    scale = 1.2;
//            }
            
            float offsetX = aimX - halfWidth;
            scale = MIN_SCALE + (MAX_SCALE - MIN_SCALE)*(offsetX / (4 * halfWidth));
        }
        else
        {
            scale = MAX_SCALE;
        }
        
    }
    else
    {
    }
    
    if(select && this->group->getItem() != this)
    {
        this->group->selectItem(this);
    }
   
        this->setScale(scale);
        this->setLocalZOrder(scale*10);
}

void HeroCard::setRecruitStatus()
{
    auto label = cocos2d::Sprite::createWithSpriteFrameName(FRAME_RECRUIT_FILE_NAME);
    //label->setPosition(cocos2d::Point(cardImage->getSize().width/2,cardImage->getSize().height/2));
    cardImage->addChild(label);
}
/*************************************************************************************/


/*********************************guess Panel****************************************************/
bool TravenGuessPanel::init()
{
    LAYER_INIT;
    
    showWidget(TRAVEN_GUESS_PANEL,true,true,false,true,"酒馆",TitleStyle::BEAUTIFUL,-10,FRAME_GUESS_BG_NAME);
    
    oneBtn              =       getButtonFromeWidgetWithWidgetName(rootWidget, HERO_LAYOUT, "oneBnt");
    twoBtn              =       getButtonFromeWidgetWithWidgetName(rootWidget, HERO_LAYOUT, "twoBtn");
    beginBtn            =       getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "beginBtn");
    guessFiveBtn        =       getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "guessFiveBtn");
    winBtn              =       getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "winBtn");
    questionBtn         =       getButtonFromeWidgetWithWidgetName(rootWidget, HERO_LAYOUT, "questionBtn");
    costLabel           =       getTextFromWidgetWithName(rootWidget, BMF_LAYOUT, "costLabel");
    favorLabel          =       getTextFromWidgetWithName(rootWidget, BMF_LAYOUT, "favorLabel");
//    leftDilogLabel      =       getTextFromWidgetWithName(rootWidget, BMF_LAYOUT, "leftDilogLabel");
//    rightDilogLabel     =       getTextFromWidgetWithName(rootWidget, BMF_LAYOUT, "rightDilogLabel");
    leftDilogImage      =       getImageViewFromeWidgetWithName(rootWidget, HERO_LAYOUT, "leftDilogImage");
    rightDilogImage      =       getImageViewFromeWidgetWithName(rootWidget, HERO_LAYOUT, "rightDilogImage");
    selfHeroImage       =       getImageViewFromeWidgetWithName(rootWidget, HERO_LAYOUT, "selfHeroImage");
    recruitHeroImage    =       getImageViewFromeWidgetWithName(rootWidget, HERO_LAYOUT, "recruitHeroImage");
    luckBar             =       getLoadingBarFromWidgetWithName(rootWidget, HERO_LAYOUT, "luckBar");
    
    turnSpr = SelfSprite::create();
    
    getImageViewFromeWidgetWithName(rootWidget, HERO_LAYOUT, "pubgoodImage")->addChild(turnSpr);
    mvc::AnimatInfo info;
    info.duration = 0.05f;
    info.animationName = COIN_NORMAL_ANIMATION_NAME;
    info.frameName = COIN_RESOURCE;
    info.totalFrames = 8;
    mvc::UITool::getAnimateWithInfo(info);

    info.duration = 0.1f;
    info.animationName = COIN_ONE_ANIMATION_NAME;
    info.frameName = COIN_RESOURCE;
    info.totalFrames = 5;
    mvc::UITool::getAnimateWithInfo(info);
    
    info.duration = 0.1f;
    info.animationName = COIN_TWO_ANIMATION_NAME;
    info.frameName = COIN_RESOURCE;
    info.totalFrames = 8;
    mvc::UITool::getAnimateWithInfo(info,true);
    
    turnSpr->setDisplayFrameWithAnimationName(COIN_NORMAL_ANIMATION_NAME, 0);
    new TravenGuessMediator(this);
    
    canBreak = true;
    firstRound = true;
    leftDilog = nullptr;
    rightDilog = nullptr;
    selectCoinType = COIN_TYPE::ZERO;
    
    CHILD_CLICK_CALLBACK
    {
        IF_CHILD_TOUCH_ENDED
        {
            if(ref == oneBtn)
            {
                selectCoinType = COIN_TYPE::ONE;
                selectCoin();
            }
            else if(ref == twoBtn)
            {
                selectCoinType = COIN_TYPE::TWO;
                selectCoin();
            }
            else if(ref == beginBtn)
            {
                startGuess();
//                if(!turnSpr->getIsRunning() && selectCoinType != COIN_TYPE::ZERO)
//                {
//                    
//                    startGetFavor();
//                    showCoinAnimate(COIN_NORMAL_ANIMATION_NAME);
//                    publishEvent(WEB_HERO_GET_LIKING);
//                }
            }
            else if(ref == guessFiveBtn)
            {
                autoType = AUTO_TYPE::AUTO;
                startGuess();
//                if(!turnSpr->getIsRunning() && selectCoinType != COIN_TYPE::ZERO)
//                {
//                    startGetFavor();
//                    showCoinAnimate(COIN_NORMAL_ANIMATION_NAME);
//                    publishEvent(WEB_HERO_GET_LIKING);
//                }

            }
            else if(ref == winBtn)
            {
                willLiking = true;
                startGuess();
//                if(!turnSpr->getIsRunning() && selectCoinType != COIN_TYPE::ZERO)
//                {
//                 
//                    startGetFavor();
//                    showCoinAnimate(COIN_NORMAL_ANIMATION_NAME);
//                    publishEvent(WEB_HERO_GET_WILL_LIKINT);
//                }
            }
            else if(ref == questionBtn)
            {
                
            }
            else if(ref == closeBtn)
            {
                cocos2d::Director::getInstance()->getRunningScene()->addChild(TravenPanel::create());
            }
        }
    };
    
    RETURN_TRUE;
}

void TravenGuessPanel::onReallyShow()
{
    new TravenGuessMediator(this);
}

void TravenGuessPanel::removeAllDilog()
{
    if(leftDilog)
    {
        leftDilog->hide();
        leftDilog = nullptr;
    }

    if(rightDilog)
    {
        rightDilog->hide();
        rightDilog = nullptr;
    }

}

void TravenGuessPanel::showLeftDilog(std::string info,std::function<void()> callfunc)
{
    if(leftDilog)
        leftDilog->hide();
    leftDilog = CardDilog::create();
    leftDilog->callbackFunc = callfunc;
    leftDilog->show(info);
    this->addChild(leftDilog);
    leftDilog->setPosition(leftDilogImage->getPosition());

}

void TravenGuessPanel::showRightDilog(std::string info,std::function<void()> callfunc)
{
    if(rightDilog)
        rightDilog->hide();
    rightDilog = CardDilog::create();
    rightDilog->callbackFunc = callfunc;
    rightDilog->show(info,DILOG_DIR::RIGHT);
    this->addChild(rightDilog);
    rightDilog->setPosition(rightDilogImage->getPosition());
}

void TravenGuessPanel::startGuess()
{
    if(beginSelect)
    {
        CCLOG("已经开始  等待下一局..................................");
        return;
    }
    
    if(!canBreak)
    {
        CCLOG("当前局还未结束 重复选择不接受..................................");
        return;
    }
    
    removeAllDilog();
    
    beginSelect = true;
    canBreak = false;
    
    
}

void TravenGuessPanel::selectCoin()
{
    if(!beginSelect)
    {
        CCLOG("还没有开始 选择硬币无效..................................");
        return;
    }
    
    if(selectCoinType != COIN_TYPE::ZERO)
    {
        CCLOG("不能重复选择硬币........................................");
        return;
    }
    
    if(willLiking)
    {
        publishEvent(WEB_HERO_GET_WILL_LIKINT);
    }
    else
    {
        publishEvent(WEB_HERO_GET_LIKING);
    }
    
    startGetFavor();
    //showCoinAnimate(COIN_NORMAL_ANIMATION_NAME);

}

/*
 后端返回硬币面
 */
void TravenGuessPanel::setServerCoinType(int value)
{
    serverCoinType = value;
    
    if(value == selectCoinType)
        isWin = true;
    else
        isWin = false;
    
    if(autoType == AUTO_TYPE::AUTO)
    {
        if(serverCoinType == COIN_TYPE::ONE)
        {
            showSecondCoinAnimate(COIN_ONE_ANIMATION_NAME);
        }
        else if(serverCoinType == COIN_TYPE::TWO)
        {
            showSecondCoinAnimate(COIN_TWO_ANIMATION_NAME);
        }
    }
}

/*
 在拿到数据前的硬币动画
 */
void TravenGuessPanel::showCoinAnimate(std::string style,bool add)
{
    turnSpr->stopAction();
    auto anim = mvc::UITool::getAnimateWithName(style);
    int repeatCount = 8 + CCRANDOM_0_1() * 5;
    if(add)
        repeatCount = 2;
    cocos2d::Repeat* repeat = cocos2d::Repeat::create(anim, repeatCount);
    cocos2d::Sequence* seq = cocos2d::Sequence::create(repeat,cocos2d::CallFunc::create(
                                                                                        [this]()
    {
        /******通讯未完成********/
        if(serverCoinType == 0)
        {
            this->showCoinAnimate(COIN_NORMAL_ANIMATION_NAME,true);
        }
        else
        {
            if(serverCoinType == COIN_TYPE::ONE)
            {
                showSecondCoinAnimate(COIN_ONE_ANIMATION_NAME);
            }
            else if(serverCoinType == COIN_TYPE::TWO)
            {
                showSecondCoinAnimate(COIN_TWO_ANIMATION_NAME);
            }
        }
        
    }), NULL);
    turnSpr->runAction(seq);
}

/*
 播放最终效果动画（会停留在后端返回的相应面）
 */
void TravenGuessPanel::showSecondCoinAnimate(std::string style)
{
    int repeatCount = 1 + CCRANDOM_0_1() * 3;
    
    turnSpr->stopAction();
    auto anim = mvc::UITool::getAnimateWithName(style);
    cocos2d::Repeat* repeat = cocos2d::Repeat::create(anim, repeatCount);
    cocos2d::Sequence* seq = cocos2d::Sequence::create(repeat,cocos2d::CallFunc::create(
                                                                                        [this]()
    {
        this->stopRunning();
    }), NULL);
    turnSpr->runAction(seq);

}

/*
 显示英雄信息
 */
void TravenGuessPanel::showHeroInfo()
{
    favorLabel->setText(HeroCommmon::getInstance()->currentSelectPubHeroModel->liking);
    recruitDef = lofd::ConfigManager::getInstance()->pubBeforeConfig->getPubBeforeNodeById(HeroCommmon::getInstance()->currentSelectPubHeroModel->hero_id);
    LOAD_TEXTURE_FROM_PLIST_WITH_ID(recruitHeroImage,recruitDef->all_body);
    
    selfDef = lofd::ConfigManager::getInstance()->pubBeforeConfig->getPubBeforeNodeById(main::MainModel::getInstance()->userInfo->lead_base_id);
    if(selfDef)
        LOAD_TEXTURE_FROM_PLIST_WITH_ID(recruitHeroImage,selfDef->all_body);
    
    showRightDilog("我也不知道该说些什么！");
}

/*
 猜赢了对话
 */
void TravenGuessPanel::winHandler()
{
    showRightDilog(recruitDef->hero_lose_talk,
                   [this]()
                   {
                       showLeftDilog(this->recruitDef->my_win_talk,
                                     [this]()
                                     {
                                         
                                     });
                   });

}

/*
 猜输了对话
 */
void TravenGuessPanel::loseHandler()
{
    showRightDilog(recruitDef->hero_win_talk,
                   [this]()
                   {
                       showLeftDilog(this->recruitDef->my_lose_talk,
                                     [this]()
                                     {
                                         
                                     });
                   });

}

/*
 开始前对话
 */
void TravenGuessPanel::startGetFavor()
{
    showRightDilog(recruitDef->hero_start_talk,
                   [this]()
                   {
                       showLeftDilog(this->recruitDef->my_start_talk,
                                     [this]()
                                     {
                                         showCoinAnimate(COIN_NORMAL_ANIMATION_NAME);
                                     });
                   });
}

/*
 重置标志状态
 */
void TravenGuessPanel::resetStatus()
{
    this->turnSpr->stopAction();
    selectCoinType = COIN_TYPE::ZERO;
    serverCoinType = COIN_TYPE::ZERO;
    autoType = AUTO_TYPE::NORMAL;
    currentResultCount = 0;
    
    if(isWin)
        winHandler();
    else
        loseHandler();
    isWin = false;
    beginSelect = false;
    willLiking = false;
    canBreak = true;
}

/*
 硬币转动到相应面
 */
void TravenGuessPanel::stopRunning()
{
    currentResultCount++;
    if(autoType == AUTO_TYPE::AUTO && currentResultCount < hero::HeroModel::getInstance()->getLikingRet->size())
    {
        this->stopAllActions();
        
        auto delay = cocos2d::DelayTime::create(1);
        auto callFunc = cocos2d::CallFunc::create([this]
                                                  ()
          {
             setServerCoinType(StringToInt(hero::HeroModel::getInstance()->getLikingRet->at(currentResultCount)));
          });
        auto seq = cocos2d::Sequence::create(delay,callFunc, NULL);
        this->runAction(seq);
        return;
    }
    
    resetStatus();
}

TravenGuessMediator::TravenGuessMediator(TravenGuessPanel* instance)
{
    panel = instance;
    registerAttentions(4,WEB_HERO_GET_LIKING,GET_HERO_LIKING_RESPONSE,WEB_HERO_GET_WILL_LIKINT,GET_HERO_WILL_LIKING_RESPONSE);
    ATTENTION_CALLBACK
    {
        if(eventName == WEB_HERO_GET_LIKING)
        {
            DEFINE_COMMAND_PARMA(WEB_HERO_GET_LIKING);
            CREATE_COMMAND_PARMA(WEB_TYPE, panel->selectCoinType);
            CREATE_COMMAND_PARMA(WEB_AUTO, panel->autoType);
            CREATE_COMMAND_PARMA(WEB_HERO_ID, HeroCommmon::getInstance()->currentSelectPubHeroModel->hero_id);
            SEND_COMMAND(HERO_COMMAND);
        }
        else if(eventName == GET_HERO_LIKING_RESPONSE)
        {
            panel->setServerCoinType(StringToInt(hero::HeroModel::getInstance()->getLikingRet->at(0)));
        }
        else if(eventName == WEB_HERO_GET_WILL_LIKINT)
        {
            DEFINE_COMMAND_PARMA(WEB_HERO_GET_WILL_LIKINT);
            CREATE_COMMAND_PARMA(WEB_TYPE, panel->selectCoinType);
            CREATE_COMMAND_PARMA(WEB_HERO_ID, HeroCommmon::getInstance()->currentSelectPubHeroModel->hero_id);
            SEND_COMMAND(HERO_COMMAND);
        }
        else if(eventName == GET_HERO_WILL_LIKING_RESPONSE)
        {
            panel->setServerCoinType(StringToInt(hero::HeroModel::getInstance()->getWillLikingRet->at(0)));
        }
    };

    panel->registerMeditor(this);
    panel->showHeroInfo();
}

bool SelfSprite::init()
{
    if(!Sprite::init())return false;
    _isRunning = false;
//    firstSetFrame = true;
//    curFrameCount = -1;
    RETURN_TRUE;
}

void SelfSprite::setSpriteFrame(cocos2d::SpriteFrame *newFrame)
{
    Sprite::setSpriteFrame(newFrame);
//    curFrameCount++;
//    if(curFrameCount > totalAnimateFrmes)
//        curFrameCount = 1;
//    if(frameCallBack)
//    {
//        frameCallBack(curFrameCount);
//    }
    
}

cocos2d::Action * SelfSprite::runAction(cocos2d::Action* action)
{
    _isRunning = true;
    return Node::runAction(action);
}

void SelfSprite::stopAtFrameIndex(int frameIndex)
{
    stopFrameIndex = frameIndex;
}

void SelfSprite::stopAction()
{
//    if(!_isRunning)return;
    _isRunning = false;
    this->stopAllActions();
    curFrameCount = 0;
}

bool CardDilog::init()
{
    NODE_INIT;
    showWidget(TRAVEN_DILOG_PANEL);
    label = mvc::UITool::getTextFromWidgetWithName(rootWidget, "dilog", "tf");
    this->setAnchorPoint(cocos2d::Point(0.08f,0.1f));
    dilog = mvc::UITool::getImageViewFromeWidget(rootWidget, "dilog");
    scaleDuration = 1.0f;
    callbackFunc = nullptr;
    
    RETURN_TRUE;

}

void CardDilog::show(std::string info,DILOG_DIR _dir)
{
    this->dir = _dir;
    label->setText(info);
    
    float aimScale  = _dir == DILOG_DIR::LEFT?1.0f:-1.0f;
    float initScale = _dir == DILOG_DIR::LEFT?0.1f:-0.1f;
    if(dir == DILOG_DIR::RIGHT)
    {
        dilog->setScaleY(-1.0f);
        label->setScaleX(-1.0f);
        label->setPositionX(dilog->getSize().width - label->getPositionX());
    }
    this->setScale(initScale);
   // float aimScale  =  1.0f;
    //float initScale =  0.1f;

    
    auto scaleTo = cocos2d::ScaleTo::create(scaleDuration,aimScale);
    auto callFunc = cocos2d::CallFunc::create([this]
                                              ()
    {
        if(callbackFunc != nullptr)
        {
            callbackFunc();
        }
    });
    auto seq = cocos2d::Sequence::create(scaleTo,callFunc,NULL);
    this->runAction(seq);
    
}

void CardDilog::hide()
{
    this->stopAllActions();
    
    float aimScale  = dir == DILOG_DIR::LEFT?0.1f:-0.1f;
    //float initScale = dir == DILOG_DIR::LEFT?0.1f:-1.0f;
    //this->setScale(initScale);
    
    auto scaleTo = cocos2d::ScaleTo::create(scaleDuration,aimScale);
    auto callFunc = cocos2d::CallFunc::create([this]
                                              ()
                                              {
                                                  this->removeFromParent();
                                              });
    auto seq = cocos2d::Sequence::create(scaleTo,callFunc,NULL);
    this->runAction(seq);

}
/*********************************guess Panel****************************************************/


/*********************************command server****************************************************/
CREATE_COMMAND_EXECUTE(TravenCommand)
{
    COMMAND_ACTION;
    
}
/*********************************command server****************************************************/
NS_TRAVEN_END