#pragma once
#include "Texture.h"
namespace D2Maker
{


	class TextureLoader
	{
	private:
		static std::unordered_map<std::string, std::unique_ptr<Texture>> textures;

	public:
		inline static bool Exists(const std::string& name)
		{
			return textures.find(name) != textures.end();
		}

		inline static void LoadTexture(const std::string& name, const std::string& path)
		{
			if (Exists(name))
			{
				WARN("Texture name already present");
				return;
			}
			textures[name] = std::make_unique<Texture>(path);
		}

		inline static void RemoveTexture(const std::string& name)
		{
			if (!Exists(name))
			{
				WARN("Texture already deleted/Texture not added");
				return;
			}
			textures.erase(name);
		}

		inline static void RenameTexture(const std::string& oldName, const std::string& newName)
		{
			if (!Exists(oldName))
			{
				WARN("Texture nonexistent");
				return;
			}
			std::string path = textures[oldName]->GetPath();
			RemoveTexture(oldName);
			LoadTexture(newName, path);
		}

		inline static Texture* GetTexture(const std::string& name)
		{
			auto it = textures.find(name);
			if (it != textures.end())
				return it->second.get();
			return nullptr;
		}

		inline static void BindTexture(const std::string& name)
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

		inline static std::unordered_map<std::string, std::unique_ptr<Texture>>& GetMap()
		{
			return textures;
		}
	};

}
