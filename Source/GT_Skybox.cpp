
#include <iostream>
#include <GL/glew.h>

#include "SOIL/SOIL.h"
#include "GT_Skybox.h"

GT_Skybox::GT_Skybox()
{

    glGenTextures(1, &skyboxTexID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexID);

    loadCubeMaps();

    int width, height;
    unsigned char* image;
    for (GLuint i = 0; i<cubemaps.size(); i++)
    {
        image = SOIL_load_image(cubemaps[i], &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

void  GT_Skybox::createSkyboxVAO()
{
    glGenVertexArrays(1, &skyboxVAO);
    glBindVertexArray(skyboxVAO);



glBindVertexArray(0);

}


void GT_Skybox::loadCubeMaps()
{
    cubemaps.push_back("../Content/Skybox/right.jpg");
    cubemaps.push_back("../Content/Skybox/left.jpg");
    cubemaps.push_back("../Content/Skybox/top.jpg");
    cubemaps.push_back("../Content/Skybox/bottom.jpg");
    cubemaps.push_back("../Content/Skybox/back.jpg");
    cubemaps.push_back("../Content/Skybox/front.jpg");
}
