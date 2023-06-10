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

public:
	//Constructor, here assign all the fields from the private section
	ActivateOnHoverScript(GraphNode* nodePointer, GraphNode* _other, bool* _shouldWork = &ApTime::instance().shouldBtnWork) : RealtimeScript(nodePointer)
	{
		other = _other;
		shouldWork = _shouldWork;
	}

	~ActivateOnHoverScript() = default;

	void OnMouseHover()
	{
		if (*shouldWork)
			other->SetActive(true);
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