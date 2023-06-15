#pragma once


#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class MenuButtonScript : public RealtimeScript {

private:
	GraphNode* other;
	bool isNotHover;
	bool keyPressed = false;
	GLFWwindow* window;

public:
	//Constructor, here assign all the fields from the private section
	MenuButtonScript(GraphNode* nodePointer, GraphNode* _other, GLFWwindow* _window, bool _isNotHover = false) : RealtimeScript(nodePointer)
	{
		other = _other;
		window = _window;
		isNotHover = _isNotHover;
	}

	void Update()
	{
		if (isNotHover)
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
	}

	void OnMouseClicked()
	{
		other->SetActive(true);
	}

	~MenuButtonScript() = default;

	void GreatReset()
	{
		keyPressed = false;
	}
};