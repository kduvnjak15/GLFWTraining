#pragma once

#include <GT_Model.h>
#include <glm/glm.hpp>


enum class MODEL_ENUM
{
    F18,
    F22,
    AIM,
    USS
};

class GT_Actor
{
public:
    GT_Actor();
    GT_Actor(GT_Model *actorModel);
    ~GT_Actor();

    virtual void Draw() = 0;
    virtual void Integrate(GLfloat DX_) = 0;

protected:

    GT_Model* actorModel_;
    GT_Shader* actorShader_;

    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;



};
