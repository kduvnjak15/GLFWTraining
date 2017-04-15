//#include "GT_Terrain.h"

//#include "SOIL/SOIL.h"
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtc/type_ptr.hpp"

//GT_Terrain::GT_Terrain()
//    : GT_Primitive(),
//      position(glm::vec3(0.0, -50.0, -500.0))

//{
//    initValues();
//    defineTexture();
//    defineVAO();
//    defineShader();
//    draw();
//}

//void GT_Terrain::initValues()
//{
//    GLfloat a = 1000.0f;
//    GLfloat h = 0.0f;
//    // left up
//    vertices_[0] = -a;
//    vertices_[1] =  h;
//    vertices_[2] = -a;

//    vertices_[3] =  0.0f;
//    vertices_[4] =  1.0f;

//    // left down
//    vertices_[5] = -a;
//    vertices_[6] =  h;
//    vertices_[7] =  a;

//    vertices_[8] =  0.0f;
//    vertices_[9] =  0.0f;

//    // right near
//    vertices_[10] =  a;
//    vertices_[11] =  h;
//    vertices_[12] =  a;

//    vertices_[13] =  1.0f;
//    vertices_[14] =  0.0f;

//    // top right
//    vertices_[15]  =  a;
//    vertices_[16] =  h;
//    vertices_[17] = -a;

//    vertices_[18] =  1.0f;
//    vertices_[19] =  1.0f;


//    /****************************/

//    indices_[0] = 0;
//    indices_[1] = 1;
//    indices_[2] = 2;

//    indices_[3] = 2;
//    indices_[4] = 3;
//    indices_[5] = 0;

//    /****************************/

//    texCoords_[0] = 1.0f;
//    texCoords_[1] = 1.0f;

//    texCoords_[2] = 1.0f;
//    texCoords_[3] = 0.0f;

//    texCoords_[4] = 0.0f;
//    texCoords_[5] = 0.0f;

//    texCoords_[6] = 0.0f;
//    texCoords_[7] = 1.0f;
//}

//void GT_Terrain::defineTexture()
//{
//    int width, height;
//    unsigned char* image = SOIL_load_image("../Content/ocean.jpg", &width, &height, 0, SOIL_LOAD_RGB);


//    glGenTextures(1, &terrainTexture_);
//    glBindTexture(GL_TEXTURE_2D, terrainTexture_);

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//    //Set texture filtering
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//    glGenerateMipmap(GL_TEXTURE_2D);

//    SOIL_free_image_data(image);
//    glBindTexture(GL_TEXTURE_2D, 0);

//}

//void GT_Terrain::defineVAO()
//{
//    glGenVertexArrays(1, &VAO_);
//    glGenBuffers(1, &VBO_);
//    glGenBuffers(1, &TBO_);
//    glGenBuffers(1, &EBO_);

//    glBindVertexArray(VAO_);

//    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);
//    // Position attribute

//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(0);

//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
//    glEnableVertexAttribArray(1);


//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW);



//    glBindVertexArray(0);

//}

//void GT_Terrain::defineShader()
//{

//    primitiveShader_ = new GT_Shader(terrainShader, "../Shaders/terrainShader.vs", "../Shaders/terrainShader.fs");
//}

//void GT_Terrain::draw()
//{
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, terrainTexture_);

//    GLuint modelLoc = glGetUniformLocation(primitiveShader_->shaderProgram_, "model");

//    glBindVertexArray(VAO_);
//    for (int i = -50; i < 50; i++)
//        for (int j = -50; j < 50; j++)
//        {
//            glm::mat4 model = glm::mat4(1.0f);
//            model = glm::translate(model,  glm::vec3(2*vertices_[7]*i, 0.0f, 2*vertices_[7]*j));
//            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

//            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const GLvoid*)0);
//        }

//    glBindVertexArray(0);

//}

