#include <iostream>

#include "GT_MenuScene.h"


GT_MenuScene::GT_MenuScene()
    :
      GT_Scene(menuScene),
      currButton_(0),
      skybox_(GT_Locator::getSkybox()),
      ocean_(GT_Locator::getOcean()),
      menuCamFront_(glm::vec3(0.0f, 0.0f, 1.0f)),
      startAnimateGameplay_(false),
      stopAnimateGameplay_(false)
{
    buttons_.push_back("PLAY GAME");
    buttons_.push_back("OPTIONS");
    buttons_.push_back("QUIT GAME");

    carrier_ = GT_Locator::getUSSCarrier();
    fighter_ = GT_Locator::getFighter();

    nextScene_ = menuScene;

    std::cout << " GT_MenuScene initialized " << std::endl;
}


void GT_MenuScene::sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode)
{

    keys_ = keys;

    if (keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_UP]  && action == GLFW_PRESS)
    {
        if (currButton_ == 0)
            currButton_ = buttons_.size() - 1;
        else
        {
            currButton_ = --currButton_ % buttons_.size();
        }
    }

    if (keys[GLFW_KEY_RIGHT] || keys[GLFW_KEY_DOWN]&& action == GLFW_PRESS)
    {
        currButton_ = ++currButton_ % buttons_.size();
    }

    if (keys[GLFW_KEY_ENTER] && action == GLFW_PRESS)
    {
        if (currButton_ == 0)
        {
            startAnimateGameplay_ = true;

            if (startAnimateGameplay_)
                if( stopAnimateGameplay_)
                {
                    nextScene_ = gameplay;
                    GT_Locator::getGameCamera()->setCameraPos(fighter_->getPosition());
                    GT_Locator::getGameCamera()->setCameraFront(fighter_->getFront());
                    GT_Locator::getGameCamera()->setCameraRight(fighter_->getRight());
                    GT_Locator::getGameCamera()->setCameraUp(fighter_->getUp());

                }

                else
                    nextScene_ = menuScene;
            else
                nextScene_ = menuScene;
        }

        if (currButton_ == 1)
            nextScene_ = options;
        if (currButton_ == 2)
            nextScene_ = exitGame;
    }
}

void GT_MenuScene::checkKeyboardInput()
{
    return;
    if (keys_ != nullptr)
    {
        if (keys_[GLFW_KEY_Q])
            sceneCamera_->keyboardHandler(YAW_L, deltaTime_);
        if (keys_[GLFW_KEY_E])
            sceneCamera_->keyboardHandler(YAW_R, deltaTime_*2);
        if (keys_[GLFW_KEY_W])
            sceneCamera_->keyboardHandler(PITCH_D, deltaTime_);
        if (keys_[GLFW_KEY_S])
            sceneCamera_->keyboardHandler(PITCH_U, deltaTime_);
    }
}

void GT_MenuScene::animateCam()
{
    sceneCamera_->setCameraPos(fighter_->getPosition() +  glm::vec3(sin(glfwGetTime()*SPINFACTOR_), 0.0f, cos(glfwGetTime()*SPINFACTOR_))*10.0f);
    sceneCamera_->setCameraFront(glm::normalize(fighter_->getPosition() - sceneCamera_->getCameraPos()));
}

void GT_MenuScene::animateGameplay()
{
    fighter_->setPosition(fighter_->getPosition() + fighter_->getFront()*1.2f);

    std::cout << fighter_->getPosition().x<<std::endl;
    if (fighter_->getPosition().x > 400.0f)
        stopAnimateGameplay_ = true;
}

void GT_MenuScene::integrateScene(GLfloat deltaTime)
{
    deltaTime *= SPINFACTOR_;

    animateCam();
    if (startAnimateGameplay_)
        animateGameplay();
}

void GT_MenuScene::renderScene()
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

        font_->PrintLine(buttons_[i], 400.0f, 200.0f-i*45.0f, fontSize, glm::vec3(0.50, 0.1f, 1.0f));
    }


    font_->PrintLine("Time: ", 25.0f, 25.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(glfwGetTime()), 120.0f, 25.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("FPS: ", 25.0f, 50.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(1/deltaTime_), 120.0f, 50.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("Speed: ", 25.0f, 75.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(sceneCamera_->getSpeed()), 120.0f, 75.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("Altitude: ", 25.0f, 100.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(sceneCamera_->getCameraPos().y), 120.0f, 100.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));

    ////////////////////////   rendering phase   /////////////////////////////

    skybox_->Draw(sceneCamera_);
    ocean_->draw(sceneCamera_);

    carrier_->Draw(sceneCamera_);
    fighter_->Draw(sceneCamera_);

    ///////////////////////////////////////////////////////////////////////
}
