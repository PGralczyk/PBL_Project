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
	bool* gameMode;
	string buttonId;
	bool* choosen;
	bool keyPressed = false;
	GraphNode* other;

public:
	//Constructor, here assign all the fields from the private section
	MenuScript(GraphNode* nodePointer, GraphNode* _other, GLFWwindow* _window, string _buttonId, bool* _gameMode, bool* _choosen) : RealtimeScript(nodePointer)
	{
		this->window = _window;
		this->gameMode = _gameMode;
		this->buttonId = _buttonId;
		this->choosen = _choosen;
		this->other = _other;
	}

	void Update()
	{
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		{
			if (!keyPressed)
			{

				other->SetActive(!other->GetActive());
				keyPressed = true;
			}
		}
		else
		{
			keyPressed = false;
		}
	}

	void OnMouseClicked()
	{
		if (buttonId == "play")
		{
			other->SetActive(false);
		}
		else if (buttonId == "medium")
		{
			*choosen = true;
			*gameMode = false;
			node->SetActive(false);
		}
		else if (buttonId == "exit")
		{
			glfwSetWindowShouldClose(window, true);
		}
	}

	~MenuScript() = default;
};
