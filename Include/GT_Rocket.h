#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GT_Model.h"
#include "GT_Enemy.h"
#include "GT_Particle.h"

const GLfloat MISSILE_LIFE = 15.0f;
const GLfloat MISSILE_SPEED = 20.0f;

struct contrail
{
    glm::vec3 position_;
    GLfloat birthday_;
};

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

    std::vector<contrail> getParticles() { return particles_ ; }

private:

    void destroy();

    GLfloat birthday_;
    GT_Enemy* target_;
    GLboolean fired_;

    std::vector<GT_Particle*> contrail_;
    std::vector<contrail> particles_;

};
