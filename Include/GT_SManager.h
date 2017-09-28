#pragma once

#include <map>
#include "GT_Scene.h"



class GT_SManager
{
public:
    GT_SManager();

    void activateScene(sceneType sceneEnum) ;

    GT_Scene* getActiveScene() { return currentScene_; }

    ~GT_SManager();

private:

    std::map<sceneType, GT_Scene*> sceneMap_;

    GT_Scene* currentScene_;
};
