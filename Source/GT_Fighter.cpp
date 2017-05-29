#include "GT_Fighter.h"
#include "GT_Locator.h"

GT_Fighter::GT_Fighter()
    :
      GT_Aircraft(GT_Locator::getModel(F18), GT_Locator::getModel(AIM), 10)
{


    std::cout << "GT_Fighter initialized " << std::endl;

}
