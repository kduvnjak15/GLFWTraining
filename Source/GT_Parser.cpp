
#include <iostream>

#include "GT_Parser.h"
using namespace tinyxml2;

#ifndef XMLCheckResult
    #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

#define FINISH(e) { std::cout<< e <<std::endl; return; }

GT_Parser::GT_Parser(const char* inputFile)
{

#ifdef oldcode
    xmldoc_ = new tinyxml2::XMLDocument;
    xmldoc_->LoadFile(inputFile);

    if (xmldoc_->FirstChild() == nullptr) FINISH("FILE NOT FOUND!")

    XMLElement* pElement;
    XMLNode* pNode;

    pNode =xmldoc_->FirstChild();
    if (pNode == nullptr) FINISH("Root not found");

    pElement = pNode->FirstChildElement("");
    if (pElement == nullptr) FINISH("Actors node not found");
#else



#endif


}

GT_Parser::~GT_Parser()
{
    std::cout<<"~xmldoc" <<xmldoc_<<std::endl;
    delete xmldoc_;
}

