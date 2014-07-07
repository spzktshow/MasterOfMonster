//
//  SmartLoadScene.h
//  LengendOfFightDemons
//
//  Created by RockLee on 14-4-15.
//
//

#ifndef __LengendOfFightDemons__SmartLoadScene__
#define __LengendOfFightDemons__SmartLoadScene__

#include "LoadScene.h"

#define TYPE_SCENE_DATA     "typeSceneData"

#define MY_ACTOR_ID         71003
#define MY_ACTOR_PINGZHI_ID 710030

NS_LOFD_BEGIN

class LoadSceneDataContext : public lofd::LoadContext
{
public:
    LoadSceneDataContext(std::string typeValue, std::string pathValue, int idValue):LoadContext(typeValue, pathValue){
        sceneId = idValue;
    };
    ~LoadSceneDataContext(){};
    
    static LoadSceneDataContext * create(std::string typeValue, std::string pathValue, int idValue);
    
    int sceneId;
};

class SmartLoadScene : public lofd::LoadScene
{
public:
    SmartLoadScene(){};
    ~SmartLoadScene(){};
    
    virtual void loadActorStateData(std::vector<int> stateIds);
    virtual void loadSkillData(lofd::LoadContext * loadContextValue);
    virtual void update(float dt);
    
    CREATE_FUNC(SmartLoadScene);
    
    virtual bool init();
protected:
    virtual void load(lofd::LoadContext * loadContextValue);
    virtual void calculatePrecent();
    
    virtual void loadSceneData(lofd::LoadContext * loadContextValue);
    
    virtual void totalLoadComplete();
};

NS_LOFD_END;

#endif /* defined(__LengendOfFightDemons__SmartLoadScene__) */
