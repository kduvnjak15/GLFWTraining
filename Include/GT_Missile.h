#pragma once

#include "GT_Actor.h"
#include "GT_Particle.h"

#include <glm/glm.hpp>
#include <list>

const GLfloat MISSILE_SPEED = 250.0f;
const GLfloat MISSILE_LIFE = 5.0f;

class GT_Missile : public GT_Actor
{
public:

    GT_Missile(GT_Model* missileModel, void *ownerPtr, GLuint missileIndex);
    ~GT_Missile();

    void Draw(GT_Camera* tempCam);
    void Integrate(GT_Camera* tempCam, GLfloat DX_);

    void FIRE();
    bool isFired() { return fired_; }
    GLboolean isDead() { return dead_;}


protected:

    void* ownerPtr_;
    GLuint missileIndex_;
    glm::vec3 missileOffset_;

private:

    bool fired_;
    bool dead_;
    GLfloat birthday_;

    std::list<glm::vec3> contrail_;
    GLuint contrailRate_;

    GT_Particle* particle_;

};
