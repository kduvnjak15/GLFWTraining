
#include "GT_Radar.h"

GT_Radar::GT_Radar()
    : GT_Primitive("../Content/radar.jpg")
{
    std::cout << "Radar installed "<< this << std::endl;


    initValues();
    defineTexture();
    defineVAO();
    defineShader();

}

void GT_Radar::draw(GT_Camera *tempCam)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, screenPos_);
    model = glm::scale(model, screenPos_);
    model = glm::rotate(model, (GLfloat)-3.14159/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 projection = tempCam->GetProjectionMatrix();
    glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(projLoc_, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO_);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const GLvoid*)0);

    glBindVertexArray(0);
}
