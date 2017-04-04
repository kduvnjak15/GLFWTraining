#include <iostream>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GT_Shader.h"
#include "GT_Camera.h"
#include "GT_Model.h"
#include "GT_Skybox.h"
#include "GT_Alphabet.h"
#include "GT_Rocket.h"
#include "GT_Enemy.h"
#include "GT_Terrain.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define VmodelShader "../Shaders/modelShader.vs"
#define FmodelShader "../Shaders/modelShader.fs"
#define vLightShader  "../Shaders/vLightShader.vs"
#define fLightShader  "../Shaders/fLightShader.fs"
#define vsSkyboxShader "../Shaders/skybox.vs"
#define fsSkyboxShader "../Shaders/skybox.fs"
#define vsFontShader "../Shaders/fontShader.vs"
#define fsFontShader "../Shaders/fontShader.fs"
#define vsEnemyShader "../Shaders/enemyShader.vs"
#define fsEnemyShader "../Shaders/enemyShader.fs"
#define gsEnemyShader "../Shaders/enemyShader.gs"


bool keys[1024];
bool firstMouse = true;

const GLuint window_width = 1024;
const GLuint window_height = 600;
GLfloat lastX = window_width/2, lastY = window_height/2;
const GLfloat horizon = 50000;


GLfloat runTime = 0.0f;
GLfloat fps = 60.0f;

GLfloat currentFrame = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::vec3 lightPos(5.0f, 4.0f, 3.0f);

class initialCallbacks
{
public:
    virtual void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) = 0;
    virtual void MouseCallback(GLFWwindow* window, double xpos, double ypos) = 0;
};

initialCallbacks* gamePointer = NULL;


// static callbacks
static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    gamePointer->KeyboardCallback( window, key, scancode, action, mode );
}

static void MouseCB(GLFWwindow* window, double xpos, double ypos)
{
    gamePointer->MouseCallback(window, xpos, ypos);
}


class GAME : public initialCallbacks
{
public:
    GAME() : s_(0), rotate_(0), aimed_(false)

    {
        gamePointer = this;
        std::cout<<"GLFWTraining GAME class "<<std::endl;
    }
    virtual ~GAME()
    {

        delete shader_;
        delete lightShader_;
        delete cubemapShader_;
        delete skyboxShader_;
        delete fontShader_;

        delete camera_;
        delete skybox_;
        delete windowPtr_;
    }

    bool initGAME()
    {
        // GLFW initialize
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        // CreateWindow
        windowPtr_ = glfwCreateWindow(window_width, window_height, "GLFWTraining", NULL, NULL);
        if (windowPtr_ == NULL)
        {
            std::cout<<"Failed to create GLFW window"<<std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(windowPtr_);

        // GLEW initialize
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            std::cout << "Failed to initialize GLEW" << std::endl;
            return -1;
        }

        initSceneContext();
        initializeCallbacks();

        // OpenGL options
        glEnable(GL_DEPTH_TEST);


        return true;
    }



    bool Run()
    {

        loadGame();
        loadActors();



        while (!glfwWindowShouldClose(windowPtr_))
        {


            //////////////////////   timer ////////////////////////

            deltaTime = 0.0f;
            while (deltaTime < (1.0f/fps))
            {
                currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
            }
                lastFrame = currentFrame;

            ///////////////////////////////////////////////////////

            glfwPollEvents();
            do_movement();

            /////////////////////   color buffer   ////////////////

            glClearColor(0.0, 0.15f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ///////////////////////////////////////////////////////

            gameRules();


            target_ = nullptr;
            for (int i = 0; i < enemies_.size(); i++)
            {
                aimed(enemies_[i]);
            }

            updateRockets();


            //////////////////    camera movement    ///////////////
            glm::mat4 projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, horizon);


            enemy_ = enemies_[0]; // uss Carrier
            enemy_->modelPos = glm::vec3(-1000.0f, 200.0f, -2500);
            enemyShader_->Use();

            GLuint modelLoc  = glGetUniformLocation(enemyShader_->shaderProgram_, "model");
            GLuint viewLoc  = glGetUniformLocation(enemyShader_->shaderProgram_, "view");
            GLuint projLoc  = glGetUniformLocation(enemyShader_->shaderProgram_, "projection");
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, enemy_->modelPos);
            model = glm::scale(model, glm::vec3(1000.0f));
            glm::mat4 view = camera_->GetViewMatrix();
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
            enemy_->Draw(*(enemyShader_)); // wrong shader

            /****************************************************************/

            enemy_ = enemies_[1]; // F22- raptor
            enemy_->modelPos = glm::vec3(3000.0f, 500.0f, -2500);
            enemyShader_->Use();
            modelLoc  = glGetUniformLocation(enemyShader_->shaderProgram_, "model");
            viewLoc  = glGetUniformLocation(enemyShader_->shaderProgram_, "view");
            projLoc  = glGetUniformLocation(enemyShader_->shaderProgram_, "projection");
            model = glm::mat4(1.0f);

            enemy_->modelPos.x += cos(glfwGetTime()*0.1)*400;
            enemy_->modelPos.y += sin(glfwGetTime()*0.1)*100;
            enemy_->modelPos.z += sin(glfwGetTime()*0.1)*400;
            model = glm::translate(model, enemy_->modelPos);
            model = glm::rotate(model, (GLfloat)-3.14159/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.0f));
            view = camera_->GetViewMatrix();

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
            enemy_->Draw(*(enemyShader_)); // wrong shader

            /********************************************************************/

            // Fighter
            fighter_->modelPos = camera_->getCameraPos();

            shader_->Use();
            modelLoc  = glGetUniformLocation(shader_->shaderProgram_, "model");
            viewLoc  = glGetUniformLocation(shader_->shaderProgram_, "view");
            projLoc  = glGetUniformLocation(shader_->shaderProgram_, "projection");
            model = glm::mat4(1.0f);

            model = glm::translate(model, +  glm::vec3(0.0f, -5.0f, -25.0f ));
//            model =  glm::scale(model, glm::vec3(1.0f));
            model = glm::rotate(model, (GLfloat)-3.14159/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::inverse(camera_->GetViewMatrix()) * model;



            view = camera_->GetViewMatrix();
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
            fighter_->Draw(*shader_);

            /**************************************************************************/
            std::cout<<missile_<<" jj" <<std::endl;
            renderProjectiles();
            std::cout<<missile_<<" jj" <<std::endl;
            ///////////////////////////////////////////////////////////////////////////////////////
            // skybox
            glDepthFunc(GL_LEQUAL);
            skyboxShader_->Use();


            viewLoc  = glGetUniformLocation(skyboxShader_->shaderProgram_, "view");
            projLoc  = glGetUniformLocation(skyboxShader_->shaderProgram_, "projection");
            view = glm::mat4(glm::mat3(camera_->GetViewMatrix()));
            projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, 10000.0f);
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

            glBindVertexArray(skybox_->skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_->skyboxTexID);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);

            /*****************************************************************************/
            terrain_->terrainShader_->Use();
            modelLoc  = glGetUniformLocation(terrain_->terrainShader_->shaderProgram_, "model");
            viewLoc  = glGetUniformLocation(terrain_->terrainShader_->shaderProgram_, "view");
            projLoc  = glGetUniformLocation(terrain_->terrainShader_->shaderProgram_, "projection");

            model = glm::mat4(1.0f);
            view = camera_->GetViewMatrix();
            projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, horizon);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

            terrain_->drawTerrain();

            /*****************************************************************************/

            // Fonts
            projection = glm::ortho(0.0f, static_cast<GLfloat>(window_width), 0.0f, static_cast<GLfloat>(window_height));
            fontShader_->Use();
            glUniformMatrix4fv(glGetUniformLocation(fontShader_->shaderProgram_, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            font_->RenderText(*fontShader_, std::to_string(currentFrame), 25.0f, 25.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(1/deltaTime), 2.0f, 2.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(camera_->getSpeed()), 50.0f, 50.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(fly_), 10.0f, 60.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));

            // camera position
            font_->RenderText(*fontShader_, std::to_string(camera_->getCameraPos().x), 5.0f, 500.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(camera_->getCameraPos().y), 130.0f, 500.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(camera_->getCameraPos().z), 240.0f, 500.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            // camera Front
            font_->RenderText(*fontShader_, std::to_string(camera_->getCameraFront().x), 5.0f, 480.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(camera_->getCameraFront().y), 130.0f, 480.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(camera_->getCameraFront().z), 240.0f, 480.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            // enemy pos
            font_->RenderText(*fontShader_, std::to_string(actors_[0]->modelPos.x), 5.0f, 460.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(actors_[0]->modelPos.y), 130.0f, 460.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
            font_->RenderText(*fontShader_, std::to_string(actors_[0]->modelPos.z), 240.0f, 460.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));

            /////////////////////    rendering    //////////////////



             ////////////////////////////////////////////////////////
            glfwSwapBuffers(windowPtr_);
        }
glBindVertexArray(0);
        glfwTerminate();
        return true;
    }

    void renderProjectiles()
    {
        std::cout<<missile_<<std::endl;
        glm::mat4 projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, horizon);

        for (int i = 0; i < rockets_.size(); i++)
        {

            missile_ = rockets_[i]; // Missile 0
            if (missile_->dead_)
                continue;


            shader_->Use();
            GLuint modelLoc  = glGetUniformLocation(shader_->shaderProgram_, "model");
            GLuint viewLoc  = glGetUniformLocation(shader_->shaderProgram_, "view");
            GLuint projLoc  = glGetUniformLocation(shader_->shaderProgram_, "projection");

            glm::mat4 model = glm::mat4(1.0f);
            if (missile_->isFired())
            {
                glm::mat4 tran1  = glm::translate(model, missile_->modelPos);
                glm::mat4 tran2  = glm::translate(glm::mat4(1.0f),  missile_->wingSlotOffset);
                glm::mat4 align = glm::lookAt(glm::vec3(0.0f), camera_->getCameraFront(), camera_->getCameraUp());
                glm::mat4 rot   = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f));

                align = glm::inverse(align);

                std::cout<<glm::determinant(align)<<std::endl;
                tran2 = align * tran2;
                model = tran1 * tran2 * rot * scale;

            }
            else
            {
                missile_->modelPos = fighter_->modelPos ;
                glm::mat4 tran1  = glm::translate(model, fighter_->modelPos);
                glm::mat4 tran2  = glm::translate(glm::mat4(1.0f),  missile_->wingSlotOffset);
                glm::mat4 align = glm::lookAt(glm::vec3(0.0f), camera_->getCameraFront(), camera_->getCameraUp());
                glm::mat4 rot   = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f));

                align = glm::inverse(align);
                tran2 = align * tran2;
                model = tran1 * tran2 * rot * scale;

            }

            glm::mat4 view = camera_->GetViewMatrix();
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
            missile_->Draw(*shader_);

        }
    }

    void updateRockets()
    {
        if (rockets_[0]->isFired() && !rockets_[0]->dead_)
            rockets_[0]->move();
        if (rockets_[1]->isFired() && !rockets_[1]->dead_)
            rockets_[1]->move();

        std::cout<<" plauit"<<std::endl;
    }

    void handleCrash()
    {


        glfwWindowShouldClose(windowPtr_);

        glfwTerminate();

    }

    void loadGame()
    {
        camera_ = new GT_Camera();
        std::cout<<"brkovi"<<std::endl;
        shader_ = new GT_Shader(modelShader, VmodelShader, FmodelShader);
        std::cout<<"brkovi"<<std::endl;
        enemyShader_ = new GT_Shader(shaderTag::enemyShader, vsEnemyShader, fsEnemyShader, gsEnemyShader);
        std::cout<<"brkovi"<<std::endl;

        skybox_ = new GT_Skybox();
        skyboxShader_ = new GT_Shader(skyboxShader, vsSkyboxShader, fsSkyboxShader);
        font_ = new GT_Alphabet();
        fontShader_ = new GT_Shader(fontShader, vsFontShader, fsFontShader);
        terrain_ = new GT_Terrain();
    }

    void loadActors()
    {
        enemies_.push_back(new GT_Enemy("../Content/CV - Essex class/essex_scb-125_generic.obj", enemyShader_));
        enemies_.push_back(new GT_Enemy("../Content/FA-22_Raptor/FA-22_Raptor.obj", enemyShader_));

        fighter_ = new GT_Model("../Content/FA-18_RC/FA-18_RC.obj");
        fighter_->modelPos = glm::vec3(0.0f, 100.0f, -100.0f);
        fighter_->modelFront = camera_->getCameraFront();
        fighter_->modelUp = camera_->getCameraUp();

        rockets_.push_back(new GT_Rocket("../Content/AVMT300/AVMT300.3ds"));
        rockets_[0]->wingSlotOffset = glm::vec3(-3.0f, -3.0f, 0.0f);
        rockets_.push_back(new GT_Rocket("../Content/AVMT300/AVMT300.3ds"));
        rockets_[1]->wingSlotOffset = glm::vec3( 8.0f, -3.0f, 0.0f);

        actors_.push_back(fighter_);

        actors_.push_back(rockets_[0]);
        actors_.push_back(rockets_[1]);

        actors_.push_back(enemies_[0]);
        actors_.push_back(enemies_[1]);
    }

    glm::mat4 alignModel(GT_Model& model)
    {
        glm::mat4 temp(1.0f);
        glm::mat4 initRot = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        glm::vec3 right;

        right = glm::cross(model.modelFront, model.modelUp);
        glm::normalize(right);

        temp[0][2] = -model.modelFront.x;
        temp[1][2] = -model.modelFront.y;
        temp[2][2] = -model.modelFront.z;

        temp[0][1] = model.modelUp.x;
        temp[1][1] = model.modelUp.y;
        temp[2][1] = model.modelUp.z;

        temp[0][0] = right.x;
        temp[1][0] = right.y;
        temp[2][0] = right.z;




        //temp = glm::lookAt(camera_->getCameraPos(), camera_->getCameraFront(), camera_->getCameraUp());

        return  glm::inverse(temp) ;

    }

    void gameRules()
    {

    }

    void aimed(GT_Enemy* target)
    {
        glm::vec3 enemyPos = target->modelPos;
        glm::vec3 fighterPos = actors_[0]->modelPos;
        glm::vec3 diagonal =  glm::normalize( enemyPos - fighterPos );

        // enemy pos
        font_->RenderText(*fontShader_, std::to_string(diagonal.x), 5.0f, 730.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
        font_->RenderText(*fontShader_, std::to_string(diagonal.y), 130.0f, 730.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));
        font_->RenderText(*fontShader_, std::to_string(diagonal.z), 240.0f, 730.0f, .50f, glm::vec3(0.5, 0.8f, 0.2f));


        GLfloat tempDOT = glm::dot(camera_->getCameraFront(), diagonal );
        if (tempDOT > 0.997 )
        {
            font_->RenderText(*fontShader_, "AIMED!!!", 700.0f, 100.0f, 1.0f, glm::vec3(1.0, 0.0f, .4f));
            if (sin(glfwGetTime()*6) >0)
                if (rockets_[0]->isFired() && rockets_[1]->isFired())
                    font_->RenderText(*fontShader_, "OUT OF MISSILES!!!", 630.0f, 65.0f, 0.8f, glm::vec3(1.0, 0.0f, .4f));
                else
                    font_->RenderText(*fontShader_, "FIRE!!!", 710.0f, 65.0f, 0.8f, glm::vec3(1.0, 0.0f, .4f));
            target_ = target;

        }
        else
        {
//            std::cout<<    glm::dot(camera_->getCameraFront(), diagonal )<<std::endl;

        }


    }

private:
//class private function members
    void initSceneContext()
    {
        glViewport(0, 0, window_width, window_height);
    }

    void do_movement()
    {
#ifdef oldcode
        // Camera controls
        if (keys[GLFW_KEY_W])
            camera_->keyboardHandler(FORWARD, deltaTime);
        if (keys[GLFW_KEY_S])
            camera_->keyboardHandler(BACKWARD, deltaTime);
        if (keys[GLFW_KEY_A])
        {
            camera_->keyboardHandler(LEFT, deltaTime);
            rotate_ += 0.005f;
        }
        if (keys[GLFW_KEY_D])
        {
            camera_->keyboardHandler(RIGHT, deltaTime);
            rotate_ -= 0.005f;
        }
#else
    if (fly_)
        camera_->keyboardHandler(FORWARD, deltaTime);
    if (keys[GLFW_KEY_W])
        camera_->keyboardHandler(PITCH_D, deltaTime);
    if (keys[GLFW_KEY_S])
        camera_->keyboardHandler(PITCH_U, deltaTime);
    if (keys[GLFW_KEY_A])
        camera_->keyboardHandler(ROLL_L, deltaTime);
    if (keys[GLFW_KEY_D])
        camera_->keyboardHandler(ROLL_R, deltaTime);
    if (keys[GLFW_KEY_Q])
        camera_->keyboardHandler(YAW_L, deltaTime);
    if (keys[GLFW_KEY_E])
        camera_->keyboardHandler(YAW_R, deltaTime);
    if (keys[GLFW_KEY_LEFT_CONTROL])
        camera_->keyboardHandler(ACCELERATE, deltaTime);
    if (keys[GLFW_KEY_LEFT_SHIFT])
        camera_->keyboardHandler(DECELERATE, deltaTime);

#endif
    }

    void initializeCallbacks()
    {
        glfwSetKeyCallback(windowPtr_, KeyboardCB);
        glfwSetCursorPosCallback(windowPtr_, MouseCB);
    }

    void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        {
            fly_ = !fly_;
        }

        if (key == GLFW_KEY_T )
        {
            toggle_ = !toggle_;
        }

        if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
        {
            std::cout<<(target_ == nullptr)<<std::endl;
            if (target_ != nullptr)
            {
                if (!rockets_[0]->isFired())
                {
                    rockets_[0]->modelFront = camera_->getCameraFront();
                    rockets_[0]->modelUp = camera_->getCameraUp();
                    rockets_[0]->Fire(target_);
                }
                else if (!rockets_[1]->isFired())
                {
                    rockets_[1]->modelFront = camera_->getCameraFront();
                    rockets_[1]->modelUp = camera_->getCameraUp();
                    rockets_[1]->Fire(target_);
                }
            }
        }

        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;
     }

    void MouseCallback(GLFWwindow* window, double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        GLfloat xoffset = xpos -lastX;
        GLfloat yoffset = ypos -lastY;

        lastX = xpos;
        lastY = ypos;

        //camera_->mouseHandler(xoffset, -yoffset);
    }

//class private members
private:

    GLFWwindow* windowPtr_;

    // shaders
    std::map<std::string, GT_Shader> shaders_;
    GT_Shader* shader_;
    GT_Shader* enemyShader_;
    GT_Shader* lightShader_;
    GT_Shader* cubemapShader_;
    GT_Shader* skyboxShader_;
    GT_Shader* fontShader_;


    // classes
    GT_Camera* camera_;
    GT_Skybox* skybox_;
    GT_Alphabet* font_;

    // Models
    std::vector<GT_Model*> actors_;
    std::vector<GT_Enemy*> enemies_;
    std::vector<GT_Rocket*> rockets_;
    GT_Model* fighter_;
    GT_Rocket* missile_;
    GT_Enemy* enemy_;
    GT_Terrain* terrain_;

    // uniforms
    GLfloat s_;
    GLboolean fly_;
    GLboolean toggle_;
    GLboolean aimed_;
    GT_Enemy* target_ ;
    GLfloat rotate_;

    GLfloat blinker_ ;

};


int main(int argc, char** argv)
{
    GAME* gamePtr = new GAME();

    if ( !gamePtr->initGAME() )
    {
        std::cout<<" GameInit Failed! "<<std::endl;
        return -1;
    }

    if ( !gamePtr->Run() )
    {
        std::cout<<" GameInit Failed! "<<std::endl;
        return -1;
    }


    std::cout<<"GameOver"<<std::endl;
    return 0;
}
