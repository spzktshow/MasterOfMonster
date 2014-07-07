#include "LOFDPubBeforeConfig.h"NS_LOFD_BEGINPubBeforeNode* PubBeforeConfig::getPubBeforeNodeById(std::string id){	int len = defVec.size();	int checkID;	checkID = std::atoi(id.c_str());	for(int i=0;i<len;i++)	{		PubBeforeNode* def = defVec.at(i);		if(def->hero_id == checkID)		return def;	}	return nullptr;}void PubBeforeConfig::parse(std::string data){	rapidjson::Document doc;	doc.Parse<0>(data.c_str());	if(doc.HasParseError())	{		cocos2d::log("%s"," PubBeforeNode error");	}	else	{		for(int i = 0; i < cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(doc,"datas"); i++)		{			PubBeforeNode* def1 = new PubBeforeNode();			defVec.push_back(def1);			const rapidjson::Value& PubBeforeNodeValue = cocostudio::DictionaryHelper::getInstance()->getSubDictionary_json(doc,"datas" ,i);			def1->hero_name =  cocostudio::DictionaryHelper::getInstance()->getStringValue_json(PubBeforeNodeValue,"hero_name");			def1->current_favour =  cocostudio::DictionaryHelper::getInstance()->getIntValue_json(PubBeforeNodeValue,"current_favour");			def1->hero_card =  cocostudio::DictionaryHelper::getInstance()->getIntValue_json(PubBeforeNodeValue,"hero_card");			def1->skill_icon2 =  cocostudio::DictionaryHelper::getInstance()->getIntValue_json(PubBeforeNodeValue,"skill_icon2");			def1->my_guess_five =  cocostudio::DictionaryHelper::getInstance()->getStringValue_json(PubBeforeNodeValue,"my_guess_five");			def1->hero_id =  cocostudio::DictionaryHelper::getInstance()->getIntValue_json(PubBeforeNodeValue,"hero_id");			def1->hero_openui_talk =  cocostudio::DictionaryHelper::getInstance()->getStringValue_json(PubBeforeNodeValue,"hero_openui_talk");			def1->intro_featrue =  cocostudio::DictionaryHelper::getInstance()->getStringValue_json(PubBeforeNodeValue,"intro_featrue");			def1->half_body =  cocostudio::DictionaryHelper::getInstance()->getIntValue_json(PubBeforeNodeValue,"half_body");			def1->hero_bg =  cocostudio::DictionaryHelper::getInstance()->getIntValue_json(PubBeforeNodeValue,"hero_bg");			def1->job_name =  cocostudio::DictionaryHelper::getInstance()->getStringValue_json(PubBeforeNodeValue,"job_name");			def1->hero_start_talk =  cocostudio::DictionaryHelper::getInstance()->getStringValue_json(PubBeforeNodeValue,"hero_start_talk");			def1->all_body =  cocostudio::DictionaryHelper::getInstance()->getIntValue_json(PubBeforeNodeValue,"all_body");			def1->my_win_talk =  cocostudio::DictionaryHelper::getInstance()->getStringValue_json(PubBeforeNodeValue,"my_win_talk");			def1->hero_lose_talk =  cocostudio::DictionaryHelper::getInstance()->getStringValue_json(PubBeforeNodeValue,"hero_lose_talk");			def1->cost_silver =  cocostudio::DictionaryHelper::getInstance()->getIntValue_json(PubBeforeNodeValue,"cost_silver");			def1->hero_win_talk =  cocostudio::DictionaryHelper::getInstance()->getStringValue_json(PubBeforeNodeValue,"hero_win_talk");			def1->need_favour =  cocostudio::DictionaryHelper::getInstance()->getIntValue_json(PubBeforeNodeValue,"need_favour");			def1->my_lose_talk =  cocostudio::DictionaryHelper::getInstance()->getStringValue_json(PubBeforeNodeValue,"my_lose_talk");			def1->skill_icon3 =  cocostudio::DictionaryHelper::getInstance()->getIntValue_json(PubBeforeNodeValue,"skill_icon3");			def1->skill_icon1 =  cocostudio::DictionaryHelper::getInstance()->getIntValue_json(PubBeforeNodeValue,"skill_icon1");			def1->hero_guess_five =  cocostudio::DictionaryHelper::getInstance()->getStringValue_json(PubBeforeNodeValue,"hero_guess_five");		}	}}NS_LOFD_END