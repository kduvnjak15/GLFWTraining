#include "GT_Weapon.h"

#include "GT_Locator.h"

GT_Weapon::GT_Weapon(GLuint numOfMissiles, GT_Aircraft *ownerPtr)
    :
      numOfMissiles_(numOfMissiles)
{
    for (int i = 0; i < numOfMissiles; i++)
        missiles_.push_back(new GT_Missile(GT_Locator::getModel(AIM), ownerPtr, i ));
}

void GT_Weapon::Draw(GT_Camera *tempCam)
{
    int br = 0;
    std::cout << "miss" << missiles_.size() << std::endl;
    for (auto mit = missiles_.begin(); mit != missiles_.end() ; mit++)
    {
        if ((*mit)->isDead())
            continue;

        (*mit)->Draw(tempCam);
        if (!(*mit)->isFired())
            br++;

        if (br>1)
            break;
    }
}

void GT_Weapon::Integrate(GT_Camera* tempCam, GLfloat DT_)
{
    for (auto mit = missiles_.begin(); mit != missiles_.end(); mit++)
    {
        (*mit)->Integrate(tempCam, DT_);
    }
}

void GT_Weapon::fireMissile()
{
    for (int i = 0; i < missiles_.size(); i++)
        if (missiles_[i]->isFired())
            continue;
        else
        {
            missiles_[i]->FIRE();
            break;
        }
}
