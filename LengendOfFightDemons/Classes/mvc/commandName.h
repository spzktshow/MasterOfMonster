//
//  commandName.h
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-13.
//
//

#ifndef LengendOfFightDemons_commandName_h
#define LengendOfFightDemons_commandName_h

#define DEFINE_COMMAND_EXECUTE              virtual void execute(cocos2d::ValueMap* map)
#define CREATE_COMMAND_EXECUTE(__TYPE__)\
void __TYPE__::execute(cocos2d::ValueMap* map)

#define COMMAND_ACTION                      std::string action = map->at(HTTP_ACTION).asString();

#define HAS_INT(T)                          map->at(T).asInt()
#define HAS_STRING(T)                       map->at(T).asString()
#define HAS_BOOL(T)                         map->at(T).asBool()
#define HAS_FLOAT(T)                        map->at(T).asFloat()
#define HAS_DOUBLE(T)                       map->at(T).asDouble()
#define HAS_BYTE(T)                         map->at(T).asByte()
#define HAS_VALUEVECTOR(T)                  map->at(T).asValueVector()
#define HAS_VALUEMAP(T)                     map->at(T).asValueMap()
#define GAME_ACCOUNT                        "acc"
#define GAME_PASSWORD                       "pwd"
#define PHONE_MODEL                         "model"
#define GAME_TOKEN                          "token" 
/****************************************** COMMAND ******************************************************/
#define SELECT_CHARCTER_COMMAND              "selectCharacterCommand"
#define HERO_COMMAND                         "heroCommand" 
#define LOING_COMMAND                        "loginCommand"
#define MAIN_COMMAND                         "mainCommand"
#define TRAVEN_COMMAND                       "travenCommand" 
/********************************************COMMAND SOMETHING****************************************************/
#define HTTP_ACTION                          "action"

/********************************************HTTP RESPONSE****************************************************/

/********************************************酒馆模块****************************************************/
#define TRAVEN_GET_FAVOR                     "travenGetFavor" 
/********************************************酒馆模块****************************************************/
#endif
