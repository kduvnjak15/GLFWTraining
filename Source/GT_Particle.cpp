#include "GT_Particle.h"
#include "SOIL/src/SOIL.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


GT_Particle::GT_Particle()
    : GT_Primitive("../Content/fog.png")
{
    initValues();
    defineTexture();
    defineVAO();
    defineShader();
}


void GT_Particle::defineTexture()
{
    int width, height;
    unsigned char* image = SOIL_load_image(texturePath_, &width, &height, 0, SOIL_LOAD_RGBA);


    glGenTextures(1, &primitiveTexture_);
    glBindTexture(GL_TEXTURE_2D, primitiveTexture_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GT_Particle::draw()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, primitiveTexture_);

    glBindVertexArray(VAO_);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const GLvoid*)0);

    glBindVertexArray(0);
}



