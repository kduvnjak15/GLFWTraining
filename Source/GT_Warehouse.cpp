#include "GT_Warehouse.h"

GT_Skybox* GT_Warehouse::getSkybox()
{
    if (!skybox_)
        skybox_ = new GT_Skybox();

    std::cout << "return skybox " << skybox_ << std::endl;
    return skybox_;
}

GT_Ocean* GT_Warehouse::getOcean()
{
    if (!ocean_)
        ocean_ = new GT_Ocean();

    std::cout << "return ocean " << ocean_ << std::endl;
    return ocean_;
}

GT_Warehouse::~GT_Warehouse()
{
    if (skybox_) delete skybox_;
}
