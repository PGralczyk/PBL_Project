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
	GraphNode* currentScene; 
	GraphNode* otherScene;
	GraphNode* brightUI;
	GraphNode* darkUI;

	bool keyPressed = false;
	bool controlPressed = false;

	bool* lightVersion;
	bool* poof;
	bool* singleClick;
	bool forceSwap = false;

public:
	//Constructor, here assign all the fields from the private section
	RoomSwapManager(GraphNode* nodePointer, GraphNode* brightNode, GraphNode* darkNode, 
		GraphNode* brightUInode, GraphNode* darkUInode,GLFWwindow* givenWindow, GraphNode* _currentScene,
		GraphNode* _otherScene, bool* givenVersion, bool* _singleClick, bool* swapPostman = nullptr,
		bool* poof = nullptr): RealtimeScript(nodePointer)
	{
		brightWorld = brightNode;
		darkWorld = darkNode;
		window = givenWindow;
		lightVersion = givenVersion;
		this->swapPostman = swapPostman;
		this->poof = poof;
		currentScene = _currentScene;
		otherScene = _otherScene;
		singleClick = _singleClick;
		brightUI = brightUInode;
		darkUI = darkUInode;
	}

	~RoomSwapManager() = default;

	void Start()
	{
		darkWorld->SetActive(false);
	}

	void Update()
	{
		if (*poof) {
			darkWorld->SetActive(!darkWorld->GetActive());
			brightWorld->SetActive(!brightWorld->GetActive());
			brightUI->SetActive(!brightUI->GetActive());
			darkUI->SetActive(!darkUI->GetActive());

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
		if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || forceSwap))
		{
			if (!keyPressed)
			{
				forceSwap = false;
				*swapPostman = !*swapPostman;
				ApTime::instance().adviseWindow = 0.0f;
				keyPressed = true;
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

		if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) && *singleClick) || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
		{
			ApTime::instance().adviseWindow = 0.0f;
		}
	}

	void OnMouseClicked()
	{
		ApTime::instance().brightWorld = true;
		brightWorld->SetActive(true);
		darkWorld->SetActive(false);
		currentScene->SetActive(false);
		otherScene->SetActive(true);
		std::cout << otherScene->GetActive() << std::endl;
	}

	void SetForceSwap()
	{
		forceSwap = true;
	}
};