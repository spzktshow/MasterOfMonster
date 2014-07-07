//
//  heroCommand.cpp
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-13.
//
//

#include "heroCommand.h"
#include "HeroModel.h"

HeroServer::HeroServer()
{
    service->registerHttpCallBack(WEB_HERO, WEB_HERO_RECRUIT,[]
                                  (const rapidjson::Value& value)
    {
        hero::hero_listModel* nowModel2 = new hero::hero_listModel();
        hero::HeroModel::getInstance()->hero_list->push_back(nowModel2);
        
        nowModel2->magic_resistance = value["magic_resistance"].GetString();
		nowModel2->item_1 = value["item_1"].GetString();
		nowModel2->item_6 = value["item_6"].GetString();
		nowModel2->base_id = value["base_id"].GetString();
		nowModel2->armor = value["armor"].GetString();
		nowModel2->sword = value["sword"].GetString();
		nowModel2->max_hp = value["max_hp"].GetString();
		nowModel2->need_exp = value["need_exp"].GetString();
		nowModel2->colour = value["colour"].GetString();
		nowModel2->exp = value["exp"].GetString();
		nowModel2->cri = value["cri"].GetString();
		nowModel2->item_3 = value["item_3"].GetString();
		nowModel2->hero_name = value["hero_name"].GetString();
		nowModel2->ap = value["ap"].GetString();
		nowModel2->armor_penetrate = value["armor_penetrate"].GetString();
		nowModel2->skill_list->clear();
		for(int j=0; j<value["skill_list"].Size();j++)
		{
            hero::skill_listModel* nowModel3 = new hero::skill_listModel();
			nowModel2->skill_list->push_back(nowModel3);
			nowModel3->level = value["skill_list"][j]["level"].GetInt();
			nowModel3->skill_id = value["skill_list"][j]["skill_id"].GetString();
		}
        
        
		nowModel2->item_4 = value["item_4"].GetString();
		nowModel2->dr = value["dr"].GetString();
		nowModel2->is_lead = value["is_lead"].GetString();
		nowModel2->type = value["type"].GetString();
		nowModel2->ad = value["ad"].GetString();
		nowModel2->cate = value["cate"].GetString();
		nowModel2->item_5 = value["item_5"].GetString();
		nowModel2->hero_id = value["hero_id"].GetString();
		nowModel2->magic_penetrate = value["magic_penetrate"].GetString();
		nowModel2->item_2 = value["item_2"].GetString();
		nowModel2->level = value["level"].GetString();
		nowModel2->now_hp = value["now_hp"].GetString();
		nowModel2->user_id = value["user_id"].GetString();
    });
}

void HeroServer::getHerolist()
{
    START_SEVER_INFO(WEB_HERO, WEB_HERO_LIST);
    END_SERVER_INFO;
}

void HeroServer::getPub()
{
    START_SEVER_INFO(WEB_HERO, WEB_HERO_GET_PUB);
    END_SERVER_INFO;

}

void HeroServer::getLiking(int hero_id,int type, int autoType)
{
    START_SEVER_INFO(WEB_HERO, WEB_HERO_GET_LIKING);
    write.String(WEB_HERO_ID);
    write.Int(hero_id);
    write.String(WEB_TYPE);
    write.Int(type);
    write.String(WEB_AUTO);
    write.Int(autoType);
    END_SERVER_INFO;
}

void HeroServer::getWillLiking(int hero_id,int type)
{
    START_SEVER_INFO(WEB_HERO, WEB_HERO_GET_WILL_LIKINT);
    write.String(WEB_HERO_ID);
    write.Int(hero_id);
    write.String(WEB_TYPE);
    write.Int(type);
    END_SERVER_INFO;

}

void HeroServer::recruitHero(int hero_id)
{
    START_SEVER_INFO(WEB_HERO, WEB_HERO_RECRUIT);
    write.String(WEB_HERO_ID);
    write.Int(hero_id);
    END_SERVER_INFO;
}

CREATE_COMMAND_EXECUTE(HeroCommand)
{
    HeroServer* server = HeroServer::create();
    
    COMMAND_ACTION
    if (action == HERO_DRESS_EQUIP)
    {
        
    }
    else if(action == WEB_HERO_LIST)
    {
        server->getHerolist();
    }
    else if(action == WEB_HERO_GET_LIKING)
    {
        server->getLiking(HAS_INT(WEB_HERO_ID),HAS_INT(WEB_TYPE), HAS_INT(WEB_AUTO));
    }
    else if(action == WEB_HERO_GET_WILL_LIKINT)
    {
        server->getWillLiking(HAS_INT(WEB_HERO_ID), HAS_INT(WEB_TYPE));
    }
    else if(action == WEB_HERO_RECRUIT)
    {
        server->recruitHero(HAS_INT(WEB_HERO_ID));
    }
    else if(action == WEB_HERO_GET_PUB)
    {
        server->getPub();
    }

}