#ifndef __HeroModel__
#define __HeroModel__
#include "cocos2d.h"
#include "MVCFrame.h"
#include "json/document.h"

namespace hero{


class pubInfoModel : public mvc::Model
{
public:
	DEFINE_SIGANL_INSTANCE(pubInfoModel);
	
	 pubInfoModel();
	 ~pubInfoModel();
	void  initModel();
private:
public:
	std::string hero_id;
	std::string liking;
	std::string state;
};

class skill_listModel : public mvc::Model
{
public:
	DEFINE_SIGANL_INSTANCE(skill_listModel);
	
	 skill_listModel();
	 ~skill_listModel();
	void  initModel();
private:
public:
	std::string skill_id;
	std::string level;
};

class hero_listModel : public mvc::Model
{
public:
	DEFINE_SIGANL_INSTANCE(hero_listModel);
	
	 hero_listModel();
	 ~hero_listModel();
	void  initModel();
private:
public:
	std::string ad;
	std::string hero_id;
	std::string now_hp;
	std::string colour;
	std::string item_3;
	std::string item_1;
	std::string is_lead;
	std::string exp;
	std::string dr;
	std::string magic_resistance;
	std::string item_4;
	std::string max_hp;
	std::string item_5;
	std::string need_exp;
	std::string cate;
	std::string ap;
	std::string base_id;
	std::string magic_penetrate;
	std::string armor;
	std::string type;
	std::string level;
	std::string cri;
	std::string armor_penetrate;
	std::string sword;
	std::vector<skill_listModel*>* skill_list;
	std::string hero_name;
	std::string user_id;
	std::string item_6;
	std::string item_2;
};

class HeroModel : public mvc::Model
{
public:
	DEFINE_SIGANL_INSTANCE(HeroModel);
	
	 HeroModel();
	 ~HeroModel();
	void  initModel();
	void assignComplete(std::string notice);
private:
public:
	bool getWillLikingInit;
	std::vector<std::string>* getWillLikingRet;
	bool getLikingInit;
	std::vector<std::string>* getLikingRet;
	bool heroListInit;
	std::vector<hero_listModel*>* hero_list;
	bool getPubInit;
	std::vector<pubInfoModel*>* pubInfo;
};

class Hero_getWillLikingProtocol :public mvc::Protocol
{
public:
	Hero_getWillLikingProtocol();
	~Hero_getWillLikingProtocol();
	CREATE_PROTOCOL(Hero_getWillLikingProtocol);
	 virtual bool init();
	void virtual parser(char* response);
private:
	HeroModel* model;
};
class Hero_getLikingProtocol :public mvc::Protocol
{
public:
	Hero_getLikingProtocol();
	~Hero_getLikingProtocol();
	CREATE_PROTOCOL(Hero_getLikingProtocol);
	 virtual bool init();
	void virtual parser(char* response);
private:
	HeroModel* model;
};
class Hero_heroListProtocol :public mvc::Protocol
{
public:
	Hero_heroListProtocol();
	~Hero_heroListProtocol();
	CREATE_PROTOCOL(Hero_heroListProtocol);
	 virtual bool init();
	void virtual parser(char* response);
private:
	HeroModel* model;
};
class Hero_getPubProtocol :public mvc::Protocol
{
public:
	Hero_getPubProtocol();
	~Hero_getPubProtocol();
	CREATE_PROTOCOL(Hero_getPubProtocol);
	 virtual bool init();
	void virtual parser(char* response);
private:
	HeroModel* model;
};


}
#endif