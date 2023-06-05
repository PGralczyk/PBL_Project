#pragma once
#include <AL\al.h>
#include <AL\alext.h>
#include <sndfile.h>
#include <cstdlib>


class Music
{
private:
	const char* fileName;
	ALuint p_Source;
	static const int BUFFER_SAMPLES = 8192;
	static const int NUM_BUFFERS = 4;
	ALuint p_Buffers[NUM_BUFFERS];
	SNDFILE* p_SndFile;
	SF_INFO p_Sfinfo;
	short* p_Membuf;
	ALenum p_Format;
	bool loop = false;

	Music() = delete;


public:

	void Play()
	{
		ALsizei i;

		// clear any al errors
		alGetError();

		/* Rewind the source position and clear the buffer queue */
		alSourceRewind(p_Source);
		alSourcei(p_Source, AL_BUFFER, 0);

		/* Fill the buffer queue */
		for (i = 0; i < NUM_BUFFERS; i++)
		{
			/* Get some data to give it to the buffer */
			sf_count_t slen = sf_readf_short(p_SndFile, p_Membuf, BUFFER_SAMPLES);
			if (slen < 1) break;

			slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
			alBufferData(p_Buffers[i], p_Format, p_Membuf, (ALsizei)slen, p_Sfinfo.samplerate);
		}
		if (alGetError() != AL_NO_ERROR)
		{
			throw("Error buffering for playback");
		}

		/* Now queue and start playback! */
		alSourceQueueBuffers(p_Source, i, p_Buffers);
		alSourcePlay(p_Source);
		if (alGetError() != AL_NO_ERROR)
		{
			throw("Error starting playback");
		}

	}

	void Pause()
	{
		if (GetSourceState() == AL_PLAYING)
		{
			alSourcePause(p_Source);
		}
	}

	void Resume()
	{
		if (GetSourceState() == AL_PAUSED)
		{
			alSourcePlay(p_Source);
		}
	}

	void Stop()
	{
		if (GetSourceState() == AL_PLAYING)
		{
			alSourceStop(p_Source);
		}
	}

	void Replay()
	{
		//Stop();
		sf_close(p_SndFile);
		p_SndFile = sf_open(fileName, SFM_READ, &p_Sfinfo);
		Play();
	}

	void EnableLooping() 
	{
		loop = true;
	}

	void DisableLooping()
	{
		loop = false;
	}

	void SetVolume(float newVolume) 
	{
		if (0 <= newVolume && newVolume <= 1)
		{
			alSourcef(p_Source, AL_GAIN, newVolume);
		}
	}

	float GetVolume()
	{
		float v;
		alGetSourcef(p_Source, AL_GAIN, &v);
		return v;
	}

	void Mute()
	{
		SetVolume(0.0);
	}

	ALint GetSourceState()
	{
		ALint state;
		alGetSourcei(p_Source, AL_SOURCE_STATE, &state);

		return state;
	}

	void UpdateBufferStream()
	{
		//std::cout << "Updating!" << std::endl;
		ALint processed, state;

		// clear error 
		alGetError();
		/* Get relevant source info */
		alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
		alGetSourcei(p_Source, AL_BUFFERS_PROCESSED, &processed);
		if (alGetError() != AL_NO_ERROR)
		{
			//std::cout << "State error?" << std::endl;
			throw("error checking music source state");
		}

		/* Unqueue and handle each processed buffer */
		while (processed > 0)
		{
			//std::cout << "Processing!" << std::endl;
			ALuint bufid;
			sf_count_t slen;

			alSourceUnqueueBuffers(p_Source, 1, &bufid);
			processed--;

			/* Read the next chunk of data, refill the buffer, and queue it
			 * back on the source */
			slen = sf_readf_short(p_SndFile, p_Membuf, BUFFER_SAMPLES);
			if (slen > 0)
			{
				slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
				alBufferData(bufid, p_Format, p_Membuf, (ALsizei)slen,
					p_Sfinfo.samplerate);
				alSourceQueueBuffers(p_Source, 1, &bufid);
				//std::cout << "Buffer new data!" << "size: " << slen << " bytes." << std::endl;
			}
			else if (loop)
			{
				//alSourceRewind(p_Source);
				Replay();
				//std::cout << "Try Loop!" << std::endl;
			}

			if (alGetError() != AL_NO_ERROR)
			{
				//std::cout << "Error?" << std::endl;
				throw("error buffering music data");
			}
		}

		/* Make sure the source hasn't underrun */
		if (state != AL_PLAYING && state != AL_PAUSED && state != AL_STOPPED)
		{
			ALint queued;

			/* If no buffers are queued, playback is finished */
			alGetSourcei(p_Source, AL_BUFFERS_QUEUED, &queued);
			if (queued == 0)
				return;

			alSourcePlay(p_Source);
			if (alGetError() != AL_NO_ERROR)
			{
				throw("error restarting music playback");
			}
		}
	}

	ALint getSource()
	{
		return p_Source;
	}

	Music(const char* givenFileName)
	{
		alGenSources(1, &p_Source);
		alGenBuffers(NUM_BUFFERS, p_Buffers);

		std::size_t frame_size;

		p_SndFile = sf_open(givenFileName, SFM_READ, &p_Sfinfo);
		if (!p_SndFile)
		{
			throw("could not open provided music file -- check path");
		}

		/* Get the sound format, and figure out the OpenAL format */
		if (p_Sfinfo.channels == 1)
			p_Format = AL_FORMAT_MONO16;
		else if (p_Sfinfo.channels == 2)
			p_Format = AL_FORMAT_STEREO16;
		else if (p_Sfinfo.channels == 3)
		{
			if (sf_command(p_SndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				p_Format = AL_FORMAT_BFORMAT2D_16;
		}
		else if (p_Sfinfo.channels == 4)
		{
			if (sf_command(p_SndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				p_Format = AL_FORMAT_BFORMAT3D_16;
		}
		if (!p_Format)
		{
			sf_close(p_SndFile);
			p_SndFile = NULL;
			throw("Unsupported channel count from file");
		}

		frame_size = ((size_t)BUFFER_SAMPLES * (size_t)p_Sfinfo.channels) * sizeof(short);
		p_Membuf = static_cast<short*>(malloc(frame_size));

		fileName = givenFileName;

	}

	~Music()
	{
		alDeleteSources(1, &p_Source);

		if (p_SndFile)
			sf_close(p_SndFile);

		p_SndFile = nullptr;

		free(p_Membuf);

		alDeleteBuffers(NUM_BUFFERS, p_Buffers);
	}

};


