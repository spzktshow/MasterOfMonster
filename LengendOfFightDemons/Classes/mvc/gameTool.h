//
//  gameTool.h
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-4-25.
//
//

#ifndef __LengendOfFightDemons__gameTool__
#define __LengendOfFightDemons__gameTool__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace tool{
    
class NumberFormat
{
public:
    static std::string getMoneyFormatWithNumber(int value);
    static const char* getPercentFormatWithNumber(int now,int total,cocos2d::ui::LoadingBar* bar=nullptr);
};

class GameTool
{
public:
   
};


}
#endif /* defined(__LengendOfFightDemons__gameTool__) */
