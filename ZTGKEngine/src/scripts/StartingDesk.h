#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class StartingDesk;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class StartingDesk : public RealtimeScript {

private:
	GraphNode* hintTutorial;

public:
	GLFWwindow* window;

	//Constructor, here assign all the fields from the private section
	StartingDesk(GraphNode* nodePointer, GLFWwindow* _window, bool shouldBeNotHoverable = true, GraphNode* _hintTutorial = nullptr) : RealtimeScript(nodePointer)
	{
		window = _window;
		hintTutorial = _hintTutorial;
		if(shouldBeNotHoverable)
			node->isHoverable = false;
	}

	~StartingDesk() = default;

	void SetTutorial(GraphNode* tutorial)
	{
		hintTutorial = tutorial;
	}

	void Update()
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) && ApTime::instance().canUseJournal)
		{
			ApTime::instance().isDeskPosition = false;
			node->isHoverable = true;
			if (hintTutorial && ApTime::instance().canShowHintTutorial && ApTime::instance().isEasyMode)
			{
				ApTime::instance().canShowHintTutorial = false;
				hintTutorial->SetActive(true);
			}
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
private:
	bool* condition;

public:
	GraphNode* toBeActivated;

	//Constructor, here assign all the fields from the private section
	ActivateOnBool(GraphNode* nodePointer, GraphNode* _toBeActivated, bool* _condition) : RealtimeScript(nodePointer)
	{
		toBeActivated = _toBeActivated;
		condition = _condition;
	}

	~ActivateOnBool() = default;

	void Update()
	{
		if (toBeActivated->GetActive() && *condition)
		{
			enabled = false;
		}
		else if (*condition)
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
