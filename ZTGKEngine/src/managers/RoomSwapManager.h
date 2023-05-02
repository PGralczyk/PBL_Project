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
	GLFWwindow* window;

	bool keyPressed = false;

	bool *lightVersion;

public:
	//Constructor, here assign all the fields from the private section
	RoomSwapManager(GraphNode* nodePointer, GraphNode* brightNode, GraphNode* darkNode, 
		GLFWwindow* givenWindow, bool* givenVersion): RealtimeScript(nodePointer)
	{
		brightWorld = brightNode;
		darkWorld = darkNode;
		window = givenWindow;
		lightVersion = givenVersion;
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
				keyPressed = true;
				darkWorld->SetActive(!darkWorld->GetActive());
				brightWorld->SetActive(!brightWorld->GetActive());
				if (brightWorld->GetActive())
				{
					*lightVersion = true;
				}
				else
				{
					*lightVersion = false;
				}
			}
		}
		else
		{
			keyPressed = false;
		}
	}
};