//
//  MVCFrame.h
//  TestDemo
//
//  Created by tanzuoliang on 14-4-9.
//
//

#ifndef __TestDemo__MVCFrame__
#define __TestDemo__MVCFrame__

#include "cocos2d.h"
#include "network/HttpRequest.h"
#include "network/WebSocket.h"
#include "network/HttpClient.h"
#include "json/document.h"
#include "CCNS.h"
#include "CCConsole.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "ccMacros.h"
#include "json/prettywriter.h"
#include "editor-support/cocostudio/DictionaryHelper.h"
#include "gameTool.h"
#include "mvc/commandName.h"
#include "mvc/resourceName.h"
#include "TypeConver.h"
#include "mvc/preloader.h"
#include "mvc/mvcDefine.h"
#include "mvc/protocolName.h"
#include "mvc/modelEvent.h"
#include "LOFDConfigManager.h"
#include "commonUI.h"


#define  HTTP_RESPONSE_PARMA        const rapidjson::Value&

#define US_NAME_MVC "mvc"
#define START_MVC namespace mvc{
#define END_MVC }


START_MVC

class MVCCommand;
typedef MVCCommand* (*CommandInstance)(void);
class MVCCommandMap
{
public:
    static MVCCommandMap* getInstance();
    void registerCommand(const std::string commandName, mvc::MVCCommand* command);
    void registerCommand(const std::string commandName, CommandInstance instance);
    bool unRegisterCommand(const std::string commandName);
    void sendCommandByName(const std::string commandName,cocos2d::ValueMap* map);
private:
    MVCCommand* getCommandRefByName(const std::string& command);
    
    cocos2d::__Dictionary _commandMap;
    std::map<std::string,CommandInstance> commandClassMap;
    //cocos2d::__Dictionary commandClassMap;
};

class Mediator:public cocos2d::Ref
{
public:
    //virtual voi
    Mediator(){};
    ~Mediator();
    void sendCommand(std::string command,cocos2d::ValueMap* map = nullptr);
protected:
    std::function<void(const std::string eventName,void* userData)> attentionCallback;
    void registerAttentions(int len,...);
    std::string lastSubPanelTag;
private:
    void initMediator();
    std::vector<std::string> attentionVector;
    std::map<std::string,cocos2d::EventListenerCustom*> eventMap;
};


class MediatorListenerMap
{
public:
    DEFINE_SIGANL_INSTANCE(MediatorListenerMap);
    void pushNode(std::string name);
    int getCountByName(std::string name);
private:
    std::map<std::string,int> map;
};

//typedef enum
//{
//    TOP_PANEL,
//    SUB_PANEL,
//}OrionPanelType;
//
//typedef enum
//{
//    LEFT,
//    MIDDLE,
//    RIGHT
//}OrionPanelLocation;

class OrionPanel:public cocos2d::Layer
{
public:
//    OrionPanel()
//    {
//        panelType = OrionPanelType::TOP_PANEL;
//        location = OrionPanelLocation::LEFT;
//    };
    virtual void onExit();
    void showAddEffect();
    void registerMeditor(Mediator* instance);
    void moveChildToCenter(cocos2d::Node* child = nullptr);
    void addChildToCenter(cocos2d::Node* child);
    void addSubPanelToRight(mvc::OrionPanel* panel,int dir=1);
//    OrionPanelType getPanelType();
//    OrionPanelLocation getLocation();
protected:
//    OrionPanelType panelType;
//    OrionPanelLocation location;
    Mediator* mediator;
    void publishEvent(std::string name,void *optionalUserData=nullptr);
    void showWidget(std::string widgetName,bool addBg=false,bool centerHandler=false,bool containsCloseBtn=false,bool autoCreateBg = false,std::string title="",TitleStyle titleStyle = TitleStyle::NORMAL,float oy=-30,std::string bgFrameName="");
    void addResourceBar(bool addResourcePanel=true,bool showAddPaneleffct = true);
    cocos2d::ui::Button* closeBtn;
    void onChildClick(cocos2d::Ref* ref,cocos2d::ui::TouchEventType type);
    cocos2d::ui::Button* getButtonFromeWidget(cocos2d::ui::Widget* layout,std::string name,bool addListaner=true);
    cocos2d::ui::Button* getButtonFromeWidgetWithWidgetName(cocos2d::ui::Widget* layout,std::string widget,std::string name,bool addListaner=true);
    cocos2d::ui::ImageView* getImageViewFromeWidget(cocos2d::ui::Widget* layout,std::string name,bool addListaner=false);
    cocos2d::ui::ImageView* getImageViewFromeWidgetWithName(cocos2d::ui::Widget *layout, std::string widgetName,std::string name,bool addListener=false);
    cocos2d::ui::ListView* getListViewFromeWidgetWithWidgetName(cocos2d::ui::Widget* layout,std::string widgetName,std::string name);
    cocos2d::ui::TextBMFont* getTextBMFFromWidgetWithName(cocos2d::ui::Widget* layout,std::string widgetName,std::string name);
    cocos2d::ui::Text* getTextFromWidgetWithName(cocos2d::ui::Widget* layout,std::string widgetName,std::string name);
    cocos2d::ui::Layout* getLayoutFromWidget(cocos2d::ui::Widget* layout,std::string name,bool addListener=false);
    cocos2d::ui::LoadingBar* getLoadingBarFromWidgetWithName(cocos2d::ui::Widget* layout,std::string widgetName,std::string name);
    
    void closePanel();
    /**有放大效果时等最大后调用*****/
    virtual void onReallyShow(){};
    
    cocos2d::ui::TextBMFont* conventWidgetToTextBMFont(cocos2d::ui::Widget* node)
    {
        return dynamic_cast<cocos2d::ui::TextBMFont*>(node);
    }
    
    cocos2d::ui::Text* conventWidgetToText(cocos2d::ui::Widget* node)
    {
        return dynamic_cast<cocos2d::ui::Text*>(node);
    }
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    std::function<void(cocos2d::Ref* ref,cocos2d::ui::TouchEventType type)> childCickCallBack;
    cocos2d::ui::Widget* rootWidget;
    cocos2d::Node* container;
    std::string closeEventTage;
    float offsetY;
    bool windowPanel;
    bool showEffect;
private:
    void showRemoveEffect();
    mvc::OrionPanel* rightPanel;
    
};

class ItemGroup;
class BaseItemRender:public cocos2d::ui::Widget
{
public:
    void setGroup(ItemGroup* _group){group = _group;}
    virtual void select(bool){};
protected:
    void showWidget(std::string fileName);
    cocos2d::ui::Widget* rootWidget;
    ItemGroup* group;
    void registerEvent(cocos2d::ui::Widget* child);
    
    std::function<void(cocos2d::Ref* ref,cocos2d::ui::TouchEventType type)> childCickCallBack;
private:
    void onChildClick(cocos2d::Ref *ref, cocos2d::ui::TouchEventType type);
};

class ItemGroup:public cocos2d::Ref
{
public:
    //CREATE_FUNC(ItemGroup);
    ItemGroup();
    std::function<void(BaseItemRender* item)> itemClickCallback;
    //virtual bool init();
    void selectItem(BaseItemRender* item=nullptr);
    BaseItemRender* getItem();
private:
    BaseItemRender* lastItem;
};

/***************************************** POP MANAGER****************************************************/
//class PopUpManager
//{
//public:
//    DEFINE_SIGANL_INSTANCE(PopUpManager);
//    void popupPanel(OrionPanel* panel);
//    void popupPanelWithShow(OrionPanel* panel);
//    void removePanel(OrionPanel* panel);
//    void removeAllPanels();
//private:
//    void centerPopup(OrionPanel* panel);
//    void moveToCneter(OrionPanel* panel);
//    void moveToCenterAndClose(OrionPanel* panel);
//private:
//    std::vector<OrionPanel*> panelList;
//};
/***************************************** POP MANAGER****************************************************/


typedef struct AnimatInfo
{
    std::string animationName;
    std::string frameName;
    int totalFrames;
    float duration;
};

class UITool
{
public:
    static cocos2d::ui::Button* getButtonFromeWidget(cocos2d::ui::Widget* layout,std::string name)
    {
        auto btn = dynamic_cast<cocos2d::ui::Button*>(layout->getChildByName(name.c_str()));
        return btn;
    }
    
    static cocos2d::ui::Button* getButtonFromeWidgetWithWidgetName(cocos2d::ui::Widget* layout,std::string widgetName,std::string name)
    {
        auto btn = dynamic_cast<cocos2d::ui::Button*>(layout->getChildByName(widgetName.c_str())->getChildByName(name.c_str()));
        return btn;
    }
    
    static cocos2d::ui::ListView* getListViewFromeWidgetWithWidgetName(cocos2d::ui::Widget* layout,std::string widgetName,std::string name)
    {
        auto listView = dynamic_cast<cocos2d::ui::ListView*>(layout->getChildByName(widgetName.c_str())->getChildByName(name.c_str()));
        return listView;
    }
    
    static cocos2d::ui::ListView* getListViewFromeWidget(cocos2d::ui::Widget* layout,std::string name)
    {
        auto listView = dynamic_cast<cocos2d::ui::ListView*>(layout->getChildByName(name.c_str()));
        return listView;
    }
    
    static cocos2d::ui::ImageView* getImageViewFromeWidget(cocos2d::ui::Widget *layout, std::string name)
    {
        auto image = dynamic_cast<cocos2d::ui::ImageView*>(layout->getChildByName(name.c_str()));
        return image;
    }
    
    static cocos2d::ui::ImageView* getImageViewFromeWidgetWithName(cocos2d::ui::Widget *layout, std::string widgetName,std::string name)
    {
        auto image = dynamic_cast<cocos2d::ui::ImageView*>(layout->getChildByName(widgetName.c_str())->getChildByName(name.c_str()));
        return image;
    }
    
    static cocos2d::ui::TextBMFont* getTextBMFFromWidgetWithName(cocos2d::ui::Widget* layout,std::string widgetName,std::string name)
    {
        auto bmf = dynamic_cast<cocos2d::ui::TextBMFont*>(layout->getChildByName(widgetName.c_str())->getChildByName(name.c_str()));
        return bmf;
    }
    
    static cocos2d::ui::TextBMFont* getTextBMFFromWidget(cocos2d::ui::Widget* layout,std::string name)
    {
        auto bmf = dynamic_cast<cocos2d::ui::TextBMFont*>(layout->getChildByName(name.c_str()));
        return bmf;
    }
    
    static cocos2d::ui::Text* getTextFromWidgetWithName(cocos2d::ui::Widget* layout,std::string widgetName,std::string name)
    {
        auto bmf = dynamic_cast<cocos2d::ui::Text*>(layout->getChildByName(widgetName.c_str())->getChildByName(name.c_str()));
        return bmf;
    }
    
    static cocos2d::ui::Text* getTextFromWidget(cocos2d::ui::Widget* layout,std::string name)
    {
        auto bmf = dynamic_cast<cocos2d::ui::Text*>(layout->getChildByName(name.c_str()));
        return bmf;
    }
    
    static cocos2d::ui::TextField* getTextFieldFromWidget(cocos2d::ui::Widget* layout,std::string name)
    {
        auto textfiled = dynamic_cast<cocos2d::ui::TextField*>(layout->getChildByName(name.c_str()));
        return textfiled;
    }
    
    static cocos2d::ui::TextField* getTextFieldFromWidgetWithName(cocos2d::ui::Widget* layout,std::string widgetName,std::string name)
    {
        auto textfiled = dynamic_cast<cocos2d::ui::TextField*>(layout->getChildByName(widgetName.c_str())->getChildByName(name.c_str()));
        return textfiled;
    }
    
    static cocos2d::ui::Layout* getLayoutFromWidget(cocos2d::ui::Widget* layout,std::string name)
    {
        auto _layout = dynamic_cast<cocos2d::ui::Layout*>(layout->getChildByName(name.c_str()));
        return _layout;
    }
    
    static cocos2d::ui::Widget* widgetFromJson(std::string jsonName)
    {
       auto widget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(jsonName.c_str());
       return widget;
    }
    
    static cocos2d::ui::LoadingBar* getLoadingBarFromWidgetWithName(cocos2d::ui::Widget* layout,std::string widgetName,std::string name)
    {
        auto bar = dynamic_cast<cocos2d::ui::LoadingBar*>(layout->getChildByName(widgetName.c_str())->getChildByName(name.c_str()));
        return bar;
    }

    static cocos2d::ui::CheckBox* getCheckBoxFromWidgetWithName(cocos2d::ui::Widget* layout,std::string widgetName,std::string name)
    {
        auto checkBox = dynamic_cast<cocos2d::ui::CheckBox*>(layout->getChildByName(widgetName.c_str())->getChildByName(name.c_str()));
        return checkBox;
    }

    
    static cocos2d::Animate* getAnimateWithInfo(AnimatInfo& info,bool cycle = false)
    {
        cocos2d::Animation* animation = cocos2d::AnimationCache::getInstance()->getAnimation(info.animationName);
        if(!animation)
        {
            cocos2d::Vector<cocos2d::SpriteFrame*> spriteVec;
            
            for(int i = 0; i < info.totalFrames;i++)
            {
                char path[256] = {0};
                sprintf(path, info.frameName.c_str(),i);
                cocos2d::SpriteFrame* frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(path);
                if(frame)
                    spriteVec.pushBack(frame);
                else
                    CCLOG("resource %s can not find!",path);
            }
            if(cycle)
            {
                char path[256] = {0};
                sprintf(path, info.frameName.c_str(),0);
                cocos2d::SpriteFrame* frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(path);
                if(frame)
                    spriteVec.pushBack(frame);
                else
                    CCLOG("resource %s can not find!",path);

            }
            if(spriteVec.size() == 0)
                spriteVec.pushBack(cocos2d::Sprite::create("jump.png")->getSpriteFrame());
            animation = cocos2d::Animation::createWithSpriteFrames(spriteVec,info.duration);
            cocos2d::AnimationCache::getInstance()->addAnimation(animation, info.animationName);
        }
        
        return cocos2d::Animate::create(animation);
    }
    
    static cocos2d::Animate* getAnimateWithName(std::string animateName)
    {
        cocos2d::Animation* animation = cocos2d::AnimationCache::getInstance()->getAnimation(animateName);
        if(animation)
        {
            return cocos2d::Animate::create(animation);
        }
        
        return nullptr;
    }
    
//    static int ReplaceStr(char *sSrc, char *sMatchStr, char *sReplaceStr)
//    {
//        int  StringLen;
//        char caNewString[512] = {0};
//        
//        char *FindPos = strstr(sSrc, sMatchStr);
//        if( (!FindPos) || (!sMatchStr) )
//            return -1;
//        
//        while( FindPos )
//        {
//            memset(caNewString, 0, sizeof(caNewString));
//            StringLen = FindPos - sSrc;
//            strncpy(caNewString, sSrc, StringLen);
//            strcat(caNewString, sReplaceStr);
//            strcat(caNewString, FindPos + strlen(sMatchStr));
//            strcpy(sSrc, caNewString);
//            
//            FindPos = strstr(sSrc, sMatchStr);
//        }
//        
//        return 0;
//    }
};


class Model:public cocos2d::Ref
{
public:
    Model(){};
protected:
    virtual void publishNotice(std::string notice);
protected:
    
};

class MVCService;
class MVCCommand :public cocos2d::Ref
{
public:
    MVCCommand();
    ~MVCCommand();
    virtual void execute(cocos2d::ValueMap* map);
protected:
    MVCService* service;
    
};

class MVCService:public cocos2d::Ref
{
public:
    cocos2d::network::WebSocket* socket;
    cocos2d::network::HttpClient* httpClient;
public:
    MVCService();
    static MVCService* getInstance();
    
    std::string web_sid;
    
    void sendHttp(const char* buffer,int bufferLen =0);
    void sendSocket(const std::string method,const char* buffer);
    
    void registerHttpCallBack(const std::string& action,const std::string& method,const std::function<void(HTTP_RESPONSE_PARMA)>& fun);
    void registerSocketCallBack(const std::string& method,const std::function<void(cocos2d::network::HttpResponse*)>& fun);
private:
    //cocos2d::__Dictionary* httpCallBackMap;
    std::map<std::string, std::map<std::string, std::function<void(HTTP_RESPONSE_PARMA)>>> httpCallBackMap;
    //cocos2d::__Dictionary* socketCallBackMap;
    std::map<std::string, std::function<void(cocos2d::network::HttpResponse*)>> socketCallBackMap;
    void httpRespondeCallback(cocos2d::network::HttpClient* client,cocos2d::network::HttpResponse* response);
};

class BaseServer:public cocos2d::Ref
{
public:
    BaseServer()
    {
        service = mvc::MVCService::getInstance();
    }
    bool init(){return true;};
protected:
    mvc::MVCService* service;
};

class ModelPool
{
public:
    static ModelPool* getInstance();
    ModelPool();
    void registerModel(Model* model,std::string modelName);
    Model* getModelByName(std::string modelName);
private:
    std::map<std::string,Model*> modelMap;
};

#define CREATE_PROTOCOL(__TYPE__) \
static Protocol* createProtocol() \
{ \
__TYPE__ *pRet = new __TYPE__(); \
if (pRet) \
{ \
pRet->autorelease(); \
return pRet; \
} \
else \
{ \
delete pRet; \
pRet = NULL; \
return NULL; \
} \
}
class Protocol:public cocos2d::Ref
{
public:
    virtual void parser(char* response)=0;
    bool init(){return true;}
};
typedef Protocol* (*ProtocolInstance)(void);
class ProtocolPool
{
public:
    static ProtocolPool* getInstance();
    ProtocolPool();
    void registerProtocol(Protocol* protocol,std::string protocolName);
    void registerProtocol(ProtocolInstance instance, std::string trollerName, std::string actionName);
    Protocol* getProtocolByName(std::string protocolName);
private:
    std::map<std::string,Protocol*> protocolMap;
    std::map<std::string,ProtocolInstance> protocolClassMap;
};

//#define toucheventselector(_SELECTOR) (SEL_TouchEvent)(&_SELECTOR)
//#define PRELOADER_STD_FUN   std::function<void(void)>
//class PreLoader
//{
//public:
//    DEFINE_SIGANL_INSTANCE(PreLoader);
//    PreLoader(){completeCallback = nullptr;};
//    void startLoader(int len,...);
//    void loadCreateCharcater(PRELOADER_STD_FUN instance=nullptr);
//    void loadMainCity(PRELOADER_STD_FUN instance=nullptr);
//    void loadHero(PRELOADER_STD_FUN instance=nullptr);
//    void loadLoginPanel(PRELOADER_STD_FUN instance=nullptr);
//    void LoadCommonUI(PRELOADER_STD_FUN instance=nullptr);
//private:
//    std::string getResourcePath(std::string);
//    int totalLoades;
//    int curentLoades;
//    std::vector<std::string> loadMap;
//    void loadComplete();
//    void childLoadComplete();
//    PRELOADER_STD_FUN completeCallback;
//};
//
//class HttpLoading
//{
//public:
//    DEFINE_SIGANL_INSTANCE(HttpLoading);
//    HttpLoading();
//    void show();
//    void hide();
//private:
//    cocos2d::Sprite* loading;
//};
//
//class Alert
//{
//public:
//    DEFINE_SIGANL_INSTANCE(Alert);
//    void showHttpError(std::string info);
//    void showNotice(std::string info);
//};

END_MVC


#endif /* defined(__TestDemo__MVCFrame__) */
