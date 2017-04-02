#include <GT_Shader.h>
#include <iostream>
#include <sstream>
#include <fstream>


GLuint GT_Shader::addShader(const char* shaderPath, GLenum shaderType)
{
    /////////////////   preprocessing file   //////////////////////////

    std::ifstream shaderFile;

    shaderFile.open(shaderPath);
    if (!shaderFile.is_open())
    {
        std::cout<<" SHADER PATH "<<shaderPath<<" NOT FOUND!"<<std::endl;
        std::cout<<shaderTag_<<std::endl;
        return 99;
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();

    shaderFile.close();

    std::string shaderSource = shaderStream.str();
    const GLchar* shaderCode = shaderSource.c_str();

    ////////////////    compiling shader     ///////////////////////////

    GLuint shader_ = glCreateShader(shaderType);

    glShaderSource(shader_, 1, &shaderCode, NULL );
    glCompileShader(shader_);

    return shader_;
}

void GT_Shader::validateShaderProgram()
{
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader_, 512, NULL, infoLog);
        std::cout<< "ERROR:: VERTEX_SHADER_COMPILE_FAILED! "<<infoLog<<std::endl;
    }

    glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader_, 512, NULL, infoLog);
        std::cout<< "ERROR:: FRAGMENT_SHADER_COMPILE_FAILED! "<<infoLog<<std::endl;
    }

    glGetShaderiv(geometryShader_, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(geometryShader_, 512, NULL, infoLog);
        std::cout<< "ERROR:: GEOMETRY_SHADER_COMPILE_FAILED! "<<infoLog<<std::endl;
    }

    glGetProgramiv(this->shaderProgram_, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->shaderProgram_, 512, NULL, infoLog);
        std::cout<< "ERROR:: SHADER_LINKING_FAILED! "<<infoLog<<std::endl;
    }

}

GT_Shader::GT_Shader(shaderTag shadesTypeTag, const char* vsFileName, const char* fsFileName, const char* gsFileName)
    :
      vertexShader_(0),
      fragmentShader_(0),
      geometryShader_(0),
      shaderTag_(shadesTypeTag)

{

    //////////////////////      shader files      ////////////////////////
    vertexShader_ = addShader(vsFileName, GL_VERTEX_SHADER);
    fragmentShader_ = addShader(fsFileName, GL_FRAGMENT_SHADER);

    ///////////////////////     shader program    //////////////////////////////
    this->shaderProgram_ =  glCreateProgram();
    glAttachShader(this->shaderProgram_, vertexShader_);
    glAttachShader(this->shaderProgram_, fragmentShader_);
    glLinkProgram(this->shaderProgram_);

    validateShaderProgram();

    glDeleteShader(vertexShader_);
    glDeleteShader(fragmentShader_);
    glDeleteShader(geometryShader_);

}

void GT_Shader::Use()
{
    glUseProgram(this->shaderProgram_);
}

shaderTag GT_Shader::getShaderTag()
{
    return shaderTag_;
}
