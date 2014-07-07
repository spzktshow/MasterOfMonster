//
//  mvcDefine.h
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-14.
//
//

#ifndef LengendOfFightDemons_mvcDefine_h
#define LengendOfFightDemons_mvcDefine_h
#include "cocos2d.h"
//#include "preloader.h"
/********************************************** function define ****************************************/
#define START_SEVER_INFO(T1,T2) char sendInfo[1024] = {0};\
rapidjson::InsituStringStream stream(sendInfo);\
stream.PutBegin();\
rapidjson::PrettyWriter<rapidjson::InsituStringStream> write(stream);\
write.StartObject();\
write.String("c");\
write.String(T1);\
write.String("a");\
write.String(T2);\
write.String("sid");\
write.String(mvc::MVCService::getInstance()->web_sid.c_str());

#define END_SERVER_INFO   write.EndObject();\
service->sendHttp(sendInfo,strlen(sendInfo));

#define CREATE_SIGNAL_INSTANCE(__TYPE__,__INSTANCE__)\
static __TYPE__* __INSTANCE__ = nullptr;\
__TYPE__* __TYPE__::getInstance()\
{\
if(__INSTANCE__ == nullptr)\
__INSTANCE__ = new __TYPE__();\
return __INSTANCE__;\
}

#define DEFINE_CREATE_SCENE static cocos2d::Scene* createScene()

#define CREATE_SCENE(__TYPE__) \
cocos2d::Scene* __TYPE__::createScene() \
{ \
auto scene = cocos2d::Scene::create(); \
loading::PreLoader::getInstance()->currentScene = scene;\
auto layer = __TYPE__::create();\
scene->addChild(layer);\
return scene;\
}


#define DEFINE_SIGANL_INSTANCE(__TYPE__)\
static __TYPE__* getInstance();

#define REPLACE_SCENE(T)\
cocos2d::Director::getInstance()->replaceScene(T::createScene());

#define CREATE_COMMAND(__TYPE__) \
static MVCCommand* createCommand() \
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

#define DEFINE_COMMAND_PARMA(ACTION)\
cocos2d::ValueMap* param = new cocos2d::ValueMap();\
CREATE_COMMAND_PARMA(HTTP_ACTION, ACTION);

#define CREATE_COMMAND_PARMA(T1,T2)\
param->insert(std::make_pair(T1, cocos2d::Value(T2)))

#define SEND_COMMAND(T)\
sendCommand(T,param);

typedef void (cocos2d::Ref::*functionPoint)(void);
#define BASE_FUNCTION_POINT(T)        (functionPoint)(&T)

#define DEFINE_CREATE_FUNC_WITH_INIT(T)\
CREATE_FUNC(T);\
virtual bool init();

#define CREATE_FUNC_WITH_INIT(T,CODE)\
bool T::init()\
{\
if(!Layer::init())return false;\
CODE;\
return true;\
}

#define CREATE_ITEM_RENDER_FUNC_WITH_INIT(T,CODE)\
bool T::init()\
{\
if(!Node::init())return false;\
CODE;\
return true;\
}

#define DEFINE_CREATE_MEDIATOR(T1,T2)\
class T1:public mvc::Mediator\
{\
public:\
T1(T2* instnace);\
private:\
T2* panel;\
};

#define CREATE_MEDIATOR(T1,T2,CODE)\
T1::T1(T2* instance)\
{\
panel = instance;\
CODE;\
panel->registerMeditor(this);\
}

#define DEFINE_ITEMRENDER              virtual void select(bool sel)

#define ITEMRENDER_SELECT(T)\
void T::select(bool sel)

#define SET_LIST_VIEW(LIST,NUM)\
LIST->setLayoutLineCout(NUM);\
LIST->setLayoutType(cocos2d::ui::LayoutType::LAYOUT_LINEAR_VERTICAL_MUTIL_LINE);

#define FOR_FUN(T1,T2,CODE)\
for(int T1=0;T1<T2;T1++)\
{\
CODE;\
}

#define RESET_ITEM_LIST_WITH_LABEL(T)\
T->removeAllItems();\
CLEAR_ITEM_GROUP;

#define CREATE_ITEM_WITH_GROUP_AND_LIST(ITEM_CLASS,LIST,DATA)\
RESET_ITEM_LIST_WITH_LABEL(LIST);\
int len = DATA->size();\
for(int i=0;i<len;i++)\
{\
ITEM_CLASS* item = ITEM_CLASS::create();\
item->setItemModel(DATA->at(i));\
ITEM_SET_GROUP;\
LIST->pushBackCustomItem(item);\
}

#define DEFINE_SET_ITEM_MODEL(MODEL_CLASS)\
MODEL_CLASS* model;\
void setItemModel(MODEL_CLASS* instance);\

#define GET_TEXTBMF_FOME_WIDGET_WITH_NAME(NAME)\
mvc::UITool::getTextBMFFromWidgetWithName(rootWidget, BMF_LAYOUT, NAME);

#define GET_IMAGE_VIEW_FROM_WIDGET_WITH_NAME(NAME)\
mvc::UITool::getImageViewFromeWidgetWithName(rootWidget, COMMON_LAYOUT, NAME);

#define GET_BUTTON_FROM_WIDGET_WITH_NAME(NAME)\
mvc::UITool::getButtonFromeWidgetWithWidgetName(rootWidget, COMMON_LAYOUT, NAME);

#define MOVE_CENTER(T)\
cocos2d::Size size = cocos2d::Director::getInstance()->getVisibleSize();\
cocos2d::Point ori = cocos2d::Director::getInstance()->getVisibleOrigin();\
T->setPosition(cocos2d::Point(ori.x + size.width / 2, ori.y + size.height / 2));

#define StringToInt(T)\
std::atoi(T.c_str())

#define SET_TEXT_INFO_WITH_INT(TEXT,INT)\
TEXT->setText(IntToString(INT).c_str())

#define SET_PROGRESS_BAR(BAR,NOW,TOTAL)\
{\
int per = (NOW * 100) / TOTAL;\
BAR->setPercent(per);\
}


#define SET_PROGRESS_BAR_WITH_TEXT(BAR,BAR_TEXT,NOW,TOTAL)\
{\
int per = 0;\
if(TOTAL > 0)\
per = (NOW * 100) / TOTAL;\
BAR->setPercent(per);\
std::string temp("");\
std::string joinFormat = "/";\
temp = cocos2d::StringUtils::toString(NOW) +joinFormat + cocos2d::StringUtils::toString(TOTAL);\
BAR_TEXT->setText(temp.c_str());\
}

#define LOAD_TEXTURE_FROM_PLIST_WITH_ID(ICON,ID)\
{\
std::string no = IntToString(ID);\
no += ".png";\
ICON->loadTexture(no.c_str(),cocos2d::ui::TextureResType::UI_TEX_TYPE_PLIST);\
}

#define LOAD_TEXTURE_FROM_PLIST_WITH_STRING_ID(ICON,ID)\
{\
std::string no = ID;\
no += ".png";\
ICON->loadTexture(no.c_str(),cocos2d::ui::TextureResType::UI_TEX_TYPE_PLIST);\
}

#define TRANSLATE_TYPE(NEW_TYPE,OLD_TYPE)\
dynamic_cast<NEW_TYPE*>(OLD_TYPE)

#define REGISTER_ITEM_EVENT(T)\
registerEvent(T);\
CHILD_CLICK_CALLBACK\
{\
    IF_CHILD_TOUCH_ENDED\
    {\
        if(ref == T)\
        {\
            this->group->selectItem(this);\
        }\
    }\
};


//#define MOVE_CHILD_TO_CENTER(CHILD,PARENT)\
//float _x = (PARENT->getContentSize().width - CHILD->getContentSize().width) / 2;\
//float _y = (PARENT->getContentSize().height - CHILD->getContentSize().height) / 2;\
//CHILD->setPosition(cocos2d::Point(_x,_y));
#define MOVE_CHILD_TO_CENTER(CHILD,PARENT)\
float _x = (PARENT->getSize().width ) / 2;\
float _y = (PARENT->getSize().height) / 2;\
CHILD->setPosition(cocos2d::Point(_x,_y));

#define REGISTER_COMMAND(NAME,CLASS)\
mvc::MVCCommandMap::getInstance()->registerCommand(NAME,&CLASS::createCommand)

#define GET_STRING_INGORE_TYPE(T1,T2)\
if(T2.IsInt())\
T1 = IntToString(T2.GetInt());\
else if(T2.IsString())\
T1 = T2.GetString();

#define PUSH_ELEMENT_INGORE_TYPE(LIST,ELE)\
if(ELE.IsString())\
LIST->push_back(ELE.GetString());\
else if(ELE.IsInt())\
LIST->push_back(IntToString(ELE.GetInt()));

#define UPDATE_VALUE(STRING,VALUE)\
int newValue = StringToInt(STRING) + VALUE;\
STRING = IntToString(newValue);
/********************************************** function define ****************************************/

/********************************************** code define ****************************************/
#define LOCAL_SAVE                     cocos2d::UserDefault::getInstance()
#define REGISTER_PROTOCOL              mvc::ProtocolPool::getInstance()->registerProtocol
#define Conn(x,y)                      x##y
#define ToString(x)                    #x
#define IntToString(i)                 cocos2d::StringUtils::toString(i)
#define CEIL(x,y)                      ((x)/(y)) + ((x)%(y)>0?1:0)
#define HERO_DEF(HERO_ID)              lofd::ConfigManager::getInstance()->actorConfig->getActorDefById(StringToInt(HERO_ID))
#define CURRENT_SELECT_HERO_DEF        HERO_DEF(HeroCommmon::getInstance()->currentSelectHero->base_id)
#define SKILL_DEF(SKILL_ID)            lofd::ConfigManager::getInstance()->skillBeforeConfig->getSkillDefByID(StringToInt(SKILL_ID))

#define DEFINE_ITME_GROUP              mvc::ItemGroup* itemGroup
#define CREATE_ITEM_GROUP              itemGroup = new mvc::ItemGroup();ITEM_CLICK_CALLBACK
#define RELEASE_ITEM_GROUP             itemGroup->release()
#define ITEM_SET_GROUP                 item->setGroup(itemGroup)
#define CLEAR_ITEM_GROUP               if(itemGroup)itemGroup->selectItem()
#define RESET_ITEM_LIST                itemList->removeAllItems();itemGroup->selectItem();
#define LAYER_INIT                     if(!Layer::init())return false
#define NODE_INIT                      if(!cocos2d::Node::init())return false
#define RETURN_TRUE                    return true
#define SET_MODEL                      model = instance

#define ITEM_CLICK_CALLBACK            itemGroup->itemClickCallback = [this](mvc::BaseItemRender* item)
#define ATTENTION_CALLBACK             attentionCallback = [this](const std::string eventName,void* userData)
#define CHILD_CLICK_CALLBACK           childCickCallBack = [this](cocos2d::Ref* ref,cocos2d::ui::TouchEventType type)
#define IF_CHILD_TOUCH_BEGAN           if(type == cocos2d::ui::TouchEventType::TOUCH_EVENT_BEGAN)
#define IF_CHILD_TOUCH_MOVED           if(type == cocos2d::ui::TouchEventType::TOUCH_EVENT_MOVED)
#define IF_CHILD_TOUCH_ENDED           if(type == cocos2d::ui::TouchEventType::TOUCH_EVENT_ENDED)
#define IF_CHILD_TOUCH_CANCELED        if(type == cocos2d::ui::TouchEventType::TOUCH_EVENT_CANCELED)
#define BUTTON                         cocos2d::ui::Button*
#define BMFONT                         cocos2d::ui::TextBMFont*
#define TEXTFIELD                      cocos2d::ui::TextFiled*
#define TEXT                           cocos2d::ui::Text*
#define LIST_VIEW                      cocos2d::ui::ListView*
#define IMAGE_VIEW                     cocos2d::ui::ImageView*
#define CHECK_BOX                      cocos2d::ui::CheckBox*
#define LAYOUT                         cocos2d::ui::Layout*
#define SCROLL_VIEW                    cocos2d::ui::ScrollView*
#define PROGRESS_BAR                   cocos2d::ui::LoadingBar*
/********************************************** code define ****************************************/

/********************************************** static define ****************************************/
#define COMMON_LAYOUT                  "commonLayout"
#define BMF_LAYOUT                     "bmfLayout"
#define NUM_LAYOUT                     "numLayout"
#define HERO_LAYOUT                    "heroLayout"

#define REMOVE_WONDOW_PANEL            "removeWindowPanel"
#define ASYNC_LOAD_COMPLETE            "loadComplete"
#define SHOW_HERO_INFO_PANEL           "showHeroInfoPanel"

#define HERO_QUALITY_ONE               "HAui_frame_white.png"
#define HERO_QUALITY_TWO               "HAui_frame_green.png"
#define HERO_QUALITY_THREE             "HAui_frame_blue.png"
#define HERO_QUALITY_FOUR              "HAui_frame_violet.png"
#define HERO_QUALITY_FIVE              "HAui_frame_orange.png"
/********************************************** static define ****************************************/

#endif
