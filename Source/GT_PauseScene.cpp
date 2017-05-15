

#include "GT_PauseScene.h"

GT_PauseScene::GT_PauseScene(GT_Camera *tempCam, GT_Warehouse *warehouse)
    :
      GT_Scene(tempCam, pauseScene),
      currButton_(0),
      skybox_(new GT_Skybox()),
      ocean_(new GT_Ocean())
{
    buttons_.push_back("Resume Game");
    buttons_.push_back("Leave Game");

    skybox_= new GT_Skybox();
    ocean_= new GT_Ocean();

    nextScene_ = pauseScene;

    requisite_ = warehouse->getAircraft(F18);
    std::cout << " GT_PauseScene initialized " << std::endl;

}

void GT_PauseScene::checkKeyboardInput()
{
    if (keys_ != nullptr)
    {
        if (keys_[GLFW_KEY_Q])
            sceneCamera_->keyboardHandler(YAW_L, deltaTime_);
        if (keys_[GLFW_KEY_E])
            sceneCamera_->keyboardHandler(YAW_R, deltaTime_*2);
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

    font_->PrintLine("PAUSE ", 600.0f, 400.0f, 1.0f, glm::vec3(1.0, 0.1f, 0.1f));


    ////////////////////////   rendering phase   /////////////////////////////

    skybox_->Draw(sceneCamera_);
    ocean_->draw(sceneCamera_);
//    requisite_->Draw(sceneCamera_);

}

void GT_PauseScene::integrateScene(GLfloat deltaTime)
{
    deltaTime *= SPINFACTOR_;
    if (sin(glfwGetTime()*0.01)> 0)
        sceneCamera_->keyboardHandler(YAW_L, deltaTime);
    else
        sceneCamera_->keyboardHandler(YAW_R, deltaTime);

}


void GT_PauseScene::sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode)
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
        else if (currButton_ == 1)
            nextScene_ = menuScene;
    }

    std::cout << "cB "<< currButton_ <<std::endl;
}

