#include "GT_IntroScene.h"
#include "GT_Locator.h"

GT_IntroScene::GT_IntroScene()
    :GT_Scene(introScene)

{
    nextScene_ = introScene;
}

void GT_IntroScene::renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    checkKeyboardInput();

    GT_Utils::drawImage(*(GT_Locator::getImage()));
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
}

