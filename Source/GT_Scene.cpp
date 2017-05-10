#include <iostream>

#include "GT_Scene.h"
#include "GT_Skybox.h"
#include "GT_Ocean.h"


GT_Scene::GT_Scene(GT_Camera* tempCam, sceneType sceneEnum)
    :
    currentScene_(sceneEnum),
    lastFrame_(glfwGetTime()),
    skybox_(new GT_Skybox),
    ocean_(new GT_Ocean),
    HUD_(new GT_HUD),
    camera_ ( tempCam )
{

    std::cout << "Scene initialized " << this << std::endl;
}


void GT_Scene::integrateScene(GLfloat dT)
{
    for(int i = 0; i < this->aircrafts_.size(); i++)
    {
        aircrafts_[i]->Integrate(dT);
    }
}

void GT_Scene::renderScene()
{
    lastFrame_ = glfwGetTime();
    /////////////// timer func ////////////////////////
    GLfloat deltaTime = glfwGetTime() - lastFrame_;


    //////////// update movement /////////////////////
    integrateScene(deltaTime);


    /////////// draw phase //////////////////////
    for (auto it = aircrafts_.begin(); it != aircrafts_.end(); it++)
    {
        (*it)->Draw(camera_);
    }

    skybox_->Draw(camera_);
    ocean_->draw(camera_);
    HUD_->draw(camera_);

}

GT_Scene::~GT_Scene()
{
    std::cout << "Scene deconstructor" << this << std::endl;
}
