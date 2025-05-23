#pragma once
#include "../../Engine/EngineExport.h"
namespace D2Maker
{
	namespace GUI
	{
		namespace GUIAPI
		{
			class GUIWindow
			{
			public:
				static ImGuiWindowFlags flags;
				static void StartFrame()
				{
					ImGui_ImplOpenGL3_NewFrame();
					ImGui_ImplGlfw_NewFrame();
					ImGui::NewFrame();
				}
				static void CreateWindow(int x, int y, int width,int height,std::string title)
				{
					ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
					ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
					ImGui::Begin(title.c_str(),nullptr);


				}
				static void CreateFixedWindow(int x, int y, int width, int height, std::string title)
				{
					ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
					ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
					ImGui::Begin(title.c_str(), nullptr,flags);
				}
				static void EndWindow()
				{
					ImGui::End();
				}

				static void EndFrame(GLFWwindow* window)
				{
					ImGui::Render();
					int display_w, display_h;
					glfwGetFramebufferSize(window, &display_w, &display_h);
					glViewport(0, 0, display_w, display_h);
					glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
					glfwSwapBuffers(window);
					glfwPollEvents();
				}
			};
		}
	}
}
