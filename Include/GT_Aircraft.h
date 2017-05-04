#pragma once


#include <GT_Actor.h>
#include <glm/glm.hpp>


enum AIRCRAFT
{
    F18,
    F22,
    AIM,
    USS
};

class GT_Aircraft : public GT_Actor
{
public:

    GT_Aircraft(GT_Model* aircraftModel);
    ~GT_Aircraft();

    void Draw();
    void Integrate(GLfloat DX_);

private:

    GT_Shader* aircraftShader_;
};
