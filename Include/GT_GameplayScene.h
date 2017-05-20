#pragma once

#include "GT_Scene.h"

#include "GT_Fighter.h"


class GT_GameplayScene : public GT_Scene
{
public:
    GT_GameplayScene(GT_Camera* tempCam, GT_Warehouse* warehouse);

    void renderScene();

    void checkKeyboardInput();

    void sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode);

private:

    void integrateScene(GLfloat deltaTime);

    void integrateAircrafts(GLfloat deltaTime);
    void renderAircrafts();

    void missileFIRE();

    GT_Skybox* skybox_;
    GT_Ocean* ocean_;

    GT_Fighter* fighter_;

    std::vector<GT_Actor*> actors_;
    std::vector<GT_Aircraft*> aircrafts_;

};

