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

		Audio(const std::string& filepath) : filepath(filepath),bufferID(0),sourceID(0)
		{
			std::vector<short> audioData;
			ALenum format;
			ALsizei freq;
			if (!LoadOGG(filepath, audioData, format, freq))
			{
				WARN("failed to load audio:" + filepath);
				return;
			}
			alGenBuffers(1, &bufferID);
			alBufferData(bufferID, format, audioData.data(), static_cast<ALsizei>(audioData.size() * sizeof(short)), freq);
			alGenSources(1, &sourceID);
			alSourcei(sourceID, AL_BUFFER, bufferID);
			alSourcef(sourceID, AL_GAIN, 1.0f);
			

		}

		bool LoadOGG(const std::string filepath, std::vector<short>& output, ALenum& format, ALsizei& freq)
		{
			std::ifstream file(filepath, std::ios::binary | std::ios::ate);
			if (!file.is_open())
			{
				WARN("failed to open audio file: " + filepath);
				return false;
			}
			std::streamsize size = file.tellg();
			file.seekg(0, std::ios::beg);
			std::vector<unsigned char> buffer(size);
			if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
			{
				WARN("failed to read audio file: " + filepath);
				return false;
			}

			file.close();
			int error;
			stb_vorbis* vorbis = stb_vorbis_open_memory(buffer.data(), static_cast<int>(size), &error, nullptr);
			if (!vorbis)
			{
				WARN("stb_vorbis_open_memory failed: " + filepath);
				return false;
			}
			stb_vorbis_info info=stb_vorbis_get_info(vorbis);

			int channels = info.channels;
			freq = info.sample_rate;
			int totalSamples = stb_vorbis_stream_length_in_samples(vorbis) * channels;
			output.resize(totalSamples);

			int samplesDecoded = stb_vorbis_get_samples_short_interleaved(vorbis, channels, 
				output.data(),totalSamples);

			stb_vorbis_close(vorbis);

			if (samplesDecoded <= 0)
			{
				WARN("stb_vorbis_get_samples_short_interleaved failed: " + filepath);
				return false;
			}

			if (channels == 1)
			{
				format = AL_FORMAT_MONO16;
			}
			else if (channels == 2)
			{
				format = AL_FORMAT_STEREO16;
			}
			else
			{
				WARN("unsupported number of channels in file:" + filepath);
				return false;
			}

			return true;

		}

		~Audio()
		{
			alDeleteSources(1, &sourceID);
			alDeleteBuffers(1, &bufferID);
		}

		void Play() const
		{
			alSourcePlay(sourceID);
		}

		void Stop() const
		{
			alSourceStop(sourceID);
		}

		void Pause() const
		{
			alSourcePause(sourceID);
		}

		std::string GetFilePath() const
		{
			return filepath;
		}

		unsigned int& GetSource()
		{
			return sourceID;
		}

	};

}