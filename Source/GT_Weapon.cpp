#include "GT_Weapon.h"

#include "GT_Locator.h"

GT_Weapon::GT_Weapon(GLuint numOfMissiles, GT_Aircraft *ownerPtr)
    :
      numOfMissiles_(numOfMissiles)
{
    for (int i = 0; i < numOfMissiles; i++)
    {
        missiles_.push_back(new GT_Missile(GT_Locator::getModel(AIM), ownerPtr, i ));
        GT_Locator::getHUD()->appendMissile(missiles_.back());
    }
}

void GT_Weapon::Draw(GT_Camera *tempCam)
{

    Integrate(0.0f);

    int br = 0;
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

void GT_Weapon::Integrate(GLfloat DT_)
{
    for (auto mit = missiles_.begin(); mit != missiles_.end(); mit++)
    {
        (*mit)->Integrate(DT_);
    }
}

void GT_Weapon::fireMissile()
{

    for (int i = 0; i < missiles_.size(); i++)
        if (missiles_[i]->isFired())
        {
            continue;
        }
        else
        {
            missiles_[i]->FIRE(target_);
            break;
        }
}
