#include "GT_Ocean.h"

#include "SOIL/SOIL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

GT_Ocean::GT_Ocean()
    : GT_Primitive("../Content/ocean.jpg")
{
    sideA_ = 1000.0f;
    sideB_ = sideA_;
    initValues();
    defineTexture();
    defineVAO();
    defineShader();
    draw();
}

GT_Ocean::GT_Ocean(const char* textureImage)
    : GT_Primitive(textureImage)
{
    sideA_ = 1000.0f;
    sideB_ = sideA_;

    initValues();
    defineTexture();
    defineVAO();
    defineShader();
    draw();
}

void GT_Ocean::draw()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, primitiveTexture_);

    GLuint modelLoc = glGetUniformLocation(primitiveShader_->shaderProgram_, "model");

    glBindVertexArray(VAO_);
    for (int i = -50; i < 50; i++)
        for (int j = -50; j < 50; j++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model,  glm::vec3(2*vertices_[7]*i, 0.0f, 2*vertices_[7]*j));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const GLvoid*)0);
        }
    glBindVertexArray(0);
}
