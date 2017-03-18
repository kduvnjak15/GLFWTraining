#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string.h>

class GT_Shader
{
public:
    GT_Shader(const char* vsFileName = nullptr, const char* fsFileName = nullptr, const char* gsFileName = nullptr);

    void Use();

    GLuint shaderProgram_;


private:

    void validateShaderProgram();

    GLuint addShader(const char* shaderPath, GLenum shaderType);

private:

    GLuint vertexShader_;
    GLuint fragmentShader_;
    GLuint geometryShader_;

};

