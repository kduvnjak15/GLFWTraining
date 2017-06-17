#include "GT_Predator.h"

GT_Predator::GT_Predator(GLuint missileNum, GT_Aircraft* predatorAircraft)
    :
        GT_Weapon(missileNum, predatorAircraft),
        predatorMODE_(false)
{

}


void GT_Predator::fireMissile()
{
    for (int i = 0; i < missiles_.size(); i++)
        if (missiles_[i]->isFired() )
        {
            if (missiles_[i]->isDead())
            {
                continue;
            }
            else
            {
                return;
            }
        }
        else
        {
            std::cout << " wrect "<< std::endl;
            missiles_[i]->FIRE(target_);
            break;
        }
}

