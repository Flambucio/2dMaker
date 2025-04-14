#pragma once
#include "../common/Core.h"
#include "../ECS/Entity.h"
#include "../Graphics/Texture.h"
namespace D2Maker 
{


    static class Window
    {
        static GLFWwindow* window;
    public:
        static void Initialize()
        {
            if (!glfwInit())
            {
                //throw error
                ERROR("failed to init glfw");
                return;
            }


            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            window = glfwCreateWindow(1000, 700, "game", NULL, NULL);
            if (!window)
            {
                ERROR("failed to create window");

                glfwTerminate();
                return;
            }
            glfwMakeContextCurrent(window);
            GLenum err = glewInit();
            if (err!=GLEW_OK)
            {
                ERROR("failed to init glew");
                ERROR((const char*)glewGetErrorString(err));
                glfwTerminate();
                return;
            }

        }
        



        static void RunWindow()
        {
            //GRAPHICS
            float positions[] = {
                    -0.5f,-0.5f,0.0f,0.0f,
                     0.5f,-0.5f, 1.0f,0.0f,
                     0.5f, 0.5f, 1.0f,1.0f,
                    -0.5f, 0.5f, 0.0f,1.0f
            };

            unsigned int indices[] = {
                0,1,2,
                2,3,0
            };
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            

            GLuint vao;
            VertexArray va;
            VertexBuffer vb(positions, 4 * 4 * sizeof(float));
            VertexBufferLayout layout;
            layout.Push<float>(2);
            layout.Push <float>(2);
            va.AddBuffer(vb, layout);
           

            IndexBuffer ib(indices, 6);

            Shaders shaderprogram;
            shaderprogram.Bind();

            Renderer renderer;
            Texture texture("Engine/Resources/TestAssets/image.png",0);
            texture.Bind();
            shaderprogram.SetUniform1i("u_Texture", 0);
            //ECS
            EntityManager em;
            Entity entity1 = em.createEntity();
            Entity entity2 = em.createEntity();
            em.addComponent<Transform>(entity1, 1, 1, 100, 100, 0);//trace
            em.addComponent<Collider>(entity2);//warning
            em.addComponent<Collider>(entity1);//trace
            float r = 0.0f;
            float increment = 0.05f; 
            while (!glfwWindowShouldClose(window))
            {
                renderer.Clear();
                renderer.Draw(va, ib, shaderprogram);

                glfwSwapBuffers(window);
                glfwPollEvents();
            }
        }


        static void Destruct()
        {
            glfwTerminate();
            
        }

    };

    
}
