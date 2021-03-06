
#include "GT_Radar.h"
#include "glm/gtx/vector_angle.hpp"
#include "glm/trigonometric.hpp"

#include "GT_Locator.h"


GT_Radar::GT_Radar()
    : GT_Primitive("../Content/radar.png")
{
    std::cout << "Radar installed "<< this << std::endl;

    transparent_ = true;

    initValues();
    defineTexture();
    defineVAO();
    defineShader();

    radarScreenPos_ =  glm::vec3(0.7f, 0.4f, -0.999f);
}

glm::vec2 GT_Radar::calcCoordinates(glm::vec3 dir)
{
    glm::vec3 front = GT_Locator::getGameCamera()->getCameraFront();

    front.y = 0.0;
    dir.y   = 0.0;

    GLfloat radius = glm::length(dir) / 10000.0f;
    if (radius > 0.13)
        radius = 0.13;

    dir   = glm::normalize(dir);
    front = glm::normalize(front);
    GLfloat angle = glm::orientedAngle(front, dir, glm::vec3(0.0f, 1.0f, 0.0f));

    return glm::vec2(angle, radius);

}

void GT_Radar::scanRadar(GT_Camera* tempCam)
{
    coordinates_.clear();
    coordinates_.push_back( glm::vec3(calcCoordinates(GT_Locator::getUSSCarrier()->getPosition() - GT_Locator::getFighter()->getPosition()), 3));

    for (int i = 0; i < radarEnemyList_->size(); i++)
    {
        glm::vec3 dir = (*(radarEnemyList_))[i]->getPosition() - tempCam->getCameraPos();
        coordinates_.push_back( glm::vec3(calcCoordinates(dir), 2.0f));
    }

    auto mit = radarMissileList_.begin();
    while (mit != radarMissileList_.end())
    {
        if ((*mit)->isFired())
        {
            if ((*mit)->isDead())
            {
                mit = radarMissileList_.erase(mit);
                continue;
            }
            else
            {
                glm::vec3 dir = (*mit)->getPosition() - tempCam->getCameraPos();
                coordinates_.push_back( glm::vec3(calcCoordinates(dir), 1.0f));
            }
        }

        mit++;
    }

}

void GT_Radar::draw(GT_Camera *tempCam)
{
    scanRadar(tempCam);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, primitiveTexture_);

    primitiveShader_->Use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, radarScreenPos_);
    model = glm::scale(model, glm::vec3(.15f, .15f, 1.0f));
    model = glm::rotate(model, (GLfloat)glfwGetTime()*3, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, (GLfloat)-3.14159/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = tempCam->GetViewMatrix();
    model = glm::inverse(view) * model;
    glm::mat4 projection = tempCam->GetProjectionMatrix();
    glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc_, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc_, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO_);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const GLvoid*)0);

    glBindVertexArray(0);

    drawAvatars(tempCam);

}

void GT_Radar::drawAvatars(GT_Camera *tempCam)
{
    for (int i = 0; i < coordinates_.size(); i++)
    {
        if (coordinates_[i].z > 0 )
            enemyAvatarObj_.draw(tempCam, coordinates_[i].x, coordinates_[i].y, coordinates_[i].z);
    }
}

