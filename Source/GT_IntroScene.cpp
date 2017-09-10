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

        if (bannerSound_ != GT_Locator::getLand() )
        bannerSound_ = GT_Locator::getLand();
        sound_.setBuffer(*bannerSound_);
        banner_played = false;
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

    banner_->Draw();


    if (!banner_played)
    {
        bannerSound_ = GT_Locator::getSpray();
        sound_.setBuffer( *bannerSound_);

        sound_.play();
        banner_played = true;
    }
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

