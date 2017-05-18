#pragma once

#include "GT_Model.h"
#include "GT_Camera.h"

#include <glm/glm.hpp>

class GT_Missile
{
public:

    GT_Missile(GT_Model* missileModel, void* ownerPtr);
    ~GT_Missile();

    void Draw(const GT_Camera* tempCam);
    void Integrate(const GLfloat DX_);

    void setPosition(const glm::vec3 pos_);

protected:

    void* ownerPtr_;

    GT_Model* missileModel_;
    GT_Shader* missileShader_;

    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;

};
