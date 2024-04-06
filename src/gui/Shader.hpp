// Shaders in GLSL

#pragma once

#include <glad/glad.h>  // include glad to get all the required OpenGL headers

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>

namespace vino {

class Shader {
public:
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    // use/activate the shader
    void use() const;
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3Float(const std::string& name, float, float, float) const;
    void setVec4Float(
            const std::string& name, float, float, float, float) const;
    void setMat4FloatV(const std::string& name, const glm::mat4& mat) const;

    unsigned int get_id() { return id; }

private:
    // the program id
    unsigned int id{};
};

inline Shader::~Shader()
{
    glDeleteProgram(id);
}

inline Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // retrieve the vertex/fragment source code from filePath
    std::string   vertex_code;
    std::string   fragment_code;
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;

    // ensure ifstream objects can throw exceptions:
    v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open files
        v_shader_file.open(vertexPath);
        f_shader_file.open(fragmentPath);
        std::stringstream v_shader_stream;
        std::stringstream f_shader_stream;
        // read file's buffer contents into streams
        v_shader_stream << v_shader_file.rdbuf();
        f_shader_stream << f_shader_file.rdbuf();
        // close file handlers
        v_shader_file.close();
        f_shader_file.close();
        // convert stream into string
        vertex_code = v_shader_stream.str();
        fragment_code = f_shader_stream.str();
    } catch (std::ifstream::failure& e) {
        // std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* ptrVShaderCode = vertex_code.c_str();
    const char* ptrFShaderCode = fragment_code.c_str();

    // 2. compile shaders
    unsigned int vertex;
    unsigned int fragment;
    int          success;
    char         info_log[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &ptrVShaderCode, nullptr);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, info_log);
        // std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
        //           << info_log << std::endl;
    }

    // similar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &ptrFShaderCode, nullptr);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, info_log);
        // std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
        //           << info_log << std::endl;
    }

    // shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    // print linking errors if any
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, nullptr, info_log);
        // std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
        //           << info_log << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer
    // necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

inline void Shader::use() const
{
    glUseProgram(id);
}

inline void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(
            glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
}

inline void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

inline void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

inline void Shader::setVec3Float(
        const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

inline void Shader::setVec4Float(
        const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

inline void Shader::setMat4FloatV(
        const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,
            glm::value_ptr(mat));
}

}  // namespace vino