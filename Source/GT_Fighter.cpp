#include "GT_Fighter.h"

GT_Fighter::GT_Fighter(GT_Warehouse *warehouse)
    :
      GT_Aircraft(warehouse->getModel(F18), warehouse->getModel(AIM), 10)
{


    std::cout << "GT_Fighter initialized " << std::endl;

}
