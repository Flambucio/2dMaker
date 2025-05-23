#pragma once
#include "Windows/Create.h"
//independed window used to select projects

namespace D2Maker
{
    namespace GUI
    {
        enum class ProjectSelectedFlag
        {
            NONE = -1,
            SELECTED,
        };

        class BootWindow
        {
        public:
            GLFWwindow* window = nullptr;
            ProjectSelectedFlag prjFlag = ProjectSelectedFlag::NONE;
            BootWindow()
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
                glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

                window = glfwCreateWindow(DEFAULT_WIN_W, DEFAULT_WIN_H, "2DMaker", NULL, NULL);
                if (!window)
                {
                    ERROR("failed to create window");

                    glfwTerminate();
                    return;
                }
                glfwMakeContextCurrent(window);
                glfwSetWindowAspectRatio(window, 16, 9);
                GLenum err = glewInit();
                if (err != GLEW_OK)
                {
                    ERROR("failed to init glew");
                    ERROR((const char*)glewGetErrorString(err));
                    glfwTerminate();
                    return;
                }

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);
                glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
                glfwSetKeyCallback(window, KeyCallback);
                ImGui::CreateContext();

                // 2. Configuri lo stile (opzionale)
                ImGui::StyleColorsDark();

                // 3. Inizializzi il backend (GLFW, SDL, ecc.)
                ImGui_ImplGlfw_InitForOpenGL(window, true);
                ImGui_ImplOpenGL3_Init("#version 330");
            }

            void RunBoot()
            {
                std::vector<std::string> prjs;
                FileSys::GetProjects();
                for (std::string prj : FileSys::projectNames)
                {
                    prjs.push_back(prj);
                }
                APIBootWindowCreate ABWC{ prjs };
                while (!BootWindowShouldClose())
                {
                    GUIAPI::GUIWindow::StartFrame();
                    ABWC.Update();

                    GUIAPI::GUIWindow::EndFrame(window);




                }

                glfwTerminate();
            }

            
            bool BootWindowShouldClose()
            {
                if (glfwWindowShouldClose(window) || !(FileSys::currentProject == ""))
                {
                    if (FileSys::currentProject != "") { prjFlag = ProjectSelectedFlag::SELECTED; }
                    return true;
                }
                return false;
            }

            static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                if (action == GLFW_PRESS)
                {
                    EventManager::PushEvent(static_cast<Keys>(key));
                }
                else if (action == GLFW_RELEASE)
                {
                    EventManager::ReleaseKey(static_cast<Keys>(key));
                }
            }

            static inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
            {
                glViewport(0, 0, width, height);
            }
		};
	}



}