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
	bool shouldWork;
	bool keyPressed = false;
	GLFWwindow* window;

public:
	//Constructor, here assign all the fields from the private section
	MenuButtonScript(GraphNode* nodePointer, GraphNode* _other, GLFWwindow* _window, bool _shouldWork = true) : RealtimeScript(nodePointer)
	{
		other = _other;
		window = _window;
		shouldWork = _shouldWork;
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
		if (shouldWork)
			other->SetActive(true);
	}

	~MenuButtonScript() = default;
};