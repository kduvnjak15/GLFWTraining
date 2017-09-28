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

        if (*mit != NULL)
        {
            (*mit)->Integrate(DT_);
        }
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
            GT_Locator::getAudio()->playSound(MISSILE);

            numOfMissiles_--;
            if (numOfMissiles_ < 0 ) numOfMissiles_ = 0;

            break;
        }
}

void GT_Weapon::appendMissiles(int num)
{
    for (int i = 0; i < num; i++)
    {
        GT_Missile* ptr = new GT_Missile(GT_Locator::getModel(AIM), GT_Locator::getFighter(), i );
        missiles_.push_back(ptr);
        GT_Locator::getHUD()->appendMissile(missiles_.back());
        numOfMissiles_++;
    }


}
