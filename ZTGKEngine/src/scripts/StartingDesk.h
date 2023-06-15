#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class StartingDesk;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class StartingDesk : public RealtimeScript {

public:
	GLFWwindow* window;

	//Constructor, here assign all the fields from the private section
	StartingDesk(GraphNode* nodePointer, GLFWwindow* _window, bool shouldBeNotHoverable = true) : RealtimeScript(nodePointer)
	{
		window = _window;
		if(shouldBeNotHoverable)
			node->isHoverable = false;
	}

	~StartingDesk() = default;

	void Update()
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) && ApTime::instance().canSwap)
		{
			ApTime::instance().isDeskPosition = false;
			node->isHoverable = true;
		}

		if (ApTime::instance().isDeskPosition)
		{
			node->isHoverable = false;
		}
		else
		{
			node->isHoverable = true;
		}
	}

	void OnMouseClicked()
	{
		ApTime::instance().isDeskPosition = true;
	}
};

class ActivateOnBool : public RealtimeScript {

public:
	GraphNode* toBeActivated;

	//Constructor, here assign all the fields from the private section
	ActivateOnBool(GraphNode* nodePointer, GraphNode* _toBeActivated) : RealtimeScript(nodePointer)
	{
		toBeActivated = _toBeActivated;
	}

	~ActivateOnBool() = default;

	void Update()
	{
		if (toBeActivated->GetActive() && ApTime::instance().canSwap)
		{
			enabled = false;
		}
		else if (ApTime::instance().canSwap)
		{
			toBeActivated->SetActive(true);
		}
	}

	void GreatReset()
	{
		enabled = true;
	}
};

class MagicalPlant : public RealtimeScript {
private:
	GraphNode* tutorial;
public:

	//Constructor, here assign all the fields from the private section
	MagicalPlant(GraphNode* nodePointer, GraphNode* _tutorial) : RealtimeScript(nodePointer)
	{
		tutorial = _tutorial;
	}

	~MagicalPlant() = default;
	void OnMouseClicked()
	{
		ApTime::instance().canSwap = true;
		tutorial->SetActive(true);
		node->SetActive(false);
	}
};
