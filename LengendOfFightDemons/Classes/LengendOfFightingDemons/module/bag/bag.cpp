//
//  bag.cpp
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-4-30.
//
//

#include "bag.h"
#include "mainCity.h"
/************************************************************************************/
int offset = -40;

bool BagItemDescPanel::init()
{
    LAYER_INIT;
    closeEventTage = CLOSE_BAG_DESC_PANEL;
    
    showWidget(BAG_ITEM_DESC_PANEL,false,true,false,true,"",TitleStyle::NO_TITLE,offset);
    
    getBtn = getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "getBtn");
    sellBtn = getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "sellBtn");
    attckDescBMF = getTextBMFFromWidgetWithName(rootWidget, BMF_LAYOUT, "descBMF");
    equipDescBMF = getTextBMFFromWidgetWithName(rootWidget, BMF_LAYOUT, "addressBMF");
    hasLabel = getTextBMFFromWidgetWithName(rootWidget, BMF_LAYOUT, "hasLabel");
    costLabel = getTextBMFFromWidgetWithName(rootWidget, BMF_LAYOUT, "costLabel");
    
    currentSelectItem = BagItemRender::create();
    currentSelectItem->setAnchorPoint(cocos2d::Point(0.5f,0.5f));
    auto image = getImageViewFromeWidgetWithName(rootWidget, HERO_LAYOUT, "bagIcon");
    image->addChild(currentSelectItem);
    currentSelectItem->setPosition(cocos2d::Point(-image->getSize().width/2, -image->getSize().height/2));
    new BagItemDescMediator(this);
    
    RETURN_TRUE;
}

CREATE_MEDIATOR(BagItemDescMediator,BagItemDescPanel,
{
    registerAttentions(1,"");
    ATTENTION_CALLBACK
    {
        
    };
});

/************************************************************************************/


/************************************************************************************/
CREATE_SCENE(BagMainPanel);
void BagMainPanel::showItems()
{
    itemList->removeAllItems();
    itemGroup->selectItem();
    
    for (int i=0; i<30; i++)
    {
        BagItemRender* item = BagItemRender::create();
        item->setGroup(itemGroup);
        itemList->pushBackCustomItem(item);
    }
}

BagMainPanel::~BagMainPanel()
{
    itemGroup->release();
}

bool BagMainPanel::init()
{
    LAYER_INIT;
    closeEventTage = REMOVE_WONDOW_PANEL;
    
    showWidget(BAG_MAIN_PANEL,true,true,false,true,"背包",TitleStyle::BAG,offset);
    allBtn = getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "allBtn");
    equipBtn = getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "equipBtn");
    materialBtn = getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "materialBtn");
    propBtn = getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, "propBtn");
    itemList = mvc::UITool::getListViewFromeWidget(rootWidget,"itemList");
  
    SET_LIST_VIEW(itemList,4);
    
    CHILD_CLICK_CALLBACK
    {
        IF_CHILD_TOUCH_ENDED
        {
            if(ref == allBtn)
            {
                updateBarStatus(true,false,false,false);
            }
            else if(ref == equipBtn)
            {
                updateBarStatus(false,true,false,false);
            }
            else if (ref == propBtn)
            {
                updateBarStatus(false,false,true,false);
                publishEvent("");
            }
            else if(ref == materialBtn)
            {
                updateBarStatus(false,false,false,true);
            }
        }
    };
    
    updateBarStatus(true,false,false,false);
    
    itemGroup = new mvc::ItemGroup();
    itemGroup->itemClickCallback = [this](mvc::BaseItemRender* item)
    {
        
        this->addSubPanelToRight(BagItemDescPanel::create(),-1);
    };
    addResourceBar();
    new BagMainMediator(this);
    
    RETURN_TRUE;
}

void BagMainPanel::updateBarStatus(bool all,bool equip,bool prop,bool material)
{
    allBtn->setBright(!all);
    allBtn->setTouchEnabled(!all);
    
    equipBtn->setBright(!equip);
    equipBtn->setTouchEnabled(!equip);
    
    propBtn->setBright(!prop);
    propBtn->setTouchEnabled(!prop);
    
    materialBtn->setBright(!material);
    materialBtn->setTouchEnabled(!material);
}

CREATE_MEDIATOR(BagMainMediator,BagMainPanel,
{
    registerAttentions(2,CLOSE_BAG_DESC_PANEL,CLOSE_BAG_SELL_PANEL);
    ATTENTION_CALLBACK
    {
        if(eventName == CLOSE_BAG_SELL_PANEL)
        {
            panel->moveChildToCenter();
        }
        else if(eventName == CLOSE_BAG_DESC_PANEL)
        {
            panel->moveChildToCenter();
        }
    };
    
    panel->showItems();
});

bool BagItemRender::init()
{
    NODE_INIT;
    lightFrame = nullptr;
    showWidget(BAG_MAIN_ITEMRENDER);
    icon = mvc::UITool::getImageViewFromeWidget(rootWidget, "icon");
    //icon->setTouchEnabled(true);
    //itemContainer = mvc::UITool::getLayoutFromWidget(rootWidget, "itemContainer");
    registerEvent(icon);
    
    CHILD_CLICK_CALLBACK
    {
        IF_CHILD_TOUCH_ENDED
        {
            if(ref == icon)
            {
                group->selectItem(this);
            }
        }
    };
    
    RETURN_TRUE;
}

ITEMRENDER_SELECT(BagItemRender)
{
    if(sel)
    {
        addSelectFrame();
    }
    else
    {
        removeSelectFrame();
    }
}

void BagItemRender::addSelectFrame()
{
    removeSelectFrame();
    
    lightFrame = cocos2d::Sprite::createWithSpriteFrameName(ITEM_SELECT_FRAME);
    //lightFrame->setAnchorPoint(cocos2d::Point(0.5f,0.5f));
    //lightFrame->setContentSize(this->getSize());
    //MOVE_CHILD_TO_CENTER(lightFrame,this);
    rootWidget->addChild(lightFrame);
    lightFrame->setPosition(57, 57);
    CCLOG("lightFrame width is %f",lightFrame->getContentSize().width);
}

void BagItemRender::removeSelectFrame()
{
    if(lightFrame)
    {
        lightFrame->removeFromParent();
        lightFrame = nullptr;
    }
}
/************************************************************************************/

/************************************************************************************/
CREATE_FUNC_WITH_INIT(BagSellPanel,
{
    closeEventTage = CLOSE_BAG_SELL_PANEL;
    showWidget(BAG_SELL_PANEL,false,true,false,true,"",TitleStyle::NO_TITLE,offset);
    
    CHILD_CLICK_CALLBACK
    {
        IF_CHILD_TOUCH_ENDED
        {
            
        }
    };
    
    new BagSellMediator(this);
})

CREATE_MEDIATOR(BagSellMediator, BagSellPanel,
{
    registerAttentions(1,"");
    ATTENTION_CALLBACK
    {
        
    };
})
/************************************************************************************/