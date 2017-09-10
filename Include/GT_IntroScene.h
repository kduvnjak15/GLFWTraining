#pragma once

#include "GT_Scene.h"
#include "GT_Primitive.h"

class GT_IntroScene : public GT_Scene
{
public:

    GT_IntroScene();

    void renderScene();
    void checkKeyboardInput();
    void sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode);

private:

    GT_Image* wallpaper_;

    GLfloat start;

};
