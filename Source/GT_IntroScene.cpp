#include "GT_IntroScene.h"
#include "GT_Locator.h"
#include "time.h"

GT_IntroScene::GT_IntroScene()
    :GT_Scene(introScene)

{
    wallpaper_  = new GT_Image("../Content/intro.jpg");
    machina_    = new GT_Image("../Content/machina2.jpg");

    start = 0;
    machina_->setBrightness(0);
    wallpaper_->setBrightness(0);

    bannerSound_ = GT_Locator::getSpray();
    sound_.setBuffer(*(bannerSound_));
    dirtySound_ = true;

    banner_ = machina_;
    nextScene_ = introScene;
}

void GT_IntroScene::setSceneBrightness()
{
    if (cos(start)>= 0)
    {
        banner_->setBrightness(sin(start));

    }
    else
    {
        banner_->setBrightness(1.0f);
    }

}

void GT_IntroScene::integrateScene(GLfloat deltaTime_)
{
    start += deltaTime_;

    if (start > 2*PI )
    {
        banner_ = wallpaper_;
        dirtySound_ = true;
    }

    setSceneBrightness();

    if (start > 4*PI)
    {
        nextScene_ = menuScene;
    }


}

void GT_IntroScene::renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    updateClock();

    checkKeyboardInput();

    integrateScene(deltaTime_);

    if (wallpaper_played)
    {
        GT_Locator::getFonts()->PrintLine("Press SPACE to continue", 220, 30, 1.0f, glm::vec4(0.0f, 0.1f, 1.0f, sin(start*5)*0.5 + 0.5));
    }
    banner_->Draw();

    if (!machina_played)
    {
        sound_.play();
        machina_played = true;
        dirtySound_ = false;
    }

    else if (!wallpaper_played && dirtySound_)
    {
        sound_.setBuffer(*(GT_Locator::getLand()));
        sound_.play();
        wallpaper_played = true;
    }

}

void GT_IntroScene::checkKeyboardInput()
{
    if (keys_ != nullptr)
    {
        if (keys_[GLFW_KEY_SPACE])
        {
            sound_.stop();
            nextScene_ = menuScene;
        }
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

