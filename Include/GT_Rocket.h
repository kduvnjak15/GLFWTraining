#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GT_Model.h"
#include "GT_Enemy.h"
#include "GT_Particle.h"

const GLfloat MISSILE_LIFE = 5.0f;
const GLfloat MISSILE_SPEED = 20.0f;

class GT_Rocket : public  GT_Model
{
public:

    GT_Rocket(const char* path);
    ~GT_Rocket();

    void Fire(GT_Enemy *target);

    GLboolean isFired();

    void move();

    glm::vec3 wingSlotOffset;

    GLboolean dead_;

    std::vector<glm::vec3> getParticles() { return particles_ ; }

private:

    void destroy();

    GLfloat birthday_;
    GT_Enemy* target_;
    GLboolean fired_;

    std::vector<GT_Particle*> contrail_;
    std::vector<glm::vec3> particles_;

};
