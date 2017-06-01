#pragma once

#include "GT_Scene.h"
#include "GT_Locator.h"
#include "GT_USSCarrier.h"

class GT_MenuScene : public GT_Scene
{
    public:

    GT_MenuScene();

    void renderScene();

    void checkKeyboardInput();

    void sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode);

    void integrateScene(GLfloat deltaTime);


private:

    void animateCam();
    void animateGameplay();

    std::vector<const char* > buttons_;
    int currButton_;

    GT_Skybox*      skybox_;
    GT_Ocean*       ocean_;
    GT_USSCarrier*  carrier_;
    GT_Fighter*     fighter_;

    glm::vec3 menuCamFront_;

    const GLfloat SPINFACTOR_ = 0.8f;
    bool startAnimateGameplay_;
    bool stopAnimateGameplay_;
};
