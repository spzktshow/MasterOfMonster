#ifndef __LengendOfFightDemons__LOFDhero_before__#define __LengendOfFightDemons__LOFDhero_before__#include "lengendOfFightDemons.h"#include "external/json/rapidjson.h"#include "external/json/document.h"#include "editor-support/cocostudio/DictionaryHelper.h"NS_LOFD_BEGINclass HeroBeforeDef{public:	int half_body;	int move_speed;	int arms;	std::string hero_name;	int colour;	int up_int;	int head_icon;	std::string intro;	int need_item;	int next_id;	int all_body;	int base_cri;	int behavior_id;	int last_id;	int up_agi;	int ai_id;	int job_id;	std::string intro_feature;	std::string job_name;	int sex;	int type;	int base_dr;	int armor_type;	int atk_speed;	int hero_id;	int up_str;	int scout_range;	int normal_hurt_type;	int cate;	int camp_id;	int need_res;	//std::string skill_id;    std::vector<int> skillIds;	int atk_range;};class HeroBeforeConfig{public:	HeroBeforeDef* getHeroBeforeDefById(int);	std::vector<HeroBeforeDef*> defVec;	 void parse(std::string);    bool hasParse;};NS_LOFD_END#endif