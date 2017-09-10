#include "GT_IntroScene.h"
#include "GT_Locator.h"
#include "time.h"

GT_IntroScene::GT_IntroScene()
    :GT_Scene(introScene)

{
    wallpaper_ = GT_Locator::getImage();
    //wallpaper_ = new GT_Image(0.0f, 0.0f, 1.0f, 1.0f);
    //wallpaper_ = new GT_Image(1.0f, 0.0f, 0.2f);

    //wallpaper_->defineImageScreenCoordinates(-0.95, -0.95, 0.5,0.5);
    start = 0;
    nextScene_ = introScene;
}

void GT_IntroScene::renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    checkKeyboardInput();

    start += 0.025;
    wallpaper_->setTransparency(sin(start));
    std::cout << sin(start)<<std::endl;
    GT_Utils::drawImage(*(wallpaper_));
}

void GT_IntroScene::checkKeyboardInput()
{
    if (keys_ != nullptr)
    {
        if (keys_[GLFW_KEY_SPACE])
            nextScene_ = menuScene;


    }
}

void GT_IntroScene::sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode)
{
    keys_ = keys;

    if (keys[GLFW_KEY_ESCAPE] && action == GLFW_PRESS)
    {
        nextScene_ = exitGame;
    }
}

