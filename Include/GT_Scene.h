#pragma once 

#include <vector>
#include "GT_Aircraft.h"

#include "GT_Skybox.h"
#include "GT_Ocean.h"
#include "GT_HUD.h"

enum sceneType
{
    gameplay,
    pauseScene,
    introScene
};



class GT_Scene
{

public:
    GT_Scene(GT_Camera* tempCam);
    GT_Scene(sceneType sceneEnum);

    ~GT_Scene();

    sceneType getCurrentSceneType() { return currentScene_; }

    void renderScene();

    void setAircrafts(std::vector<GT_Aircraft*> aircrafts) { aircrafts_ = aircrafts; }

    void setCamera(GT_Camera* tempCam) { camera_ = tempCam; }

private:

    GLfloat lastFrame_;

    void integrateScene(GLfloat dT);


    sceneType currentScene_;
    std::vector<GT_Aircraft*> aircrafts_;

    GT_Skybox* skybox_;
    GT_Ocean*  ocean_;

    GT_Camera* camera_;

};
