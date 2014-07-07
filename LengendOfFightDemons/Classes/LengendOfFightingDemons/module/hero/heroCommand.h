//
//  heroCommand.h
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-13.
//
//

#ifndef __LengendOfFightDemons__heroCommand__
#define __LengendOfFightDemons__heroCommand__

#include <iostream>
#include "MVCFrame.h"
#include "hero.h"

class HeroServer:public mvc::BaseServer
{
public:
    CREATE_FUNC(HeroServer);
    HeroServer();
    void getHerolist();
    /*
     @param int $hero_id    招募英雄ID
     @param int $type      押注类型  传递1或者2
     @param int $auto      是否自动  默认为0不连猜 1为连猜5次
     return {"controllerName":"Hero","actionName":"getLiking","state":1,"data":{"ret":[2],"_print_content":""}}
     */
    void getLiking(int hero_id,int type,int autoType);
    
    /*
     亲密度必中玩法
     @param int $heroId     招募英雄ID
     @param int $type      押注类型  传递1或者2
     return {"controllerName":"Hero","actionName":"getWillLiking","state":1,"data":{"ret":[2],"_print_content":""}}
     */
    void getWillLiking(int hero_id,int type);
    
    /*
     招募英雄
     @param int $hero_id     招募英雄ID
     return {"controllerName":"Hero","actionName":"recruitHero","state":1,"data":{"_print_content":""}}
     */
    void recruitHero(int hero_id);
    
    /*
     return {"controllerName":"Hero","actionName":"getPub","state":1,
     "data":{"pubInfo":[  //英雄ID      亲密度              状态  是否拥有
     {"hero_id":710010,"liking":"1000","state":1},
     {"hero_id":710011,"liking":"1000","state":0},
     {"hero_id":710012,"liking":"1000","state":0}
     ],"_print_content":""}};
     */
    void getPub();
    
    
};

class HeroCommand:public mvc::MVCCommand
{
public:
    DEFINE_COMMAND_EXECUTE;
    CREATE_COMMAND(HeroCommand);
private:
};

#endif /* defined(__LengendOfFightDemons__heroCommand__) */
