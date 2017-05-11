#pragma once

#include "GT_Scene.h"

class GT_GameplayScene : public GT_Scene
{
public:
    GT_GameplayScene(GT_Camera* camera_, sceneType sceneEnum);

    void renderScene();

    void sceneKeyboardHandler(int key, int scancode, int action, int mode);

};

