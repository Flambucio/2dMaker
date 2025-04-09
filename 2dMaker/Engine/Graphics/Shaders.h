#pragma once

#include "../common/Core.h"

namespace D2Maker 
{


    class Shaders
    {
        GLuint compileShader(GLenum type, const char* source)
        {
            GLuint shader = glCreateShader(type);
            glShaderSource(shader, 1, &source, nullptr);
            glCompileShader(shader);

            GLint success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                GLint logLength;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
                std::vector<char> errorLog(logLength);
                glGetShaderInfoLog(shader, logLength, nullptr, errorLog.data());
                ERROR("ERROR::SHADER::COMPILATION_FAILED\n" + std::string(errorLog.data()));
            }

            return shader;
        }

        std::string ReadShaderFile(const std::string& path)
        {
            std::ifstream file(path);
            if (!file.is_open())
            {
                ERROR("SHADER FILE NOT FOUNT %s", path.c_str());
                return"";
            }
            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();


        }

    public:
        GLuint CreateShaderProgram()
        {
            const std::string vertexShaderPath = ".. / .. / Resources / Shaders / Vertex.glsl";
            const std::string fragmentShaderPath = "../../Resources/Shaders/Fragment.glsl";

            const std::string vertexShaderSource = ReadShaderFile(vertexShaderPath);
            const std::string fragmentShaderSource = ReadShaderFile(fragmentShaderPath);

            GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderPath.c_str());
            GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderPath.c_str());

            GLuint shaderProgram = CreateShaderProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            return shaderProgram;
        }
    };


}