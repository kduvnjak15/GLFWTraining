#pragma once

#include <GT_Model.h>
#include <glm/glm.hpp>
#include "GT_Camera.h"

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

    virtual void Draw(GT_Camera* tempCam) = 0;
    virtual void Integrate(GLfloat DX_) = 0;

    void setPosition(glm::vec3 pos_);

protected:

    GT_Model* actorModel_;
    GT_Shader* actorShader_;

    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;



};
