#pragma once
#include "Audio.h"

namespace D2Maker
{
	static class AudioLoader
	{
	private:
		static std::unordered_map<std::string, std::unique_ptr<Audio>> audios;
	public:
		static bool Exists(const std::string& name)
		{
			return audios.find(name) != audios.end();
		}
		static void LoadAudio(const std::string& name, const std::string& filepath)
		{	
			if (Exists(name))
			{
				WARN("audio name already present");
				return;
			}
			audios[name] = std::make_unique<Audio>(filepath);
		}
		static void RemoveAudio(const std::string& name)
		{
			if (!Exists(name))
			{
				WARN("audio does not exist");
				return;
			}
			audios.erase(name);
		}
		static Audio* GetAudio(const std::string& name)
		{
			auto it = audios.find(name);
			if (it != audios.end())
			{
				return it->second.get();
			}
			return nullptr;

		}
		static void PlayAudio(const std::string& name)
		{
			Audio* audio = GetAudio(name);
			if (audio)
			{
				audio->Play();
			}
			else
			{
				WARN("attempted to play nonexistent audio");
			}
		}
		static void StopAudio(const std::string&name)
		{
			Audio* audio = GetAudio(name);
			if (audio)
			{
				audio->Stop();
			}
			else
			{
				WARN("Attempted to stop nonexistent audio");
			}
		}

		static std::unordered_map<std::string, std::unique_ptr<Audio>>& GetMap()
		{
			return audios;
		}

		static unsigned int& GetSource(const std::string& name)
		{
			return audios[name]->GetSource();
		}
	};
}
