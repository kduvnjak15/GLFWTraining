#pragma once 

#include <set>
#include <vector>
#include "GT_Camera.h"
#include "GT_Alphabet.h"


enum sceneType
{
    gameplay,
    pauseScene,
    menuScene,
    options,
    exitGame,
    nonType
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
    virtual void checkKeyboardInput() = 0;
    virtual void sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode) = 0;

    void setCamera(GT_Camera* tempCam) { sceneCamera_ = tempCam; }

    sceneType nextScene_;

    std::set<int> keysEnable_;
    bool keyPressed_;
    GLfloat keyDelay_;

protected:    

    void updateClock();

    GLfloat lastFrame_;
    GLfloat deltaTime_;

    sceneType currentScene_;

    GT_Camera* sceneCamera_;

    GT_Alphabet* font_;

    bool* keys_;

};
