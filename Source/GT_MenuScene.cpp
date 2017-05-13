#include <iostream>

#include "GT_MenuScene.h"

#include "GT_Alphabet.h"

GT_MenuScene::GT_MenuScene(GT_Camera* tempCam, GT_Warehouse *warehousePtr)
    :
      GT_Scene(tempCam, introScene),
      currButton_(0),
      skybox_(new GT_Skybox()),
      ocean_(new GT_Ocean())
{
    buttons_.push_back("PLAY GAME");
    buttons_.push_back("OPTIONS");
    buttons_.push_back("QUIT GAME");

    skybox_ = new GT_Skybox();
    ocean_  = new GT_Ocean();
    requisite_ = warehousePtr->getAircraft(USS);
    requisite_->setPosition(glm::vec3(-200.0f, 450.f, 500.f));


    std::cout << " GT_MenuScene initialized " << std::endl;
}


void GT_MenuScene::sceneKeyboardHandler(bool* keys, int key, int scancode, int action, int mode)
{
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
        std::cout << "right pres " <<buttons_.size()<< std::endl;
        if (currButton_ == 0 )
            currentScene_
    }


    std::cout << "cB "<< currButton_ <<std::endl;


}

void GT_MenuScene::integrateScene(GLfloat deltaTime)
{
    deltaTime *= 0.5f;
    if (sin(glfwGetTime()*0.01)> 0)
        sceneCamera_->keyboardHandler(YAW_L, deltaTime);
    else
        sceneCamera_->keyboardHandler(YAW_R, deltaTime);

}

void GT_MenuScene::renderScene()
{
    GLfloat deltaTime =  glfwGetTime() - lastFrame_;
    lastFrame_ = glfwGetTime();

    integrateScene(deltaTime);


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
    font_->PrintLine( std::to_string(1/deltaTime), 120.0f, 50.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("Speed: ", 25.0f, 75.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(sceneCamera_->getSpeed()), 120.0f, 75.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
    font_->PrintLine("Altitude: ", 25.0f, 100.0f, .50f, glm::vec3(1.0, 0.1f, 0.1f));
    font_->PrintLine( std::to_string(sceneCamera_->getCameraPos().y), 120.0f, 100.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));


    skybox_->Draw(sceneCamera_);
    ocean_->draw(sceneCamera_);
    requisite_->Draw(sceneCamera_);
}
