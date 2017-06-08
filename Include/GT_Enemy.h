#pragma once

#include "GT_Weapon.h"

class GT_Enemy : public GT_Aircraft, public GT_Weapon
{
public:

    GT_Enemy();

    void Draw(GT_Camera* tempCam);

    void Integrate(GT_Camera* tempCam, GLfloat DT_);

private:

    const GLfloat weaponRange_ = 500.0f;

};
