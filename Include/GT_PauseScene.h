#pragma once

#include "GT_Scene.h"

#include "GT_Warehouse.h"

class GT_PauseScene : public GT_Scene
{
public:
    GT_PauseScene(GT_Camera* tempCam, GT_Warehouse* warehouse);

    void renderScene();

    void checkKeyboardInput();

    void sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode);

    void integrateScene(GLfloat deltaTime_);

private:

    std::vector<const char* > buttons_;
    int currButton_;
    GT_Skybox* skybox_;
    GT_Ocean* ocean_;
    GT_Aircraft* requisite_;

    const GLfloat SPINFACTOR_ = 0.5f;

};
