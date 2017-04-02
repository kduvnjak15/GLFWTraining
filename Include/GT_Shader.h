#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

class GT_Shader
{
public:
    GT_Shader(const char* shaderName , const char* vsFileName = nullptr, const char* fsFileName = nullptr, const char* gsFileName = nullptr);

    void Use();

    GLuint shaderProgram_;

    const char* getShaderTag();


private:

    void validateShaderProgram();

    GLuint addShader(const char* shaderPath, GLenum shaderType);

private:

    const char* shaderTag_;

    GLuint vertexShader_;
    GLuint fragmentShader_;
    GLuint geometryShader_;

};

