//
//  hero.cpp
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-4.
//
//

#include "hero.h"
#include "mainCity.h"
/****************************************************   英雄面板   *********************************************/
CREATE_SCENE(HeroPanel);
bool HeroPanel::init()
{
    if(!Layer::init())return false;
    
    showWidget(HERO_MAIN_PANEL,true,true,true);
    
   // closeBtn = getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "closeBtn");
    advancedBtn = getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "advancedBtn");
    propertyBtn = getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "propertyBtn");
    skillBtn = getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "skillBtn");
    addGoldBtn = getButtonFromeWidgetWithWidgetName(rootWidget, HERO_LAYOUT, "addGoldBtn");
    
    auto heroLayout = rootWidget->getChildByName(HERO_LAYOUT);
    
    weaponIcon = getImageViewFromeWidget(heroLayout, "weaponsIcon",true);
    glovesIcon = getImageViewFromeWidget(heroLayout, "glovesIcon",true);
    shoesIcon = getImageViewFromeWidget(heroLayout, "shoesIcon",true);
    clothesIcon = getImageViewFromeWidget(heroLayout, "clothesIcon",true);
    beltIcon = getImageViewFromeWidget(heroLayout, "beltIcon",true);
    omamentsIcon = getImageViewFromeWidget(heroLayout, "ormamentsIcon",true);
    heroIcon = getImageViewFromeWidget(heroLayout, "heroIcon");
    
    expBar = getLoadingBarFromWidgetWithName(rootWidget, HERO_LAYOUT, "expBar");
    expTf = getTextBMFFromWidgetWithName(rootWidget, NUM_LAYOUT, "levelTf");
   
    goldBar = getLoadingBarFromWidgetWithName(rootWidget, HERO_LAYOUT, "goldBar");
    goldTf = getTextBMFFromWidgetWithName(rootWidget, NUM_LAYOUT, "goldTf");
    
    fightValueTf = getTextBMFFromWidgetWithName(rootWidget, NUM_LAYOUT, "fightValueTf");
//    cocos2d::ui::TextBMFont* levelBM = dynamic_cast<cocos2d::ui::TextBMFont*>(rootWidget->getChildByName("bmfLayout")->getChildByName("levelBMF"));
//    levelBM->setText(NumberFormat::getPercentFormatWithNumber(33,456));
    levelTf = getTextBMFFromWidgetWithName(rootWidget, BMF_LAYOUT, "levelBMF");
    
    equipMap.insert(std::make_pair(EquipType::WEAPONS,new HeroEquip(weaponIcon)));
    equipMap.insert(std::make_pair(EquipType::GLOVES,new HeroEquip(glovesIcon)));
    equipMap.insert(std::make_pair(EquipType::SHOES,new HeroEquip(shoesIcon)));
    equipMap.insert(std::make_pair(EquipType::CLOTHES,new HeroEquip(clothesIcon)));
    equipMap.insert(std::make_pair(EquipType::BELT,new HeroEquip(beltIcon)));
    equipMap.insert(std::make_pair(EquipType::ORNAMENTS,new HeroEquip(omamentsIcon)));
    
    closeEventTage = REMOVE_WONDOW_PANEL;
    
    CHILD_CLICK_CALLBACK
    {
        if(type == cocos2d::ui::TouchEventType::TOUCH_EVENT_ENDED)
        {
            if(ref == advancedBtn)
            {
                publishEvent(SHOW_ADVANCE);
            }
            else if(ref == propertyBtn)
            {
                publishEvent(SHOW_PROPERTY);
            }
            else if(ref == skillBtn)
            {
                publishEvent(SHOW_SKILL_PANEL);
            }
            else if(ref == addGoldBtn)
            {
                
            }
            else if(ref == weaponIcon)
            {
                HeroCommmon::getInstance()->selectEquipType(EquipType::WEAPONS);
            }
            else if(ref == glovesIcon)
            {
                HeroCommmon::getInstance()->selectEquipType(EquipType::GLOVES);
            }
            else if(ref == shoesIcon)
            {
                HeroCommmon::getInstance()->selectEquipType(EquipType::SHOES);
            }
            else if(ref == clothesIcon)
            {
                HeroCommmon::getInstance()->selectEquipType(EquipType::CLOTHES);
            }
            else if(ref == beltIcon)
            {
                HeroCommmon::getInstance()->selectEquipType(EquipType::BELT);
            }
            else if(ref == omamentsIcon)
            {
                HeroCommmon::getInstance()->selectEquipType(EquipType::ORNAMENTS);
            }

        }
    };
 
    new HeroPanelMeditor(this);
    addResourceBar();
    return true;
}

void HeroPanel::initHeroInfo()
{
    lofd::ActorDef* actorDef = CURRENT_SELECT_HERO_DEF;
    std::string bodyName = IntToString(actorDef->all_body);
    bodyName += ".png";
    heroIcon->loadTexture(bodyName.c_str(),cocos2d::ui::TextureResType::UI_TEX_TYPE_PLIST);
    
    levelTf->setText(HeroCommmon::getInstance()->currentSelectHero->level.c_str());
    fightValueTf->setText(HeroCommmon::getInstance()->currentSelectHero->sword.c_str());
    SET_PROGRESS_BAR_WITH_TEXT(expBar,expTf,StringToInt(HeroCommmon::getInstance()->currentSelectHero->exp),StringToInt(HeroCommmon::getInstance()->currentSelectHero->need_exp));
    SET_PROGRESS_BAR_WITH_TEXT(goldBar,goldTf,0,actorDef->need_item);
}

void HeroPanel::dressEquip()
{
    HeroEquip* equip = equipMap.at(HeroCommmon::getInstance()->getSelectEquipType());
    if(equip)
        equip->showEquip(HeroCommmon::getInstance()->selectEquip);
}

HeroPanel::~HeroPanel()
{
    std::unordered_map<int,HeroEquip*>::iterator it = equipMap.begin();
    while(it != equipMap.end())
    {
        delete it->second;
        it++;
    }
    
    equipMap.clear();
}

HeroPanelMeditor::HeroPanelMeditor(HeroPanel* panel)
{
    heroPanel = panel;
    registerAttentions(11,HERO_SELECT_EQUIP,SHOW_SELECT_EQUIP,CLOSE_HERO_PROPERTY,SHOW_PROPERTY,SHOW_ADVANCE,CLOSE_ADVANCED,SHOW_SKILL_PANEL,CLOSE_SKILL_PANEL,SHOW_WEAR_EQUIP_PANEL,CLOSE_WEAR_EQUIP_PANEL,HERO_DRESS_EQUIP);
    ATTENTION_CALLBACK
    {
        if(eventName == HERO_SELECT_EQUIP || eventName == CLOSE_HERO_PROPERTY || eventName == CLOSE_ADVANCED || eventName == CLOSE_SKILL_PANEL || eventName == CLOSE_WEAR_EQUIP_PANEL)//关闭面板
        {
            lastSubPanelTag = "null";
            heroPanel->moveChildToCenter();
        }
        else if(eventName == SHOW_SELECT_EQUIP)
        {
            if(std::strcmp(eventName.c_str(), lastSubPanelTag.c_str()) != 0)
            {
                lastSubPanelTag = eventName;
                heroPanel->addSubPanelToRight(HeroSelectEquip::create());

            }
        }
        else if(eventName == SHOW_PROPERTY)
        {
            if(std::strcmp(eventName.c_str(), lastSubPanelTag.c_str()) != 0)
            {
                lastSubPanelTag = eventName;
                heroPanel->addSubPanelToRight(HeroPropertyPanel::create());
                
            }
        }
        else if(eventName == SHOW_ADVANCE)
        {
            if(std::strcmp(eventName.c_str(), lastSubPanelTag.c_str()) != 0)
            {
                lastSubPanelTag = eventName;
                heroPanel->addSubPanelToRight(HeroAdvancePanel::create());
                
            }
        }
        else if(eventName == SHOW_SKILL_PANEL)
        {
            if(std::strcmp(eventName.c_str(), lastSubPanelTag.c_str()) != 0)
            {
                lastSubPanelTag = eventName;
                heroPanel->addSubPanelToRight(HeroSkillPanel::create());
                
            }
        }
        else if(eventName == SHOW_WEAR_EQUIP_PANEL)
        {
            if(std::strcmp(eventName.c_str(), lastSubPanelTag.c_str()) != 0)
            {
                lastSubPanelTag = eventName;
                heroPanel->addSubPanelToRight(HeroWearEquipPanel::create());
                
            }
        }
        else if(eventName == HERO_DRESS_EQUIP)
        {
            heroPanel->dressEquip();
        }

    };
    
    heroPanel->initHeroInfo();
    heroPanel->registerMeditor(this);
    
    
};

HeroEquip::HeroEquip(cocos2d::ui::ImageView* instance)
{
    equipFrame = instance;
    equipIcon = nullptr;
}

void HeroEquip::showEquip(std::string fileName)
{ 
    if(_fileName == fileName)return;
    _fileName = fileName;
    if(equipIcon)
    {
        equipIcon->removeFromParent();
        equipIcon = nullptr;
    }
    if(fileName == "dressOff")return;
    equipIcon = cocos2d::Sprite::createWithSpriteFrameName(fileName);
    equipFrame->addChild(equipIcon);
    CCLOG("dress equip success!");
}
/****************************************************   英雄面板   *********************************************/



/****************************************************   英雄选择装备面板   *********************************************/
void HeroSelectEquip::onExit()
{
    OrionPanel::onExit();
    HeroCommmon::getInstance()->selectEquipType(EquipType::NONE);
    delete group;
}

bool HeroSelectEquip::isDressNewEquip()
{
    return true;
}

bool HeroSelectEquip::init()
{
    if(!Layer::init())return false;
    
    showWidget(HERO_SELECT_EQUIP_PANEL,false,true,true);
    //closeBtn = getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "closeBtn");
    equipBtn = getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "equipBtn");
    //this->setContentSize(rootWidget->getContentSize());
    equipFrame = getImageViewFromeWidgetWithName(rootWidget,COMMON_LAYOUT, "currentEquipIcon");
    equipList = getListViewFromeWidgetWithWidgetName(rootWidget, HERO_LAYOUT, "list");
    equipList->setLayoutLineCout(3);
    //equipList->cocos2d::ui::ScrollView::setDirection(cocos2d::ui::SCROLLVIEW_DIR::SCROLLVIEW_DIR_VERTICAL);
    equipList->setLayoutType(cocos2d::ui::LayoutType::LAYOUT_LINEAR_VERTICAL_MUTIL_LINE);
   // rootWidget->setAnchorPoint(cocos2d::Point(0.5f,0.5f));
    //addChildToCenter(rootWidget);
    closeEventTage = HERO_SELECT_EQUIP;
  
    showUnDressTip = true;
    
    CHILD_CLICK_CALLBACK
    {
        if(type == cocos2d::ui::TouchEventType::TOUCH_EVENT_ENDED)
        {
            if(ref == equipBtn)
            {
                if(isDressNewEquip())
                {
                    if(showUnDressTip)
                    {
                        if(HeroCommmon::getInstance()->selectEquip != "")
                        {
                            publishEvent(HERO_DRESS_EQUIP);
                        }
                        else
                        {
                            loading::Alert::getInstance()->showNotice("请先选择一个装备！");
                        }
                    }
                    else
                    {
                        publishEvent(HERO_DRESS_EQUIP);
                        equipBtn->setTouchEnabled(false);
                        
                    }

                }
                else
                {
                    loading::Alert::getInstance()->showNotice("该装备已经穿戴！");
                }
                //closePanel();
            }
        }
    };
    
    group = new mvc::ItemGroup();
    group->itemClickCallback = [this](mvc::BaseItemRender* item)
    {
        HeroCommmon::getInstance()->selectEquip = "HAui_arrow.png";
        showSelectEquipType("HAui_arrow.png");
        equipBtn->setTouchEnabled(true);
    };
    
    new HeroSelectEquipMeditor(this);
    return true;
}

void HeroSelectEquip::showEquipList()
{
    group->selectItem(nullptr);
    if(showUnDressTip)
    {
        HeroCommmon::getInstance()->selectEquip = "";
    }
    else
    {
        equipBtn->setTouchEnabled(false);
    }
    equipList->removeAllItems();
    equipList->scrollToTop(0.1f, false);
    int len = CEIL(35,3);
    for(int i=0;i<len;i++)
    {
        auto item = HeroEquipItemRender::create();
        item->setGroup(group);
        equipList->pushBackCustomItem(item);
    }
}

void HeroSelectEquip::showSelectEquipType(std::string type)
{
    if(currentEquip)
    {
        currentEquip->removeFromParent();
    }
    
    currentEquip = cocos2d::Sprite::createWithSpriteFrameName(type);
    //currentEquip->setContentSize(equipFrame->getSize());
    currentEquip->setScale(equipFrame->getSize().width / currentEquip->getContentSize().width);
    //CCLOG("WIDTH = %f height = %f",equipFrame->getSize().width,equipFrame->getSize().height);
    equipFrame->addChild(currentEquip);
}

HeroSelectEquipMeditor::HeroSelectEquipMeditor(HeroSelectEquip* panel)
{
    selectPanel = panel;
    registerAttentions(1,SELECT_HERO_EQUIP_TYPE);
    ATTENTION_CALLBACK
    {
        if(eventName == SELECT_HERO_EQUIP_TYPE)
        {
            selectPanel->showSelectEquipType(HeroCommmon::getInstance()->getSelectEquipTypeFileName());
            selectPanel->showEquipList();
        }
    };
    
    selectPanel->showSelectEquipType(HeroCommmon::getInstance()->getSelectEquipTypeFileName());
    selectPanel->showEquipList();
    selectPanel->registerMeditor(this);
}

bool HeroEquipItemRender::init()
{
    if(!Node::init())return false;
    showWidget(HERO_EQUIP_ITEMRENDER);
    icon = mvc::UITool::getImageViewFromeWidget(rootWidget, "ImageView_836");
    registerEvent(icon);
    CHILD_CLICK_CALLBACK
    {
        if(type == cocos2d::ui::TouchEventType::TOUCH_EVENT_ENDED)
        {
            if(ref == icon)
            {
                group->selectItem(this);
            }
        }
    };
    
    return true;
}

void HeroEquipItemRender::select(bool sel)
{
    if(sel)
    {
        addSelctFrame();
    }
    else
    {
        removeSelctFrame();
    }
}

void HeroEquipItemRender::addSelctFrame()
{
    removeSelctFrame();
    
    selctFrame = cocos2d::Sprite::createWithSpriteFrameName(ITEM_SELECT_FRAME);
    icon->addChild(selctFrame);
    //selctFrame->setScale(<#float scale#>)
}

void HeroEquipItemRender::removeSelctFrame()
{
    if(selctFrame && selctFrame->getParent())
        selctFrame->removeFromParent();
}
/****************************************************   英雄选择装备面板   *********************************************/


/****************************************************   英雄属性面板   *********************************************/
bool HeroPropertyPanel::init()
{
    if(!Layer::init())return false;
    
    showWidget(HERO_PROPERTY_PANEL,false,true,true);
   // this->setContentSize(rootWidget->getContentSize());
    //closeBtn = getButtonFromeWidgetWithWidgetName(rootWidget,COMMON_LAYOUT,"closeBtn");
    list = getListViewFromeWidgetWithWidgetName(rootWidget, HERO_LAYOUT, "list");
    //list->setLayoutLineCout(2);
    list->cocos2d::ui::ScrollView::setDirection(cocos2d::ui::SCROLLVIEW_DIR::SCROLLVIEW_DIR_VERTICAL);
    list->setLayoutType(cocos2d::ui::LayoutType::LAYOUT_LINEAR_VERTICAL);
    closeEventTage = CLOSE_HERO_PROPERTY;
    
    CHILD_CLICK_CALLBACK
    {
        if(type == cocos2d::ui::TouchEventType::TOUCH_EVENT_ENDED)
        {
            
        }
    };
    
    for(int i=0;i < 10;i++)
    {
        list->pushBackCustomItem(HeroPropertyItemRender::create());
    }
    
    new HerpPropertyPanelMediator(this);
    return true;
}

HerpPropertyPanelMediator::HerpPropertyPanelMediator(HeroPropertyPanel* instance)
{
    panel = instance;
    
    ATTENTION_CALLBACK
    {
        
    };
    
    panel->registerMeditor(this);
}

bool HeroPropertyItemRender::init()
{
    if(!Node::init())return false;
    
    showWidget(HERO_PROPERTY_ITEMRENDER);
    //auto rootWidget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("hero_introduceItemRender.ExportJson");
    //auto rootWidget = mvc::UITool::widgetFromJson("hero_introduceItemRender.ExportJson");
    icon = mvc::UITool::getImageViewFromeWidget(rootWidget, "icon");
    nameLabel =  mvc::UITool::getTextBMFFromWidget(rootWidget,"nameLabel");

    valueLabel = mvc::UITool::getTextFromWidget(rootWidget,"value");
    return true;
}
/****************************************************   英雄属性面板   *********************************************/


/****************************************************   英雄进阶面板   *********************************************/
bool HeroAdvancePanel::init()
{
    if(!Layer::init())return false;
    
    showWidget(HERO_ADVANCE_PANEL,false,true,true);
    //closeBtn = getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "closeBtn");
    
    nowIcon = getImageViewFromeWidgetWithName(rootWidget, COMMON_LAYOUT, "nowIcon");
    afterIcon = getImageViewFromeWidgetWithName(rootWidget, COMMON_LAYOUT, "afterIcon");
    
    closeEventTage = CLOSE_ADVANCED;
    CHILD_CLICK_CALLBACK
    {
        if(type == cocos2d::ui::TouchEventType::TOUCH_EVENT_ENDED)
        {
            
        }
    };
    
    new HeroAdvanceMediator(this);
    return true;
}

HeroAdvanceMediator::HeroAdvanceMediator(HeroAdvancePanel* instance)
{
    panel = instance;
    
    ATTENTION_CALLBACK
    {
        
    };
    panel->registerMeditor(this);
}
/****************************************************   英雄进阶面板   *********************************************/


/****************************************************   英雄技能面板   *********************************************/
HeroSkillPanel::~HeroSkillPanel()
{
    RELEASE_ITEM_GROUP;
}

bool HeroSkillPanel::init()
{
    if(!Layer::init())return false;
    
    showWidget(HERO_SKILL_PANEL,false,true,true);
    list = getListViewFromeWidgetWithWidgetName(rootWidget, HERO_LAYOUT, "list");
    
    closeEventTage = CLOSE_SKILL_PANEL;
    CHILD_CLICK_CALLBACK
    {
        if(type == cocos2d::ui::TouchEventType::TOUCH_EVENT_ENDED)
        {
            
        }
    };
    
    CREATE_ITEM_GROUP
    {
        
    };
    
    new HeroSkillMediator(this);
    return true;
}

void HeroSkillPanel::showSkillList()
{
    CREATE_ITEM_WITH_GROUP_AND_LIST(HeroSkillItemRender, list, HeroCommmon::getInstance()->currentSelectHero->skill_list);
}

HeroSkillMediator::HeroSkillMediator(HeroSkillPanel* instance)
{
    panel = instance;
    ATTENTION_CALLBACK
    {
        
    };
    
    lofd::ConfigManager::getInstance()->createSkillBeforeConfig();
    
    panel->registerMeditor(this);
    panel->showSkillList();
}


CREATE_ITEM_RENDER_FUNC_WITH_INIT(HeroSkillItemRender,
{
    showWidget(HERO_SKILL_ITEMRENDER);
    
    nameBMF = mvc::UITool::getTextBMFFromWidgetWithName(rootWidget, BMF_LAYOUT, "nameBMF");
    levelBMF = mvc::UITool::getTextBMFFromWidgetWithName(rootWidget, BMF_LAYOUT, "levelBMF");
    needSliver = mvc::UITool::getTextFromWidgetWithName(rootWidget, NUM_LAYOUT, "needSliver");
    needSoul = mvc::UITool::getTextFromWidgetWithName(rootWidget, NUM_LAYOUT, "needSoul");
    addBtn = mvc::UITool::getButtonFromeWidgetWithWidgetName(rootWidget, HERO_LAYOUT, "addBtn");
    icon = mvc::UITool::getImageViewFromeWidgetWithName(rootWidget, HERO_LAYOUT, "icon");
    
    REGISTER_ITEM_EVENT(addBtn);
})

ITEMRENDER_SELECT(HeroSkillItemRender)
{
    
}

void HeroSkillItemRender::setItemModel(hero::skill_listModel *instance)
{
    SET_MODEL;
    lofd::SkillBeforeDef* def = SKILL_DEF(model->skill_id);
    nameBMF->setText(def->skill_name.c_str());
    levelBMF->setText(IntToString(model->level).c_str());
    SET_TEXT_INFO_WITH_INT(needSliver, def->up_need_silver);
    SET_TEXT_INFO_WITH_INT(needSoul, def->up_need_soul);
}
/****************************************************   英雄技能面板   *********************************************/


/****************************************************   装备穿戴查看面板   *********************************************/
bool HeroWearEquipPanel::init()
{
    if(!Layer::init())return false;
    showWidget(HERO_WEAR_EQUIP_PANEL,false,true,true);
    
    equipFrame = getImageViewFromeWidgetWithName(rootWidget,HERO_LAYOUT,"equipIcon");
    nameBMF = getTextBMFFromWidgetWithName(rootWidget,BMF_LAYOUT,"nameBMF");
    descBMF = getTextBMFFromWidgetWithName(rootWidget,BMF_LAYOUT,"descBMF");
    descRequireBMF = getTextBMFFromWidgetWithName(rootWidget,BMF_LAYOUT,"descRequireBMF");
    exchangeBtn = getButtonFromeWidgetWithWidgetName(rootWidget,COMMON_LAYOUT,"exchangeBtn");
    dressOffBtn = getButtonFromeWidgetWithWidgetName(rootWidget,COMMON_LAYOUT,"dressOffBtn");
    descList = getListViewFromeWidgetWithWidgetName(rootWidget,HERO_LAYOUT,"descList");
    
    closeEventTage = CLOSE_WEAR_EQUIP_PANEL;
    
    new HeroWearEquipMediator(this);
    
    return true;
}

void HeroWearEquipPanel::onExit()
{
    Layer::onExit();
    HeroCommmon::getInstance()->selectEquipType(EquipType::NONE);
}

HeroWearEquipMediator::HeroWearEquipMediator(HeroWearEquipPanel* instance)
{
    panel = instance;
    registerAttentions(3,SELECT_WEAR_EQUIP,HERO_EXCHANGE_WEAR_EQUIP,HERO_DRESS_OFF_EQUIP);
    ATTENTION_CALLBACK
    {
        if(eventName == SELECT_WEAR_EQUIP)
        {
            
        }
        else if(eventName == HERO_EXCHANGE_WEAR_EQUIP)
        {
            
        }
        else if(eventName == HERO_DRESS_OFF_EQUIP)
        {
            
        }
    };
    panel->registerMeditor(this);
}
/****************************************************   装备穿戴查看面板   *********************************************/


/****************************************************   英雄一览面板   *********************************************/
CREATE_SCENE(HeroLookAtPanel);
bool HeroLookAtPanel::init()
{
    if(!Layer::init())return false;
    showWidget(HERO_LOOK_AT_HERO_PANEL,true,true,true);
    list = getListViewFromeWidgetWithWidgetName(rootWidget, HERO_LAYOUT, "list");
    list->setLayoutLineCout(2);
    
    list->cocos2d::ui::ScrollView::setDirection(cocos2d::ui::SCROLLVIEW_DIR::SCROLLVIEW_DIR_VERTICAL);
    list->setLayoutType(cocos2d::ui::LayoutType::LAYOUT_LINEAR_VERTICAL_MUTIL_LINE);
    closeEventTage = CLOSE_LOOK_AT_HERO_PANEL;
    
    CREATE_ITEM_GROUP
    {
        hero::hero_listModel* model = TRANSLATE_TYPE(HeroLookAtItemRender, item)->model;
        HeroCommmon::getInstance()->currentSelectHero = model;
//        HeroPanel* tempPanel = HeroPanel::create();
//        tempPanel->showAddEffect();
//        this->getParent()->addChild(tempPanel);
//        tempPanel = nullptr;
        //这个时候不需要开启主场景
//        closeEventTage = "";
//        publishEvent(SHOW_HERO_INFO_PANEL);
//        this->closePanel();
        REPLACE_SCENE(HeroPanel);
    };
    
    closeEventTage = REMOVE_WONDOW_PANEL;
    addResourceBar();
    new HeroLookAtMediator(this);
    return true;
}

HeroLookAtPanel::~HeroLookAtPanel()
{
    RELEASE_ITEM_GROUP;
}

void HeroLookAtPanel::showHero()
{
    CREATE_ITEM_WITH_GROUP_AND_LIST(HeroLookAtItemRender,list,hero::HeroModel::getInstance()->hero_list);
}


HeroLookAtMediator::HeroLookAtMediator(HeroLookAtPanel* instance)
{
    panel = instance;
    registerAttentions(1,GET_HERO_LIST);
    ATTENTION_CALLBACK
    {
        if(eventName == GET_HERO_LIST)
        {
            panel->showHero();
        }
    };
    
    panel->registerMeditor(this);
    
    lofd::ConfigManager::getInstance()->createActorConfig();
    
    DEFINE_COMMAND_PARMA(WEB_HERO_LIST);
    SEND_COMMAND(HERO_COMMAND);
}

bool HeroLookAtItemRender::init()
{
    if(!Node::init())return false;
    
    showWidget(HERO_LOOK_AT_HERO_ITEMRENDER);
    this->setSize(cocos2d::Size(510,163));
    nameBMF = GET_TEXTBMF_FOME_WIDGET_WITH_NAME("nameBMF");
    heroIcon = GET_IMAGE_VIEW_FROM_WIDGET_WITH_NAME("heroIcon");
    qualityIcon = GET_IMAGE_VIEW_FROM_WIDGET_WITH_NAME("qualityIcon");
    expBar = mvc::UITool::getLoadingBarFromWidgetWithName(rootWidget,HERO_LAYOUT, "expBar");
    powerTf = GET_TEXTBMF_FOME_WIDGET_WITH_NAME("powerTf");
    quickTf = GET_TEXTBMF_FOME_WIDGET_WITH_NAME("quickTf");
    intelTf = GET_TEXTBMF_FOME_WIDGET_WITH_NAME("intelTf");
    phyTf =  GET_TEXTBMF_FOME_WIDGET_WITH_NAME("phyTf");
    
    REGISTER_ITEM_EVENT(heroIcon);
    return true;
}

std::string HeroLookAtItemRender::getHeroQualityFileNameWithID(int id)
{
    switch(id)
    {
        case 1:
            return HERO_QUALITY_ONE;
            break;
        case 2:
            return HERO_QUALITY_TWO;
            break;
        case 3:
            return HERO_QUALITY_THREE;
            break;
        case 4:
            return HERO_QUALITY_FOUR;
            break;
        case 5:
            return HERO_QUALITY_FIVE;
            break;
    }
    
    return HERO_QUALITY_ONE;
}

void HeroLookAtItemRender::select(bool sel)
{
    if(sel)
    {
        
    }
    else
    {
        
    }
}

void HeroLookAtItemRender::setItemModel(hero::hero_listModel *instance)
{
    SET_MODEL;
    std::string l("Lv.");
    std::string line(" ");
    std::string nl = l + model->level + line + model->hero_name;
    nameBMF->setText(nl.c_str());
    
    lofd::ActorDef* acotrDef = HERO_DEF(model->base_id);
    if(acotrDef)
    {
        SET_TEXT_INFO_WITH_INT(powerTf,acotrDef->up_str);
        SET_TEXT_INFO_WITH_INT(quickTf,acotrDef->up_agi);
        SET_TEXT_INFO_WITH_INT(intelTf,acotrDef->up_int);
        SET_TEXT_INFO_WITH_INT(phyTf,acotrDef->up_con);
        SET_PROGRESS_BAR(expBar,StringToInt(model->exp),StringToInt(model->need_exp));
        LOAD_TEXTURE_FROM_PLIST_WITH_ID(heroIcon,acotrDef->head_icon);
        qualityIcon->loadTexture(getHeroQualityFileNameWithID(acotrDef->color).c_str(),cocos2d::ui::TextureResType::UI_TEX_TYPE_PLIST);
    }
    
}
/****************************************************   英雄一览面板   *********************************************/


/****************************************************   HeroCommmon   *********************************************/
CREATE_SIGNAL_INSTANCE(HeroCommmon,heroCommonInstance);

HeroCommmon::HeroCommmon()
{
    wearResourceMap[EquipType::WEAPONS]     = HEROUI_ICON_WEAPONS;
    wearResourceMap[EquipType::GLOVES]      = HEROUI_ICON_GLOVES;
    wearResourceMap[EquipType::SHOES]       = HEROUI_ICON_SHOES;
    wearResourceMap[EquipType::CLOTHES]     = HEROUI_ICON_CLOTHES;
    wearResourceMap[EquipType::BELT]        = HEROUI_ICON_BELT;
    wearResourceMap[EquipType::ORNAMENTS]   = HEROUI_ICON_ORNAMENTS;
    
    currentSelectHero = nullptr;
    currentSelectPubHeroModel = nullptr;
}

HeroCommmon::~HeroCommmon()
{
    if(currentSelectHero != nullptr)
    {
        currentSelectHero->release();
        currentSelectHero = nullptr;
    }
    wearResourceMap.clear();
}

void HeroCommmon::selectEquipType(EquipType type)
{
    if(equipType == type)return;
    equipType = type;
    if(type == EquipType::NONE)return;
    if(true)
    {
        publishNotice(SHOW_SELECT_EQUIP);
        publishNotice(SELECT_HERO_EQUIP_TYPE);
    }
    else
    {
        publishNotice(SHOW_WEAR_EQUIP_PANEL);
        publishNotice(SELECT_WEAR_EQUIP);
    }
}

EquipType HeroCommmon::getSelectEquipType()
{
    //CCLOG("type is %s",wearResourceMap[equipType]);
    return equipType;
}

std::string HeroCommmon::getSelectEquipTypeFileName()
{
    CCASSERT(equipType != EquipType::NONE,"getSelectEquipTypeFileName 还没有选择装备类型");
    return wearResourceMap[equipType];
}

/****************************************************   HeroCommmon   *********************************************/