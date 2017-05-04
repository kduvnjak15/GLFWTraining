#include <iostream>

#include "GT_Scene.h"



GT_Scene::GT_Scene()
    : currentScene_(introScene)
{
    std::cout << "Scene initialized " << this << std::endl;
}

GT_Scene::GT_Scene(sceneType sceneEnum)
    : currentScene_(sceneEnum)
{
    std::cout << "Scene initialized " << this << std::endl;
}

void GT_Scene::integrateScene()
{

}

void GT_Scene::renderScene()
{

}

GT_Scene::~GT_Scene()
{
    std::cout << "Scene deconstructor" << this << std::endl;
}
