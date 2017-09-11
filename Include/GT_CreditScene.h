#pragma once

#include "GT_Scene.h"
#include "GT_Locator.h"


class GT_CreditScene: public GT_Scene
{
public:
    GT_CreditScene();

    void renderScene();

    void checkKeyboardInput();

    void sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode);

private:

    GT_Skybox* skybox_;
    GT_Ocean* ocean_;
    GLfloat camTimer_;

    GT_Fighter*    fighter_;
    GT_USSCarrier* carrier_;

    void integrateScene(GLfloat deltaTime);

    void printMSG();

    GT_Image* canvas_;

    sf::Sound oceanSound_;
    bool dirtySound_ = true;
    glm::vec3 menuCamFront_;

};
