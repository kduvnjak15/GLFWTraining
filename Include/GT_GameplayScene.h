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

    void integrateScene(GLfloat deltaTime);

private:

    GT_Skybox* skybox_;
    GT_Ocean* ocean_;

    GT_Aircraft* fighter_;

};

