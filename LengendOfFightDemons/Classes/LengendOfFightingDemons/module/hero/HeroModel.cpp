#include "HeroModel.h"

namespace hero{


CREATE_SIGNAL_INSTANCE(HeroModel,HeroModelInstance);

HeroModel::HeroModel()
{
	initModel();
}

HeroModel::~HeroModel()
{
}

void HeroModel::initModel()
{
	getWillLikingRet = new std::vector<std::string>();
	getLikingRet = new std::vector<std::string>();
	hero_list = new std::vector<hero_listModel*>();
	pubInfo = new std::vector<pubInfoModel*>();
}

void HeroModel::assignComplete(std::string notice)
{
	publishNotice(notice);
}

CREATE_SIGNAL_INSTANCE(pubInfoModel,pubInfoModelInstance);

pubInfoModel::pubInfoModel()
{
	initModel();
}

pubInfoModel::~pubInfoModel()
{
}

void pubInfoModel::initModel()
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

CREATE_SIGNAL_INSTANCE(hero_listModel,hero_listModelInstance);

hero_listModel::hero_listModel()
{
	initModel();
}

hero_listModel::~hero_listModel()
{
}

void hero_listModel::initModel()
{
	skill_list = new std::vector<skill_listModel*>();
}

bool Hero_getWillLikingProtocol::init()
{
	if(!mvc::Protocol::init())return false;
	return true;
}

Hero_getWillLikingProtocol::Hero_getWillLikingProtocol()
{
	model = HeroModel::getInstance();
}

Hero_getWillLikingProtocol::~Hero_getWillLikingProtocol()
{

}

void Hero_getWillLikingProtocol::parser(char* response)
{

	rapidjson::Document doc;
	doc.Parse<0>(response);
	if(doc.HasParseError())
	{
		CCLOG("GetParserError %s\n",doc.GetParseError());
		return;
	}
	model->getWillLikingRet->clear();
	for(int i=0; i<doc["data"]["ret"].Size();i++)
	{
		PUSH_ELEMENT_INGORE_TYPE(model->getWillLikingRet, doc["data"]["ret"][i]);
	}


	model->getWillLikingInit = true;

	model->assignComplete("Hero_getWillLikingDataInit");
}

bool Hero_getLikingProtocol::init()
{
	if(!mvc::Protocol::init())return false;
	return true;
}

Hero_getLikingProtocol::Hero_getLikingProtocol()
{
	model = HeroModel::getInstance();
}

Hero_getLikingProtocol::~Hero_getLikingProtocol()
{

}

void Hero_getLikingProtocol::parser(char* response)
{

	rapidjson::Document doc;
	doc.Parse<0>(response);
	if(doc.HasParseError())
	{
		CCLOG("GetParserError %s\n",doc.GetParseError());
		return;
	}
	model->getLikingRet->clear();
	for(int i=0; i<doc["data"]["ret"].Size();i++)
	{
		PUSH_ELEMENT_INGORE_TYPE(model->getLikingRet, doc["data"]["ret"][i]);
	}


	model->getLikingInit = true;

	model->assignComplete("Hero_getLikingDataInit");
}

bool Hero_heroListProtocol::init()
{
	if(!mvc::Protocol::init())return false;
	return true;
}

Hero_heroListProtocol::Hero_heroListProtocol()
{
	model = HeroModel::getInstance();
}

Hero_heroListProtocol::~Hero_heroListProtocol()
{

}

void Hero_heroListProtocol::parser(char* response)
{

	rapidjson::Document doc;
	doc.Parse<0>(response);
	if(doc.HasParseError())
	{
		CCLOG("GetParserError %s\n",doc.GetParseError());
		return;
	}
	model->hero_list->clear();
	for(int i=0; i<doc["data"]["hero_list"].Size();i++)
	{
		hero_listModel* nowModel2 = new hero_listModel();
		model->hero_list->push_back(nowModel2);
		GET_STRING_INGORE_TYPE(nowModel2->ad ,doc["data"]["hero_list"][i]["ad"]);
		GET_STRING_INGORE_TYPE(nowModel2->hero_id ,doc["data"]["hero_list"][i]["hero_id"]);
		GET_STRING_INGORE_TYPE(nowModel2->now_hp ,doc["data"]["hero_list"][i]["now_hp"]);
		GET_STRING_INGORE_TYPE(nowModel2->colour ,doc["data"]["hero_list"][i]["colour"]);
		GET_STRING_INGORE_TYPE(nowModel2->item_3 ,doc["data"]["hero_list"][i]["item_3"]);
		GET_STRING_INGORE_TYPE(nowModel2->item_1 ,doc["data"]["hero_list"][i]["item_1"]);
		GET_STRING_INGORE_TYPE(nowModel2->is_lead ,doc["data"]["hero_list"][i]["is_lead"]);
		GET_STRING_INGORE_TYPE(nowModel2->exp ,doc["data"]["hero_list"][i]["exp"]);
		GET_STRING_INGORE_TYPE(nowModel2->dr ,doc["data"]["hero_list"][i]["dr"]);
		GET_STRING_INGORE_TYPE(nowModel2->magic_resistance ,doc["data"]["hero_list"][i]["magic_resistance"]);
		GET_STRING_INGORE_TYPE(nowModel2->item_4 ,doc["data"]["hero_list"][i]["item_4"]);
		GET_STRING_INGORE_TYPE(nowModel2->max_hp ,doc["data"]["hero_list"][i]["max_hp"]);
		GET_STRING_INGORE_TYPE(nowModel2->item_5 ,doc["data"]["hero_list"][i]["item_5"]);
		GET_STRING_INGORE_TYPE(nowModel2->need_exp ,doc["data"]["hero_list"][i]["need_exp"]);
		GET_STRING_INGORE_TYPE(nowModel2->cate ,doc["data"]["hero_list"][i]["cate"]);
		GET_STRING_INGORE_TYPE(nowModel2->ap ,doc["data"]["hero_list"][i]["ap"]);
		GET_STRING_INGORE_TYPE(nowModel2->base_id ,doc["data"]["hero_list"][i]["base_id"]);
		GET_STRING_INGORE_TYPE(nowModel2->magic_penetrate ,doc["data"]["hero_list"][i]["magic_penetrate"]);
		GET_STRING_INGORE_TYPE(nowModel2->armor ,doc["data"]["hero_list"][i]["armor"]);
		GET_STRING_INGORE_TYPE(nowModel2->type ,doc["data"]["hero_list"][i]["type"]);
		GET_STRING_INGORE_TYPE(nowModel2->level ,doc["data"]["hero_list"][i]["level"]);
		GET_STRING_INGORE_TYPE(nowModel2->cri ,doc["data"]["hero_list"][i]["cri"]);
		GET_STRING_INGORE_TYPE(nowModel2->armor_penetrate ,doc["data"]["hero_list"][i]["armor_penetrate"]);
		GET_STRING_INGORE_TYPE(nowModel2->sword ,doc["data"]["hero_list"][i]["sword"]);
		nowModel2->skill_list->clear();
		for(int j=0; j<doc["data"]["hero_list"][i]["skill_list"].Size();j++)
		{
			skill_listModel* nowModel3 = new skill_listModel();
			nowModel2->skill_list->push_back(nowModel3);
			GET_STRING_INGORE_TYPE(nowModel3->skill_id ,doc["data"]["hero_list"][i]["skill_list"][j]["skill_id"]);
			GET_STRING_INGORE_TYPE(nowModel3->level ,doc["data"]["hero_list"][i]["skill_list"][j]["level"]);
		}


		GET_STRING_INGORE_TYPE(nowModel2->hero_name ,doc["data"]["hero_list"][i]["hero_name"]);
		GET_STRING_INGORE_TYPE(nowModel2->user_id ,doc["data"]["hero_list"][i]["user_id"]);
		GET_STRING_INGORE_TYPE(nowModel2->item_6 ,doc["data"]["hero_list"][i]["item_6"]);
		GET_STRING_INGORE_TYPE(nowModel2->item_2 ,doc["data"]["hero_list"][i]["item_2"]);
	}

	model->heroListInit = true;

	model->assignComplete("Hero_heroListDataInit");
}

bool Hero_getPubProtocol::init()
{
	if(!mvc::Protocol::init())return false;
	return true;
}

Hero_getPubProtocol::Hero_getPubProtocol()
{
	model = HeroModel::getInstance();
}

Hero_getPubProtocol::~Hero_getPubProtocol()
{

}

void Hero_getPubProtocol::parser(char* response)
{

	rapidjson::Document doc;
	doc.Parse<0>(response);
	if(doc.HasParseError())
	{
		CCLOG("GetParserError %s\n",doc.GetParseError());
		return;
	}
	model->pubInfo->clear();
	for(int i=0; i<doc["data"]["pubInfo"].Size();i++)
	{
		pubInfoModel* nowModel2 = new pubInfoModel();
		model->pubInfo->push_back(nowModel2);
		GET_STRING_INGORE_TYPE(nowModel2->hero_id ,doc["data"]["pubInfo"][i]["hero_id"]);
		GET_STRING_INGORE_TYPE(nowModel2->liking ,doc["data"]["pubInfo"][i]["liking"]);
		GET_STRING_INGORE_TYPE(nowModel2->state ,doc["data"]["pubInfo"][i]["state"]);
	}


	model->getPubInit = true;

	model->assignComplete("Hero_getPubDataInit");
}



}