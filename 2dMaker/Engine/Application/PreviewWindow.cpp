#include "Window.h"


namespace D2Maker
{
    PreviewTextureWindow::PreviewTextureWindow()
    {
        layout.Push<float>(2);
        layout.Push<float>(2);
    }

    void PreviewTextureWindow::Destruct()
    {
        glfwDestroyWindow(window);
        windowOpen = false;
        delete shaderProgram;
        shaderProgram = nullptr;
        window = nullptr;
    }

    void PreviewTextureWindow::Initialize(std::string name)
    {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        window = Window::ConstructPreviewWindow();
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, Window::framebuffer_size_callback);
        
        shaderProgram = new Shaders();
        windowOpen = true;
        bufferName = name;
       
    }

    void PreviewTextureWindow::Update()
    {
        if (window && shaderProgram)
        {
            if (glfwWindowShouldClose(window))
            {
                Destruct();
                return;
            }
            glfwMakeContextCurrent(window);
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);
            renderer.Clear();
            shaderProgram->Bind();
            IndexBuffer ibo(indices, 6);
            VertexArray vao;
            vao.Bind();
            VertexBuffer vbo(vertices, 4 * 4 * sizeof(float));
            vao.AddBuffer(vbo, layout);
            if (!TextureLoader::Exists(bufferName)) return;
            TextureLoader::BindTexture(bufferName);
            renderer.Draw(vao, ibo, *shaderProgram);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
}





