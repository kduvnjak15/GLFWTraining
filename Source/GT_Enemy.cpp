#include "GT_Enemy.h"
#include "GLFW/glfw3.h"
#include "GT_Locator.h"

#include <time.h>

GT_Enemy::GT_Enemy()
    : GT_Aircraft(GT_Locator::getModel(F22)),
      GT_Weapon(2, this),
      dead_(false)
{
    target_ = GT_Locator::getUSSCarrier();
    targetPos_ = target_->getPosition();

    setScale(1.0f);

    GLfloat rnum = rand() / GLfloat(RAND_MAX);    

    setPosition(glm::vec3( rnum *1000+3000.0f, 400, rnum * 1000));

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

    int br = 0;
    for (auto mit = missiles_.begin(); mit != missiles_.end() ; mit++)
    {
        (*mit)->Draw(tempCam);
        if (!(*mit)->isFired())
            br++;

        if (br>1)
            break;
    }
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

        this->front_ = glm::normalize(target_->getPosition() - this->position_);
        this->front_.y = 0;

        this->up_ = glm::vec3(0.0f, 1.0f, 0.0f);
        this->right_ = glm::cross(front_, up_);

        this->position_ += this->front_;
    }
}


void GT_Enemy::explode()
{
    explodeTime_ = glfwGetTime();
    explode_ = true;
}
