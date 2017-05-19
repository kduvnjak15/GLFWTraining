#pragma once


#include "GT_Actor.h"
#include "GT_Missile.h"
#include <glm/glm.hpp>


class GT_Aircraft : public GT_Actor
{
public:

    GT_Aircraft(GT_Model* aircraftModel, GT_Model* missileModel);
    ~GT_Aircraft();

    void Draw(GT_Camera *tempCam);
    void Integrate(GLfloat DT_);

    void loadMissiles(GT_Model* missileModel);

    glm::vec3 getPosition() { return position_; }

private:

    GLuint numOfMissiles_;
    GT_Model* missileModel_;
    std::vector<GT_Missile* > missiles_;
};
