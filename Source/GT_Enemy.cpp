#include "GT_Enemy.h"
#include "GLFW/glfw3.h"
#include "GT_Locator.h"

GT_Enemy::GT_Enemy()
    : GT_Aircraft(GT_Locator::getModel(F22), GT_Locator::getModel(AIM))
{
    std::cout<< "Enemy initialized" << this<<std::endl;
}
