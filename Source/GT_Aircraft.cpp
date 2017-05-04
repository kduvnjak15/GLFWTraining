#include <iostream>

#include "GT_Aircraft.h"


GT_Aircraft::GT_Aircraft(GT_Model *aircraftModel)
    : GT_Actor(),
      aircraftShader_(new GT_Shader(aircraftShader, "../Shaders/aircraftShader.vs", "../Shaders/aircraftShader.fs", "../Shaders/aircraftShader.gs"))
{

    std::cout << "GT_Aircraft initialized " << this << std::endl;
}

void GT_Aircraft::Draw()
{
    if (actorModel_) actorModel_->Draw(*aircraftShader_);
}

void GT_Aircraft::Integrate(GLfloat DX_)
{
    position_ += front_*DX_;
    std::cout << "Integrate aircraft " << std::endl;
}

GT_Aircraft::~GT_Aircraft()
{

    delete aircraftShader_;

    std::cout << "GT_Aircraft dtor" << this << std::endl;
}

