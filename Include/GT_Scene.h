#pragma once 

#include <vector>
#include "GT_Camera.h"

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

    virtual ~GT_Scene();

    sceneType getCurrentSceneType() { return currentScene_; }

    virtual void renderScene() = 0;

    void setCamera(GT_Camera* tempCam) { sceneCamera_ = tempCam; }

    virtual void sceneKeyboardHandler(int key, int scancode, int action, int mode) = 0;


protected:

    GLfloat lastFrame_;

    sceneType currentScene_;

    GT_Camera* sceneCamera_;


};
