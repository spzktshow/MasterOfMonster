//
//  mainCommand.cpp
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-14.
//
//

#include "mainCommand.h"
#include "LoginModel.h"

void MainServer::start()
{
    START_SEVER_INFO(WEB_MAIN, WEB_MAIN_START);
    END_SERVER_INFO;
}

CREATE_COMMAND_EXECUTE(MainCommand)
{
    MainServer* server = MainServer::create();
    
    COMMAND_ACTION;
    if(action == WEB_MAIN_START)
    {
        server->start();
    }
    else
    {
        
    }
}