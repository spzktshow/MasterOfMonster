//
//  gameRegister.h
//  TestDemo
//
//  Created by tanzuoliang on 14-4-18.
//
//

#ifndef __TestDemo__gameRegister__
#define __TestDemo__gameRegister__

#include <iostream>

class GameRegister
{
public:
    GameRegister();
    static GameRegister* getInstance();
private:
    void registerModel();
    void registerProtocol();
    void registerCommand();
    void registerBaseResource();
};

#endif /* defined(__TestDemo__gameRegister__) */
