#pragma once

#include "GT_Ocean.h"
#include "GT_Skybox.h"

#include "GT_Aircraft.h"


class GT_Warehouse
{
public:

    GT_Skybox*      getSkybox();
    GT_Ocean*       getOcean();    
    GT_Model*       getModel(MODEL_TYPE modelEnum);

    void loadModels();
    void defineActors();

    ~GT_Warehouse();


private:


    GT_Skybox* skybox_;
    GT_Ocean* ocean_;

    std::vector<GT_Model*> models_;
    std::map< MODEL_TYPE, GT_Model*> modelMap_;
    std::map< MODEL_TYPE, GT_Actor*> actorMap_;

    std::vector<GT_Actor*> actors_;



};
