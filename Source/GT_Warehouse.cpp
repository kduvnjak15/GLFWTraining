#include "GT_Warehouse.h"

GT_Skybox* GT_Warehouse::getSkybox()
{
    if (!skybox_)
        skybox_ = new GT_Skybox();

    return skybox_;
}

GT_Ocean* GT_Warehouse::getOcean()
{
    if (!ocean_)
        ocean_ = new GT_Ocean();

    return ocean_;
}

GT_Aircraft* GT_Warehouse::getAircraft(AIRCRAFT aircraftEnum)
{
    return aircraftMap_[aircraftEnum];
}

GT_Model* GT_Warehouse::getMissileModel()
{
    return modelMap_[AIM];
}

void GT_Warehouse::loadModels()
{
    models_.push_back(new GT_Model("../Content/CV - Essex class/essex_scb-125_generic.obj"));
    modelMap_.insert(std::pair<AIRCRAFT, GT_Model*>(USS, models_[0]));
    models_.push_back(new GT_Model("../Content/FA-22_Raptor/FA-22_Raptor.obj"));
    modelMap_.insert(std::pair<AIRCRAFT, GT_Model*>(F22, models_[1]));
    models_.push_back(new GT_Model("../Content/FA-18_RC/FA-18_RC.obj"));
    modelMap_.insert(std::pair<AIRCRAFT, GT_Model*>(F18, models_[2]));
    models_.push_back(new GT_Model("../Content/AVMT300/AVMT300.3ds"));
    modelMap_.insert(std::pair<AIRCRAFT, GT_Model*>(AIM, models_[3]));
}

void GT_Warehouse::defineAircrafts()
{
    aircafts_.push_back(new GT_Aircraft(modelMap_[F18], modelMap_[AIM]));
    aircraftMap_.insert(std::pair<AIRCRAFT, GT_Aircraft*>(F18, aircafts_[0]));
    aircafts_.push_back(new GT_Aircraft(modelMap_[F22], modelMap_[AIM]));
    aircraftMap_.insert(std::pair<AIRCRAFT, GT_Aircraft*>(F22, aircafts_[1]));
}



GT_Warehouse::~GT_Warehouse()
{
    if (skybox_) delete skybox_;
    if (ocean_)  delete ocean_;

    for (auto it = models_.begin(); it != models_.end(); it++)
    {
        models_.erase(it);
    }
}
