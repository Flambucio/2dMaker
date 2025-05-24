#pragma once
#include "../../Engine/EngineExport.h"
namespace D2Maker
{
	namespace GUI
	{
		namespace GUIAPI
		{
			class Button
			{
			private:

				int width;
				int height;
				std::string id;
			public:
				Button(int width,int height,std::string id)
				{
					this->width = width;
					this->height = height;
					this->id = id;
				}

				bool Active()
				{
					return	ImGui::Button(id.c_str(), ImVec2(width, height));
				}
			};
		}
	}
}
