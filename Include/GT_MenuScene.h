#pragma once

#include <vector>
#include "GT_Scene.h"

#include "GT_Warehouse.h"

enum buttons
{
    PlayGame,
    QuitGame
};

class GT_MenuScene : public GT_Scene
{
    public:
    GT_MenuScene(GT_Camera *tempCam, GT_Warehouse* warehousePtr);

    void renderScene();

    void integrateScene(GLfloat deltaTime);

    void sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode);

private:

    std::vector<const char* > buttons_;
    int currButton_;
    GT_Skybox* skybox_;
    GT_Ocean* ocean_;
    GT_Aircraft* requisite_;

};
