#pragma once
#include "Texture.h"
namespace D2Maker
{


	static class TextureLoader
	{
	private:
		static std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
	public:

		static bool Exists(const std::string& name)
		{
			return textures.find(name) != textures.end();
		}

		

		static void LoadTexture(const std::string& name, const std::string& path,int orderInLayer)
		{
			if (Exists(name))
			{
				WARN("Texture name already present")
				return;
			}
			textures[name] = std::make_unique<Texture>(path, orderInLayer);
		}

		static void RemoveTexture(const std::string& name)
		{
			if (Exists(name))
			{
				textures.erase(name);
				return;
			}
			WARN("Texture already deleted/Texture not added");
		}

		static void ModifyOrderInLayer(const std::string& name,int newOrderInLayer)
		{
			if (Exists(name))
			{
				std::string path = textures[name]->GetPath();
				RemoveTexture(name);
				LoadTexture(name, path, newOrderInLayer);
				return;
			}
			WARN("Texture nonexistent");
		}

		static void RenameTexture(const std::string& name,const std::string& newName)
		{
			if (Exists(name))
			{
				int orderInLayer = textures[name]->GetOrderInLayer();
				std::string path = textures[name]->GetPath();
				RemoveTexture(name);
				LoadTexture(newName, path, orderInLayer);
			}
			WARN("Texture nonexistent");
		}


		


		
	};



}
