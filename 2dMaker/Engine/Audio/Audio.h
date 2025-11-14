#pragma once
#include "../common/Core.h"
namespace D2Maker
{


	class Audio
	{
	private:
		std::string filepath;
		unsigned int  bufferID;
		unsigned int sourceID;
	public:

		Audio(const std::string& filepath);
		bool LoadOGG(const std::string filepath, std::vector<short>& output, ALenum& format, ALsizei& freq);

		inline ~Audio()
		{
			alDeleteSources(1, &sourceID);
			alDeleteBuffers(1, &bufferID);
		}

		inline void Play() const
		{
			alSourcePlay(sourceID);
		}

		inline void Stop() const
		{
			alSourceStop(sourceID);
		}

		inline void Pause() const
		{
			alSourcePause(sourceID);
		}

		inline std::string GetFilePath() const
		{
			return filepath;
		}

		inline unsigned int& GetSource()
		{
			return sourceID;
		}

	};

}