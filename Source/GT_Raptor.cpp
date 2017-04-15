#include "GT_Raptor.h"

GT_Raptor::GT_Raptor(const char* modelPath)
    : GT_Enemy(modelPath)
{
    std::cout << "Raptor initialized " << this << std::endl;
}

void GT_Raptor::Draw(GT_Camera *tempCam)
{
    enemyShader_->Use();

    modelPos = glm::vec3(-1000.0f, 800.0f, -2500);

    modelPos.x += (cos(glfwGetTime()*0.1)+1)*400;
    modelPos.y += (sin(glfwGetTime()*0.1)+1)*100;
    modelPos.z += (sin(glfwGetTime()*0.1)+1)*400;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, modelPos);
    model = glm::rotate(model, (GLfloat)-3.14159/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(10.0f));

    glm::mat4 view       = tempCam->GetViewMatrix();
    glm::mat4 projection = tempCam->GetProjectionMatrix();
    glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc_, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc_, 1, GL_FALSE, glm::value_ptr(projection));

    if (isHit())
        glUniform1i(hitLoc_, 10);
    glUniform1f(timeLoc_, explosionTime());

    GT_Model::Draw(*enemyShader_);


}
