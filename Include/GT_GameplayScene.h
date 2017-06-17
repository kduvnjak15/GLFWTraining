#pragma once

#include "GT_Scene.h"
#include "GT_Locator.h"
#include "GT_Enemy.h"
#include "GT_HUD.h"

class GT_GameplayScene : public GT_Scene
{
public:
    GT_GameplayScene();

    void renderScene();

    void checkKeyboardInput();

    void sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode);

private:

    void nextLevel();

    void checkCrosshair();

    void printMSG();

    void integrateScene(GLfloat deltaTime);

    void integrateAircrafts(GLfloat deltaTime);
    void renderAircrafts();

    void missileFIRE();

    GLuint level_;

    GT_Skybox* skybox_;
    GT_Ocean* ocean_;

    GT_Fighter* fighter_;
    GT_USSCarrier* ussCarrier_;

    std::vector<GT_Actor*> actors_;
    std::vector<GT_Enemy*> enemies_;

    GT_HUD* hud_;

};

