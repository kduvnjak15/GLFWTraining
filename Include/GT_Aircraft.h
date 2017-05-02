#pragma once

#include <GT_Model.h>
#include <glm/glm.hpp>

class GT_Aircraft
{
public:
    GT_Aircraft();
    ~GT_Aircraft();

    void Draw();

private:

    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;

    GT_Model* model_;
    GT_Shader* aircraftShader_;
};
