#include "GT_Actor.h"

GT_Actor::GT_Actor()
{
    std::cout << "GT_Actor initialized "<< this << std::endl;
}

GT_Actor::GT_Actor(GT_Model* actorModel)
{

}

void GT_Actor::setPosition(glm::vec3 pos_)
{
    position_ = pos_;
}

GT_Actor::~GT_Actor()
{

}



