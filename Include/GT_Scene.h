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

enum class keyPressed
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    ENTER,
    ESC
};

class GT_Scene
{

public:
    GT_Scene(GT_Camera* tempCam, sceneType sceneEnum);
    GT_Scene();

    ~GT_Scene();

    sceneType getCurrentSceneType() { return currentScene_; }

    void renderScene();

    void setAircrafts(std::vector<GT_Aircraft*> aircrafts) { aircrafts_ = aircrafts; }

    void setCamera(GT_Camera* tempCam) { camera_ = tempCam; }

    virtual void sceneKeyboardHandler() = 0;


protected:

    GLfloat lastFrame_;

    void integrateScene(GLfloat dT);

    sceneType currentScene_;
    std::vector<GT_Aircraft*> aircrafts_;

    GT_Skybox* skybox_;
    GT_Ocean*  ocean_;

    GT_Camera* camera_;
    GT_HUD* HUD_;

};
