#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class MenuScript : public RealtimeScript {

private:
	GLFWwindow* window;
	string buttonId;
	GraphNode* other;
	bool isFirstTime = true;
	GraphNode* firstTime;

public:
	//Constructor, here assign all the fields from the private section
	MenuScript(GraphNode* nodePointer, GraphNode* _other, GLFWwindow* _window, string _buttonId, 
		GraphNode* _firstTime = new GraphNode()) : RealtimeScript(nodePointer)
	{
		this->window = _window;
		this->buttonId = _buttonId;
		this->other = _other;
		this->firstTime = _firstTime;
	}

	void OnMouseClicked()
	{
		if (buttonId == "goBack")
		{
			if (!isFirstTime)
				other->SetActive(false);
			else
			{
				firstTime->SetActive(true);
				isFirstTime = false;
			}
		}
		else if (buttonId == "goNext")
			other->SetActive(true);
		else if (buttonId == "exit")
			glfwSetWindowShouldClose(window, true);
		else if (buttonId == "startGame")
		{
			firstTime->SetActive(false);
			other->SetActive(false);
		}
	}

	~MenuScript() = default;
};
