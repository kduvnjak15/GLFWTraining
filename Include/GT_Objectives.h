#pragma once

#include <vector>
#include <string>

class GT_Objective
{
public:

    GT_Objective()
    {
        currentLevelHeadline_ = "";
        currentLevelObjective_.clear();
    }

    void defineLevel(int currLevel);

    std::string currentLevelHeadline_;
    std::vector<std::string> currentLevelObjective_;


};
