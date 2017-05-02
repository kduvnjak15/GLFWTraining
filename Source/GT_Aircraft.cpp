#include <iostream>

#include "GT_Aircraft.h"



GT_Aircraft::GT_Aircraft()
    : aircraftShader_( new GT_Shader(aircraftShader, "../Shaders/aircraftShader.vs", "../Shaders/aircraftShader.fs", "../Shaders/aircraftShader.fs"))
{


    std::cout << "GT_Aircraft initialized " << this << std::endl;
}

GT_Aircraft::~GT_Aircraft()
{

    delete aircraftShader_;

    std::cout << "GT_Aircraft dtor" << this << std::endl;
}

