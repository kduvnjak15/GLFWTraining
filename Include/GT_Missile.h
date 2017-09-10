#pragma once

#include "GT_Aircraft.h"
#include "GT_Particle.h"
#include "SFML/Audio.hpp"

#include <glm/glm.hpp>
#include <list>

const GLfloat MISSILE_SPEED    = 600.0f;
const GLfloat MISSILE_LIFE     = 15.0f;
const GLfloat MISSILE_AGILITY  = 2.0f;

class GT_Missile : public GT_Actor
{
public:

    GT_Missile(GT_Model* missileModel, GT_Aircraft* ownerPtr, GLuint missileIndex);
    ~GT_Missile();

    void Draw(GT_Camera* tempCam);
    void Integrate(GLfloat DX_);

    void FIRE(GT_Aircraft* target);
    bool isFired() { return fired_; }

    GLboolean isDead() { return dead_;}

    glm::vec3 getPosition() {return position_; }

protected:

    GT_Aircraft* ownerPtr_;
    GLuint missileIndex_;
    glm::vec3 missileOffset_;

private:

    void aimEnemy();

    bool fired_;
    bool dead_;
    GLfloat birthday_;
    GT_Aircraft* enemyTarget_;


    std::list<glm::vec3> contrail_;
    GLuint contrailRate_;

    GT_Particle* particle_;

    sf::Sound sound_;
    bool dirtySound_ = true;


};
