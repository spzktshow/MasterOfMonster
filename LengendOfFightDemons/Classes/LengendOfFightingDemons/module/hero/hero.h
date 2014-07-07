//
//  hero.h
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-4.
//
//

#ifndef __LengendOfFightDemons__hero__
#define __LengendOfFightDemons__hero__

#include <iostream>
#include "MVCFrame.h"
#include "HeroModel.h"
#include "LOFDPubBeforeConfig.h"

/**********************************   事件名  *********************************************************/
#define HERO_SELECT_EQUIP              "closeheroSelectEquipPanel"
#define SHOW_SELECT_EQUIP              "showSelectEquipPanel"
#define CLOSE_HERO_PROPERTY            "closeHeroProertyPanel"
#define SHOW_PROPERTY                  "showPropertyPanel"
#define CLOSE_ADVANCED                 "closeHeroAdvanedPanel"
#define SHOW_ADVANCE                   "showHeroAdvancePanel"
#define CLOSE_SKILL_PANEL              "closeHeroSkillPanel"
#define SHOW_SKILL_PANEL               "showHeroSkillPanel"
#define CLOSE_WEAR_EQUIP_PANEL         "closeHeroWearEquipPanel"
#define SHOW_WEAR_EQUIP_PANEL          "showHeroWearEquipPanel"
#define CLOSE_LOOK_AT_HERO_PANEL       "closeHeroLookAtHeroPanel"
#define SHOW_LOOK_AT_HERO_PANEL        "showHeroLokkAtHeroPanel"
#define SELECT_HERO_EQUIP_TYPE         "selectHeroEquipType"
#define SELECT_WEAR_EQUIP              "selectWearEquip"
#define HERO_EXCHANGE_WEAR_EQUIP       "heroExchangeWearEquip"
#define HERO_DRESS_OFF_EQUIP           "heroDressOffEquip"

#define HERO_DRESS_EQUIP               "heroDressEquip"
/**********************************   资源名  *********************************************************/
#define HEROUI_ICON_WEAPONS            "heroui_icon_weapons.png"
#define HEROUI_ICON_GLOVES             "heroui_icon_gloves.png"
#define HEROUI_ICON_SHOES              "heroui_icon_shoes.png"
#define HEROUI_ICON_CLOTHES            "heroui_icon_clothes.png"
#define HEROUI_ICON_BELT               "heroui_icon_belt.png"
#define HEROUI_ICON_ORNAMENTS          "heroui_icon_ornaments.png"

typedef enum
{
    WEAPONS,
    GLOVES,
    SHOES,
    CLOTHES,
    BELT,
    ORNAMENTS,
    NONE
}EquipType;

typedef mvc::OrionPanel* (*create_panel)(void);

class HeroEquip;

class HeroPanel:public mvc::OrionPanel
{
public:
    DEFINE_CREATE_SCENE;
    CREATE_FUNC(HeroPanel);
    virtual bool init();
    ~HeroPanel();
    void dressEquip();
    void initHeroInfo();
private:
    cocos2d::ui::Button* advancedBtn;
    cocos2d::ui::Button* propertyBtn;
    cocos2d::ui::Button* skillBtn;
    cocos2d::ui::Button* addGoldBtn;
    
    cocos2d::ui::ImageView* weaponIcon;
    cocos2d::ui::ImageView* glovesIcon;
    cocos2d::ui::ImageView* shoesIcon;
    cocos2d::ui::ImageView* clothesIcon;
    cocos2d::ui::ImageView* beltIcon;
    cocos2d::ui::ImageView* omamentsIcon;
    
    cocos2d::ui::ImageView* heroIcon;
    
    std::unordered_map<int,HeroEquip*> equipMap;
//    std::map<std::string,create_panel> createPanelMap;
    cocos2d::ui::LoadingBar* expBar;
    cocos2d::ui::LoadingBar* goldBar;
    cocos2d::ui::TextBMFont* expTf;
    cocos2d::ui::TextBMFont* goldTf;
    cocos2d::ui::TextBMFont* fightValueTf;
    BMFONT levelTf;

};

class HeroPanelMeditor:public mvc::Mediator
{
public:
    HeroPanelMeditor(HeroPanel* panel);
private:
    HeroPanel* heroPanel;
};

class HeroEquip
{
public:
    HeroEquip(cocos2d::ui::ImageView* instance);
    ~HeroEquip(){equipFrame = nullptr;}
    void showEquip(std::string fileName="dressOff");
private:
    cocos2d::ui::ImageView* equipFrame;
    cocos2d::Sprite* equipIcon;
    std::string _fileName;
};

/***********************************  选择装备面板  *******************************************/
class HeroSelectEquip:public mvc::OrionPanel
{
public:
    CREATE_FUNC(HeroSelectEquip);
    virtual void onExit();
    virtual bool init();
    void showSelectEquipType(std::string type);
    void showEquipList();
private:
    cocos2d::ui::Button* equipBtn;
    cocos2d::ui::ImageView* equipFrame;
    cocos2d::ui::ListView* equipList;
    cocos2d::Sprite* currentEquip;
    mvc::ItemGroup* group;
    bool showUnDressTip;
    bool isDressNewEquip();
};

class HeroSelectEquipMeditor:public mvc::Mediator
{
public:
    HeroSelectEquipMeditor(HeroSelectEquip* panel);
private:
    HeroSelectEquip* selectPanel;
};

class HeroEquipItemRender:public mvc::BaseItemRender
{
public:
    CREATE_FUNC(HeroEquipItemRender);
    virtual bool init();
    virtual void select(bool);
private:
    cocos2d::ui::ImageView* icon;
    cocos2d::Sprite* selctFrame;
    
    void addSelctFrame();
    void removeSelctFrame();
};

/***********************************  属性面板  *******************************************/
class HeroPropertyPanel:public mvc::OrionPanel
{
public:
    CREATE_FUNC(HeroPropertyPanel);
    virtual bool init();
private:
    cocos2d::ui::ListView* list;
};

class HerpPropertyPanelMediator:public mvc::Mediator
{
public:
    HerpPropertyPanelMediator(HeroPropertyPanel* instance);
private:
    HeroPropertyPanel* panel;
};

class HeroPropertyItemRender:public mvc::BaseItemRender
{
public:
    CREATE_FUNC(HeroPropertyItemRender);
    virtual bool init();
private:
    cocos2d::ui::ImageView* icon;
    cocos2d::ui::TextBMFont* nameLabel;
    cocos2d::ui::Text* valueLabel;
};

/***********************************  进阶面板  *******************************************/
class HeroAdvancePanel:public mvc::OrionPanel
{
public:
    CREATE_FUNC(HeroAdvancePanel);
    virtual bool init();
private:
    cocos2d::ui::ImageView* nowIcon;
    cocos2d::ui::ImageView* afterIcon;
};

class HeroAdvanceMediator:public mvc::Mediator
{
public:
    HeroAdvanceMediator(HeroAdvancePanel* instance);
private:
    HeroAdvancePanel* panel;
};

/***********************************  英雄技能面板  *******************************************/
class HeroSkillPanel:public mvc::OrionPanel
{
public:
    CREATE_FUNC(HeroSkillPanel);
    virtual bool init();
    void showSkillList();
    ~HeroSkillPanel();
private:
    cocos2d::ui::ListView* list;
    DEFINE_ITME_GROUP;
};

class HeroSkillMediator:public mvc::Mediator
{
public:
    HeroSkillMediator(HeroSkillPanel* instance);
private:
    HeroSkillPanel* panel;
};

class HeroSkillItemRender:public mvc::BaseItemRender
{
public:
    CREATE_FUNC(HeroSkillItemRender);
    virtual bool init();
    DEFINE_ITEMRENDER;
    DEFINE_SET_ITEM_MODEL(hero::skill_listModel);
private:
    cocos2d::ui::ImageView* icon;
    cocos2d::ui::TextBMFont* nameBMF;
    cocos2d::ui::TextBMFont* levelBMF;
    cocos2d::ui::Text* needSoul;
    cocos2d::ui::Text* needSliver;
    
    cocos2d::ui::Button* addBtn;
};
/***********************************  装备穿戴查看面板  *******************************************/

class HeroWearEquipPanel:public mvc::OrionPanel
{
public:
    CREATE_FUNC(HeroWearEquipPanel);
    virtual bool init();
    virtual void onExit();
private:
    cocos2d::ui::ImageView* equipFrame;
    cocos2d::Sprite* equipIcon;
    cocos2d::ui::TextBMFont* nameBMF;
    cocos2d::ui::TextBMFont* descBMF;
    cocos2d::ui::TextBMFont* descRequireBMF;
    cocos2d::ui::Button* exchangeBtn;
    cocos2d::ui::Button* dressOffBtn;
    cocos2d::ui::ListView* descList;
};

class HeroWearEquipMediator:public mvc::Mediator
{
public:
    HeroWearEquipMediator(HeroWearEquipPanel* instance);
private:
    HeroWearEquipPanel* panel;
};

/***********************************  英雄一览面板  *******************************************/
class HeroLookAtPanel:public mvc::OrionPanel
{
public:
    DEFINE_CREATE_SCENE;
    CREATE_FUNC(HeroLookAtPanel);
    virtual bool init();
    ~HeroLookAtPanel();
    void showHero();
private:
    cocos2d::ui::ListView* list;
    DEFINE_ITME_GROUP;
};

class HeroLookAtMediator:public mvc::Mediator
{
public:
    HeroLookAtMediator(HeroLookAtPanel* instance);
private:
    HeroLookAtPanel* panel;
};

class HeroLookAtItemRender:public mvc::BaseItemRender
{
public:
    DEFINE_CREATE_FUNC_WITH_INIT(HeroLookAtItemRender);
    DEFINE_ITEMRENDER;
    
    DEFINE_SET_ITEM_MODEL(hero::hero_listModel);
private:
    cocos2d::ui::LoadingBar* expBar;
    cocos2d::ui::ImageView* heroIcon;
    IMAGE_VIEW qualityIcon;
    cocos2d::ui::TextBMFont* nameBMF;
    BMFONT powerTf;
    BMFONT quickTf;
    BMFONT intelTf;
    BMFONT phyTf;
    
    std::string getHeroQualityFileNameWithID(int);
};

class HeroCommmon:public mvc::Model
{
public:
    HeroCommmon();
    ~HeroCommmon();
    DEFINE_SIGANL_INSTANCE(HeroCommmon);
    
    /**********  选择的装备类型 ************/
    void selectEquipType(EquipType type);
    EquipType getSelectEquipType();
    
     /**********  选择的装备资源名 ************/
    std::string getSelectEquipTypeFileName();
    
public:
    hero::hero_listModel* currentSelectHero;
    hero::pubInfoModel*  currentSelectPubHeroModel;
    std::string selectEquip;
    //int select_traven_hero_id;
private:
    EquipType equipType;
    
     /**********  不同类型装备资源默认资源名 ************/
    std::map<EquipType,std::string> wearResourceMap;
};
#endif /* defined(__LengendOfFightDemons__hero__) */
