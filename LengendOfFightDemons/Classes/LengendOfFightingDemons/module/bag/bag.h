//
//  bag.h
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-4-30.
//
//

#ifndef __LengendOfFightDemons__bag__
#define __LengendOfFightDemons__bag__

#include <iostream>
#include "cocos2d.h"
#include "editor-support/cocostudio/DictionaryHelper.h"
#include "ui/CocosGUI.h"
#include "ApplicationContext.h"
#include "MVCFrame.h"
#include "SimpleAudioEngine.h"
#include "gameTool.h"

/**************************** define **************************************/
#define BAG_SHOW_ALL                   "bagShowAll"
#define BAG_SHOW_EQUIP                 "bagShowEquip"
#define BAG_SHOW_MATERIAL              "bagShowMetrial"
#define BAG_SHOW_PROP                  "bagShowProp"

#define CLOSE_BAG_DESC_PANEL           "closeBagDescPanel"
#define CLOSE_BAG_SELL_PANEL           "closeBagSellPanel"
/**************************** define **************************************/

/**************************** BagItemDescPanel **************************************/
class BagItemDescPanel:public mvc::OrionPanel
{
public:
    CREATE_FUNC(BagItemDescPanel);
    mvc::BaseItemRender* currentSelectItem;
private:
    virtual bool init();
    BUTTON getBtn;
    BUTTON sellBtn;
    SCROLL_VIEW descScrollView;
    BMFONT attckDescBMF;
    BMFONT equipDescBMF;
    BMFONT hasLabel;
    BMFONT costLabel;
};


class BagItemDescMediator:public mvc::Mediator
{
public:
    BagItemDescMediator(BagItemDescPanel* instance);
private:
    BagItemDescPanel* panel;
};
/**************************** BagItemDescPanel **************************************/

/**************************** BagMainPanel **************************************/
class BagMainPanel:public mvc::OrionPanel
{
public:
    DEFINE_CREATE_SCENE;
    DEFINE_CREATE_FUNC_WITH_INIT(BagMainPanel);
    ~BagMainPanel();
    void showItems();
    void updateBarStatus(bool all,bool equip,bool prop,bool material);
private:
    BUTTON allBtn;
    BUTTON equipBtn;
    BUTTON propBtn;
    BUTTON materialBtn;
    LIST_VIEW itemList;
    mvc::ItemGroup* itemGroup;
};

DEFINE_CREATE_MEDIATOR(BagMainMediator,BagMainPanel);

class BagItemRender:public mvc::BaseItemRender
{
public:
    DEFINE_ITEMRENDER;
    DEFINE_CREATE_FUNC_WITH_INIT(BagItemRender);
private:
    cocos2d::Sprite* lightFrame;
    LAYOUT itemContainer;
    IMAGE_VIEW icon;
    void addSelectFrame();
    void removeSelectFrame();
};
/**************************** BagMainPanel **************************************/

/**************************** BagSellPanel **************************************/
class BagSellPanel:public mvc::OrionPanel
{
public:
    DEFINE_CREATE_FUNC_WITH_INIT(BagSellPanel);
};


DEFINE_CREATE_MEDIATOR(BagSellMediator,BagSellPanel);
/**************************** BagSellPanel **************************************/

#endif /* defined(__LengendOfFightDemons__bag__) */
