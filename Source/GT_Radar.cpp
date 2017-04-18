
#include "GT_Radar.h"

#include "GT_Raptor.h"
#include "GT_USSCarrier.h"
#include "GT_Rocket.h"

#include "glm/gtx/vector_angle.hpp"


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

void GT_Radar::setupAvatar()
{





}

void GT_Radar::scanRadar(GT_Camera* tempCam)
{
    coordinates_.clear();
    for (int i = 1; i < radarActorList_.size(); i++)
    {
        glm::vec3 dir = radarActorList_[i]->modelPos - tempCam->getCameraPos();
        dir = glm::normalize(dir);

        glm::vec2 dir2D = glm::vec2(dir.x, dir.z);
        glm::vec2 front2D = glm::vec2(tempCam->getCameraFront().x, tempCam->getCameraFront().z);

        GLfloat angle = glm::angle(dir2D,front2D );
        std::cout << angle<<" in radians" <<std::endl;
        dir /=10.0f;
        std::cout<<i << "-> "<<dir.x<<", "<<dir.z<<std::endl;
        coordinates_.push_back(glm::vec2(dir.x, dir.z));
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

    for (int i = 1; i < radarActorList_.size(); i++)
    {
        GT_Rocket* dinRocket;
        if (dynamic_cast<GT_Rocket*> (radarActorList_[i]) )
        {
            dinRocket = dynamic_cast<GT_Rocket*> (radarActorList_[i]);
            if (dinRocket->isFired())
                enemyAvatarObj_.draw(tempCam, coordinates_[i].x, coordinates_[i].y, 1);
        }
        else if (dynamic_cast<GT_Raptor*> (radarActorList_[i]))
            enemyAvatarObj_.draw(tempCam, coordinates_[i].x, coordinates_[i].y, 2);
        else if (dynamic_cast<GT_USSCarrier*> (radarActorList_[i]))
            enemyAvatarObj_.draw(tempCam, coordinates_[i].x, coordinates_[i].y, 3);
    }
}

void GT_Radar::drawAvatars(GT_Camera *tempCam)
{
    //enemyAvatarObj_.draw(tempCam);
}

