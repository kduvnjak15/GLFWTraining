#pragma once

#include "GT_Weapon.h"
#include "GT_Intelligence.h"

class GT_Enemy : public GT_Aircraft, public GT_Weapon
{
public:

    GT_Enemy();

    void Draw(GT_Camera* tempCam);

    void Integrate(GT_Camera* tempCam, GLfloat DT_);

    void explode();

    bool isDead() { return dead_; }

    ~GT_Enemy() { std::cout << "enemy deleted " << this << std::endl; }

private:

    const GLfloat weaponRange_ = 500.0f;

    GLboolean dead_;

    GLfloat rndnum();
};
