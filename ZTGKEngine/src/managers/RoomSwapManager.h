#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"
#include "../scripts/RealtimeScript.h"
#include "ApTime.h"
#include "../SoundBuffer.h"
#include "../SoundSource.h"

class GraphNode;
//class SoundBuffer;
//class SoundSource;

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

	int wasTutorialUsed = 0;

	bool keyPressed = false;
	bool controlPressed = false;

	bool* lightVersion;
	bool* poof;
	bool* singleClick;
	bool* forceSwap;
	bool canClick;
	bool initialCanClick;

	SoundSource speaker;
	SoundSource doorSpeaker;

	GraphNode* tutorial;
	GraphNode* otherTutorial;

public:
	//Constructor, here assign all the fields from the private section
	RoomSwapManager(GraphNode* nodePointer, GraphNode* brightNode, GraphNode* darkNode, 
		GraphNode* brightUInode, GraphNode* darkUInode,GLFWwindow* givenWindow, GraphNode* _currentScene,
		GraphNode* _otherScene, bool* givenVersion, bool* _singleClick, bool* _forceSwap, bool* swapPostman = nullptr,
		bool* poof = nullptr, GraphNode* _tutorial = nullptr, GraphNode* _otherTutorial = nullptr, bool _canClick = true): RealtimeScript(nodePointer)
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
		forceSwap = _forceSwap;
		canClick = _canClick;
		tutorial = _tutorial;
		otherTutorial = _otherTutorial;
		initialCanClick = _canClick;
	}

	~RoomSwapManager() = default;

	void Start()
	{
		darkWorld->SetActive(false);
	}

	void Update()
	{
		if (*poof) {
			if (wasTutorialUsed == 1)
			{
				otherTutorial->SetActive(true);
				wasTutorialUsed = 2;
			}
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
		if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && ApTime::instance().canSwap || *forceSwap))
		{
			if (!keyPressed)
			{
				if (wasTutorialUsed == 0 && tutorial != nullptr)
				{
					tutorial->SetActive(false);
					wasTutorialUsed = 1;
				}

				*forceSwap = false;
				*swapPostman = !*swapPostman;
				ApTime::instance().adviseWindow = 0.0f;
				keyPressed = true;
				if (ApTime::instance().brightWorld) 
				{
					speaker.Play(SoundBuffer::get()->getSound("plantGrow"));
				}
				else
				{
					speaker.Play(SoundBuffer::get()->getSound("plantShrink"));
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

		if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) && *singleClick) || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
		{
			ApTime::instance().adviseWindow = 0.0f;
		}

		if ((ApTime::instance().currentPuzzleState == 1 || ApTime::instance().currentPuzzleState == 6) && ApTime::instance().adviseWindow > 0)
		{
			node->forceHover = true;
		}
	}

	void OnMouseClicked()
	{
		if (canClick)
		{
			doorSpeaker.Play(SoundBuffer::get()->getSound("tuptup"));

			if (ApTime::instance().currentPuzzleState == 1 || ApTime::instance().currentPuzzleState == 6)
			{
				ApTime::instance().currentPuzzleState++;
			}

			ApTime::instance().isBuzzzing = !ApTime::instance().isBuzzzing;

			ApTime::instance().brightWorld = true;
			brightWorld->SetActive(true);
			darkWorld->SetActive(false);
			brightUI->SetActive(true);
			darkUI->SetActive(false);
			*lightVersion = true;
			currentScene->SetActive(false);
			otherScene->SetActive(true);
			
		}
		else
		{
			doorSpeaker.Play(SoundBuffer::get()->getSound("doorClosed"));
		}
	}

	void SetForceSwap()
	{
		*forceSwap = true;
	}

	void MakeClickable()
	{
		canClick = true;
	}

	void GreatReset()
	{
		keyPressed = false;
		controlPressed = false;
		canClick = initialCanClick;
		wasTutorialUsed = 0;
	}
};