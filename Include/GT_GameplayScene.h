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

    void resetGameplay();


private:

    void nextLevel();
    void checkCrosshair();

    void printMSG();

    void integrateScene(GLfloat deltaTime);
    void checkForBouncingBox();

    void integrateAircrafts(GLfloat deltaTime);
    void renderAircrafts();

    void missileFIRE();

    GLuint level_;

    GT_Skybox* skybox_;
    GT_Ocean* ocean_;

    GT_Fighter* fighter_;
    GT_USSCarrier* ussCarrier_;

    std::vector<GT_Enemy*> enemies_;

    GT_HUD* hud_;

    const GLfloat aimRange_    = 2500.0f;
    const GLfloat weaponRange_ = 1200.0f;

    sf::Sound sound_;
    sf::Sound beepSound_;
    bool dirtySound_ = true;
    GLfloat soundVolume_ = 100;

    const GLfloat bouncingBoxThreshold_ = 20000.0f;
    GLfloat tooFarMSGtime_ = -1;
};

