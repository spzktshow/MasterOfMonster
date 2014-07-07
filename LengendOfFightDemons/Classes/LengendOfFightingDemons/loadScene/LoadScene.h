//
//  LoadScene.h
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-15.
//
//

#ifndef __LengendOfFightDemons__LoadScene__
#define __LengendOfFightDemons__LoadScene__

#include "lengendOfFightDemons.h"
#include "FileFormat.h"
NS_LOFD_BEGIN

#define TYPE_CONFIG_JSON        "typeConfigJSON"
#define TYPE_TEXTURE_PLIST      "typeTexturePlist"
#define TYPE_TEXTURE            "typeTexture"

#define TYPE_SCENE              "typeScene"

class LoadContext : public cocos2d::Ref
{
public:
    LoadContext(std::string typeValue, std::string pathValue){
        type = typeValue;
        path = pathValue;
    };
    ~LoadContext(){};
    
    std::string type;
    std::string path;
    
    static LoadContext * create(std::string typeValue, std::string pathValue);
};

class LoadConfigContext : public lofd::LoadContext
{
public:
    LoadConfigContext(std::string typeValue, std::string pathValue, std::string configTypeValue):LoadContext(typeValue, pathValue),configType(configTypeValue){
    }
    ~LoadConfigContext(){};
    
    std::string configType;
    
    static LoadConfigContext * create(std::string typeValue, std::string pathValue, std::string configTypeValue);
};

class LoadTextureAndPlistContext : public lofd::LoadContext
{
public:
    LoadTextureAndPlistContext(std::string typeValue, std::string pathName, std::string fileFormat):LoadContext(typeValue, pathName),texturePath(pathName + fileFormat),plistPath(pathName + FILE_PLIST)
    {};
    ~LoadTextureAndPlistContext(){};
    
    std::string texturePath;
    std::string plistPath;
    
    static LoadTextureAndPlistContext * create(std::string typeValue, std::string pathName, std::string fileFormat);
};

class LoadTextureContext : public lofd::LoadContext
{
public:
    LoadTextureContext(std::string typeValue, std::string pathValue):LoadContext(typeValue, pathValue){};
    ~LoadTextureContext(){};
    
    static LoadTextureContext * create(std::string typeValue, std::string pathValue);
};

class LoadSceneUtils
{
public:
    static cocos2d::Vector<lofd::LoadTextureContext *> createBySceneConfig();
};

class LoadScene : public cocos2d::Scene
{
public:
    LoadScene():isLoading(false)
    , currentIndex(0)
    , processRect(0, 0, 1, 50)
    , maxProcessLength(600)
    {};
    ~LoadScene(){
    };
    
    /********进度条矩形**********/
    cocos2d::Rect processRect;
    float maxProcessLength;
    
    void setLoadContexts(cocos2d::Vector<lofd::LoadContext *> loadContextsValue);
    cocos2d::Vector<lofd::LoadContext *> loadContexts;
    
    void start();//it's auto when onEnter handler
    
    virtual void onEnterTransitionDidFinish();

    virtual void draw(cocos2d::Renderer *renderer, const kmMat4& transform, bool transformUpdated);
    
    virtual bool init();
    CREATE_FUNC(LoadScene);
protected:
    bool isLoading;
    int currentIndex;
    
    virtual void loadConfigJson(lofd::LoadContext * loadContextValue);
    virtual void loadTexture(lofd::LoadContext * loadContextValue);
    virtual void loadTextureAndPlist(lofd::LoadContext * loadContextValue);
    
    virtual void loadTextureAndPlistAsyncHandler(cocos2d::Texture2D *textureValue);
    virtual void loadTextureHandler(cocos2d::Texture2D *textureValue);
    
    virtual void load(lofd::LoadContext * loadContextValue);
    
    virtual void loadComplete();
    virtual void totalLoadComplete();
    
    cocos2d::CustomCommand _customCommand;
    virtual void onDraw(const kmMat4 &transform, bool transformUpdated);
    
    virtual void calculatePrecent();
    
    cocos2d::LabelTTF * label;
};

NS_LOFD_END;
#endif /* defined(__LengendOfFightDemons__LoadScene__) */
