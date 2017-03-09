#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "GT_Shader.h"
#include "GT_Texture.h"
#include "GT_Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLuint window_width = 800;
const GLuint window_height = 600;

#define VShader "../Shaders/shader.vs"
#define FShader "../Shaders/shader.fs"
#define vLampShader  "../Shaders/vLampShader.vs"
#define fLampShader  "../Shaders/fLampShader.fs"

bool keys[1024];
bool firstMouse = true;
GLfloat lastX = window_width/2, lastY = window_height/2;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::vec3 lightPos(5.0f, 5.0f, 5.0f);


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
    GAME() : s_(0)

    {
        gamePointer = this;
        std::cout<<"GLFWTraining GAME class "<<std::endl;
    }
    ~GAME() = default;

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

        return true;
    }

    bool Run()
    {
        /////////////////////            VAO bussiness       //////////////////////////

        glGenVertexArrays(1, &VAO_);

        glGenBuffers(1, &VBO_);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(VAO_);
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (const GLvoid*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        ///////////////////////////////////////////////////////////////////////////////
        GLuint lightVAO;
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);
        // we only need to bind to the VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        // Set the vertex attributes (only position data for the lamp)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const GLvoid*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);


        camera_ = new GT_Camera();

        shader_ = new GT_Shader(VShader, FShader);
        shader_->Use();
        lampShader_ = new GT_Shader(vLampShader, fLampShader);
        lampShader_->Use();


        texture1 = new GT_Texture("../Content/bricks.jpg");
        texture2 = new GT_Texture("../Content/sun.jpg");

        texture1->Bind(GL_TEXTURE0);
        texture2->Bind(GL_TEXTURE1);

        GLint uniformInt = glGetUniformLocation(shader_->shaderProgram_, "uniformS_");
        glUniform1f(uniformInt, s_);



        while (!glfwWindowShouldClose(windowPtr_))
        {

            //////////////////////   timer ////////////////////////
            GLfloat currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            ///////////////////////////////////////////////////////

            glfwPollEvents();
            do_movement();

            /////////////////////   color buffer   ////////////////

            glClearColor(0.0, 0.15f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ///////////////////////////////////////////////////////
            shader_->Use();
            GLint objectColorLoc = glGetUniformLocation(shader_->shaderProgram_, "objectColor");
            GLint lightColorLoc  = glGetUniformLocation(shader_->shaderProgram_, "lightColor");
            glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
            glUniform3f(lightColorLoc,  0.0f, 0.0f, 1.0f);
            glUniform1f(glGetUniformLocation(shader_->shaderProgram_, "uniformS_"), s_);

            //////////////////    camera movement    ///////////////
            glm::mat4 view = camera_->GetViewMatrix();
            glm::mat4 projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, 1000.0f);
            // Get the uniform locations
            GLint modelLoc = glGetUniformLocation(shader_->shaderProgram_, "model");
            GLint viewLoc = glGetUniformLocation(shader_->shaderProgram_, "view");
            GLint projLoc = glGetUniformLocation(shader_->shaderProgram_, "projection");
            // Pass the matrices to the shader
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));



            // Draw cube container
            glBindVertexArray(VAO_);
            glm::mat4 model;
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);

            // also draw light source
            lampShader_->Use();
            modelLoc = glGetUniformLocation(lampShader_->shaderProgram_, "model");
            viewLoc  = glGetUniformLocation(lampShader_->shaderProgram_, "view");
            projLoc  = glGetUniformLocation(lampShader_->shaderProgram_, "projection");

            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
            model = glm::mat4();
            model = glm::translate(model, lightPos);
            model = glm::scale(model, glm::vec3(0.1f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);

            /////////////////////    rendering    //////////////////

            glUniform1f(uniformInt, s_);

             ////////////////////////////////////////////////////////
            glfwSwapBuffers(windowPtr_);
        }
glBindVertexArray(0);
        glfwTerminate();
        return true;
    }

private:
//class private function members
    void initSceneContext()
    {
        glViewport(0, 0, window_width, window_height);
    }

    void do_movement()
    {
        // Camera controls
        if (keys[GLFW_KEY_W])
            camera_->keyboardHandler(FORWARD, deltaTime);
        if (keys[GLFW_KEY_S])
            camera_->keyboardHandler(BACKWARD, deltaTime);
        if (keys[GLFW_KEY_A])
            camera_->keyboardHandler(LEFT, deltaTime);
        if (keys[GLFW_KEY_D])
            camera_->keyboardHandler(RIGHT, deltaTime);
    }

    void initializeCallbacks()
    {
        glfwSetKeyCallback(windowPtr_, KeyboardCB);
        glfwSetCursorPosCallback(windowPtr_, MouseCB);
    }

    void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q  && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        {
            if (s_ == 0)
                s_ = 1;
            else if (s_ == 1)
                s_ = 0;
        }

        if (key == GLFW_KEY_U )
        {
            s_ += 0.05f;
            if (s_ > 1)
                s_ = 1.0f;
        }

        if (key == GLFW_KEY_I )
        {
            s_ -= 0.05f;
            if (s_ < 0)
                s_ = 0.0f;
        }

        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;
        std::cout<<s_<<std::endl;
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

        camera_->mouseHandler(xoffset, -yoffset);
    }

    GLfloat vertices[108] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };


//class private members
private:
    GLuint VBO_;
    GLuint TBO_;
    GLuint VAO_;
    GLuint EBO_;
    GLFWwindow* windowPtr_;

    GT_Shader* shader_;
    GT_Shader* lampShader_;


    GT_Texture* texture1;
    GT_Texture* texture2;
    GT_Camera* camera_;


    GLfloat s_;

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

