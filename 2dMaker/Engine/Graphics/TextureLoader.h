#pragma once
#include "Texture.h"
namespace D2Maker
{


	class TextureLoader
	{
	private:
		static std::unordered_map<std::string, std::unique_ptr<Texture>> textures;

	public:
		static bool Exists(const std::string& name)
		{
			return textures.find(name) != textures.end();
		}

		static void LoadTexture(const std::string& name, const std::string& path, int orderInLayer)
		{
			if (Exists(name))
			{
				WARN("Texture name already present");
				return;
			}
			textures[name] = std::make_unique<Texture>(path, orderInLayer);
		}

		static void RemoveTexture(const std::string& name)
		{
			if (!Exists(name))
			{
				WARN("Texture already deleted/Texture not added");
				return;
			}
			textures.erase(name);
		}

		static void ModifyOrderInLayer(const std::string& name, int newOrderInLayer)
		{
			if (!Exists(name))
			{
				WARN("Texture nonexistent");
				return;
			}

			std::string path = textures[name]->GetPath();
			RemoveTexture(name);
			LoadTexture(name, path, newOrderInLayer);
		}

		static void RenameTexture(const std::string& oldName, const std::string& newName)
		{
			if (!Exists(oldName))
			{
				WARN("Texture nonexistent");
				return;
			}

			int orderInLayer = textures[oldName]->GetOrderInLayer();
			std::string path = textures[oldName]->GetPath();
			RemoveTexture(oldName);
			LoadTexture(newName, path, orderInLayer);
		}

		static Texture* GetTexture(const std::string& name)
		{
			auto it = textures.find(name);
			if (it != textures.end())
				return it->second.get();
			return nullptr;
		}

		static void BindTexture(const std::string& name)
		{
			Texture* tex = GetTexture(name);
			if (tex)
			{
				tex->Bind();
			}
			else
			{
				WARN("Attempted to bind nonexistent texture");
			}
		}

		static std::unordered_map<std::string, std::unique_ptr<Texture>>& GetMap()
		{
			return textures;
		}

		// Debug/Test method
		static void TestFunction()
		{
			LoadTexture("erbucio", "Engine/Resources/TestAssets/image.png", 0);
			BindTexture("erbucio");
		}
	};
		


		
	



}
