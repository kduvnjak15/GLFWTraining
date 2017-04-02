
#include "GT_Rocket.h"

GT_Rocket::GT_Rocket(const char* path)
    :
      GT_Model(path),
      fired_(false)
{

}

void GT_Rocket::Fire(GT_Model* target)
{
    if (!fired_)
    {
        target_ = target->modelPos;
        fired_ = true;
    }

}

GLboolean GT_Rocket::isFired()
{
    return this->fired_;
}

void GT_Rocket::move()
{
    //std::cout<< "movam raketu"<<this<<std::endl;
    this->modelPos += rocketSpeed * glm::normalize(target_ - this->modelPos);

}
