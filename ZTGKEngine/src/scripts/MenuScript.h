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

public:
	//Constructor, here assign all the fields from the private section
	MenuScript(GraphNode* nodePointer, GraphNode* _other, GLFWwindow* _window, string _buttonId) : RealtimeScript(nodePointer)
	{
		this->window = _window;
		this->buttonId = _buttonId;
		this->other = _other;
	}

	void OnMouseClicked()
	{
		if (buttonId == "goBack")
		{
			other->SetActive(false);
		}
		else if (buttonId == "goNext")
		{
			other->SetActive(true);
		}
		else if (buttonId == "exit")
		{
			glfwSetWindowShouldClose(window, true);
		}
	}

	~MenuScript() = default;
};
