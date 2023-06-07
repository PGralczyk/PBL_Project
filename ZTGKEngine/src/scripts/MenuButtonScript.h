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
	GLFWwindow* window;
	bool* gameMode;
	string gameModeId;
	bool* choosen;
	bool keyPressed = false;

public:
	//Constructor, here assign all the fields from the private section
	MenuButtonScript(GraphNode* nodePointer, GLFWwindow* _window, string _gameModeId, bool* _gameMode, bool* _choosen) : RealtimeScript(nodePointer)
	{
		this->window = _window;
		this->gameMode = _gameMode;
		this->gameModeId = _gameModeId;
		this->choosen = _choosen;
	}

	void Start()
	{
		//node->SetActive(false);
	}

	void Update()
	{
		std::cout << node->GetActive() << std::endl;
		if (choosen)
		{
			//node->SetActive(false);
		}

		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		{
			if (!keyPressed)
			{
				node->SetActive(!node->GetActive());
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
		if (gameModeId == "easy")
		{
			*choosen = true;
			*gameMode = true;
			node->SetActive(false);
		}
		else if (gameModeId == "medium")
		{
			*choosen = true;
			*gameMode = false;
			node->SetActive(false);
		}
	}

	~MenuButtonScript() = default;
};