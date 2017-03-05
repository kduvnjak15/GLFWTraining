#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "GT_Shader.h"
#include "GT_Texture.h"
#include "GT_Camera.h"

//#include "GT_Skybox.h"

const GLuint window_width = 800;
const GLuint window_height = 600;

#define VShader "../Shaders/shader.vs"
#define FShader "../Shaders/shader.fs"


bool keys[1024];
bool firstMouse = true;
GLfloat lastX = window_width/2, lastY = window_height/2;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

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
        glGenVertexArrays(1, &VAO_);
        glBindVertexArray(VAO_);

        glEnableVertexAttribArray(0);
        glGenBuffers(1, &VBO_);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (const GLvoid*)0);
        glEnableVertexAttribArray(0);

        glEnableVertexAttribArray(1);
        glGenBuffers(1, &TBO_);
        glBindBuffer(GL_ARRAY_BUFFER, TBO_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (const GLvoid*)0);
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &EBO_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



glBindVertexArray(0);

        camera_ = new GT_Camera();

        shader_ = new GT_Shader(VShader, FShader);
        shader_->Use();

        texture1 = new GT_Texture("../Content/bricks.jpg");
        texture2 = new GT_Texture("../Content/sun.jpg");

        texture1->Bind(GL_TEXTURE0);
        texture2->Bind(GL_TEXTURE1);

        GLint uniformInt = glGetUniformLocation(shader_->shaderProgram_, "texSampler");
        glUniform1i(uniformInt, 0);



glBindVertexArray(VAO_);

        while (!glfwWindowShouldClose(windowPtr_))
        {

            //////////////////////   timer ////////////////////////
            GLfloat currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            ///////////////////////////////////////////////////////

            glfwPollEvents();
            do_movement();

            //////////////////    camera movement    ///////////////
            glm::mat4 view = camera_->GetViewMatrix();
            glm::mat4 projection = glm::perspective(ZOOM, (window_width*1.0f)/window_height, 0.1f, 1000.0f);
            // Get the uniform locations
            GLint modelLoc = glGetUniformLocation(shader_->shaderProgram_, "model");
            GLint viewLoc = glGetUniformLocation(shader_->shaderProgram_, "view");
            GLint projLoc = glGetUniformLocation(shader_->shaderProgram_, "projection");
            // Pass the matrices to the shader

            glm::mat4 model = glm::mat4(1.0f);

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

           // std::cout<<camera_->getCameraPos().x<<", "<<camera_->getCameraPos().y<<", "<<camera_->getCameraPos().z<<"; "<<std::endl;

            /////////////////////    rendering    //////////////////

            glClearColor(0.0, 0.15f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUniform1i(uniformInt, s_);

            glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT, 0);

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

        camera_->mouseHandler(xoffset, -yoffset);
    }

    GLfloat vertices[12] = {
    0.5f, 0.5f, 0.0f, // Top Right
    0.5f, -0.5f, 0.0f, // Bottom Right
    -0.5f, -0.5f, 0.0f, // Bottom Left
    -0.5f, 0.5f, 0.0f // Top Left
    };


    GLuint indices[6] = { 2, 1, 0, 2, 3, 0};

    GLfloat texCoord[8] = {1.0f, 1.0f,    1.0f, 0.0f,    0.0f, 0.0f,    0.0f, 1.0f};


//class private members
private:
    GLuint VBO_;
    GLuint TBO_;
    GLuint VAO_;
    GLuint EBO_;
    GLFWwindow* windowPtr_;

    GT_Shader* shader_;
    GT_Texture* texture1;
    GT_Texture* texture2;
    GT_Camera* camera_;

    GLint s_;

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

