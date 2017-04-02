#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GT_Model.h"

const GLfloat rocketSpeed = 1.0f;

class GT_Rocket : public  GT_Model
{
public:

    GT_Rocket(const char* path);

    void Fire(GT_Model *target);

    GLboolean isFired();

    void move();



private:

    glm::vec3 target_;
    GLboolean fired_;
};
