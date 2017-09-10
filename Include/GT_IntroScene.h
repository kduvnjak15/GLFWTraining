#pragma once

#include "GT_Scene.h"
#include "GT_Primitive.h"

#include "SFML/Audio.hpp"

class GT_IntroScene : public GT_Scene
{
public:

    GT_IntroScene();

    void integrateScene(GLfloat deltaTime_);

    void renderScene();
    void checkKeyboardInput();
    void sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode);

private:

    GT_Image* machina_;
    GT_Image* wallpaper_;

    GT_Image* banner_;

    sf::Sound sound_;
    sf::SoundBuffer* bannerSound_;
    bool banner_played = false;


    GLfloat start;

    void setSceneBrightness();

};
