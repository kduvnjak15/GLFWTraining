#pragma once

#include "GT_Ocean.h"
#include "GT_Skybox.h"

class GT_Warehouse
{
public:

    GT_Skybox* getSkybox();
    GT_Ocean* getOcean();

    ~GT_Warehouse();


private:

    GT_Skybox* skybox_;
    GT_Ocean* ocean_;
};
