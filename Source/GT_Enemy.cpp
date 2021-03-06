#include "GT_Enemy.h"
#include "GLFW/glfw3.h"
#include "GT_Locator.h"

#include <time.h>

GT_Enemy::GT_Enemy()
    : GT_Aircraft(GT_Locator::getModel(F22)),
      GT_Predator(2, this),
      dead_(false)
{
    target_ = GT_Locator::getUSSCarrier();
    targetPos_ = target_->getPosition();
    this->front_ = glm::normalize(targetPos_ - position_);
    this->up_    = glm::vec3(0.0f, 1.0f, 0.0f);
    this->right_ = glm::normalize(glm::cross(this->front_, this->up_));

    setScale(5.0f);

    GLfloat rnum = rand() / GLfloat(RAND_MAX);    

    setPosition(glm::vec3( rnum *1000+5000.0f, rnum* 500 + 1000, sin(rnum) * 1000));

    GT_Weapon::Integrate(0.0f);
    std::cout<< "Enemy initialized" << this<<std::endl;
}

void GT_Enemy::Draw(GT_Camera *tempCam)
{
    if ( dead_)
    {
        return ;
    }

    GT_Aircraft::Draw(tempCam);
    GT_Weapon::Draw(tempCam);

}

void GT_Enemy::Integrate(GT_Camera *tempCam, GLfloat DT_)
{

    if (this->position_.y < 0)
    {
        dead_ = true;
        return;
    }

    if (this->explode_)
    {
        this->position_ += glm::vec3(0.0f, -1.0f*(glfwGetTime() - explodeTime_ )*(glfwGetTime() - explodeTime_ ), 0.0f);
        this->position_ += this->front_*2.0f;
    }
    else
    {

        if (glm::length(GT_Locator::getFighter()->getPosition() - this->position_)< 20)
        {
            GT_Locator::getFighter()->explode();
            GT_Aircraft::explode();
        }


        glm::vec3 dir = target_->getPosition() - this->position_;
        glm::vec3 frontDir = this->front_;

        dir = glm::normalize(dir);
        frontDir = glm::normalize(frontDir);

        GLfloat dotP = glm::dot(dir,frontDir);
        while (dotP < 0.99999)
        {
            dir = glm::normalize(dir + frontDir);
            dotP = glm::dot(dir, frontDir);
        }

        this->front_ = dir;
        if (!predatorMODE_)
            this->front_.y = 0;

        this->up_ = glm::vec3(0.0f, 1.0f, 0.0f);
        this->right_ = glm::cross(front_, up_);


        this->position_ += this->front_*5.0f;

    }

    GT_Weapon::Integrate(DT_);
}

GT_Enemy::~GT_Enemy()
{
    for (int i = 0 ; i < missiles_.size(); i++)
    {
        missiles_[i]->selfDestruct();
    }
    std::cout << "enemy deleted " << this << std::endl;
}
