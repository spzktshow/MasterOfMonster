#ifndef __MainModel__
#define __MainModel__
#include "cocos2d.h"
#include "MVCFrame.h"
#include "json/document.h"

namespace main{


class userInfoModel : public mvc::Model
{
public:
	DEFINE_SIGANL_INSTANCE(userInfoModel);
	
	 userInfoModel();
	 ~userInfoModel();
	void  initModel();
private:
public:
	std::string action_time;
	std::string open_spirit;
	std::string is_pay_reward;
	std::string sid;
	std::string prestige;
	std::string user_id;
	std::string user_name;
	std::string register_ip;
	std::string lead_base_id;
	std::string horse;
	std::string lead_level;
	std::string is_pay;
	std::string user_openid;
	std::string league;
	std::string login_time;
	std::string auth_pass;
	std::string vip;
	std::string play_sword;
	std::string sex;
	std::string bag_item_num;
	std::string step_name;
	std::string user_account;
	std::string register_time;
	std::string lead_id;
	std::string level;
	std::string is_battle;
	std::string spirit_id;
	std::string key;
	std::string country;
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
	std::string level;
	std::string skill_id;
};

class leadInfoModel : public mvc::Model
{
public:
	DEFINE_SIGANL_INSTANCE(leadInfoModel);
	
	 leadInfoModel();
	 ~leadInfoModel();
	void  initModel();
private:
public:
	std::string cate;
	std::string is_lead;
	std::string magic_resistance;
	std::string colour;
	std::string user_id;
	std::string item_3;
	std::string item_2;
	std::string magic_penetrate;
	std::string armor;
	std::string exp;
	std::string item_1;
	std::string hero_id;
	std::string sword;
	std::vector<skill_listModel*>* skill_list;
	std::string item_5;
	std::string need_exp;
	std::string hero_name;
	std::string max_hp;
	std::string ad;
	std::string dr;
	std::string type;
	std::string item_6;
	std::string base_id;
	std::string level;
	std::string ap;
	std::string armor_penetrate;
	std::string item_4;
	std::string now_hp;
	std::string cri;
};

class userResModel : public mvc::Model
{
public:
	DEFINE_SIGANL_INSTANCE(userResModel);
	
	 userResModel();
	 ~userResModel();
	void  initModel();
private:
public:
	std::string energy;
	std::string diamond;
	std::string pub_liking;
	std::string battle_soul;
	std::string silver;
	std::string beast_soul;
	std::string pvp_icon;
};

class MainModel : public mvc::Model
{
public:
	DEFINE_SIGANL_INSTANCE(MainModel);
	
	 MainModel();
	 ~MainModel();
	void  initModel();
	void assignComplete(std::string notice);
private:
public:
	bool startInit;
	std::string server_time;
	std::string login_url;
	userResModel* userRes;
	leadInfoModel* leadInfo;
	userInfoModel* userInfo;
};

class Main_startProtocol :public mvc::Protocol
{
public:
	Main_startProtocol();
	~Main_startProtocol();
	CREATE_PROTOCOL(Main_startProtocol);
	 virtual bool init();
	void virtual parser(char* response);
private:
	MainModel* model;
};


}
#endif