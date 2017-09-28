#pragma once 

#include <set>
#include <vector>
#include "GT_Camera.h"
#include "GT_Alphabet.h"
#include "GT_Utils.h"

enum sceneType
{
    introScene,
    gameplay,
    pauseScene,
    menuScene,
    exitGame,
    credits,
    gameover,
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

    GT_Scene(sceneType sceneEnum);

    virtual ~GT_Scene();

    sceneType getCurrentSceneType() { return currentScene_; }

    virtual void renderScene() = 0;
    virtual void checkKeyboardInput() = 0;
    virtual void sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode) = 0;

    void setCamera(GT_Camera* tempCam) { sceneCamera_ = tempCam; }

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

    const GLfloat SPINFACTOR_ = 0.01f;


};
