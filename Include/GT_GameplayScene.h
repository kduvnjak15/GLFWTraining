#pragma once

#include "GT_Scene.h"

#include "GT_Warehouse.h"


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



    GT_Skybox* skybox_;
    GT_Ocean* ocean_;

    std::vector<GT_Actor*> actors_;
    std::vector<GT_Aircraft*> aircrafts_;

};

