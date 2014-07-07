//
//  MVCFrame.cpp
//  TestDemo
//
//  Created by tanzuoliang on 14-4-9.
//
//

#include "MVCFrame.h"
#include "json/prettywriter.h"

#define HTTP_IP            "http://192.168.1.240:8080/web/gateway.php"
#include "ApplicationContext.h"
#include "mainCity.h"
START_MVC


void mvc::MVCCommand::execute(cocos2d::ValueMap* map)
{
    
}

MVCCommand::MVCCommand()
{
    service = MVCService::getInstance();
    //service->retain();
}

MVCCommand::~MVCCommand()
{
    //service->release();
}

void Model::publishNotice(std::string notice)
{
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(notice);
}

void Mediator::sendCommand(std::string command,cocos2d::ValueMap* map)
{
    mvc::MVCCommandMap::getInstance()->sendCommandByName(command,map);
}

void Mediator::initMediator()
{
    for(auto name:attentionVector)
    {
        MediatorListenerMap::getInstance()->pushNode(name);
        eventMap[name] = cocos2d::Director::getInstance()->getEventDispatcher()->addCustomEventListener
        (name,
         [this](cocos2d::EventCustom* custom)
         {
             
             if(attentionCallback)
             {
                 attentionCallback(custom->getEventName(),custom->getUserData());
             }
         }
         
         
         );
        
    }
    
}

Mediator::~Mediator()
{
    
    for(auto name:attentionVector)
    {
        if(MediatorListenerMap::getInstance()->getCountByName(name) == 0)
        {
            cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(name);
        }
        else
        {
            cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(eventMap.at(name));
        }
    }
    
}

void Mediator::registerAttentions(int len,...)
{
    va_list list;
    va_start(list, len);
    for (int i= 0; i < len; i++) {
        attentionVector.push_back((std::string)va_arg(list, char*));
    }
    va_end(list);
    
    initMediator();
}



CREATE_SIGNAL_INSTANCE(MediatorListenerMap, mediatorListenerMapInstance);

void MediatorListenerMap::pushNode(std::string name)
{
    int count = 1;
    if(map.find(name) == map.end())
    {
        map.insert(std::make_pair(name, count));

    }
    else
    {
        count = map[name] + 1;
        map[name] = count;
    }
}

int MediatorListenerMap::getCountByName(std::string name)
{
    int count = map.at(name);
    map[name] = --count;
    return count;
}

void OrionPanel::onExit()
{
    Node::onExit();
    if(mediator)
        mediator->release();
    
    showEffect = false;
    rightPanel = nullptr;
}

void OrionPanel::showAddEffect()
{
    this->setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);
    this->setTouchEnabled(true);
    showEffect = true;
    if(container)
    {
        container->setAnchorPoint(cocos2d::Point(0.5f,0.5f));
        container->setScale(0.1f);
//        cocos2d::Size size = cocos2d::Director::getInstance()->getVisibleSize();
//        cocos2d::Point ori = cocos2d::Director::getInstance()->getVisibleOrigin();
//        rootWidget->setPosition(cocos2d::Point(ori.x + size.width / 2, ori.y + size.height / 2));
        addChildToCenter(container);
        auto scale = cocos2d::ScaleTo::create(0.1f, 1);
        auto callFunc = cocos2d::CallFunc::create([this](){onReallyShow();});
        container->runAction(cocos2d::Sequence::create(scale,callFunc,NULL));

    }
}

void OrionPanel::addChildToCenter(cocos2d::Node* child)
{
    cocos2d::Size size = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Point ori = cocos2d::Director::getInstance()->getVisibleOrigin();
    child->setPosition(ori.x + size.width / 2, offsetY +　ori.y + size.height / 2);
}

void OrionPanel::addSubPanelToRight(mvc::OrionPanel* panel,int dir)
{
    if(rightPanel == nullptr)
    {
        cocos2d::Size rootSize = rootWidget->getContentSize();
        cocos2d::Size panelSize = panel->getContentSize();
        cocos2d::Size screenSize = designResolutionSize;
        
        float halfScreenWidth = screenSize.width / 2;
        
        cocos2d::MoveBy* leftMove;
        cocos2d::MoveBy* rightMove;
        int startX = (screenSize.width - (rootSize.width + panelSize.width)) / 2;
        if(dir == 1)
        {
            
             leftMove = cocos2d::MoveBy::create(0.1f,cocos2d::Point(startX + (rootSize.width/2) - halfScreenWidth,0));
             rightMove = cocos2d::MoveBy::create(0.1f,cocos2d::Point(startX + rootSize.width + (panelSize.width/2) - halfScreenWidth,0));
            
             container->runAction(leftMove);
             panel->runAction(rightMove);
        }
        else
        {
            leftMove = cocos2d::MoveBy::create(0.1f,cocos2d::Point(startX + (panelSize.width/2) - halfScreenWidth,0));
            rightMove = cocos2d::MoveBy::create(0.1f,cocos2d::Point(startX + panelSize.width + (rootSize.width/2)- halfScreenWidth,0));
            
            container->runAction(rightMove);
            panel->runAction(leftMove);
        }

    }
    else
    {
        panel->setPosition(rightPanel->getPosition());
        rightPanel->removeFromParent();
    }
    rightPanel = panel;
    this->addChild(panel);
}

void OrionPanel::moveChildToCenter(cocos2d::Node* child)
{
    cocos2d::Size size = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Point ori = cocos2d::Director::getInstance()->getVisibleOrigin();
    auto move = cocos2d::MoveTo::create(0.1f,cocos2d::Point(ori.x + size.width / 2, offsetY + ori.y + size.height / 2));
    if(child != nullptr)child->runAction(move);
    else
    {
        container->runAction(move);
        rightPanel = nullptr;
    }
}

void OrionPanel::addResourceBar(bool addResourcePanel,bool showAddPaneleffct)
{
    if(addResourcePanel)
        this->addChild(Resource::create());
    if(showAddPaneleffct)
        this->showAddEffect();
}

void OrionPanel::showWidget(std::string widgetName,bool addBg,bool centerHandler,bool containsCloseBtn,bool autoCreateBg,std::string title,TitleStyle titleStyle,float oy,std::string bgFrameName)
{
    offsetY = oy;
    if(addBg)
    {
//        auto bg = cocos2d::LayerColor::create(cocos2d::Color4B(0,0,0,55), cocos2d::Director::getInstance()->getVisibleSize().width, cocos2d::Director::getInstance()->getVisibleSize().height);
        //auto bg = cocos2d::Sprite::create("panelBg.png");
        auto bg = cocos2d::Sprite::createWithSpriteFrameName(UI_SCREEN_BG);
        bg->setAnchorPoint(cocos2d::Point(0,0));
        this->addChild(bg);
    }
    
    container = cocos2d::Node::create();
    container->setAnchorPoint(cocos2d::Point(0.0f,0.0f));
    this->addChild(container);
    
    rootWidget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(widgetName.c_str());
    
    if(autoCreateBg)
    {
        BaseUIFrame* frame = BaseUIFrame::create();
        frame->setFrameSize(rootWidget->getSize().width, rootWidget->getSize().height, title,titleStyle,bgFrameName);
        closeBtn = frame->closeBtn;
        closeBtn->addTouchEventListener(this,cocos2d::ui::SEL_TouchEvent(&OrionPanel::onChildClick));
        if(centerHandler)
        {
            //frame->setAnchorPoint(cocos2d::Point(0.5f,0.5f));
            float _x =  -rootWidget->getSize().width/2;
            float _y =  -rootWidget->getSize().height/2;
            frame->setPosition(cocos2d::Point(_x,_y));
            frame->colorLayer->setPosition(cocos2d::Point(_x,_y));
        }
        
        container->addChild(frame->colorLayer);
        container->addChild(rootWidget);
        container->addChild(frame);
        
    }
    else
    {
        container->addChild(rootWidget);
    }
    
    if(centerHandler)
    {
        rootWidget->setAnchorPoint(cocos2d::Point(0.5f,0.5f));
        this->setContentSize(rootWidget->getContentSize());
        //container->setAnchorPoint(cocos2d::Point(0.5f,0.5f));
        addChildToCenter(container);
    }

    
    if(containsCloseBtn)
        closeBtn = getButtonFromeWidgetWithWidgetName(rootWidget,COMMON_LAYOUT,"closeBtn");

}

void OrionPanel::showRemoveEffect()
{
    cocos2d::ScaleTo* scale = cocos2d::ScaleTo::create(0.1f, 0.1f);
    cocos2d::Sequence* seq = cocos2d::Sequence::create(scale,cocos2d::CallFunc::create([this]()
    {
        
        if(closeEventTage == REMOVE_WONDOW_PANEL)
        {
            REPLACE_SCENE(MainCityScene);
        }
        else
        {
            publishEvent(closeEventTage);
            this->removeFromParent();
        }
    }),NULL);

    if(rightPanel)
    {
        this->runAction(seq);
    }
    else
    {
        container->runAction(seq);
    }
}

bool OrionPanel::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
//    cocos2d::Point p = this->getPosition();
//    cocos2d::Size size = this->getContentSize();
      cocos2d::Point touchPoint = touch->getLocation();
//    if(touchPoint.x > p.x && touchPoint.x < p.x +　size.width && touchPoint.y > )
    cocos2d::Rect rect = this->getBoundingBox();
    if(touchPoint.x > rect.origin.x && touchPoint.x < rect.origin.x + rect.size.width && touchPoint.y > rect.origin.y && touchPoint.y < rect.origin.y +rect.size.height)
    {
        return true;
    }
    return false;
}

//OrionPanelType OrionPanel::getPanelType()
//{
//    return panelType;
//}

void OrionPanel::publishEvent(std::string name,void *optionalUserData)
{
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(name);
}

void OrionPanel::registerMeditor(mvc::Mediator* instance)
{
    mediator = instance;
}

cocos2d::ui::Button* OrionPanel::getButtonFromeWidget(cocos2d::ui::Widget* layout,std::string name,bool addListener)
{
    //auto btn = dynamic_cast<cocos2d::ui::Button*>(layout->getChildByName(name.c_str()));
    auto btn = UITool::getButtonFromeWidget(layout,name);
    if(btn && addListener)
        btn->addTouchEventListener(this,cocos2d::ui::SEL_TouchEvent(&OrionPanel::onChildClick));
    return btn;
}

cocos2d::ui::Button* OrionPanel::getButtonFromeWidgetWithWidgetName(cocos2d::ui::Widget* layout,std::string widgetName,std::string name,bool addListener)
{
    //auto btn = dynamic_cast<cocos2d::ui::Button*>(layout->getChildByName(widgetName.c_str())->getChildByName(name.c_str()));
    auto btn = UITool::getButtonFromeWidgetWithWidgetName(layout,widgetName,name);
    if(btn && addListener)
        btn->addTouchEventListener(this,cocos2d::ui::SEL_TouchEvent(&OrionPanel::onChildClick));
    return btn;
}

cocos2d::ui::ListView* OrionPanel::getListViewFromeWidgetWithWidgetName(cocos2d::ui::Widget* layout,std::string widgetName,std::string name)
{
//    auto listView = dynamic_cast<cocos2d::ui::ListView*>(layout->getChildByName(widgetName.c_str())->getChildByName(name.c_str()));
//    return listView;
    return UITool::getListViewFromeWidgetWithWidgetName(layout,widgetName,name);
}

cocos2d::ui::ImageView* OrionPanel::getImageViewFromeWidget(cocos2d::ui::Widget *layout, std::string name,bool addListener)
{
    //auto image = dynamic_cast<cocos2d::ui::ImageView*>(layout->getChildByName(name.c_str()));
    auto image = UITool::getImageViewFromeWidget(layout,name);
    if(image && addListener)
        image->addTouchEventListener(this,cocos2d::ui::SEL_TouchEvent(&OrionPanel::onChildClick));
    return image;
}

cocos2d::ui::ImageView* OrionPanel::getImageViewFromeWidgetWithName(cocos2d::ui::Widget *layout, std::string widgetName,std::string name,bool addListener)
{
   // auto image = dynamic_cast<cocos2d::ui::ImageView*>(layout->getChildByName(widgetName.c_str())->getChildByName(name.c_str()));
    auto image = UITool::getImageViewFromeWidgetWithName(layout,widgetName,name);
    if(image && addListener)
        image->addTouchEventListener(this,cocos2d::ui::SEL_TouchEvent(&OrionPanel::onChildClick));
    return image;
}

cocos2d::ui::TextBMFont* OrionPanel::getTextBMFFromWidgetWithName(cocos2d::ui::Widget* layout,std::string widgetName,std::string name)
{
//    auto bmf = dynamic_cast<cocos2d::ui::TextBMFont*>(layout->getChildByName(widgetName.c_str())->getChildByName(name.c_str()));
//    return bmf;
    return UITool::getTextBMFFromWidgetWithName(layout,widgetName,name);
}

cocos2d::ui::Text* OrionPanel::getTextFromWidgetWithName(cocos2d::ui::Widget* layout,std::string widgetName,std::string name)
{
    return UITool::getTextFromWidgetWithName(layout,widgetName,name);
}

cocos2d::ui::LoadingBar* OrionPanel::getLoadingBarFromWidgetWithName(cocos2d::ui::Widget* layout,std::string widgetName,std::string name)
{
    return UITool::getLoadingBarFromWidgetWithName(layout,widgetName,name);

}

cocos2d::ui::Layout* OrionPanel::getLayoutFromWidget(cocos2d::ui::Widget* layout,std::string name,bool addListener)
{
    auto _panel = UITool::getLayoutFromWidget(layout,name);
    if(_panel && addListener)
    {
        //_panel->setTouchEnabled(addListener);
        _panel->addTouchEventListener(this,cocos2d::ui::SEL_TouchEvent(&OrionPanel::onChildClick));
    }
    return _panel;
}


void OrionPanel::onChildClick(cocos2d::Ref *ref, cocos2d::ui::TouchEventType type)
{
    if(childCickCallBack)
        childCickCallBack(ref,type);
    if(type == cocos2d::ui::TouchEventType::TOUCH_EVENT_ENDED)
    {
        if(ref == closeBtn)
        {
            closePanel();
        }
    }
}

void OrionPanel::closePanel()
{
    if (showEffect) {
        showRemoveEffect();
    }
    else
    {
        publishEvent(closeEventTage);
        this->removeFromParent();
    }

}

void BaseItemRender::showWidget(std::string fileName)
{
    rootWidget = UITool::widgetFromJson(fileName);
    
    this->addChild(rootWidget);
    this->setSize(rootWidget->getSize());
}

void BaseItemRender::registerEvent(cocos2d::ui::Widget* child)
{
    child->addTouchEventListener(this,cocos2d::ui::SEL_TouchEvent(&BaseItemRender::onChildClick));
}

void BaseItemRender::onChildClick(cocos2d::Ref *ref, cocos2d::ui::TouchEventType type)
{
    if(childCickCallBack)
        childCickCallBack(ref,type);
}

BaseItemRender* ItemGroup::getItem()
{
    return lastItem;
}

ItemGroup::ItemGroup()
{
    lastItem=nullptr;
    //itemClickCallback = nullptr;
}

void ItemGroup::selectItem(BaseItemRender* item)
{
    if(lastItem == item)
    {
        CCLOG("---------选择相同 --------------------");
        return;
    }
    if(lastItem)
        lastItem->select(false);
    
    lastItem = item;
    
    if(item)
    {
        item->select(true);
        
        if(itemClickCallback)
        {
            itemClickCallback(item);
        }

    }
}

static MVCCommandMap *_sharedMap = nullptr;
MVCCommandMap* MVCCommandMap::getInstance()
{
    if(!_sharedMap)
    {
        _sharedMap = new MVCCommandMap();
        //_sharedMap->_commandMap = new cocos2d::__Dictionary();
    }
    return _sharedMap;
}

void MVCCommandMap::registerCommand(const std::string commandName, mvc::MVCCommand* command)
{
    if(!_commandMap.objectForKey(commandName))
    {
        _commandMap.setObject(command, commandName);
    }
}

void MVCCommandMap::registerCommand(const std::string commandName, CommandInstance instance)
{
    commandClassMap[commandName] = instance;
//    if(!commandClassMap.objectForKey(commandName))
//    {
//        commandClassMap.setObject(instance, commandName);
//    }

}

bool MVCCommandMap::unRegisterCommand(const std::string  commandName)
{
    commandClassMap.erase(commandName);
//    if(commandClassMap.objectForKey(commandName))
//    {
//        commandClassMap.removeObjectForKey(commandName);
//        return true;
//    }
    
    if(_commandMap.objectForKey(commandName))
    {
        _commandMap.removeObjectForKey(commandName);
        return true;
    }
    
    return false;
}

void MVCCommandMap::sendCommandByName(const std::string commandName,cocos2d::ValueMap* map)
{
//    auto command = _commandMap.objectForKey(commandName);
//    if(command)
//    {
//        mvc::MVCCommand* cmd = (mvc::MVCCommand*)command;
//        if(cmd)
//        {
//            cmd->execute(map);
//            delete map;
//        }
//    }
    CommandInstance command = commandClassMap[commandName];
    if(command)
    {
        mvc::MVCCommand* cmd = command();
        if(cmd)
        {
            cmd->execute(map);
        }

    }
}

static MVCService* instance=nullptr;

MVCService* MVCService::getInstance()
{
    if(instance==nullptr)
        instance = new MVCService();
    return instance;
}

MVCService::MVCService()
{
    //httpCallBackMap = new cocos2d::__Dictionary();
    //socketCallBackMap = new cocos2d::__Dictionary();
    
    httpClient = cocos2d::network::HttpClient::getInstance();
    socket = new cocos2d::network::WebSocket();
}

void  MVCService::registerHttpCallBack(const std::string& action, const std::string& method, const std::function<void(HTTP_RESPONSE_PARMA)>& fun)
{
     if(httpCallBackMap.find(action) == httpCallBackMap.end())
     {
         std::map<std::string, std::function<void(HTTP_RESPONSE_PARMA)>> temp;
         //httpCallBackMap.insert(std::map<std::string, std::map<std::string, std::function<void(void* userData)>*>>::value_type(action,temp));
         httpCallBackMap[action] = temp;
     }
    
   // httpCallBackMap[action].insert(std::map<std::string,std::function<void(void* userData)>*>::value_type(method,fun));
    httpCallBackMap[action][method] = fun;
        
}

void MVCService::sendHttp(const char* buffer,int bufferLen)
{
    loading::HttpLoading::getInstance()->showHttpLoading();
    
    cocos2d::network::HttpRequest* req = new cocos2d::network::HttpRequest();
    req->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    req->setResponseCallback(this, cocos2d::network::SEL_HttpResponse(&MVCService::httpRespondeCallback));
    req->setUrl(HTTP_IP);
    req->setRequestData(buffer,bufferLen);
    httpClient->send(req);
    req->release();
    
    CCLOG("require info is %s",buffer);
    
}

void MVCService::httpRespondeCallback(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* reSponse)
{
    loading::HttpLoading::getInstance()->hideHttpLoading();
    
    std::vector<char>* backData = reSponse->getResponseData();
    int len = backData->size();
    std::string responseString;
    for(int i= 0; i< len;i++)
    {
        responseString += backData->at(i);
    }
    CCLOG("get response back info is %s",responseString.c_str());
    
    rapidjson::Document doc;
    doc.Parse<0>(responseString.c_str());
    if(doc.HasParseError())
	{
		CCLOG("GetParserError %s\n",doc.GetParseError());
	}
    else
    {
        if(doc["state"].GetInt() != 1)
        {
            CCLOG("通讯报错呀！");
            loading::Alert::getInstance()->showHttpError(doc["data"]["errorMsg"].GetString());
        }
        else
        {
            
            std::string c = doc["controllerName"].GetString();
            std::string a = doc["actionName"].GetString();
            
            CCLOG("------------------------ c = %s  a = %s-------------------------",c.c_str(),a.c_str());
            
            if(c == WEB_LOGIN && a == WEB_INDEX)
            {
                web_sid = doc["data"]["ret"]["token"].GetString();
            }
            
            Protocol* protocol = ProtocolPool::getInstance()->getProtocolByName(c + a);
            if(protocol)
            {
                protocol->parser((char*)responseString.c_str());
                //return;
            }
            else
            {
                if(httpCallBackMap.find(c) != httpCallBackMap.end())
                {
                    //                const rapidjson::Value& DictionaryHelper::getSubDictionary_json(const rapidjson::Value &root, const char* key)
                    //                {
                    //                    return root[key];
                    //                }
                    httpCallBackMap[c][a](doc["data"]);
                    std::string _eventName = c + "_" + a + "DataInit";
                    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(_eventName);
                }

            }
        }
    }
}


//void BaseServer::WriteNormalInfo(rapidjson::PrettyWriter<rapidjson::InsituStringStream>& write,std::string controlName,std::string actionName)
//{
//    //write.String(LOGIN_BASE_ID);
//    //write.Int(heroID);
//    write.String("c");
//    write.String(controlName);
//    write.String("a");
//    write.String(actionName);
//}

//static ModelPool* modelInstance = nullptr;
CREATE_SIGNAL_INSTANCE(ModelPool, modelInstance);
//ModelPool* ModelPool::getInstance()
//{
//    if(modelInstance == nullptr)
//        modelInstance = new ModelPool();
//    return modelInstance;
//}

ModelPool::ModelPool()
{
    
}

void ModelPool::registerModel(mvc::Model* model, std::string modelName)
{
    if(modelMap.find(modelName) == modelMap.end())
        modelMap[modelName] = model;
}

mvc::Model* ModelPool::getModelByName(std::string modelName)
{
    if(modelMap.find(modelName) != modelMap.end())
        return modelMap[modelName];
    return nullptr;
}

//static ProtocolPool* protocolInstance = nullptr;

CREATE_SIGNAL_INSTANCE(ProtocolPool, protocolInstance);

//ProtocolPool* ProtocolPool::getInstance()
//{
//    if(protocolInstance == nullptr)
//        protocolInstance = new ProtocolPool();
//    return protocolInstance;
//}

ProtocolPool::ProtocolPool()
{
    
}

void ProtocolPool::registerProtocol(mvc::Protocol* protocal, std::string protocolName)
{
    if(protocolMap.find(protocolName) == protocolMap.end())
        protocolMap[protocolName] = protocal;
}

void ProtocolPool::registerProtocol(ProtocolInstance instance,std::string trollerName, std::string actionName)
{
    //std::string protocolName = moonsugar::TypeConver::combinationChar(trollerName,actionName);
    std::string protocolName = trollerName + actionName;
    if(protocolClassMap.find(protocolName) == protocolClassMap.end())
        protocolClassMap[protocolName] = instance;
}

mvc::Protocol* ProtocolPool::getProtocolByName(std::string protocolName)
{
    if(protocolMap.find(protocolName) != protocolMap.end())
    {
        return protocolMap[protocolName];
    }
    else
    {
        ProtocolInstance instance = protocolClassMap[protocolName];
        if(instance)
            return instance();
    }
    return nullptr;
}
/***************************************** POP MANAGER****************************************************/
//CREATE_SIGNAL_INSTANCE(PopUpManager, popUpManagerInstance);
//
//void PopUpManager::popupPanel(mvc::OrionPanel *panel)
//{
//    int len = panelList.size();
//    if(len == 0)
//    {
//        centerPopup(panel);
//        panel->showAddEffect();
//    }
//    else
//    {
//        bool replace_subPaenl = false;
//        
//        for(auto spanel:panelList)
//        {
//            if(spanel->getPanelType() == panel->getPanelType())
//            {
//                spanel->release();
//                spanel->removeFromParent();
//                panelList->erase(spanel);
//                replace_subPaenl = true;
//                break;
//            }
//        }
//    }
//    
//    panel->retain();
//    panelList->push_back(panel);
//    
//    len = panelList.size();
//    if(len == 2)
//    {
//        OrionPanel* firstPanel = panelList.at(0);
//        OrionPanel* secondPanel = panelList.at(1);
//        
//        cocos2d::Size rootSize = firstPanel->getContentSize();
//        cocos2d::Size panelSize = secondPanel->getContentSize();
//        cocos2d::Size screenSize = designResolutionSize;
//        
//        float halfScreenWidth = screenSize.width / 2;
//        
//        cocos2d::MoveBy* leftMove;
//        cocos2d::MoveBy* rightMove;
//        int startX = (screenSize.width - (rootSize.width + panelSize.width)) / 2;
//        
//        if(secondPanel->getLocation() == OrionPanelLocation::RIGHT)
//        {
//            
//            leftMove = cocos2d::MoveBy::create(0.1f,cocos2d::Point(startX + (rootSize.width/2) - halfScreenWidth,0));
//            rightMove = cocos2d::MoveBy::create(0.1f,cocos2d::Point(startX + rootSize.width + (panelSize.width/2) - halfScreenWidth,0));
//            
//            firstPanel->runAction(leftMove);
//            secondPanel->runAction(rightMove);
//        }
//        else
//        {
//            leftMove = cocos2d::MoveBy::create(0.1f,cocos2d::Point(startX + (panelSize.width/2) - halfScreenWidth,0));
//            rightMove = cocos2d::MoveBy::create(0.1f,cocos2d::Point(startX + panelSize.width + (rootSize.width/2)- halfScreenWidth,0));
//            
//            firstPanel->runAction(rightMove);
//            secondPanel->runAction(leftMove);
//        }
//
//    }
//    else
//    {
//        //目前还没遇到 后面补充
//    }
//}
//
//void PopUpManager::popupPanelWithShow(mvc::OrionPanel *panel)
//{
//    popupPanel(panel);
//    cocos2d::Director::getInstance()->getRuningScene()->addChild(panel);
//}
//
//void PopUpManager::removePanel(mvc::OrionPanel *panel)
//{
//    panelList.erase(panel);
//    panel->release();
//    
//    if(panel->getPanelType() == OrionPanelType::SUB_PANEL)
//    {
//        moveToCneter(panelList.at(0));
//    }
//    
//    panel->closePanel();
//}
//
//void PopUpManager::removeAllPanels()
//{
//    for(auto panel:panelList)
//    {
//        if(panel->getPanelType == OrionPanelType::TOP_PANEL)
//        {
//            moveToCenterAndClose(panel);
//        }
//        else
//        {
//            panel->closePanel();
//        }
//    }
//    
//    panelList.clear();
//}
//
//void PopUpManager::moveToCneter(OrionPanel* panel)
//{
//    cocos2d::Size size = cocos2d::Director::getInstance()->getVisibleSize();
//    cocos2d::Point ori = cocos2d::Director::getInstance()->getVisibleOrigin();
//    //panel->setPosition(ori.x + size.width / 2, offsetY +　ori.y + size.height / 2);
//    auto moveTo = cocos2d::MoveTo::create(0.1f,cocos2d::Point(ori.x + size.width / 2, offsetY +　ori.y + size.height / 2));
//    panel->runAction(moveTo);
//}
//
//void PopUpManager::moveToCenterAndClose(OrionPanel* panel)
//{
//    cocos2d::Size size = cocos2d::Director::getInstance()->getVisibleSize();
//    cocos2d::Point ori = cocos2d::Director::getInstance()->getVisibleOrigin();
//    auto moveTo = cocos2d::MoveTo::create(0.1f,cocos2d::Point(ori.x + size.width / 2, offsetY +　ori.y + size.height / 2));
//    auto call = cocos2d::CallFunc::create([&panel](){panel->closePanel();});
//    panel->runAction(cocos2d::Sequence::create(moveTo,call,NULL));
//
//}
//
//void PopUpManager::centerPopup(OrionPanel* panel)
//{
//    cocos2d::Size size = cocos2d::Director::getInstance()->getVisibleSize();
//    cocos2d::Point ori = cocos2d::Director::getInstance()->getVisibleOrigin();
//    panel->setPosition(ori.x + size.width / 2, offsetY +　ori.y + size.height / 2);
//}
/***************************************** POP MANAGER****************************************************/

//CREATE_SIGNAL_INSTANCE(PreLoader, preloaderInstance);
//void PreLoader::startLoader(int len, ...)
//{
//    va_list list;
//    va_start(list, len);
//    totalLoades = len;
//    curentLoades = 0;
//    for(int i=0; i<len;i++)
//    {
//        std::string plistName = va_arg(list, char*);
//        loadMap.push_back(plistName);
//        cocos2d::Director::getInstance()->getTextureCache()->addImageAsync(getResourcePath(plistName),
//                                                                           [this](cocos2d::Texture2D* texture)
//                                                                           {
//                                                                               childLoadComplete();
//                                                                               
//                                                                           });
//    }
//
//}
//
//void PreLoader::childLoadComplete()
//{
//    curentLoades++;
//    
//    if(curentLoades >= totalLoades)
//        loadComplete();
//}
//
//void PreLoader::loadComplete()
//{
//    std::vector<std::string>::iterator it = loadMap.begin();
//    while(it != loadMap.end())
//    {
//        auto cache = cocos2d::SpriteFrameCache::getInstance();
//        if(cache)
//            cache->addSpriteFramesWithFile(*it);
//        CCLOG("current load resource is %s",it->c_str());
//        it++;
//    }
//    
//    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(ASYNC_LOAD_COMPLETE);
//    if(completeCallback)
//    {
//        completeCallback();
//        completeCallback = nullptr;
//    }
//}
//
//void PreLoader::loadCreateCharcater(PRELOADER_STD_FUN instance)
//{
//    completeCallback = instance;
//    
//    startLoader(3,HERO_7100418_ANIMATE,HERO_7100419_1_ANIMATE,HERO_7100418_2_ANIMATE);
//}
//
//void PreLoader::loadLoginPanel(PRELOADER_STD_FUN instance)
//{
//    completeCallback = instance;
//    
//    startLoader(2,COMMON_UI,REGISTER_PLAYER_PLIST);
//}
//
//void PreLoader::LoadCommonUI(PRELOADER_STD_FUN instance)
//{
//    
//}
//
//void PreLoader::loadMainCity(PRELOADER_STD_FUN instance)
//{
//    completeCallback = instance;
//}
//
//void PreLoader::loadHero(PRELOADER_STD_FUN instance)
//{
//    completeCallback = instance;
//}
//
//std::string PreLoader::getResourcePath(std::string pszPlist)
//{
//    std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(pszPlist);
//    cocos2d::ValueMap dict = cocos2d::FileUtils::getInstance()->getValueMapFromFile(fullPath);
//    
//    std::string texturePath("");
//    
//    if (dict.find("metadata") != dict.end())
//    {
//        cocos2d::ValueMap& metadataDict = dict["metadata"].asValueMap();
//        // try to read  texture file name from meta data
//        texturePath = metadataDict["textureFileName"].asString();
//    }
//    
//    if (!texturePath.empty())
//    {
//        // build texture path relative to plist file
//        texturePath = cocos2d::FileUtils::getInstance()->fullPathFromRelativeFile(texturePath.c_str(), pszPlist);
//    }
//    else
//    {
//        // build texture path by replacing file extension
//        texturePath = pszPlist;
//        
//        // remove .xxx
//        size_t startPos = texturePath.find_last_of(".");
//        texturePath = texturePath.erase(startPos);
//        
//        // append .png
//        texturePath = texturePath.append(".png");
//        
//        CCLOG("cocos2d: SpriteFrameCache: Trying to use file %s as texture", texturePath.c_str());
//    }
//
//    return texturePath;
//}
//
//CREATE_SIGNAL_INSTANCE(HttpLoading,httpLoadingInstance);
//HttpLoading::HttpLoading()
//{
//    
//}
//
//void HttpLoading::show()
//{
//    
//}
//
//void HttpLoading::hide()
//{
//    
//}
//
//CREATE_SIGNAL_INSTANCE(Alert,alertInstance);
//void Alert::showHttpError(std::string info)
//{
//    CCLOG("Error http info is %s",info.c_str());
//}
//
//void Alert::showNotice(std::string info)
//{
//    CCLOG("Notice info is %s",info.c_str());
//}

END_MVC