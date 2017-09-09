#include "GT_Image.h"
#include "SOIL/SOIL.h"

#define DISParr(e) for (int i = 0; i< (sizeof(e)/sizeof(GLfloat)); i++) std::cout << e[i] << std::endl;
#define DISP(e) std::cout<< e << std::endl;

GT_Image::GT_Image(const char *texturePath)
    :
      texturePath_(texturePath)
{
    initScreenCoords();
    defineVAO();
    defineShader();
    defineTexture();

    std::cout << "slika ctor "<< this << std::endl;
}


void GT_Image::initScreenCoords()
{
    // Top left
    imageCoords_[0] = -1.0f;
    imageCoords_[1] =  1.0f;
    // Bottom left
    imageCoords_[2] = -1.0f;
    imageCoords_[3] = -1.0f;
    // Bottom right
    imageCoords_[4] =  1.0f;
    imageCoords_[5] = -1.0f;
    // Top right
    imageCoords_[6] =  1.0f;
    imageCoords_[7] =  1.0f;

    ////////////////////////////////

    imageIndices_[0] = 0;
    imageIndices_[1] = 1;
    imageIndices_[2] = 2;

    imageIndices_[3] = 0;
    imageIndices_[4] = 2;
    imageIndices_[5] = 3;

    ////////////////////////////////

    textureCoords_[0] = 0.0f;
    textureCoords_[1] = 0.0f;

    textureCoords_[2] = 0.0f;
    textureCoords_[3] = 1.0f;

    textureCoords_[4] = 1.0f;
    textureCoords_[5] = 1.0f;

    textureCoords_[6] = 1.0f;
    textureCoords_[7] = 0.0f;

}

void GT_Image::defineVAO()
{
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);
    glGenBuffers(1, &TBO_);
   // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(imageCoords_), imageCoords_, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindBuffer(GL_ARRAY_BUFFER, TBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords_), textureCoords_, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
//    glBindBuffer(GL_ARRAY_BUFFER,0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(imageIndices_), imageIndices_, GL_STATIC_DRAW);


       // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
   //    glBindBuffer(GL_ARRAY_BUFFER, 0);

       // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
       //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

       // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
       // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
       glBindVertexArray(0);
}

void GT_Image::defineShader()
{
    imageShader_ = new GT_Shader(aircraftShader, "../Shaders/imageShader.vs", "../Shaders/imageShader.fs");
    DISP("define shader")
}


void GT_Image::defineTexture()
{
    int width, height;
    GLenum alphaChannel, soilAlpha;
    if (transparent_)
    {
        alphaChannel = GL_RGBA;
        soilAlpha    = SOIL_LOAD_RGBA;
    }
    else
    {
        alphaChannel = GL_RGB;
        soilAlpha    = SOIL_LOAD_RGB;
    }
    unsigned char* image = SOIL_load_image(texturePath_, &width, &height, 0, soilAlpha);


    glGenTextures(1, &textureHandle_);
    glBindTexture(GL_TEXTURE_2D, textureHandle_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, alphaChannel, width, height, 0, alphaChannel, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    DISP("define text")
}


void GT_Image::Draw()
{

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureHandle_);

    this->imageShader_->Use();

    glBindVertexArray(VAO_);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);


}
