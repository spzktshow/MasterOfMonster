//
//  mainCommand.h
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-14.
//
//

#ifndef __LengendOfFightDemons__mainCommand__
#define __LengendOfFightDemons__mainCommand__

#define MAIN_START_RESPONSE        "Main_startDataInit"

#include <iostream>
#include "MVCFrame.h"

class MainServer:public mvc::BaseServer
{
public:
    CREATE_FUNC(MainServer);
    void start();
};

class MainCommand :public mvc::MVCCommand
{
public:
    CREATE_COMMAND(MainCommand);
    DEFINE_COMMAND_EXECUTE;
};
#endif /* defined(__LengendOfFightDemons__mainCommand__) */
