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

    setScale(20.0f);

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

    if (this->position_.y < -20)
    {
        dead_ = true;
        return;
    }

    if (this->explode_)
    {
        this->position_ += glm::vec3(0.0f, -1.0f*(glfwGetTime() - explodeTime_ )*(glfwGetTime() - explodeTime_ ), 0.0f);
        this->position_ += this->front_;
    }
    else
    {

        glm::vec3 dir = target_->getPosition() - this->position_;
        dir = glm::normalize(dir);
        GLfloat dotP = glm::dot(dir, this->front_);
        while (dotP < 0.2)
        {
            dir = glm::normalize(dir + this->front_);
            dotP = glm::dot(dir, this->front_);
        }

        this->front_ = dir;
        if (!predatorMODE_)
            this->front_.y = 0;

        this->up_ = glm::vec3(0.0f, 1.0f, 0.0f);
        this->right_ = glm::cross(front_, up_);


        this->position_ += this->front_*3.0f;

    }

    GT_Weapon::Integrate(DT_);
}

