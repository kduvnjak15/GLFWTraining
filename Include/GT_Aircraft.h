#pragma once


#include "GT_Actor.h"

#include "GT_Missile.h"
#include <glm/glm.hpp>



class GT_Aircraft : public GT_Actor
{
public:

    GT_Aircraft(GT_Model* aircraftModel, GT_Model* missileModel);
    GT_Aircraft(GT_Model* aircraftModel, GT_Model* missileModel, GLuint missileCount);
    ~GT_Aircraft();

    virtual void Draw(GT_Camera *tempCam);

    void Integrate(GT_Camera* tempCam,  GLfloat DT_);

    void fireMissile();

    glm::vec3 getPosition() { return position_; }
    glm::vec3 setPosition(glm::vec3 positionSetter_) { position_ = positionSetter_;}

    GLuint numOfMissiles_;

protected:

    GLuint explodeLoc_;
    GLuint explode_;

    GT_Model* missileModel_;
    std::vector<GT_Missile* > missiles_;

    GLfloat aircraftSpeed_;


};
