#include "GT_SManager.h"

#include "GT_IntroScene.h"
#include "GT_MenuScene.h"
#include "GT_GameplayScene.h"
#include "GT_GameoverScene.h"
#include "GT_PauseScene.h"
#include "GT_CreditScene.h"


GT_SManager::GT_SManager()
{
    sceneMap_.insert(std::pair< sceneType, GT_Scene*>(introScene, new GT_IntroScene()));
    sceneMap_.insert(std::pair< sceneType, GT_Scene*>(menuScene, new GT_MenuScene()));
    sceneMap_.insert(std::pair< sceneType, GT_Scene*>(gameplay, new GT_GameplayScene()));
    sceneMap_.insert(std::pair< sceneType, GT_Scene*>(gameover, new GT_GameoverScene()));
    sceneMap_.insert(std::pair< sceneType, GT_Scene*>(pauseScene, new GT_PauseScene()));
    sceneMap_.insert(std::pair< sceneType, GT_Scene*>(credits, new GT_CreditScene()));
    sceneMap_.insert(std::pair< sceneType, GT_Scene*>(exitGame, nullptr ));

    activateScene( introScene );
}


void GT_SManager::activateScene(sceneType sceneEnum)
{
    currentScene_ = sceneMap_[sceneEnum];
}

GT_SManager::~GT_SManager()
{
    std::map<sceneType,GT_Scene*>::iterator sit;
    for (sit =sceneMap_.begin(); sit != sceneMap_.end(); sit++ )
    {
        if (sit != sceneMap_.end())
            if (sit->second != nullptr)
                delete sit->second;
    }


}
