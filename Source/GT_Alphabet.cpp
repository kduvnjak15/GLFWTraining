#include <iostream>

#include "GT_Alphabet.h"

GT_Alphabet::GT_Alphabet()
{

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout<< "ERROR::FREETYPE: Could not initi FreeType Library" <<std::endl;
    }

    FT_Face face;
    if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
    {
        std::cout << "ERRROR::FREETYPE Failed to load font " << std::endl;
    }



}
