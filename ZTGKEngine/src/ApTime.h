#pragma once
#include <GLFW/glfw3.h>
#include "Windows.h"
#include <map>
#include <AL/al.h>
#include "./SoundSource.h"
#include "./SoundBuffer.h"


class SoundSource;
class SoundBuffer;
class Music;

//WHAT IS THIS CLASS?
//It's a utility class meant for stuff like counting deltaTime and so on.
//HOW TO USE IT?
//Easy. It's updated each frame in main. If you want to use it just type
//ApTime::instance() anywhere in the program.
class ApTime {
private:
	float timeRatio = 1;

	//Used for counting deltaTime and last frame time
	void UpdateDeltaTime()
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = (currentFrame - lastFrame) * timeRatio;
		lastFrame = currentFrame;

		if (adviseWindow > 0)
		{
			adviseWindow -= deltaTime;
		}
		else
		{
			adviseWindow = 0;
		}
	}

	ApTime() {}
	ApTime(const ApTime&) {}
	~ApTime() = default;

public:
	//You can access deltaTime or time of the last frame anywhere in the app. Just type:
	//ApTime::instance().deltaTime
	//ApTime::instance().lastFrame
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	string pickedElementId = "";
	bool brightWorld = true;
	bool isChessPosition = false;
	bool isDeskPosition = true;
	bool isBookPosition = false;
	bool isPlantPosition = false;
	bool isEasyMode = false;
	bool isBuzzzing = true;
	bool canSwap = false;
	bool shouldBtnWork = true;
	bool isFirstTime = true;
	bool canUseJournal = false;
	SoundSource* mainMusicSpeaker;
	SoundSource* mainAmbientSpeaker;
	//std::map<string, Music*> gameMusic;
	int currentPuzzleState = 0;
	float adviseWindow = 0;
	bool isSwitching = false;
	bool forceReset = false;
	bool canShowHintTutorial = true;
	bool isMenuOpen = true;
	double offsetX = 0.0;
	double offsetY = 0.0;
	double withinWindowCursorPosX = 0.0;
	double withinWindowCursorPosY = 0.0;
	bool hitnWorks;
	bool isMenuClosed;

	void GreatReset()
	{
		timeRatio = 1;
		deltaTime = 0.0f;
		lastFrame = 0.0f;
		pickedElementId = "";
		brightWorld = true;
		isChessPosition = false;
		isDeskPosition = true;
		isBookPosition = false;
		isEasyMode = false;
		isBuzzzing = true;
		canSwap = false;
		shouldBtnWork = true;
		isFirstTime = true;
		currentPuzzleState = 0;
		adviseWindow = 0;
		isPlantPosition = false;
		forceReset = false;
		canShowHintTutorial = true;
		isMenuOpen = true;
		canUseJournal = false;

		mainMusicSpeaker->Stop();
		mainAmbientSpeaker->Stop();

		while ((mainMusicSpeaker->GetState() == AL_PLAYING || mainAmbientSpeaker->GetState() == AL_PLAYING) && (alGetError() == AL_NO_ERROR || alGetError() == AL_INVALID_NAME))
		{
		}
		mainMusicSpeaker->Play(SoundBuffer::get()->getSound("menuTheme"));
	}

	//Updates each frame
	void Update()
	{
		UpdateDeltaTime();
		hitnWorks = IsHintWork();
		isMenuClosed = IsMenuClosed();
	}

	//It speeds up or slows down everything that uses delta time(0.5f = 2 times slower,
	//3.0f = 3 times faster and so on...)
	//To change time ratio just type ApTime::instance().SetTimeRatio(newRatio) anywhere in the app
	void SetTimeRatio(float ratio)
	{
		timeRatio = ratio;
	}

	float GetTimeRatio()
	{
		return timeRatio;
	}

	bool IsHintWork()
	{
		return !isMenuOpen && isEasyMode;
	}

	bool IsMenuClosed()
	{
		return !isMenuOpen;
	}

	static ApTime& instance()
	{
		static ApTime instance_;
		return instance_;
	}
};