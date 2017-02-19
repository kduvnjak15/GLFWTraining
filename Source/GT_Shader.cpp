#include <GT_Shader.h>
#include <iostream>
#include <string.h>
#include <fstream>



GT_Shader::GT_Shader(const char* vsFileName, const char* fsFileName)
{

    if (!ReadShader(vsFileName, vertexShaderSource))
        std::cout<< " Vertex Shader File invalid! "<<std::endl;
    if (!ReadShader(fsFileName, fragmentShaderSource))
        std::cout<< " Fragment Shader File invalid! "<<std::endl;


    ///////////////////     vertex shader    //////////////////////////////////
    vertexShader_ = glCreateShader(GL_VERTEX_SHADER);

    const GLchar* p[1];
    p[0] = vertexShaderSource.c_str();
    GLint Lengths[1];
    Lengths[0]= strlen( p[0] );


    glShaderSource(vertexShader_, 1, p, Lengths);
    glCompileShader(vertexShader_);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader_, 512, NULL, infoLog);
        std::cout<< "ERROR:: VERTEX_SHADER_COMPILE_FAILED! "<<infoLog<<std::endl;
    }


    /////////////////////    fragment shader    ////////////////////////////////
    fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);

    p[0] = fragmentShaderSource.c_str();
    Lengths[0]= strlen( p[0] );

    glShaderSource(fragmentShader_, 1, p, Lengths);
    glCompileShader(fragmentShader_);

    glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader_, 512, NULL, infoLog);
        std::cout<< "ERROR:: FRAGMENT_SHADER_COMPILE_FAILED! "<<infoLog<<std::endl;
    }


    ///////////////////////     shader program    //////////////////////////////
    shaderProgram_ = glCreateProgram();
    glAttachShader(shaderProgram_, vertexShader_);
    glAttachShader(shaderProgram_, fragmentShader_);
    glLinkProgram(shaderProgram_);

    glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram_, 512, NULL, infoLog);
        std::cout<< "ERROR:: FRAGMENT_SHADER_COMPILE_FAILED! "<<infoLog<<std::endl;
    }

    glUseProgram(shaderProgram_);

    glDeleteShader(vertexShader_);
    glDeleteShader(fragmentShader_);

}


bool GT_Shader::ReadShader(const char* shaderFileName, std::string& outFile )
{
    bool ret = false;
    std::ifstream shaderFile(shaderFileName);

    if (shaderFile.is_open())
    {
        std::string line;
        while (getline(shaderFile, line)) {
            outFile.append(line);
            outFile.append("\n");
        }

        shaderFile.close();

        ret = true;
    }
    else {
        ret = false;
    }

    return ret;
}
