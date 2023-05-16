#pragma once
#include <GLFW/glfw3.h>
#include "Windows.h"


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
	}

	ApTime(){}
	ApTime(const ApTime&){}
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

	//Updates each frame
	void Update()
	{
		UpdateDeltaTime();
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

	static ApTime& instance()
	{
		static ApTime instance_;
		return instance_;
	}
};