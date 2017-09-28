

#include "GT_PauseScene.h"

#include "GT_Locator.h"

GT_PauseScene::GT_PauseScene()
    :
      GT_Scene(menuScene),
      currButton_(0),
      skybox_(GT_Locator::getSkybox()),
      ocean_(GT_Locator::getOcean())
{
    buttons_.push_back("RESUME GAME");
    buttons_.push_back("LEAVE GAME");

    carrier_ = GT_Locator::getUSSCarrier();
    fighter_ = GT_Locator::getFighter();

    std::cout << " GT_PauseScene initialized " << std::endl;
}

void GT_PauseScene::checkKeyboardInput()
{
    if (keys_ != nullptr)
    {
        if (keys_[GLFW_KEY_Q])
            camTimer_-=3.0f;
        if (keys_[GLFW_KEY_E])
            camTimer_+=1.0f;
    }
}

void GT_PauseScene::renderScene()
{

    ////////////////////////// timer //////////////////////////////////////
    updateClock();
    ///////////////////////////////////////////////////////////////////////

    checkKeyboardInput();

    //////////////////////    integration   ///////////////////////////////

    integrateScene(deltaTime_);

    ///////////////////////////////////////////////////////////////////////

    /////////////////////   reset color     //////////////////////////////
    glClearColor(0.0, 0.15f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ///////////////////////////////////////////////////////////////////////

    for (int i = 0; i < buttons_.size(); i++)
    {
        GLfloat fontSize;
        if (i == currButton_)
            fontSize = 1.0f;
        else
            fontSize = 0.4f;

        font_->PrintLine(buttons_[i], 0.45f, 0.50f-i*0.050f, fontSize, glm::vec3(0.0, 0.3f, 0.2f));
    }

    font_->PrintLine("PAUSE ", 0.700f, 0.70f, 1.2f, glm::vec3(0.0, 0.4f, 0.3f));

    ////////////////////////   rendering phase   /////////////////////////////

    skybox_->Draw(sceneCamera_);
    ocean_->draw(sceneCamera_);

    fighter_->Draw(sceneCamera_);
    carrier_->Draw(sceneCamera_);

}

void GT_PauseScene::animateCam()
{
    sceneCamera_->setCameraPos(fighter_->getPosition() +  glm::vec3(sin(camTimer_*SPINFACTOR_), 0.0f, cos(camTimer_*SPINFACTOR_))*10.0f);
    sceneCamera_->setCameraFront(glm::normalize(fighter_->getPosition() - sceneCamera_->getCameraPos()));
}

void GT_PauseScene::integrateScene(GLfloat deltaTime)
{
    camTimer_ += 1.0f;
    animateCam();
}


void GT_PauseScene::sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode)
{
    keys_ = keys;

    if (keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_UP] && action == GLFW_PRESS)
    {

        if (currButton_ == 0)
            currButton_ = buttons_.size() - 1;
        else
        {
            currButton_ = --currButton_ % buttons_.size();
        }
    }

    if (keys[GLFW_KEY_RIGHT] || keys[GLFW_KEY_DOWN] && action == GLFW_PRESS)
    {
        currButton_ = ++currButton_ % buttons_.size();
    }

    if (keys[GLFW_KEY_ENTER] && action == GLFW_PRESS)
    {
        if (currButton_ == 0)
            GT_Locator::getSceneManager()->activateScene( gameplay );
        else if (currButton_ == 1)
            GT_Locator::getSceneManager()->activateScene( menuScene );
    }    
}

