#include "GT_Objectives.h"

void GT_Objective::defineLevel(int currLevel)
{
    if (currLevel == 0)
    {
        currentLevelHeadline_ = "Level 1";
        currentLevelObjective_.push_back("Destroy Enemy aircraft");
    }

    if (currLevel == 1)
    {
        currentLevelHeadline_ = "Level 2";
        currentLevelObjective_.push_back("Destroy Enemy aircrafts");
        currentLevelObjective_.push_back("Destroy Enemy warship");
    }


}


