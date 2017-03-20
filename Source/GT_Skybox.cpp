
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

    createSkybox();

    std::cout<<"skybox defined"<<std::endl;

}


void  GT_Skybox::createSkybox()
{
    // Setup cube VAO
    glGenVertexArrays(1, &cubeVAO);

    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);

    glBindVertexArray(0);


    // Setup skybox VAO
    glGenVertexArrays(1, &skyboxVAO);

    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

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
