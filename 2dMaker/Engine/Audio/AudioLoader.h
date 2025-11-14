#pragma once
#include "Audio.h"

namespace D2Maker
{
	class AudioLoader
	{
	private:
		static std::unordered_map<std::string, std::unique_ptr<Audio>> audios;
	public:
		inline static bool Exists(const std::string& name)
		{
			return audios.find(name) != audios.end();
		}
		static void LoadAudio(const std::string& name, const std::string& filepath);
		static void RemoveAudio(const std::string& name);
		static Audio* GetAudio(const std::string& name);
		static void PlayAudio(const std::string& name);
		static void StopAudio(const std::string& name);
		inline static std::unordered_map<std::string, std::unique_ptr<Audio>>& GetMap()
		{
			return audios;
		}

		inline static unsigned int& GetSource(const std::string& name)
		{
			return audios[name]->GetSource();
		}
	};
}
