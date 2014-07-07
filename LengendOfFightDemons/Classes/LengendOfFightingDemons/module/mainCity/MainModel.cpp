#include "MainModel.h"

namespace main{


CREATE_SIGNAL_INSTANCE(MainModel,MainModelInstance);

MainModel::MainModel()
{
	initModel();
}

MainModel::~MainModel()
{
}

void MainModel::initModel()
{
	userRes = new userResModel();
	leadInfo = new leadInfoModel();
	userInfo = new userInfoModel();
}

void MainModel::assignComplete(std::string notice)
{
	publishNotice(notice);
}

CREATE_SIGNAL_INSTANCE(userInfoModel,userInfoModelInstance);

userInfoModel::userInfoModel()
{
	initModel();
}

userInfoModel::~userInfoModel()
{
}

void userInfoModel::initModel()
{
}

CREATE_SIGNAL_INSTANCE(skill_listModel,skill_listModelInstance);

skill_listModel::skill_listModel()
{
	initModel();
}

skill_listModel::~skill_listModel()
{
}

void skill_listModel::initModel()
{
}

CREATE_SIGNAL_INSTANCE(leadInfoModel,leadInfoModelInstance);

leadInfoModel::leadInfoModel()
{
	initModel();
}

leadInfoModel::~leadInfoModel()
{
}

void leadInfoModel::initModel()
{
	skill_list = new std::vector<skill_listModel*>();
}

CREATE_SIGNAL_INSTANCE(userResModel,userResModelInstance);

userResModel::userResModel()
{
	initModel();
}

userResModel::~userResModel()
{
}

void userResModel::initModel()
{
}

bool Main_startProtocol::init()
{
	if(!mvc::Protocol::init())return false;
	return true;
}

Main_startProtocol::Main_startProtocol()
{
	model = MainModel::getInstance();
}

Main_startProtocol::~Main_startProtocol()
{

}

void Main_startProtocol::parser(char* response)
{

	rapidjson::Document doc;
	doc.Parse<0>(response);
	if(doc.HasParseError())
	{
		CCLOG("GetParserError %s\n",doc.GetParseError());
		return;
	}
	model->server_time = doc["data"]["server_time"].GetDouble();
	GET_STRING_INGORE_TYPE(model->login_url ,doc["data"]["login_url"]);
	GET_STRING_INGORE_TYPE(model->userRes->energy ,doc["data"]["userRes"]["energy"]);
	GET_STRING_INGORE_TYPE(model->userRes->diamond ,doc["data"]["userRes"]["diamond"]);
	GET_STRING_INGORE_TYPE(model->userRes->pub_liking ,doc["data"]["userRes"]["pub_liking"]);
	GET_STRING_INGORE_TYPE(model->userRes->battle_soul ,doc["data"]["userRes"]["battle_soul"]);
	GET_STRING_INGORE_TYPE(model->userRes->silver ,doc["data"]["userRes"]["silver"]);
	GET_STRING_INGORE_TYPE(model->userRes->beast_soul ,doc["data"]["userRes"]["beast_soul"]);
	GET_STRING_INGORE_TYPE(model->userRes->pvp_icon ,doc["data"]["userRes"]["pvp_icon"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->cate ,doc["data"]["leadInfo"]["cate"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->is_lead ,doc["data"]["leadInfo"]["is_lead"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->magic_resistance ,doc["data"]["leadInfo"]["magic_resistance"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->colour ,doc["data"]["leadInfo"]["colour"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->user_id ,doc["data"]["leadInfo"]["user_id"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->item_3 ,doc["data"]["leadInfo"]["item_3"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->item_2 ,doc["data"]["leadInfo"]["item_2"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->magic_penetrate ,doc["data"]["leadInfo"]["magic_penetrate"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->armor ,doc["data"]["leadInfo"]["armor"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->exp ,doc["data"]["leadInfo"]["exp"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->item_1 ,doc["data"]["leadInfo"]["item_1"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->hero_id ,doc["data"]["leadInfo"]["hero_id"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->sword ,doc["data"]["leadInfo"]["sword"]);
	model->leadInfo->skill_list->clear();
	for(int i=0; i<doc["data"]["leadInfo"]["skill_list"].Size();i++)
	{
		skill_listModel* nowModel2 = new skill_listModel();
		model->leadInfo->skill_list->push_back(nowModel2);
		GET_STRING_INGORE_TYPE(nowModel2->level ,doc["data"]["leadInfo"]["skill_list"][i]["level"]);
		GET_STRING_INGORE_TYPE(nowModel2->skill_id ,doc["data"]["leadInfo"]["skill_list"][i]["skill_id"]);
	}


	GET_STRING_INGORE_TYPE(model->leadInfo->item_5 ,doc["data"]["leadInfo"]["item_5"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->need_exp ,doc["data"]["leadInfo"]["need_exp"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->hero_name ,doc["data"]["leadInfo"]["hero_name"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->max_hp ,doc["data"]["leadInfo"]["max_hp"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->ad ,doc["data"]["leadInfo"]["ad"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->dr ,doc["data"]["leadInfo"]["dr"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->type ,doc["data"]["leadInfo"]["type"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->item_6 ,doc["data"]["leadInfo"]["item_6"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->base_id ,doc["data"]["leadInfo"]["base_id"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->level ,doc["data"]["leadInfo"]["level"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->ap ,doc["data"]["leadInfo"]["ap"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->armor_penetrate ,doc["data"]["leadInfo"]["armor_penetrate"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->item_4 ,doc["data"]["leadInfo"]["item_4"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->now_hp ,doc["data"]["leadInfo"]["now_hp"]);
	GET_STRING_INGORE_TYPE(model->leadInfo->cri ,doc["data"]["leadInfo"]["cri"]);
	GET_STRING_INGORE_TYPE(model->userInfo->action_time ,doc["data"]["userInfo"]["action_time"]);
	GET_STRING_INGORE_TYPE(model->userInfo->open_spirit ,doc["data"]["userInfo"]["open_spirit"]);
	GET_STRING_INGORE_TYPE(model->userInfo->is_pay_reward ,doc["data"]["userInfo"]["is_pay_reward"]);
	GET_STRING_INGORE_TYPE(model->userInfo->sid ,doc["data"]["userInfo"]["sid"]);
	GET_STRING_INGORE_TYPE(model->userInfo->prestige ,doc["data"]["userInfo"]["prestige"]);
	GET_STRING_INGORE_TYPE(model->userInfo->user_id ,doc["data"]["userInfo"]["user_id"]);
	GET_STRING_INGORE_TYPE(model->userInfo->user_name ,doc["data"]["userInfo"]["user_name"]);
	GET_STRING_INGORE_TYPE(model->userInfo->register_ip ,doc["data"]["userInfo"]["register_ip"]);
	GET_STRING_INGORE_TYPE(model->userInfo->lead_base_id ,doc["data"]["userInfo"]["lead_base_id"]);
	GET_STRING_INGORE_TYPE(model->userInfo->horse ,doc["data"]["userInfo"]["horse"]);
	GET_STRING_INGORE_TYPE(model->userInfo->lead_level ,doc["data"]["userInfo"]["lead_level"]);
	GET_STRING_INGORE_TYPE(model->userInfo->is_pay ,doc["data"]["userInfo"]["is_pay"]);
	GET_STRING_INGORE_TYPE(model->userInfo->user_openid ,doc["data"]["userInfo"]["user_openid"]);
	GET_STRING_INGORE_TYPE(model->userInfo->league ,doc["data"]["userInfo"]["league"]);
	GET_STRING_INGORE_TYPE(model->userInfo->login_time ,doc["data"]["userInfo"]["login_time"]);
	GET_STRING_INGORE_TYPE(model->userInfo->auth_pass ,doc["data"]["userInfo"]["auth_pass"]);
	GET_STRING_INGORE_TYPE(model->userInfo->vip ,doc["data"]["userInfo"]["vip"]);
	GET_STRING_INGORE_TYPE(model->userInfo->play_sword ,doc["data"]["userInfo"]["play_sword"]);
	GET_STRING_INGORE_TYPE(model->userInfo->sex ,doc["data"]["userInfo"]["sex"]);
	GET_STRING_INGORE_TYPE(model->userInfo->bag_item_num ,doc["data"]["userInfo"]["bag_item_num"]);
	GET_STRING_INGORE_TYPE(model->userInfo->step_name ,doc["data"]["userInfo"]["step_name"]);
	GET_STRING_INGORE_TYPE(model->userInfo->user_account ,doc["data"]["userInfo"]["user_account"]);
	GET_STRING_INGORE_TYPE(model->userInfo->register_time ,doc["data"]["userInfo"]["register_time"]);
	GET_STRING_INGORE_TYPE(model->userInfo->lead_id ,doc["data"]["userInfo"]["lead_id"]);
	GET_STRING_INGORE_TYPE(model->userInfo->level ,doc["data"]["userInfo"]["level"]);
	GET_STRING_INGORE_TYPE(model->userInfo->is_battle ,doc["data"]["userInfo"]["is_battle"]);
	GET_STRING_INGORE_TYPE(model->userInfo->spirit_id ,doc["data"]["userInfo"]["spirit_id"]);
	GET_STRING_INGORE_TYPE(model->userInfo->key ,doc["data"]["userInfo"]["key"]);
	GET_STRING_INGORE_TYPE(model->userInfo->country ,doc["data"]["userInfo"]["country"]);
	model->startInit = true;

	model->assignComplete("Main_startDataInit");
}



}