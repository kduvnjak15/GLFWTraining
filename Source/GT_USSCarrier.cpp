#include "GT_USSCarrier.h"

GT_USSCarrier::GT_USSCarrier(const char* modelPath)
    : GT_Enemy(modelPath)
{
    std::cout << "USSCarrier initialized " << this << std::endl;

}

void GT_USSCarrier::Draw(GT_Camera *tempCam)
{
    enemyShader_->Use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, modelPos);
    model = glm::scale(model, glm::vec3(1000.0f));

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
