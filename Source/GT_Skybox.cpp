
#include <iostream>
#include <GL/glew.h>

#include <string>

#include "SOIL/src/SOIL.h"
#include "GT_Skybox.h"

GT_Skybox::GT_Skybox()
    : skyboxShader_(new GT_Shader(skyboxShader, "../Shaders/skybox.vs", "../Shaders/skybox.fs"))
{

    glGenTextures(1, &skyboxTexID);
    std::cout<<"Konstruktor "<<skyboxTexID<<std::endl;

    glActiveTexture(GL_TEXTURE0);

    loadCubeMaps();

    int width, height;
    unsigned char* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexID);

    for (GLuint i = 0; i<cubemaps.size(); i++)
    {
        image = SOIL_load_image(cubemaps[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
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


void GT_Skybox::Draw(GT_Camera* tempCam)
{
    glDepthFunc(GL_LEQUAL);
    skyboxShader_->Use();

    viewLoc_  = glGetUniformLocation(skyboxShader_->shaderProgram_, "view");
    projLoc_  = glGetUniformLocation(skyboxShader_->shaderProgram_, "projection");
    glm::mat4 view = glm::mat4(glm::mat3(tempCam->GetViewMatrix()));
    glm::mat4 projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, 10000.0f);
    glUniformMatrix4fv(viewLoc_, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc_, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(this->skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyboxTexID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
  //  glDepthFunc(GL_LESS);



}
