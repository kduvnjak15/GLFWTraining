#include "GT_Ocean.h"

#include "SOIL/src/SOIL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

GT_Ocean::GT_Ocean()
    : GT_Primitive("../Content/ocean.jpg"),
      modelLoc_( glGetUniformLocation(this->primitiveShader_->shaderProgram_, "model")),
      viewLoc_( glGetUniformLocation(this->primitiveShader_->shaderProgram_, "view")),
      projLoc_( glGetUniformLocation(this->primitiveShader_->shaderProgram_, "proj"))
{
    sideA_ = 10000.0f;
    sideB_ = sideA_;
    initValues();
    defineTexture();
    defineVAO();
    defineShader();

}

GT_Ocean::GT_Ocean(const char* textureImage)
    : GT_Primitive(textureImage),
      modelLoc_( glGetUniformLocation(this->primitiveShader_->shaderProgram_, "model")),
      viewLoc_( glGetUniformLocation(this->primitiveShader_->shaderProgram_, "view")),
      projLoc_( glGetUniformLocation(this->primitiveShader_->shaderProgram_, "proj"))
{
    sideA_ = 10000.0f;
    sideB_ = sideA_;

    initValues();
    defineTexture();
    defineVAO();
    defineShader();

    std::cout << "GT_Ocean initialized "<< std::endl;
}

void GT_Ocean::draw(GT_Camera* tempCam)
{
    this->primitiveShader_->Use();
    modelLoc_  = glGetUniformLocation(this->primitiveShader_->shaderProgram_, "model");
    projLoc_   = glGetUniformLocation(this->primitiveShader_->shaderProgram_, "projection");
    viewLoc_   = glGetUniformLocation(this->primitiveShader_->shaderProgram_, "view");

    glm::mat4 view       = tempCam->GetViewMatrix();
    glm::mat4 projection = tempCam->GetProjectionMatrix();

    glUniformMatrix4fv(viewLoc_, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc_, 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, primitiveTexture_);

    GLuint modelLoc = glGetUniformLocation(primitiveShader_->shaderProgram_, "model");

    glBindVertexArray(VAO_);
    for (int i = -20; i < 20; i++)
        for (int j = -20; j < 20; j++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model,  glm::vec3(2*vertices_[7]*i, 0.0f, 2*vertices_[7]*j));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const GLvoid*)0);
        }
    glBindVertexArray(0);
}
