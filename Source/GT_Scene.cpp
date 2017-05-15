#include <iostream>

#include "GT_Scene.h"
#include "GT_Skybox.h"
#include "GT_Ocean.h"


GT_Scene::GT_Scene(GT_Camera* tempCam, sceneType sceneEnum)
    :
    currentScene_(sceneEnum),
    lastFrame_(glfwGetTime()),
    deltaTime_(0),
    sceneCamera_ (tempCam),
    font_(new GT_Alphabet(tempCam)),    
    keypressed_(false)
{

    std::cout << "Scene initialized " << this << std::endl;
}

void GT_Scene::updateClock()
{
    deltaTime_ =  glfwGetTime() - lastFrame_;
    lastFrame_ = glfwGetTime();
}

GT_Scene::~GT_Scene()
{
    std::cout << "Scene deconstructor" << this << std::endl;
}
