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
				static void StartFrame();
				static void CreateWindow(int x, int y, int width, int height, std::string title);
				static void CreateWindow(int x, int y, int width, int height, std::string title, std::vector<ImGuiCond_> flagsIn);
				static void CreateFixedWindow(int x, int y, int width, int height, std::string title);
				static void EndFrame(GLFWwindow* window);
				inline static void EndWindow(){ImGui::End();}
			};
		}
	}
}
