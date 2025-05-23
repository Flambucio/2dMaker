#pragma once
#include "../../Engine/EngineExport.h"
#include "../Utils/Coords.h"
//independed window used to select projects

namespace D2Maker
{
	namespace GUI
	{
        enum class ProjectSelectedFlag
        {
            NONE=-1,
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

                window = glfwCreateWindow(DEFAULT_WIN_W, DEFAULT_WIN_H, CAPTION, NULL, NULL);
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
                while (!BootWindowShouldClose())
                {
                    ImGui_ImplOpenGL3_NewFrame();
                    ImGui_ImplGlfw_NewFrame();
                    ImGui::NewFrame();

                    // Costruisci la finestra GUI
                    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Always);
                    ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_Always);

                    ImGuiWindowFlags flags =
                        ImGuiWindowFlags_NoResize
                        | ImGuiWindowFlags_NoMove
                        | ImGuiWindowFlags_NoCollapse
                        | ImGuiWindowFlags_NoTitleBar;

                    ImGui::Begin("BootWindow", nullptr, flags);

                    ImGui::Text("Controlli del motore:");

                    static int dropdown1 = 0;
                    const char* opzioni1[] = { "Basso", "Medio", "Alto" };
                    ImGui::Text("Dettagli grafica:");
                    if (ImGui::BeginCombo("Qualità", opzioni1[dropdown1])) {
                        for (int i = 0; i < IM_ARRAYSIZE(opzioni1); i++) {
                            bool selected = (dropdown1 == i);
                            if (ImGui::Selectable(opzioni1[i], selected))
                                dropdown1 = i;
                            if (selected) ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    static int dropdown2 = 0;
                    const char* opzioni2[] = { "Facile", "Normale", "Difficile" };
                    ImGui::Text("Difficoltà:");
                    if (ImGui::BeginCombo("Livello", opzioni2[dropdown2])) {
                        for (int i = 0; i < IM_ARRAYSIZE(opzioni2); i++) {
                            bool selected = (dropdown2 == i);
                            if (ImGui::Selectable(opzioni2[i], selected))
                                dropdown2 = i;
                            if (selected) ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::End(); // Fine finestra

                    // Rendering ImGui
                    ImGui::Render();
                    int display_w, display_h;
                    glfwGetFramebufferSize(window, &display_w, &display_h);
                    glViewport(0, 0, display_w, display_h);
                    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                    // Swap buffers e gestisci eventi
                    glfwSwapBuffers(window);
                    glfwPollEvents();
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