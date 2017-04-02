#pragma once
#include "tinyxml2.h"

class GT_Parser
{
public:
    GT_Parser(const char* inputFile);
    ~GT_Parser();

private:
    tinyxml2::XMLDocument* xmldoc_;

};
