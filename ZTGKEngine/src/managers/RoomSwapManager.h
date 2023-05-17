#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"
#include "../scripts/RealtimeScript.h"
#include "ApTime.h"
#include "../scripts/OtherTestRealtimeScript.h"

class GraphNode;

//WHAT IS THIS CLASS?
//A test of implementing realtime script
class RoomSwapManager : public RealtimeScript {
private:
	GraphNode* brightWorld;
	GraphNode* darkWorld;
	GraphNode* fadeNode;
	GLFWwindow* window;
	bool* swapPostman;

	bool keyPressed = false;
	bool controlPressed = false;

	bool *lightVersion;

public:
	//Constructor, here assign all the fields from the private section
	RoomSwapManager(GraphNode* nodePointer, GraphNode* brightNode, GraphNode* darkNode, 
		GLFWwindow* givenWindow, bool* givenVersion, bool* swapPostman = nullptr): RealtimeScript(nodePointer)
	{
		brightWorld = brightNode;
		darkWorld = darkNode;
		window = givenWindow;
		lightVersion = givenVersion;
		this->swapPostman = swapPostman;
	}

	~RoomSwapManager() = default;

	void Start()
	{
		darkWorld->SetActive(false);
	}

	void Update()
	{
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			if (!keyPressed)
			{
				//*swapPostman = !*swapPostman;
				ApTime::instance().adviseWindow = 0.0f;
				keyPressed = true;
				darkWorld->SetActive(!darkWorld->GetActive());
				brightWorld->SetActive(!brightWorld->GetActive());
				if (brightWorld->GetActive())
				{
					*lightVersion = true;
					ApTime::instance().brightWorld = true;
				}
				else
				{
					*lightVersion = false;
					ApTime::instance().brightWorld = false;
				}
			}
		}
		else
		{
			keyPressed = false;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && ApTime::instance().isEasyMode)
		{
			if(!controlPressed)
			{
				controlPressed = true;
				ApTime::instance().adviseWindow = 3.0f;
			}
		}
		else
		{
			controlPressed = false;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
		{
			ApTime::instance().adviseWindow = 0.0f;
		}
	}
};