#include "Dropdown.h"
#include "Menu.h"
#include "WindowGUI.h"
//since most of the functions are small, all the small ones are all inline
//the larger ones are all defined here
namespace D2Maker
{
	namespace GUI
	{
		namespace GUIAPI
		{
			void Dropdown::Update()
			{
				if (currentValueIndex == -1 || values.empty()) return;

				ImGui::Text("%s", label.c_str());

				if (ImGui::BeginCombo(combo_id.c_str(), values[currentValueIndex].c_str()))
				{
					for (int i = 0; i < values.size(); ++i)
					{
						bool selected = (currentValueIndex == i);
						if (ImGui::Selectable(values[i].c_str(), selected))
							currentValueIndex = i;

						if (selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}

			void Dropdown::SetCurrVal(std::string newVal)
			{
				for (int i = 0;i < values.size();i++)
				{
					if (values[i] == newVal)
					{
						currentValueIndex = i;
					}
				}
			}

			void Menu::Update()
			{
				for (int i = 0;i < values.size();i++)
				{
					bool isSelected = (i == currentValueIndex);

					if (!isSelected)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
					}

					if (ImGui::Button(values[i].c_str(), ImVec2(bWidth, bHeight)))
					{
						currentValueIndex = i;
					}

					if (!isSelected)
					{
						ImGui::PopStyleColor(3);
					}
				}
			}

			void GUIWindow::StartFrame()
			{
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
			}

			void GUIWindow::CreateWindow(int x, int y, int width, int height, std::string title)
			{
				ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
				ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
				ImGui::Begin(title.c_str(), nullptr);
			}

			void GUIWindow::CreateWindow(int x, int y, int width, int height, std::string title, std::vector<ImGuiCond_> flagsIn)
			{
				ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize;
				for (int i = 0;i < flagsIn.size();++i) { flags |= flagsIn[i]; }
				ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
				ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
				ImGui::Begin(title.c_str(), nullptr, flags);
			}

			void GUIWindow::CreateFixedWindow(int x, int y, int width, int height, std::string title)
			{
				ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
				ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
				ImGui::Begin(title.c_str(), nullptr, flags);
			}

			void GUIWindow::EndFrame(GLFWwindow* window)
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
		}
	}
}