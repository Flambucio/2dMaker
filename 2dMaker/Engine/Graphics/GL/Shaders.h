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
        Shaders(const std::string& filepath=SHADER_PATH) : filepath(filepath), rendererID(0)
        {
            rendererID = CreateShaderProgram();
        }
        ~Shaders()
        {
            glDeleteProgram(rendererID);
        }
    private:
        std::string filepath;
        unsigned int rendererID;
        std::unordered_map<std::string, unsigned int> uniformLocationCache;



        int CompileShader(GLuint type, const std::string& source)
        {
            GLuint id = glCreateShader(type);
            const char* src = source.c_str();
            glShaderSource(id, 1, &src, nullptr);
            glCompileShader(id);

            int result;
            glGetShaderiv(id, GL_COMPILE_STATUS, &result);
            if (result == GL_FALSE)
            {
                int length;
                glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
                char* message = (char*)alloca(length * sizeof(char));
                glGetShaderInfoLog(id, length, &length, message);
                ERROR(message);
                glDeleteShader(id);
                return 0;
            }

            return id;
        }

        static std::string ReadShaderFile(const std::string& path)
        {
            std::ifstream file(path);
            if (!file.is_open())
            {
                ERROR("SHADER FILE NOT FOUND %s", path.c_str());
                return "";
            }
            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }
        static ShaderSources ParseShader(const std::string& path)
        {
            enum class ShaderType
            {
                NONE = -1, VERTEX = 0, FRAGMENT = 1
            };
            ShaderType type = ShaderType::NONE;

            std::ifstream stream(path);
            std::string line;
            std::stringstream ss[2];
            while (getline(stream, line))
            {
                if (line.find("#shader") != std::string::npos)
                {
                    if (line.find("vertex") != std::string::npos)
                    {
                        type = ShaderType::VERTEX;
                    }
                    else if (line.find("fragment") != std::string::npos)
                    {
                        type = ShaderType::FRAGMENT;
                    }
                }
                else
                {
                    ss[(int)type] << line << '\n';
                }
            }

            return { ss[0].str(),ss[1].str() };


        }

    public:
        GLuint CreateShaderProgram()
        {
            ShaderSources source = ParseShader("Engine/Resources/Shaders/Shader.shader");


            GLuint vsID = CompileShader(GL_VERTEX_SHADER, source.Vertex);
            GLuint fsID = CompileShader(GL_FRAGMENT_SHADER, source.Fragment);

            TRACE("VERTEX");
            TRACE(source.Vertex);
            TRACE("END VERTEX");
            TRACE("--------------------------------------------------------------\n");
            TRACE("FRAGMENT");
            TRACE(source.Fragment);
            TRACE("END FRAGMENT");
            TRACE("--------------------------------------------------------------\n");

            GLuint program = glCreateProgram();
            glAttachShader(program, vsID);
            glAttachShader(program, fsID);
            glLinkProgram(program);
            glValidateProgram(program);

            glDeleteShader(vsID);
            glDeleteShader(fsID);

            return program;
        }


        void Bind()
        {
            glUseProgram(rendererID);
        }

        void Unbind()
        {
            glUseProgram(0);
        }

        void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
        {

            glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
        }

        unsigned int GetUniformLocation(const std::string& name)
        {
            if (uniformLocationCache.find(name) != uniformLocationCache.end())
            {
                return uniformLocationCache[name];
            }
            unsigned int location = glGetUniformLocation(rendererID, name.c_str());
            if (location == -1)
            {
                WARN("UNIFORM:");
                WARN(name.c_str());
                WARN("NOT FOUND")

            }

            uniformLocationCache[name] = location;
            
            return location;

        }

    };
 
}