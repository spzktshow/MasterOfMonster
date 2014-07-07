//
//  preloader.cpp
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-14.
//
//

#include "preloader.h"
#include "mainCity.h"
#include "createCharacter.h"
#include "hero.h"
#include "bag.h"
#include "traven.h"

START_LOADING
/*****************************************************************************************/
CREATE_SIGNAL_INSTANCE(PreLoader, preloaderInstance);
void PreLoader::startLoader(int len, ...)
{
    showLoading();
    
    va_list list;
    va_start(list, len);
    totalLoades = len;
    curentLoades = 0;
    for(int i=0; i<len;i++)
    {
        std::string plistName = va_arg(list, char*);
        loadMap.push_back(plistName);
        cocos2d::Director::getInstance()->getTextureCache()->addImageAsync(getResourcePath(plistName),
                                                                           [this](cocos2d::Texture2D* texture)
                                                                           {
                                                                               childLoadComplete();
                                                                               
                                                                           });
    }
    
}

void PreLoader::showLoading()
{
    auto scene = cocos2d::Scene::create();
    loader = LoadPercent::create();
    scene->addChild(loader);
    if(cocos2d::Director::getInstance()->getRunningScene() == nullptr)
        cocos2d::Director::getInstance()->runWithScene(scene);
    else
        cocos2d::Director::getInstance()->replaceScene(scene);
}

void PreLoader::hideLoading()
{
//    loader->removeFromParent();
//    loader = nullptr;
}

void PreLoader::unLoader(int len,...)
{
    va_list list;
    va_start(list, len);
    for(int i=0; i<len;i++)
    {
        std::string plistName = va_arg(list, char*);
        cocos2d::SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(plistName);
        REMOVE_TEXTURE(getResourcePath(plistName));

    }
}

void PreLoader::childLoadComplete()
{
    curentLoades++;
    if(curentLoades >= totalLoades)
    {
        loadComplete();
    }
    else
    {
        int per = curentLoades*100/totalLoades;
        loader->setPercent(per);
    }
}

void PreLoader::loadComplete()
{
    hideLoading();
    
    std::vector<std::string>::iterator it = loadMap.begin();
    while(it != loadMap.end())
    {
        auto cache = cocos2d::SpriteFrameCache::getInstance();
        if(cache)
            cache->addSpriteFramesWithFile(*it);
        CCLOG("current load resource is %s",it->c_str());
        it++;
    }
    loadMap.clear();
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(ASYNC_LOAD_COMPLETE);
    if(completeCallback)
    {
        completeCallback();
        completeCallback = nullptr;
    }
}
/*****************************************************************************************/



/******************************* 创建角色 **************************************/
void PreLoader::loadCreateCharcater(PRELOADER_STD_FUN instance)
{
    completeCallback = instance;
    if(instance == nullptr)
    {
        completeCallback = []()
        {
            REPLACE_SCENE(SelectCharacterPanel);
        };
    }
    startLoader(5,SKILL_ICON,HERO_7100418_ANIMATE,HERO_7100419_1_ANIMATE,HERO_7100418_2_ANIMATE,CREATE_CHARACTER_UI);
}
/******************************* 创建角色 **************************************/


/******************************* 登录 **************************************/
void PreLoader::loadLoginPanel(PRELOADER_STD_FUN instance)
{
    completeCallback = instance;
    
    startLoader(3,COMMON_UI,REGISTER_PLAYER_PLIST,LOADING_UI);
}

void PreLoader::unLoadLoginPanel()
{
    unLoader(1,REGISTER_PLAYER_PLIST);
}
/******************************* 登录 **************************************/


void PreLoader::LoadCommonUI(PRELOADER_STD_FUN instance)
{
    
}


/******************************* 主城 **************************************/
void PreLoader::loadMainCity(PRELOADER_STD_FUN instance)
{
    if(instance)
    {
        completeCallback = instance;
    }
    else
    {
        completeCallback = []()
        {
            REPLACE_SCENE(MainCityScene);
        };
    }
    
    startLoader(3,MAIN_UI,MAIN_BUILD_EFFECT,MAIN_CLOUD_EFFECT);
}
/******************************* 主城 **************************************/


/*******************************酒馆**************************************/
void PreLoader::loadTraven(PRELOADER_STD_FUN instance)
{
    CHECK_CALLBACK(traven::TravenPanel);
    startLoader(9,TRAVEN_UI,TRAVEN_HERO_7200100,TRAVEN_HERO_7200200,TRAVEN_HERO_7200300,TRAVEN_HERO_7200400,TRAVEN_HERO_7200500,TRAVEN_HERO_7200600,TRAVEN_HERO_7200700,TRAVEN_HERO_7300100);
}
/*******************************酒馆**************************************/



/*******************************英雄查看**************************************/
void PreLoader::loadHero(PRELOADER_STD_FUN instance)
{
    CHECK_CALLBACK(HeroLookAtPanel)
    startLoader(3,HERO_BODY,HERO_UI,HERO_HEAD);
}

void PreLoader::loadHeroInfo(PRELOADER_STD_FUN instance)
{
    CHECK_CALLBACK(HeroPanel);
    immediatorComplete();
}

void PreLoader::LoadBag(PRELOADER_STD_FUN instance)
{
    CHECK_CALLBACK(BagMainPanel);
    startLoader(1,BAG_UI);
}

void PreLoader::immediatorComplete()
{
    if(completeCallback)
    {
        completeCallback();
        completeCallback = nullptr;
    }

}

std::string PreLoader::getResourcePath(std::string pszPlist)
{
    std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(pszPlist);
    cocos2d::ValueMap dict = cocos2d::FileUtils::getInstance()->getValueMapFromFile(fullPath);
    
    std::string texturePath("");
    
    if (dict.find("metadata") != dict.end())
    {
        cocos2d::ValueMap& metadataDict = dict["metadata"].asValueMap();
        // try to read  texture file name from meta data
        texturePath = metadataDict["textureFileName"].asString();
    }
    
    if (!texturePath.empty())
    {
        // build texture path relative to plist file
        texturePath = cocos2d::FileUtils::getInstance()->fullPathFromRelativeFile(texturePath.c_str(), pszPlist);
    }
    else
    {
        // build texture path by replacing file extension
        texturePath = pszPlist;
        
        // remove .xxx
        size_t startPos = texturePath.find_last_of(".");
        texturePath = texturePath.erase(startPos);
        
        // append .png
        texturePath = texturePath.append(".png");
        
        CCLOG("cocos2d: SpriteFrameCache: Trying to use file %s as texture", texturePath.c_str());
    }
    
    return texturePath;
}


CREATE_FUNC_WITH_INIT(LoadPercent,
{
    label = cocos2d::Label::create();
    label->setPosition(cocos2d::Point(200,200));
    addChild(label);
})

void LoadPercent::setPercent(int per)
{
    label->setString(cocos2d::StringUtils::toString(per));
}

LoadCircle::LoadCircle()
{
    
    layer = cocos2d::LayerColor::create(cocos2d::Color4B(0,0,0,255), designResolutionSize.width, designResolutionSize.height);
    addChild(layer);
    
    loading = cocos2d::Sprite::create();
    mvc::AnimatInfo info;
    info.animationName = "httpLoading_01";
    info.duration = 0.1f;
    info.frameName = "httpLoading_01_00%02d.png";
    info.totalFrames = 7;
    loading->runAction(cocos2d::RepeatForever::create(mvc::UITool::getAnimateWithInfo(info)));
    
    MOVE_CENTER(loading);
    addChild(loading);

}

void LoadCircle::onExit()
{
    hasExit = true;
    Layer::onExit();
    CCLOG("-----------------------------LoadCircle::onExit()---------------  scene tag %d",cocos2d::Director::getInstance()->getRunningScene()->getTag());

}

CREATE_SIGNAL_INSTANCE(HttpLoading,httpLoadingInstance);
HttpLoading::HttpLoading()
{
    circle = nullptr;
    loadCount = 0;
}

void HttpLoading::showHttpLoading()
{
    //if(!PreLoader::getInstance()->curentLayer)return;
    circle = new LoadCircle();
    circle->hasExit = false;
    circle->autorelease();
    //PreLoader::getInstance()->curentLayer->addChild(circle);
    PreLoader::getInstance()->currentScene->addChild(circle);
     CCLOG("-----------------------------showHttpLoading  scene tag %d",cocos2d::Director::getInstance()->getRunningScene()->getTag());
    loadCount++;
    
}

void HttpLoading::hideHttpLoading()
{
    //if(!PreLoader::getInstance()->curentLayer)return;
    loadCount--;
    CCLOG("--------------------------------------------current LoadCount is %d",loadCount);
    if(loadCount != 0)
    {
        CCLOG("current LoadCount is %d",loadCount);
        circle = nullptr;
        return;
    }
    if(circle && !circle->hasExit)
    {
        circle->removeFromParent();
        circle = nullptr;
    }
}

CREATE_SIGNAL_INSTANCE(Alert,alertInstance);
void Alert::showHttpError(std::string info)
{
    CCLOG("Error http info is %s",info.c_str());
}

void Alert::showNotice(std::string info)
{
    CCLOG("Notice info is %s",info.c_str());
}
    
END_LOADING
