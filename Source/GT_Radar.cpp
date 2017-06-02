
#include "GT_Radar.h"
#include "glm/gtx/vector_angle.hpp"
#include "glm/trigonometric.hpp"


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

void GT_Radar::scanRadar(GT_Camera* tempCam)
{
//    coordinates_.clear();
//    glm::vec3 front = tempCam->getCameraFront();
//    for (int i = 1; i < radarActorList_.size(); i++)
//    {
//        glm::vec3 dir = radarActorList_[i]->modelPos - tempCam->getCameraPos();
//        front.y = 0.0;
//        dir.y   = 0.0;

//        GLfloat radius = glm::length(dir) / 10000.0f;
//        if (radius > 0.13)
//            radius = 0.13;

//        dir   = glm::normalize(dir);
//        front = glm::normalize(front);
//        GLfloat angle = glm::orientedAngle(front, dir, glm::vec3(0.0f, 1.0f, 0.0f));

//        if (dynamic_cast<GT_Rocket*> (radarActorList_[i]) )
//        {
//            GT_Rocket* dinRocket = dynamic_cast<GT_Rocket*> (radarActorList_[i]);
//            if (dinRocket->isFired() && !dinRocket->dead_)
//                coordinates_.push_back(glm::vec3(angle, radius, 1));
//            else
//                coordinates_.push_back(glm::vec3(angle, radius, -1));
//        }
//        else if (dynamic_cast<GT_Raptor*> (radarActorList_[i]))
//        {
//            GT_Raptor* dinRaptor = dynamic_cast<GT_Raptor*> (radarActorList_[i]);
//            if (!dinRaptor->isHit())
//                coordinates_.push_back(glm::vec3(angle, radius, 2));
//            else
//                coordinates_.push_back(glm::vec3(angle, radius, -2));
//        }
////        else if (dynamic_cast<GT_USSCarrier*> (radarActorList_[i]))
////        {
////            GT_USSCarrier* dinCarrier = dynamic_cast<GT_USSCarrier*> (radarActorList_[i]);
////            if (!dinCarrier->isHit())
////                coordinates_.push_back(glm::vec3(angle, radius, 3));
////            else
////                coordinates_.push_back(glm::vec3(angle, radius, -3));
////        }
//    }
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
    for (int i = 0; i < radarActorList_.size(); i++)
    {
        if (coordinates_[i].z > 0 )
            enemyAvatarObj_.draw(tempCam, coordinates_[i].x, coordinates_[i].y, coordinates_[i].z);
    }
}

