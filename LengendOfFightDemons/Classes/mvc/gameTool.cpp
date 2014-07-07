//
//  gameTool.cpp
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-4-25.
//
//

#include "gameTool.h"

namespace tool{

    std::string NumberFormat::getMoneyFormatWithNumber(int value)
    {
        if(value < 1000)
            return cocos2d::StringUtils::toString(value);
        std::string format("");
        int formatIndex = 0;
        int count = 0;
        std::string num = cocos2d::StringUtils::toString(value);
        int len = num.length();
        
        for(int i = len - 1;i >=0;i--)
        {
            format += num[i];
            count++;
            if(count == 3 && i > 0)
            {
                format += ',';
                count = 0;
            }
        }
        
        std::string result("");
        len = format.length();
        
        for(int i = len-1;i>=0;i--)
        {
            result += format[i];
        }
        return result;
    }

    //cocos2d::Sprite* BMNum::getBMNumWithStringAndType(std::string num, BMNumType type)
    //{
    //    auto container = cocos2d::Sprite::create();
    //    int count = num.size();
    //
    //    for(int i=0;i<count;i++)
    //    {
    //        char info[30] = {0};
    //        if(type == BMNumType::demage)
    //            sprintf(info, "script_critical_damage_%c.png",num.at(i));
    //        else if(type == BMNumType::addBlood)
    //            sprintf(info, "script_critical_damage_%c.png",num.at(i));
    //        auto num = cocos2d::Sprite::createWithSpriteFrameName(info);
    //        //num->setPositionX(num->getContentSize().width * i);
    //        num->setPositionX(34*i);
    //        container->addChild(num);
    //    }
    //
    //    return container;
    //}

}