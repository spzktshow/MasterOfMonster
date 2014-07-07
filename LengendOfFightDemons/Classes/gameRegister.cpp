//
//  gameRegister.cpp
//  TestDemo
//
//  Created by tanzuoliang on 14-4-18.
//
//

#include "gameRegister.h"
#include "MVCFrame.h"
#include "createCharacter.h"
#include "LoginModel.h"
#include "heroCommand.h"
#include "registerAndLogin.h"
#include "MainModel.h"
#include "mainCommand.h"
#include "HeroModel.h"
#include "traven.h"

CREATE_SIGNAL_INSTANCE(GameRegister, gameInstance);

GameRegister::GameRegister()
{
    registerModel();
    registerProtocol();
    registerCommand();
    
    registerBaseResource();
}

void GameRegister::registerModel()
{
    
}

void GameRegister::registerProtocol()
{
   // mvc::ProtocolPool::getInstance()->registerProtocol(new Login_doActiveProtocol(),moonsugar::TypeConver::combinationChar(WEB_LOGIN,WEB_DO_ACTIVE));
    REGISTER_PROTOCOL(&Login_doActiveProtocol::createProtocol,WEB_LOGIN,WEB_DO_ACTIVE);
    REGISTER_PROTOCOL(&Login_checkActiveProtocol::createProtocol,WEB_LOGIN,WEB_CHECK_ACTIVE);
    REGISTER_PROTOCOL(&Login_indexProtocol::createProtocol,WEB_LOGIN,WEB_INDEX);
    REGISTER_PROTOCOL(&main::Main_startProtocol::createProtocol,WEB_MAIN,WEB_MAIN_START);
    REGISTER_PROTOCOL(&hero::Hero_heroListProtocol::createProtocol,WEB_HERO,WEB_HERO_LIST);
    REGISTER_PROTOCOL(&hero::Hero_getLikingProtocol::createProtocol,WEB_HERO,WEB_HERO_GET_LIKING);
    REGISTER_PROTOCOL(&hero::Hero_getPubProtocol::createProtocol,WEB_HERO,WEB_HERO_GET_PUB);
    REGISTER_PROTOCOL(&hero::Hero_getWillLikingProtocol::createProtocol,WEB_HERO,WEB_HERO_GET_WILL_LIKINT);
}

void GameRegister::registerCommand()
{
   // mvc::MVCCommandMap::getInstance()->registerCommand(SELECT_CHARCTER_COMMAND,new SelectCharacterCommand());
    REGISTER_COMMAND(SELECT_CHARCTER_COMMAND,SelectCharacterCommand);
    REGISTER_COMMAND(LOING_COMMAND,LoginCommand);
    REGISTER_COMMAND(MAIN_COMMAND,MainCommand);
    REGISTER_COMMAND(HERO_COMMAND,HeroCommand);
    REGISTER_COMMAND(TRAVEN_COMMAND,traven::TravenCommand);
}

void GameRegister::registerBaseResource()
{
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(COMMON_UI);
}