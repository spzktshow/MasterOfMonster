//
//  registerAndLogin.h
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-13.
//
//

#ifndef __LengendOfFightDemons__registerAndLogin__
#define __LengendOfFightDemons__registerAndLogin__

#define LOGIN_GAME                      "loginGame"

#include <iostream>
#include "MVCFrame.h"

#define LOGIN_RESPONSE              "Login_indexDataInit"
#define LOGIN_CHECK_ACTIVE_RESPONSE "Login_checkActiveDataInit"

#define AUTO_LOGIN_GAME             "autoEnterGame"
#define REMEBER_GAME_PASSWORD       "remeberGamePassworld"
#define PLAYER_NAME                 "playerName"
#define PLAYER_PASSWORD             "playerPassworld"


class LoginPanel:public mvc::OrionPanel
{
public:
    DEFINE_CREATE_SCENE;
    CREATE_FUNC(LoginPanel);
    virtual bool init();
    void initPanel();
    
    cocos2d::ui::TextField* nameTf;
    cocos2d::ui::TextField* passwordTf;
private:
    cocos2d::ui::Button* loginBtn;
    cocos2d::ui::CheckBox* autoLoginCheckBox;
    cocos2d::ui::CheckBox* passwordCheckBox;
    cocos2d::ui::Button* nameBtn;
    cocos2d::ui::Layout* commonLayout;
    int moveOffset=70;
    bool attackIME;
    bool isAutoLogin;
    bool isRemeberPassword;
private:
    void onTextFiledHandler(cocos2d::Ref* ref,cocos2d::ui::TextFiledEventType type);
    void onCheckBoxHandler(cocos2d::Ref* ref,cocos2d::ui::CheckBoxEventType type);
};

class LoginMediator:public mvc::Mediator
{
public:
    LoginMediator(LoginPanel* instance);
private:
    LoginPanel* panel;
};

class LoginServer:public mvc::BaseServer
{
public:
    CREATE_FUNC(LoginServer);
    void login(std::string name,std::string password,std::string model);
    void checkActive(std::string token);
};

class LoginCommand:public mvc::MVCCommand
{
public:
    CREATE_COMMAND(LoginCommand);
    DEFINE_COMMAND_EXECUTE;
};

#endif /* defined(__LengendOfFightDemons__registerAndLogin__) */
