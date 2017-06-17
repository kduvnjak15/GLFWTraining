#pragma once 

#include <glm/glm.hpp>
#include <vector>
#include "GT_Primitive.h"

#include "GT_Enemy.h"

class enemyAvatar : public GT_Primitive
{
public:
    friend class GT_Radar;

    enemyAvatar() : GT_Primitive()
    {
        initValues();
        defineVAO();
        defineShader();

        worldPos_ = glm::vec3(0.7f, 0.4f, -0.999f);
        std::cout << "Avatar spawned " << this << std::endl;
    }

    void defineShader()
    {
        primitiveShader_ = new GT_Shader(particleShader, "../Shaders/HUDShader.vs", "../Shaders/HUDShader.fs");
        modelLoc_   =  glGetUniformLocation(primitiveShader_->shaderProgram_, "model");
        viewLoc_    = glGetUniformLocation(primitiveShader_->shaderProgram_, "view");
        projLoc_    = glGetUniformLocation(primitiveShader_->shaderProgram_, "projection");
        colorLoc_    = glGetUniformLocation(primitiveShader_->shaderProgram_, "CPUcolor");
    }

    void draw(GT_Camera* tempCam, GLfloat angle, GLfloat radius, GLuint type)
    {

        this->primitiveShader_->Use();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(glm::mat4(1.0f), worldPos_);
        model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0.0f, radius, 0));
        if (type == 1)
            model = glm::scale(model, glm::vec3(.005f, .005f, 1.0f));
        else if (type == 3)
            model = glm::scale(model, glm::vec3(.015f, .015f, 1.0f));
        else
            model = glm::scale(model, glm::vec3(.01f, .01f, 1.0f));

        model = glm::rotate(model, (GLfloat)-3.14159/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 view = tempCam->GetViewMatrix();
        model = glm::inverse(view) * model;
        glm::mat4 projection = tempCam->GetProjectionMatrix();
        glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc_, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc_, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1i(colorLoc_, type);
        glBindVertexArray(this->VAO_);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const GLvoid*)0);

        glBindVertexArray(0);

    }

    glm::vec3 worldPos_;
    GLuint colorLoc_;
};

class GT_Radar : public GT_Primitive
{
public:

    friend class GT_HUD;

    GT_Radar();

    void scanRadar(GT_Camera *tempCam);

    void draw(GT_Camera* tempCam);    

private:


    enemyAvatar enemyAvatarObj_;

    std::vector<glm::vec3> coordinates_;

    void setRadarEnemyList(std::vector<GT_Enemy*>& enemyList) {radarEnemyList_ = &enemyList;}
    void appendMissileToRadarList(GT_Missile* missileInstance) { radarMissileList_.push_back(missileInstance);}

    glm::vec2 calcCoordinates(glm::vec3 dir);

    void drawAvatars(GT_Camera* tempCam);

    std::vector<GT_Enemy*>* radarEnemyList_;
    std::vector<GT_Missile*> radarMissileList_;

    glm::vec3 radarScreenPos_;

};
