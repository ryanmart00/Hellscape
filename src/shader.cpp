#include "shader.hpp"

#include <glad/gl.h>
#include "constants.hpp"
#include <fstream>
#include <sstream>
#include <iostream>


void replaceAll(std::string& data, std::string search, std::string replace)
{
    size_t pos = data.find(search);
    while(pos != std::string::npos)
    {
        data.replace(pos, search.size(), replace);
        pos = data.find(search, pos + replace.size());
    }
}

Shader::Shader(const int numDirLights, const int numPointLights,
        const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
    // 1. get the vertex/fragment shader source code
    std::string vStr, fStr, gStr;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    // ensure ifstreams can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    //load files
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        // pipe buffer content into streams
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // close files
        vShaderFile.close();
        fShaderFile.close();

        // store the results
        vStr = vShaderStream.str();
        fStr = fShaderStream.str();

        // again for geometry
        if (geometryPath != nullptr) 
        {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            gStr = gShaderStream.str();
        }
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR: Shader file(s) were not successfully read" << std::endl;
        std::exit(1);
    }
    replaceAll(vStr, "@NUM_DIR_LIGHTS@", std::to_string(numDirLights));
    replaceAll(vStr, "@NUM_POINT_LIGHTS@", std::to_string(numPointLights));
    replaceAll(vStr, "@NUM_MAX_BONES@", std::to_string(MAX_BONES));
    replaceAll(fStr, "@NUM_DIR_LIGHTS@", std::to_string(numDirLights));
    replaceAll(fStr, "@NUM_POINT_LIGHTS@", std::to_string(numPointLights));
    replaceAll(fStr, "@NUM_MAX_BONES@", std::to_string(MAX_BONES));
    replaceAll(gStr, "@NUM_DIR_LIGHTS@", std::to_string(numDirLights));
    replaceAll(gStr, "@NUM_POINT_LIGHTS@", std::to_string(numPointLights));
    replaceAll(gStr, "@NUM_MAX_BONES@", std::to_string(MAX_BONES));
    const char* vertexCode = vStr.c_str();
    const char* fragmentCode = fStr.c_str();
    const char* geometryCode = gStr.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment, geometry;
    
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glCompileShader(vertex);

    // report errors
    checkCompileErrors(vertexPath, vertex, "VERTEX");

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, NULL);
    glCompileShader(fragment);

    // report errors
    checkCompileErrors(fragmentPath, fragment, "FRAGMENT");

    // geometry shader
    if(geometryPath != nullptr)
    {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &geometryCode, NULL);
        glCompileShader(geometry);

        // report errors
        checkCompileErrors(geometryPath, geometry, "GEOMETRY");
    }

    // 3. Link Shaders
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if(geometryPath != nullptr)
    {   
        glAttachShader(ID, geometry);
    }
    glLinkProgram(ID);

    // report errors
    checkCompileErrors(vertexPath, ID, "PROGRAM");
    
    // 4. delete Shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}    

void Shader::use()
{
    glUseProgram(ID);
}


void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(const char* name, unsigned int shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::" << name << type << "\n" << infoLog
                      << std::endl;
            std::exit(1);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
                      << std::endl;
            std::exit(1);
        }
    }
}
