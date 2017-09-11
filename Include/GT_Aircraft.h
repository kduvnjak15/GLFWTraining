#pragma once


#include "GT_Actor.h"

#include <glm/glm.hpp>
#include "SFML/Audio.hpp"

class GT_Aircraft : public GT_Actor
{
public:

    GT_Aircraft(GT_Model* aircraftModel);
    ~GT_Aircraft();

    virtual void Draw(GT_Camera *tempCam);

    virtual void Integrate(GT_Camera* tempCam,  GLfloat DT_);

    glm::vec3 getPosition() { return position_; }
    glm::vec3 getFront()    { return front_; }
    glm::vec3 getRight()    { return right_; }
    glm::vec3 getUp()       { return up_; }

    GLboolean explode_;

    void explode();

protected:

    GLuint explodeLoc_;
    GLuint explodeTime_;

    GLfloat aircraftSpeed_;
    sf::Sound sound_;
    bool dirtySound_ = true;


};
