#include <iostream>

#include "GT_MenuScene.h"

#include "GT_Alphabet.h"

GT_MenuScene::GT_MenuScene(GT_Camera* tempCam, GT_Warehouse *warehousePtr, void *functors)
    :
      GT_Scene(tempCam, introScene),
      currButton_(0),
      skybox_(new GT_Skybox()),
      ocean_(new GT_Ocean()),
      functors_(functors)
{
    buttons_.push_back("PLAY GAME");
    buttons_.push_back("OPTIONS");
    buttons_.push_back("QUIT GAME");

    skybox_ = new GT_Skybox();
    ocean_  = new GT_Ocean();
    requisite_ = warehousePtr->getAircraft(USS);
    requisite_->setPosition(glm::vec3(-500.0f, 450.f, 2700.f));


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
            std::cout << "left pres " << buttons_.size() << std::endl;
            currButton_ = --currButton_ % buttons_.size();
        }
    }

    if (keys[GLFW_KEY_RIGHT] && action == GLFW_PRESS)
    {
        std::cout << "right pres " <<buttons_.size()<< std::endl;
        currButton_ = ++currButton_ % buttons_.size();
    }

    if (keys[GLFW_KEY_ENTER] && action == GLFW_PRESS)
    {
        if (currButton_ == 2)
            exit_ = true;


        std::cout << "right pres " <<buttons_.size()<< std::endl;
    }

    std::cout << "cB "<< currButton_ <<std::endl;


}

void GT_MenuScene::checkKeyboardInput()
{
    if (keys_[GLFW_KEY_Q])
        sceneCamera_->keyboardHandler(YAW_L, deltaTime_);
    if (keys_[GLFW_KEY_E])
        sceneCamera_->keyboardHandler(YAW_R, deltaTime_*2);
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
    deltaTime_ =  glfwGetTime() - lastFrame_;
    lastFrame_ = glfwGetTime();

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
