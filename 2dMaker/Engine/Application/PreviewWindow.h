#pragma once
#include "Scene.h"
namespace D2Maker
{
    class PreviewTextureWindow
    {
    private:
        GLFWwindow* window=nullptr;
        VertexBufferLayout layout;
        Shaders* shaderProgram=nullptr;
        Renderer renderer;
        std::string bufferName="";
        const unsigned int indices[6] = { 0,1,2,2,3,0 };
        const float vertices[16]=
        {
            -1.0f, -1.0f,     0.0f, 1.0f,
             1.0f, -1.0f,     1.0f, 1.0f,
             1.0f,  1.0f,     1.0f, 0.0f,
            -1.0f,  1.0f,     0.0f, 0.0f
        };

    public:
        bool windowOpen=false;
        PreviewTextureWindow();


        void Update();
        void Initialize(std::string name);
        void Destruct();
    };
}
