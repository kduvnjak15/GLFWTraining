#include "GT_Enemy.h"
#include "GLFW/glfw3.h"
#include "GT_Locator.h"

GT_Enemy::GT_Enemy()
    : GT_Aircraft(GT_Locator::getModel(F22)),
      GT_Weapon(2, this)
{
    target_ = GT_Locator::getUSSCarrier();
    targetPos_ = target_->getPosition();

    setScale(1.0f);
    setPosition(glm::vec3(1000, 400, 0));
    std::cout<< "Enemy initialized" << this<<std::endl;
}

void GT_Enemy::Draw(GT_Camera *tempCam)
{
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
    this->front_ = glm::normalize(target_->getPosition() - this->position_);
    this->up_ = glm::vec3(0.0f, 1.0f, 0.0f);
    this->right_ = glm::cross(front_, up_);

    this->position_ += this->front_;
}

