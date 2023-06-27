#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class ActivateOnHoverScript;

class ActivateOnHoverScript : public RealtimeScript {
private:
	GraphNode* other;
	bool* shouldWork;
	bool playSound;

	SoundSource speaker;

public:
	//Constructor, here assign all the fields from the private section
	ActivateOnHoverScript(GraphNode* nodePointer, GraphNode* _other, bool _playSound = false, bool* _shouldWork = &ApTime::instance().shouldBtnWork) : RealtimeScript(nodePointer)
	{
		other = _other;
		shouldWork = _shouldWork;
		playSound = _playSound;
	}

	~ActivateOnHoverScript() = default;

	void OnMouseHover()
	{
		if (*shouldWork)
		{
			other->SetActive(true);
			if (playSound)
			{
				speaker.Play(SoundBuffer::get()->getSound("hover"));
			}
		}
	}
};

class JournalScript;

class DeactivateRMB : public RealtimeScript {
private:
	string soundToBePlayed;
	SoundSource speaker;

public:
	GraphNode* object;
	GLFWwindow* window;

	//Constructor, here assign all the fields from the private section
	DeactivateRMB(GraphNode* nodePointer, GLFWwindow* _window, string _soundToBePlayed = "") : RealtimeScript(nodePointer)
	{
		window = _window;
		soundToBePlayed = _soundToBePlayed;
		node->isHoverable = false;
	}

	~DeactivateRMB() = default;

	void Update()
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
		{
			speaker.Play(SoundBuffer::get()->getSound(soundToBePlayed));
			node->SetActive(false);
		}
	}
};


class DeactivateAfterTime : public RealtimeScript {
private:

	float timer = 0;
	float goalTime;

public:

	//Constructor, here assign all the fields from the private section
	DeactivateAfterTime(GraphNode* nodePointer, float _goalTimer) : RealtimeScript(nodePointer)
	{
		goalTime = _goalTimer;
		node->isHoverable = false;
	}

	void Update()
	{
		if (timer >= goalTime)
		{
			timer = 0;
			node->SetActive(false);
		}
		timer += ApTime::instance().deltaTime;
	}

	void GreatReset()
	{
		timer = 0;
	}
};