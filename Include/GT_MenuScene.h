#pragma once

#include <vector>
#include "GT_Scene.h"

#include "GT_Skybox.h"
#include "GT_Ocean.h"

enum buttons
{
    PlayGame,
    QuitGame
};

class GT_MenuScene : public GT_Scene
{
    public:
    GT_MenuScene(GT_Camera *tempCam);

    void renderScene();

    void sceneKeyboardHandler(int key, int scancode, int action, int mode);

private:

    std::vector<const char* > buttons_;
    uint currButton_;
    GT_Skybox* skybox_;
    GT_Ocean* Ocean_;

};
