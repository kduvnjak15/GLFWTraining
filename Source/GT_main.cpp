#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GT_Shader.h"
#include "GT_Texture.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define VShader "../Shaders/shader.vs"
#define FShader "../Shaders/shader.fs"

class initialCallbacks
{
public:
    virtual void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) = 0;
};

initialCallbacks* gamePointer = NULL;


// static callbacks
static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    gamePointer->KeyboardCallback( window, key, scancode, action, mode );
}





class GAME : public initialCallbacks
{
public:
    GAME() : s(0)

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
        windowPtr_ = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GLFWTraining", NULL, NULL);
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


        shader_ = new GT_Shader(VShader, FShader);
        shader_->Use();


        texture_ = new GT_Texture("../Content/bricks.jpg");
        texture_->Bind();

        GLint uniformInt = glGetUniformLocation(shader_->shaderProgram_, "texSampler");
        glUniform1i(uniformInt, 0);



glBindVertexArray(VAO_);


        while (!glfwWindowShouldClose(windowPtr_))
        {

            glfwPollEvents();
            /////////////////////    rendering    //////////////////

            glClearColor(0.0, 0.15f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    void initializeCallbacks()
    {
        glfwSetKeyCallback(windowPtr_, KeyboardCB);
    }

    void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q  && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        {
            s+=0.02f;
        }
    }

    GLfloat vertices[12] = {
    0.5f, 0.5f, 0.0f, // Top Right
    0.5f, -0.5f, 0.0f, // Bottom Right
    -0.5f, -0.5f, 0.0f, // Bottom Left
    -0.5f, 0.5f, 0.0f // Top Left
    };


    GLuint indices[6] = { 2, 1, 0, 2, 3, 0};

    GLfloat texCoord[12] = {0.0f, 0.0f,    1.0f, 0.0f,    1.0f, 1.0f,    0.0f, 0.0f,     1.0f, 1.0f,     1.0f, 1.0f};


//class private members
private:
    GLuint VBO_;
    GLuint TBO_;
    GLuint VAO_;
    GLuint EBO_;
    GLFWwindow* windowPtr_;

    GT_Shader* shader_;
    GT_Texture* texture_;

    GLfloat s;

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

