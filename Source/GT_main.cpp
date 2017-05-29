#include <iostream>
#include <cmath>
#include <vector>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GT_Camera.h"
#include "GT_Model.h"
#include "GT_Aircraft.h"
#include "GT_MenuScene.h"
#include "GT_GameplayScene.h"
#include "GT_PauseScene.h"
#include "GT_Locator.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SFML/Audio.hpp"

bool keys[1024];
bool firstMouse = true;

GLfloat runTime = 0.0f;
GLfloat fps = 60.0f;

GLfloat currentFrame = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat lastX = window_width/2, lastY = window_height/2;

class initialCallbacks
{
public:
    virtual void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) = 0;
    virtual void MouseCallback(GLFWwindow* window, double xpos, double ypos) = 0;
    virtual void leaveGame() = 0;
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

void quitGame()
{
    gamePointer->leaveGame();
}

static void glerror_output(GLenum source, GLenum type, GLuint eid, GLenum severity, GLsizei length, const char* message, void* ge)
{
    const char* typestr = (
        (type == GL_DEBUG_TYPE_ERROR) ? "ERROR" : (
        (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR) ? "DEPRECATED" : (
        (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR) ? "UNDEFINED" : (
        (type == GL_DEBUG_TYPE_PORTABILITY) ? "PORTABILITY" : (
        (type == GL_DEBUG_TYPE_PERFORMANCE) ? "PERFORMANCE" : (
        (type == GL_DEBUG_TYPE_OTHER) ? "OTHER" : (
        (type == GL_DEBUG_TYPE_MARKER) ? "MARKER" : (
        "UNKNOWN"
    ))))))));
    printf("GL %s from %04x, TYPE=%04x, ID=%04x, SEVERITY=%d, msg=%s", typestr, source, type, eid, severity, message);
    if(message[length-1]!='\n') printf("\n");

//    // nvidia performance warning when using glClear
//    if(type==GL_DEBUG_TYPE_PERFORMANCE && eid==0x20092 && std::cout<<message <<" Shader is going to be recompiled because the shader key based on GL state mismatches"<<std::endl;
//    return;

//    // funky nvidia error when using nsight with quadro fx 580
//    if(type==GL_DEBUG_TYPE_ERROR && eid==0x0500 &&  std::cout<< "GL_IMAGE_FORMAT_COMPATIBILITY_TYPE operation is invalid because a required extension (ARB_shader_image_load_store) is not supported"<<std::endl;)
//    return;

//    if(type != GL_DEBUG_TYPE_OTHER)
//    {	eh_logf(ge, "GLDEBUG %s from %04x, TYPE=%04x, ID=%04x, SEVERITY=%d, msg=%s\n", typestr, source, type, eid, severity, message);
//        DEBUG_BREAKPOINT();
//    }

}

class GAME : public initialCallbacks
{
public:
    GAME() : s_(0), rotate_(0), aimed_(false), crashTime_(0.0), fly_(true), currLevel_(0), levelPassed_(-1.0f)

    {
        gamePointer = this;
        std::cout<<"GLFWTraining GAME class "<<std::endl;
    }
    virtual ~GAME()
    {
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
        windowPtr_ = glfwCreateWindow(window_width, window_height, "SkyKnights", NULL, NULL);
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
        glEnable(GL_BLEND);
        glBlendFunc(GL_DST_ALPHA, GL_ONE);
        return true;
    }

    bool Run()
    {
        glfwSwapInterval(1);
        loadGame();

        std::cout << "Starting GAME LOOP "<< std::endl;
        while (!glfwWindowShouldClose(windowPtr_))
        {

            ///////////////////////////////////////////////////////

            glfwPollEvents();

            //////////////////    game rules          /////////////

            //////////////////    camera movement    ///////////////

            /////////////////////    rendering    //////////////////

            curScene_->renderScene();

            /////////////////    gameplay control  //////////////////

            if (curScene_->nextScene_ != nonType )
            {
                if (curScene_->nextScene_ == gameplay)
                {
                    std::cout << "Next scene gameplay"<< std::endl;
                    curScene_->nextScene_ = nonType;
                    curScene_ = scenes_[1];
                }
                else if (curScene_->nextScene_ == menuScene)
                {
                    std::cout << "Next scene Menuscene"<< std::endl;
                    curScene_->nextScene_ = nonType;
                    curScene_ = scenes_[0];
                }
                else if (curScene_->nextScene_ == pauseScene)
                {
                    std::cout << "Next scene pausescene" << std::endl;
                    curScene_->nextScene_ = nonType;
                    curScene_ = scenes_[2];
                }
                else if (curScene_->nextScene_ == exitGame)
                    return true;
                else
                    curScene_ = scenes_[0];
            }


            ////////////////////////////////////////////////////////

            if (crashTime_  == 0.0f)
            {
                glfwSwapBuffers(windowPtr_);
            }
            else if (glfwGetTime() - crashTime_ > 2.0f)
            {
                handleCrash();
            }


        }

        glfwTerminate();
        return true;
    }

    void leaveGame()
    {
        glfwWindowShouldClose(windowPtr_);
    }


//    void renderProjectiles()
//    {
//        glm::mat4 projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, horizon);

//        for (int i = 0; i < rockets_.size(); i++)
//        {

//            missile_ = rockets_[i]; // Missile 0
//            if (missile_->dead_)
//                continue;

//            glm::mat4 model = glm::mat4(1.0f);
//            if (missile_->isFired())
//            {
//                glm::mat4 tran1  = glm::translate(model, missile_->modelPos);
//                glm::mat4 tran2  = glm::translate(glm::mat4(1.0f),  missile_->wingSlotOffset);
//                glm::mat4 align = glm::lookAt(glm::vec3(0.0f), camera_->getCameraFront(), camera_->getCameraUp());
//                glm::mat4 rot   = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
//                glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f));

//                align = glm::inverse(align);

//                tran2 = align * tran2;
//                model = tran1 * tran2 * rot * scale;

//            }
//            else
//            {
//                missile_->modelPos = fighter_->modelPos ;
//                glm::mat4 tran1  = glm::translate(model, fighter_->modelPos);
//                glm::mat4 tran2  = glm::translate(glm::mat4(1.0f),  missile_->wingSlotOffset);
//                glm::mat4 align = glm::lookAt(glm::vec3(0.0f), camera_->getCameraFront(), camera_->getCameraUp());
//                glm::mat4 rot   = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
//                glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f));

//                align = glm::inverse(align);
//                tran2 = align * tran2;
//                model = tran1 * tran2 * rot * scale;

//            }

//            glm::mat4 view = camera_->GetViewMatrix();

//                particle_->primitiveShader_->Use();

//                const GLfloat interpolateBias = 10;
//                std::vector<contrail> container_ = missile_->getParticles();
//                for (int i = 1; i < container_.size(); i++)
//                {
//                    if (container_[i].birthday_ - glfwGetTime() < -5 )
//                        continue;

//                    for (int j = 0; j< interpolateBias; j++)
//                    {
//                        model = glm::mat4(1.0f);
//                        glm::vec3 tempParticle = (container_[i].position_ - container_[i-1].position_)/interpolateBias;

//                        glm::mat4 tran1  = glm::translate(model, container_[i].position_+tempParticle*((GLfloat)j));
//                        glm::mat4 tran2  = glm::translate(glm::mat4(1.0f),  missile_->wingSlotOffset);
//                        glm::mat4 align = glm::lookAt(glm::vec3(0.0f), camera_->getCameraFront(), camera_->getCameraUp());
//                        glm::mat4 rot   = glm::rotate(glm::mat4(1.0f), (GLfloat)-3.14159/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
//                        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, 1.0f));

//                        align = glm::inverse(align);
//                        tran2 = align * tran2;
//                        model = tran1 * tran2 * rot * scale;


//                        particle_->draw();

//                    }
//                }

//            }

//        }


    void handleCrash()
    {
        glfwWindowShouldClose(windowPtr_);

        glfwTerminate();
    }

    void loadGame()
    {
        GT_Locator_ = new GT_Locator();

        std::cout << "Loading scenes "<< std::endl;

        scenes_.push_back(new GT_MenuScene());
        scenes_.push_back(new GT_GameplayScene());
        scenes_.push_back(new GT_PauseScene());
        curScene_ = scenes_[0];

    }




private:
//class private function members
    void initSceneContext()
    {
        glViewport(0, 0, window_width, window_height);
    }

    void initializeCallbacks()
    {
        glfwSetKeyCallback(windowPtr_, KeyboardCB);
        glfwSetCursorPosCallback(windowPtr_, MouseCB);
    }

    void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {

        if (key == GLFW_KEY_ESCAPE && key == GLFW_KEY_RIGHT_SHIFT && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (key == GLFW_KEY_T )
        {
            toggle_ = !toggle_;
        }

        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;

        if (curScene_)
            curScene_->sceneKeyboardHandler(keys, key, scancode, action, mode);
        else
        {
            std::cout << "NO CURSCENE_; CATASTRPHIC CRASH" << std::endl;
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

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

    std::vector<GT_Scene*> scenes_;
    GT_Scene* curScene_;

    GT_Locator* GT_Locator_;


    std::vector<GT_Aircraft*> aircafts_;
    std::vector<GT_Model*> actors_;
    GT_Model* fighter_;
    GT_Ocean* ocean_;

    // uniforms
    GLfloat s_;
    GLfloat crashTime_;
    GLboolean fly_;
    GLboolean toggle_;

    GLboolean aimed_;
    GLfloat rotate_;

    GLfloat blinker_ ;

    // level setting
    GLfloat levelPassed_;
    GLuint currLevel_;
    GLuint numOfMissiles_;
    GLuint numOfBogies_;

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
