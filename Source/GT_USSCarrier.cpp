#include "GT_USSCarrier.h"
#include "GT_Locator.h"

GT_USSCarrier::GT_USSCarrier()
    :
      GT_Aircraft(GT_Locator::getModel(USS))
{
    this->position_ = glm::vec3(0.0f, 80.0f, 0.0f);
    this->scaleActor_ = 200.0f;
    std::cout << "USSCarrier initialized " << this << std::endl;
}



void GT_USSCarrier::Draw(GT_Camera *tempCam)
{

    if (!actorModel_)
    {
        std::cout << "actorModel non existing " << std::endl;
        return;
    }

    this->actorShader_->Use();

    glUniform1i(glGetUniformLocation(actorShader_->shaderProgram_, "material.diffuse"),  0);
    glUniform1i(glGetUniformLocation(actorShader_->shaderProgram_, "material.specular"), 1);

    glUniform3f(lightPosLoc_,    lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(viewPosLoc_,     tempCam->getCameraPos().x, tempCam->getCameraPos().y, tempCam->getCameraPos().z);
    // Set lights properties
    glUniform3f(glGetUniformLocation(actorShader_->shaderProgram_, "light.ambient"),  0.50, 0.50f, 0.50f);
    glUniform3f(glGetUniformLocation(actorShader_->shaderProgram_, "light.diffuse"),  0.80, 0.80f, 0.80f);
    glUniform3f(glGetUniformLocation(actorShader_->shaderProgram_, "light.specular"), 0.1, 0.1f, 0.1f);
    // Set material properties
    glUniform1f(glGetUniformLocation(actorShader_->shaderProgram_, "material.shininess"), 1.0f);

    glUniform1i(explodeLoc_, explode_);
    glUniform1f(glGetUniformLocation(actorShader_->shaderProgram_, "time"), glfwGetTime() - explodeTime_);

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 sc  = glm::scale(glm::mat4(1.0f), glm::vec3(scaleActor_));

    glm::mat4 tr  = glm::translate(glm::mat4(1.0f), position_ );

    glm::mat4 view = tempCam->GetViewMatrix();

    glm::mat4 projection = tempCam->GetProjectionMatrix();

    model =  tr * sc;

    glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc_,  1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc_,  1, GL_FALSE, glm::value_ptr(projection));

    actorModel_->Draw(*actorShader_);
}


void GT_USSCarrier::Integrate(GT_Camera *tempCam, GLfloat DT_)
{
    GT_Aircraft::Integrate(tempCam,DT_);

    this->position_ += glm::vec3(0.0f, -10.0f * DT_, 0.0f);
    this->front_    += glm::vec3(-10.0f * DT_, 0.0f, 0.0f);
    this->right_    += glm::vec3(0.0f, 0.0f, -10.0f * DT_);

}
