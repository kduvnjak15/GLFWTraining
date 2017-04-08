
#include "GT_Rocket.h"

GT_Rocket::GT_Rocket(const char* path)
    :
      GT_Model(path),
      fired_(false),
      birthday_(0.0f),
      dead_(false)
{

}

GT_Rocket::~GT_Rocket()
{

}

void GT_Rocket::Fire(GT_Enemy* target)
{
    if (!fired_)
    {
        target_ = target;
        fired_ = true;
        birthday_ = glfwGetTime();
        std::cout<< "Rocket "<<this<< " fires on "<< target<<std::endl;
    }

}

GLboolean GT_Rocket::isFired()
{
    return this->fired_;
}

void GT_Rocket::destroy()
{
    std::cout<<" ROCKET "<<this<<" died!"<<std::endl;
    dead_ = true;
}

void GT_Rocket::move()
{
    //std::cout<< "movam raketu"<<this<<std::endl;

    if (glfwGetTime() - birthday_ > MISSILE_LIFE)
    {
        destroy();
        return;
    }

    glm::vec3 dif = target_->modelPos - this->modelPos;
    if (glm::length(dif)< 5.0f)
    {
        if (!target_->isHit())
        {
            target_->explode();
            this->dead_ = true;
        }
        return;
    }
//    std::cout<<"dif : "<<sqrt(dif.x*dif.x + dif.y*dif.y + dif.z*dif.z) <<std::endl;
    glm::vec3 dir = glm::normalize(dif);

    if (glm::dot(modelFront , dir) <0.999)
    {
        glm::vec3 temp = glm::normalize(glm::cross(modelFront, dir ));
        modelUp = glm::normalize(glm::cross(dir, temp));

        this->modelFront = dir;
    }

//    std::cout<<"Pos: "<<modelPos.x<<", "<< modelPos.y<<modelPos.z<<std::endl;
//    std::cout<<"Front: "<<modelFront.x<<", "<< modelFront.y<<modelFront.z<<std::endl;
//    std::cout<<"Up: "<<modelUp.x<<", "<< modelUp.y<<modelUp.z<<std::endl;
//    std::cout<<"dot: "<< glm::dot(modelFront , dir)<<std::endl;



    this->modelPos += MISSILE_SPEED * this->modelFront;
    //contrail_.push_back(new GT_Particle(this->modelPos));
    particles_.push_back(this->modelPos);
    std::cout << particles_.size()<<", "<<particles_[particles_.size()-1].x<<", "<<particles_[particles_.size()-1].y <<std::endl;


}
