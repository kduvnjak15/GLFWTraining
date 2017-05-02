#include <iostream>

#include "GT_Scene.h"



GT_Scene::GT_Scene()
{
    std::cout << "Scene initialized " << this << std::endl;
}

GT_Scene::~GT_Scene()
{
    std::cout << "Scene deconstructor" << this << std::endl;
}
