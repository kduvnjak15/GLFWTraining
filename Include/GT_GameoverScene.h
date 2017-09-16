#pragma once

#include "GT_Scene.h"
#include "GT_Locator.h"

enum gameoverType{
    WIN,
    LOSE,
    DROWN
};

class GT_GameoverScene : public GT_Scene
{
public:
    GT_GameoverScene();

    void renderScene();

    void checkKeyboardInput();

    void sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode);


private:

    GT_Skybox* skybox_;
    GT_Ocean* ocean_;
    GLfloat camTimer_;

    GT_Fighter* fighter_;
    GT_USSCarrier* carrier_;

    void integrateScene(GLfloat deltaTime_);

    void printWIN();
    void printLOSE();
    void printDROWN();

    GT_Image* canvas_;

    sf::Sound oceanSound_;
    bool dirtySound_ = true;
    glm::vec3 menuCamFront_;

    gameoverType sceneEnum_;

};
