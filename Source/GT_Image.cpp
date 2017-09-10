#include "GT_Image.h"
#include "SOIL/SOIL.h"

#include "GT_Utils.h"

#define DISParr(e) for (int i = 0; i< (sizeof(e)/sizeof(GLfloat)); i++) std::cout << e[i] << std::endl;
#define DISP(e) std::cout<< e << std::endl;

GT_Image::GT_Image(const char *texturePath)
    :
      texturePath_(texturePath),
      hasTexture_(true),
      brightness_(1.0f),
      transparency_(1.0f)

{
    initScreenCoords();
    defineVAO();
    defineShader();
    defineTexture();

    std::cout << "textured slika ctor "<< this << std::endl;
}

GT_Image::GT_Image(GLfloat r_, GLfloat g_, GLfloat b_, GLfloat alpha )
    :
      hasTexture_(false),
      brightness_(1.0f),
      transparency_(1.0f)

{
    RGBA_[0] = r_;
    RGBA_[1] = g_;
    RGBA_[2] = b_;
    RGBA_[3] = alpha;


    initScreenCoords();
    defineVAO();
    defineShader();

    std::cout << "coloured slika ctor" << this << std::endl;

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

    ///////////////////////////////

}

void GT_Image::defineVAO()
{
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);
    glGenBuffers(1, &TBO_);
    glGenBuffers(1, &CBO_);
   // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(imageCoords_), imageCoords_, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindBuffer(GL_ARRAY_BUFFER, TBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords_), textureCoords_, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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

void GT_Image::defineImageCoordinates(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{

    if (x2 <= x1 || y2 <= y1)
    {
        std::cout << "epic error, wrong input image coordinates"<< std::endl;
        return;
    }

    GLuint win_w, win_h;
    GT_Utils::getWindowWidthAndHeight(win_w, win_h);

    win_w *= 0.5;
    win_h *= 0.5;

    // Top left
    imageCoords_[0] = (x1-win_w)/win_w;
    imageCoords_[1] =-(y1-win_h)/win_h;
    // Bottom left
    imageCoords_[2] = (x1-win_w)/win_w ;
    imageCoords_[3] =-(y1-win_h)/win_h - (y2-y1)/win_h;
    // Bottom right
    imageCoords_[4] = (x2-win_w)/win_w;
    imageCoords_[5] =-(y2-win_h)/win_h;
    // Top right
    imageCoords_[6] = (x2-win_w)/win_w;
    imageCoords_[7] =-(y2-win_h)/win_h + (y2-y1)/win_h;


    for (int i = 0; i < 8; i++)
    {
        std::cout << imageCoords_[i] << std::endl;
    }
    defineVAO();
}

void GT_Image::defineImageScreenCoordinates(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    if (abs(x1) > 1 || abs(y1) > 1 || abs(x2) > 1 || abs(y2) > 1 )
    {
        std::cout << "epic fail, wrong input image screen coordinates" << std::endl;
        return;
    }

    // Top left
    imageCoords_[0] = x1;
    imageCoords_[1] = -y1;
    // Bottom left
    imageCoords_[2] = x1;
    imageCoords_[3] = -y2;
    // Bottom right
    imageCoords_[4] = x2;
    imageCoords_[5] = -y2;
    // Top right
    imageCoords_[6] = x2;
    imageCoords_[7] = -y1;

    defineVAO();
}

void GT_Image::setTransparency(GLfloat alpha)
{
    transparency_ = alpha;
}

void GT_Image::setBrightness(GLfloat bright)
{
    brightness_ = bright;
}

void GT_Image::Draw()
{

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->imageShader_->Use();
    glUniform1f(glGetUniformLocation(imageShader_->shaderProgram_, "brightness"), brightness_ );
    glUniform1f(glGetUniformLocation(imageShader_->shaderProgram_, "transparency"), transparency_ );

    if (hasTexture_)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureHandle_);
    }
    else
    {
        glUniform1i(glGetUniformLocation(imageShader_->shaderProgram_, "hasTexture"), -10);
        glUniform4f(glGetUniformLocation(imageShader_->shaderProgram_, "rgba_"),RGBA_[0],RGBA_[1],RGBA_[2],RGBA_[3] );
    }

    glBindVertexArray(VAO_);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);


}

