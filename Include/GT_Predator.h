
#include "GT_Weapon.h"

class GT_Predator: public GT_Weapon
{
public:

    GT_Predator(GLuint missileNum, GT_Aircraft *predatorAircraft);

    void setPredatorMode(bool flag) { predatorMODE_ = flag; }

    void fireMissile();

protected:

    GLboolean predatorMODE_;
};
