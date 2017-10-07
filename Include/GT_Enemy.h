#pragma once

#include "GT_Predator.h"


class GT_Enemy : public GT_Aircraft, public GT_Predator
{
public:

    GT_Enemy();

    void Draw(GT_Camera* tempCam);

    void Integrate(GT_Camera* tempCam, GLfloat DT_);

    void explode();

    bool isDead() { return dead_; }

    ~GT_Enemy() ;


private:

    GLboolean dead_;

};
