#pragma once

#include <AL\al.h>
#include <iostream>

class SoundSource
{
private:
	ALuint p_Source;
	float p_Pitch = 1.f;
	float p_Gain = 1.f;
	float p_Position[3] = { 0,0,0 };
	float p_Velocity[3] = { 0,0,0 };
	bool p_LoopSound = false;
	ALuint p_Buffer = 0;

public:
	SoundSource()
	{
		alGenSources(1, &p_Source);
		alSourcef(p_Source, AL_PITCH, p_Pitch);
		alSourcef(p_Source, AL_GAIN, p_Gain);
		alSource3f(p_Source, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]);
		alSource3f(p_Source, AL_VELOCITY, p_Velocity[0], p_Velocity[1], p_Velocity[2]);
		alSourcei(p_Source, AL_LOOPING, p_LoopSound);
		alSourcei(p_Source, AL_BUFFER, p_Buffer);
	}

	~SoundSource()
	{
		alDeleteSources(1, &p_Source);
	}

	void Play(const ALuint buffer_to_play)
	{
		if (buffer_to_play != p_Buffer)
		{
			p_Buffer = buffer_to_play;
			alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
		}

		alSourcePlay(p_Source);


		/*ALint state = AL_PLAYING;
		std::cout << "playing sound\n";
		std::cout << alGetError();
		while (state == AL_PLAYING && (alGetError() == AL_NO_ERROR || alGetError() == AL_INVALID_NAME))
		{
			std::cout << "currently playing sound\n";
			alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
		}
		std::cout << "done playing sound\n";*/
	}
	void SetVolume(float newVolume)
	{
		if (0 <= newVolume && newVolume <= 1)
		{
			p_Gain = newVolume;
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
		p_Gain = 0.0;
		SetVolume(0.0);
	}

};