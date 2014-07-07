//
//  registerAndLogin.cpp
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-13.
//
//

#include "registerAndLogin.h"
#include "LoginModel.h"
#include "createCharacter.h"
#include "commonUI.h"

CREATE_SCENE(LoginPanel);
//cocos2d::Scene* LoginPanel::createScene()
//{
//    auto scene = cocos2d::Scene::create();
//    scene->addChild(LoginPanel::create());
//    return scene;
//}

bool LoginPanel::init()
{
    if(!Layer::init())return false;
    
    CHILD_CLICK_CALLBACK
    {
        IF_CHILD_TOUCH_ENDED
        {
            if(ref == loginBtn)
            {
                if(isRemeberPassword)
                {
                    LOCAL_SAVE->setStringForKey(PLAYER_NAME, nameTf->getStringValue());
                    LOCAL_SAVE->setStringForKey(PLAYER_PASSWORD, passwordTf->getStringValue());
                    CCLOG("save name is %s",nameTf->getStringValue().c_str());
                    CCLOG("save password is %s",passwordTf->getStringValue().c_str());
                    
                }
                publishEvent(LOGIN_GAME);
            }
        }
    };
    
    //this->setAnchorPoint(cocos2d::Point(0.0f,0.0f));
    
    new LoginMediator(this);
    //loading::PreLoader::getInstance()->loadLoginPanel([this](){initPanel();});
    initPanel();
    return true;
        
}

void LoginPanel::initPanel()
{
    showWidget(CREATE_REGISTER_PLAYER,false,false,true);
    
    nameBtn = getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "nameBtn");
    loginBtn = getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "loginBtn");
    autoLoginCheckBox = mvc::UITool::getCheckBoxFromWidgetWithName(rootWidget, COMMON_LAYOUT, "autoLoginCheckBox");
    autoLoginCheckBox->addEventListenerCheckBox(this, cocos2d::ui::SEL_SelectedStateEvent(&LoginPanel::onCheckBoxHandler));
    
    passwordCheckBox = mvc::UITool::getCheckBoxFromWidgetWithName(rootWidget, COMMON_LAYOUT, "passwordCheckBox");
    passwordCheckBox->addEventListenerCheckBox(this, cocos2d::ui::SEL_SelectedStateEvent(&LoginPanel::onCheckBoxHandler));
    
    nameTf = mvc::UITool::getTextFieldFromWidgetWithName(rootWidget, COMMON_LAYOUT, "nameTf");
    nameTf->addEventListenerTextField(this, cocos2d::ui::SEL_TextFieldEvent(&LoginPanel::onTextFiledHandler));
    
    passwordTf = mvc::UITool::getTextFieldFromWidgetWithName(rootWidget, COMMON_LAYOUT, "passwordTf");
    passwordTf->addEventListenerTextField(this, cocos2d::ui::SEL_TextFieldEvent(&LoginPanel::onTextFiledHandler));
    
    commonLayout = mvc::UITool::getLayoutFromWidget(rootWidget, COMMON_LAYOUT);
    
    isAutoLogin = LOCAL_SAVE->getBoolForKey(AUTO_LOGIN_GAME);
    isRemeberPassword = LOCAL_SAVE->getBoolForKey(REMEBER_GAME_PASSWORD);
    
    if(isRemeberPassword)
    {
        nameTf->setText(LOCAL_SAVE->getStringForKey(PLAYER_NAME));
        passwordTf->setText(LOCAL_SAVE->getStringForKey(PLAYER_PASSWORD));
        CCLOG("save name is %s",LOCAL_SAVE->getStringForKey(PLAYER_NAME).c_str());
        CCLOG("save password is %s",LOCAL_SAVE->getStringForKey(PLAYER_PASSWORD).c_str());
    }
    
    if(isAutoLogin)
    {
        //publishEvent(LOGIN_GAME);
    }
    
    autoLoginCheckBox->setSelectedState(isAutoLogin);
    passwordCheckBox->setSelectedState(isRemeberPassword);

//    BaseUIFrame* frame = BaseUIFrame::create();
//    frame->setFrameSize(800, 600);
//    this->addChild(frame);
//    frame->setPosition(cocos2d::Point(200,100));
    
//    auto ball = cocos2d::Sprite::create("jump.png");
//    auto node = cocos2d::Node::create();
//    node->setScale(4.0f);
//    node->addChild(ball);
//    node->setAnchorPoint(cocos2d::Point(0.0f,0.0f));
//    node->setPosition(300, 300);
//    this->addChild(node);
//    
//    node->runAction(cocos2d::ScaleTo::create(5, 0.2));

}

void LoginPanel::onTextFiledHandler(cocos2d::Ref* ref,cocos2d::ui::TextFiledEventType type)
{
    if(type == cocos2d::ui::TextFiledEventType::TEXTFIELD_EVENT_ATTACH_WITH_IME)
    {
        if(!attackIME)
        {
            attackIME = true;
            auto move = cocos2d::MoveBy::create(0.1, cocos2d::Point(0,moveOffset));
            commonLayout->runAction(move);
        }
    }
    else if(type == cocos2d::ui::TextFiledEventType::TEXTFIELD_EVENT_DETACH_WITH_IME)
    {
        if(attackIME)
        {
            attackIME = false;
            auto move = cocos2d::MoveBy::create(0.1, cocos2d::Point(0,-moveOffset));
            commonLayout->runAction(move);

        }
    }
    
}

void LoginPanel::onCheckBoxHandler(cocos2d::Ref* ref,cocos2d::ui::CheckBoxEventType type)
{
    if(ref == autoLoginCheckBox)
    {
        isAutoLogin = autoLoginCheckBox->getSelectedState();
        LOCAL_SAVE->setBoolForKey(AUTO_LOGIN_GAME, isAutoLogin);
    }
    else if(ref == passwordCheckBox)
    {
        isRemeberPassword = passwordCheckBox->getSelectedState();
        LOCAL_SAVE->setBoolForKey(REMEBER_GAME_PASSWORD, isRemeberPassword);
    }
}

LoginMediator::LoginMediator(LoginPanel* instance)
{
    panel = instance;
    registerAttentions(3,LOGIN_GAME,LOGIN_RESPONSE,LOGIN_CHECK_ACTIVE_RESPONSE);
    ATTENTION_CALLBACK
    {
        if(eventName == LOGIN_GAME)
        {
            DEFINE_COMMAND_PARMA(LOGIN_GAME);
            CREATE_COMMAND_PARMA(GAME_ACCOUNT,panel->nameTf->getStringValue());
            CREATE_COMMAND_PARMA(GAME_PASSWORD,panel->passwordTf->getStringValue());
            int modelKey = 10000 + CCRANDOM_0_1()* 10000000;
            CREATE_COMMAND_PARMA(PHONE_MODEL,cocos2d::StringUtils::toString(modelKey));
            //CREATE_COMMAND_PARMA(PHONE_MODEL,cocos2d::FileUtils::getInstance()->getIMEI(),param);
            
            SEND_COMMAND(LOING_COMMAND);
        }
        else if(eventName == LOGIN_RESPONSE)
        {
            if(LoginModel::getInstance()->ret->state == 1)
            {
                DEFINE_COMMAND_PARMA(WEB_CHECK_ACTIVE);
                CREATE_COMMAND_PARMA(HTTP_ACTION, WEB_CHECK_ACTIVE);
                CREATE_COMMAND_PARMA(GAME_TOKEN,LoginModel::getInstance()->ret->token);
                SEND_COMMAND(LOING_COMMAND);
            }
            else
            {
                loading::Alert::getInstance()->showNotice("该用户已经登录");
            }
        }
        else if(eventName == LOGIN_CHECK_ACTIVE_RESPONSE)
        {
            loading::PreLoader::getInstance()->unLoadLoginPanel();
            //loading::PreLoader::getInstance()->loadTraven();return;
           
            if(LoginModel::getInstance()->is_actived == 0)
            {
                //REPLACE_SCENE(SelectCharacterPanel);
                loading::PreLoader::getInstance()->loadCreateCharcater();
            }
            else
            {
                loading::PreLoader::getInstance()->loadMainCity();
            }
        }
            
    };
    
    panel->registerMeditor(this);
    
}

CREATE_COMMAND_EXECUTE(LoginCommand)
{
    LoginServer* server = LoginServer::create();
    
    COMMAND_ACTION;
    if(action == LOGIN_GAME)
    {
        server->login(HAS_STRING(GAME_ACCOUNT), HAS_STRING(GAME_PASSWORD), HAS_STRING(PHONE_MODEL));
    }
    else if(action == WEB_CHECK_ACTIVE)
    {
        server->checkActive(HAS_STRING(GAME_TOKEN));
    }
};


void LoginServer::login(std::string name,std::string password,std::string model)
{
    START_SEVER_INFO(WEB_LOGIN, WEB_INDEX);
    write.String(GAME_ACCOUNT);
    write.String(name.c_str());
    write.String(GAME_PASSWORD);
    write.String(password.c_str());
    write.String(PHONE_MODEL);
    write.String(model.c_str());
    END_SERVER_INFO;
}

void LoginServer::checkActive(std::string token)
{
    START_SEVER_INFO(WEB_LOGIN, WEB_CHECK_ACTIVE);
    write.String(GAME_TOKEN);
    write.String(token.c_str());
    END_SERVER_INFO;
}