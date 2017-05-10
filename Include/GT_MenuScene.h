#pragma once

#include <vector>
#include "GT_Scene.h"

enum buttons
{
    PlayGame,
    QuitGame
};

class GT_MenuScene : public GT_Scene
{
    public:
    GT_MenuScene(GT_Camera *tempCam);

    void KeyboardHandler(keyPressed keyInput);

private:

    std::vector<const char* > buttons_;
    uint currButton_;

};
