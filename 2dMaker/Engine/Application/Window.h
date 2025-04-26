#pragma once
#include "../ECS/SystemManager.h"
#include "../Events/EventManager.h"
namespace D2Maker 
{


    static class Window
    {
        static GLFWwindow* window;
    
    private:
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
        {
            glViewport(0, 0, width, height);
        }
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
        

        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (action == GLFW_PRESS)
            {
                EventManager::PushEvent(static_cast<Keys>(key));
            }
            else if(action==GLFW_RELEASE)
            {
                EventManager::ReleaseKey(static_cast<Keys>(key));
            }
        }

        static void RunWindow()
        {
            //GRAPHICS
            TextureLoader::LoadTexture("erbucio", "Engine/Resources/TestAssets/image.png", 0);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
            glfwSetKeyCallback(window, KeyCallback);
            SystemManager sm{ window };
            //ECS
            Renderer renderer;
          
            EntityManager em;
            Entity entity1 = em.createEntity();
            Entity entity2 = em.createEntity();
            Entity entiti3 = em.createEntity();
            Entity entity4 = em.createEntity();
            //1
            em.addComponent<Transform>(entity1, 200, 200, 600, 500, 0);//trace
            em.addComponent<Collider>(entity1);//trace
            em.addComponent<TextureComponent>(entity1,"erbucio",0);
            em.addComponent<Velocity>(entity1, 0, 0,0);
            //em.addComponent<RigidBody>(entity1, 10, 1.5, 10000);
            em.addComponent<Script>(entity1, "Projects/Script.txt");
            //2
            em.addComponent<Transform>(entity2, 0, 0, 1600, 900,0);
            em.addComponent<TextureComponent>(entity2, "erbucio", -1);
            //3
            em.addComponent<Transform>(entiti3, 1200, 200, 600, 500, 45);
            em.addComponent<Collider>(entiti3);
            em.addComponent<TextureComponent>(entiti3, "erbucio", 0);
            //4
            em.addComponent<Transform>(entity4, 0, 850, 1600, 100,0);
            em.addComponent<Collider>(entity4);
            em.addComponent<TextureComponent>(entity4, "erbucio", 0);

            
            TextureLoader::BindTexture("erbucio");
            

            float accumulator = 0;
            int countfps = 0;
            
            
            float r = 0.0f;
            float increment = 0.05f; 
            while (!glfwWindowShouldClose(window))
            {
                DeltaTime::Update();
                accumulator += DeltaTime::Get();
                if (DeltaTime::Get() == 0)
                {
                    TRACE("DT=NULL");
                }
                if (accumulator > 1)
                {
                    TRACE(countfps);
                    countfps = 0;
                    accumulator -= 1;
                }
                else
                {
                    countfps++;
                }
 
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                //renderer.Draw(va, ib, shaderprogram);
                sm.UpdateSystems(em);
                //system.TestRendering(entity1, em, window);
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
