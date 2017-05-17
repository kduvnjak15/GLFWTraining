#pragma once

#include "GT_Aircraft.h"

#include "GT_Ocean.h"
#include "GT_Skybox.h"

class GT_Warehouse
{
public:

    GT_Skybox*      getSkybox();
    GT_Ocean*       getOcean();
    GT_Aircraft*    getAircraft(AIRCRAFT aircraftEnum);

    void loadModels();
    void defineAircrafts();

    ~GT_Warehouse();


private:


    GT_Skybox* skybox_;
    GT_Ocean* ocean_;

    std::vector<GT_Model*> models_;
    std::map< AIRCRAFT, GT_Model*> modelMap_;
    std::map< AIRCRAFT, GT_Aircraft*> aircraftMap_;

    std::vector<GT_Aircraft*> aircafts_;



};
