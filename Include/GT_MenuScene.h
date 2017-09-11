#pragma once

#include "GT_Scene.h"
#include "GT_Locator.h"

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
    void initGameplayScene();


    std::vector<const char* > buttons_;
    int currButton_;

    GT_Skybox*      skybox_;
    GT_Ocean*       ocean_;
    GT_USSCarrier*  carrier_;
    GT_Fighter*     fighter_;

    glm::vec3 menuCamFront_;
    GLfloat camTimer_;

    sf::Sound sound_;
    sf::Sound soundJet_;
    bool dirtySound_ = true;
    bool dirtySoundJet_ = true;
    GLfloat soundJetPos_ = 0;

    const GLfloat SPINFACTOR_ = 0.01f;
    bool startAnimateGameplay_;
    bool stopAnimateGameplay_;
};
