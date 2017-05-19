#include <iostream>

#include "GT_MenuScene.h"

GT_MenuScene::GT_MenuScene(GT_Camera* tempCam, GT_Warehouse *warehousePtr)
    :
      GT_Scene(tempCam, menuScene),
      currButton_(0),
      skybox_(new GT_Skybox()),
      ocean_(new GT_Ocean())
{
    buttons_.push_back("PLAY GAME");
    buttons_.push_back("OPTIONS");
    buttons_.push_back("QUIT GAME");

    // mem leak ?
    skybox_ = new GT_Skybox();
    ocean_  = new GT_Ocean();
    requisite_ = new GT_Actor( warehousePtr->getModel(F22));
    requisite_->setPosition(glm::vec3(0.0f, 450.f, 0.0f));

    nextScene_ = menuScene;

    std::cout << " GT_MenuScene initialized " << std::endl;
}


void GT_MenuScene::sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode)
{

    keys_ = keys;

    if (keys[GLFW_KEY_LEFT]  && action == GLFW_PRESS)
    {
        if (currButton_ == 0)
            currButton_ = buttons_.size() - 1;
        else
        {
            currButton_ = --currButton_ % buttons_.size();
        }
    }

    if (keys[GLFW_KEY_RIGHT] && action == GLFW_PRESS)
    {
        currButton_ = ++currButton_ % buttons_.size();
    }

    if (keys[GLFW_KEY_ENTER] && action == GLFW_PRESS)
    {
        if (currButton_ == 0)
            nextScene_ = gameplay;
        if (currButton_ == 1)
            nextScene_ = options;
        if (currButton_ == 2)
            nextScene_ = exitGame;

    }
}

void GT_MenuScene::checkKeyboardInput()
{
    if (keys_ != nullptr)
    {
        if (keys_[GLFW_KEY_Q])
            sceneCamera_->keyboardHandler(YAW_L, deltaTime_);
        if (keys_[GLFW_KEY_E])
            sceneCamera_->keyboardHandler(YAW_R, deltaTime_*2);
    }
}

void GT_MenuScene::integrateScene(GLfloat deltaTime)
{
    deltaTime *= SPINFACTOR_;
    if (sin(glfwGetTime()*0.01)> 0)
        sceneCamera_->keyboardHandler(YAW_L, deltaTime);
    else
        sceneCamera_->keyboardHandler(YAW_R, deltaTime);

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

        font_->PrintLine(buttons_[i], 400.0f, 400.0f-i*45.0f, fontSize, glm::vec3(0.50, 0.1f, 1.0f));
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
    requisite_->Draw(sceneCamera_);

    ///////////////////////////////////////////////////////////////////////
}
