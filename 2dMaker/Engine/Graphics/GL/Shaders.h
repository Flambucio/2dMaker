#pragma once

#include "../../common/Core.h"

namespace D2Maker
{

    struct ShaderSources
    {
        std::string Vertex;
        std::string Fragment;

    };


    class Shaders
    {
    public:
        inline Shaders(const std::string& filepath=SHADER_PATH) : filepath(filepath), rendererID(0)
        {
            rendererID = CreateShaderProgram();
        }
        inline ~Shaders()
        {
            glDeleteProgram(rendererID);
        }
    private:
        std::string filepath;
        unsigned int rendererID;
        std::unordered_map<std::string, int> uniformLocationCache;



        int CompileShader(GLuint type, const std::string& source);
        static std::string ReadShaderFile(const std::string& path);
        static ShaderSources ParseShader(const std::string& path);
    public:
        GLuint CreateShaderProgram();


        inline void Bind() const
        {
            glUseProgram(rendererID);
        }

        inline void Unbind() const
        {
            glUseProgram(0);
        }

        inline void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
        {

            glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
        }

        inline void SetUniform1f(const std::string& name, float value)
        {
            glUniform1f(GetUniformLocation(name), value);
        }

        inline void SetUniform1i(const std::string& name, int value)
        {
            glUniform1i(GetUniformLocation(name), value);
        }

        int GetUniformLocation(const std::string& name);

    };
 
}