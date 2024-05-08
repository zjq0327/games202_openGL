#include "shader.h"
#include <fstream>
#include <sstream>

Shader::Shader()
{
    // 默认构造
}

Shader::Shader(const string &path, int _type)
{
    string code;
    ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try{
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        code = shaderStream.str();
    } catch (ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    
    const char *shaderCode = code.c_str();
    unsigned int _id;
    if (_type == 0)
        _id = glCreateShader(GL_VERTEX_SHADER);
    else
        _id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_id, 1, &shaderCode, NULL);
    glCompileShader(_id);
    id = _id;

    // 打印编译错误
    int success;
    char infoLog[512];
    glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

}

void Shader::setAttributeName(const vector<string> &_attributeName)
{
    attributeName = _attributeName;
}

void Shader::setUniformName(const vector<string> &_unifromName)
{
    uniformName = _unifromName;
}

void Shader::setTextureName(const vector<string> &_textureName)
{
    textureName = _textureName;
}

unsigned int Shader::getId()
{
    return id;
}

vector<string> Shader::getAttributeName()
{
    return attributeName;
}

vector<string> Shader::getUniformName()
{
    return uniformName;
}

vector<string> Shader::getTextureName()
{
    return textureName;
}

