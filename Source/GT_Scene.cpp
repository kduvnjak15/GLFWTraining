#include <iostream>

#include "GT_Scene.h"
#include "GT_Skybox.h"
#include "GT_Ocean.h"

#include "GT_Locator.h"

GT_Scene::GT_Scene(sceneType sceneEnum)
    :
    currentScene_(sceneEnum),
    lastFrame_(glfwGetTime()),
    deltaTime_(0),
    keyPressed_(false),
    keys_(nullptr)
{

    if (sceneEnum == gameplay)
        sceneCamera_ = GT_Locator::getGameCamera();
    else if (sceneEnum == pauseScene || sceneEnum == menuScene || sceneEnum == credits )
    {
        std::cout << "GT_MENUSCENE" << std::endl;
        sceneCamera_ = GT_Locator::getMenuCamera();
    }

    font_ = GT_Locator::getFonts();

    std::cout << "Scene initialized " << this << std::endl;
}

void GT_Scene::updateClock()
{
    deltaTime_ = glfwGetTime() - lastFrame_;
    lastFrame_ = glfwGetTime();
}

GT_Scene::~GT_Scene()
{
    std::cout << "Scene deconstructor" << this << std::endl;
}
