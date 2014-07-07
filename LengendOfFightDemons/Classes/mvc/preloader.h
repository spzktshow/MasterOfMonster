//
//  preloader.h
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-14.
//
//

#ifndef __LengendOfFightDemons__preloader__
#define __LengendOfFightDemons__preloader__

#include <iostream>
#include "mvcDefine.h"
#include "cocos2d.h"
#include "resourceName.h"
#include "MVCFrame.h"

#define REMOVE_TEXTURE            cocos2d::Director::getInstance()->getTextureCache()->removeTextureForKey

#define CHECK_CALLBACK(SCENE)\
if(instance)\
{\
    completeCallback = instance;\
}\
else\
{\
    completeCallback = []()\
    {\
        REPLACE_SCENE(SCENE);\
    };\
}


#define PRELOADER_STD_FUN         std::function<void(void)>
#define START_LOADING             namespace loading{
#define END_LOADING               }
START_LOADING

class LoadPercent;
class PreLoader
{
public:
    DEFINE_SIGANL_INSTANCE(PreLoader);
    PreLoader()
    {
        completeCallback = nullptr;
        loader=nullptr;
        curentLayer = nullptr;
    }
    void startLoader(int len,...);
    void loadCreateCharcater(PRELOADER_STD_FUN instance=nullptr);
    void loadMainCity(PRELOADER_STD_FUN instance=nullptr);
    void loadHero(PRELOADER_STD_FUN instance=nullptr);
    void loadHeroInfo(PRELOADER_STD_FUN instance=nullptr);
    void LoadBag(PRELOADER_STD_FUN instance=nullptr);
    void loadLoginPanel(PRELOADER_STD_FUN instance=nullptr);
    void unLoadLoginPanel();
    void LoadCommonUI(PRELOADER_STD_FUN instance=nullptr);
    
    void loadTraven(PRELOADER_STD_FUN instance=nullptr);
    
    cocos2d::Layer* curentLayer;
    cocos2d::Scene* currentScene;
private:
    std::string getResourcePath(std::string);
    int totalLoades;
    int curentLoades;
    std::vector<std::string> loadMap;
    void loadComplete();
    void childLoadComplete();
    PRELOADER_STD_FUN completeCallback;
    void unLoader(int len,...);
    
    void showLoading();
    void hideLoading();
    void immediatorComplete();
    LoadPercent* loader;
};

class LoadCircle:public cocos2d::Layer
{
public:
    LoadCircle();
    virtual void onExit();
    bool hasExit;
private:
    cocos2d::Sprite* loading;
    cocos2d::LayerColor* layer;
};

class LoadPercent:public cocos2d::Layer
{
public:
    DEFINE_CREATE_FUNC_WITH_INIT(LoadPercent);
    cocos2d::Label* label;
    void setPercent(int per);
};

class HttpLoading
{
public:
    DEFINE_SIGANL_INSTANCE(HttpLoading);
    HttpLoading();
    void showHttpLoading();
    void hideHttpLoading();
private:
    LoadCircle* circle;
    int loadCount;
};

class Alert
{
public:
    DEFINE_SIGANL_INSTANCE(Alert);
    void showHttpError(std::string info);
    void showNotice(std::string info);
};
END_LOADING
#endif /* defined(__LengendOfFightDemons__preloader__) */
