#pragma once 

#include <vector>
#include "GT_Aircraft.h"

#include "GT_Skybox.h"
#include "GT_Ocean.h"

enum sceneType
{
    gameplay,
    pauseScene,
    introScene
};



class GT_Scene
{

public:
    GT_Scene();
    GT_Scene(sceneType sceneEnum);

    ~GT_Scene();

    sceneType getCurrentSceneType() { return currentScene_; }

    void integrateScene();
    void renderScene();

    void setAircrafts(std::vector<GT_Aircraft*> aircrafts);

private:

    sceneType currentScene_;
    std::vector<GT_Aircraft*> aircrafts_;

};
