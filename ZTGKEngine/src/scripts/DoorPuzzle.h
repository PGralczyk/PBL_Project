#pragma once

#include <glm/glm.hpp>
#include "../GraphNode.h"
#include "../ApTime.h"
#include "RealtimeScript.h""

//for debug ------------------
#include "../SoundBuffer.h"
#include "../SoundSource.h"
//-----------------------------

class GraphNode;
class OtherTestRealtimeScript;
class ChessPieceScript;
class OneTimeActivatorScript;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class DoorPuzzle : public RealtimeScript {

private:
	GraphNode* puzzle;
	GLFWwindow* window;
	GraphNode* finalScreen;
	string* password;
	string passwordAnswer = "24121834";
	bool* isWon;

public:

	//Constructor, here assign all the fields from the private section
	DoorPuzzle(GraphNode* nodePointer, GraphNode* _puzzle, GraphNode* _finalScreen, GLFWwindow* _window, string* _password, bool* _isWon) : RealtimeScript(nodePointer)
	{
		puzzle = _puzzle;
		window = _window;
		password = _password;
		isWon = _isWon;
		finalScreen = _finalScreen;
	}

	~DoorPuzzle() = default;

	void OnMouseClicked()
	{
		if (*isWon)
		{
			finalScreen->SetActive(true);
		}
		else
		{
			puzzle->SetActive(true);
		}
	}

	void Update()
	{
		if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_RIGHT))
		{
			puzzle->SetActive(false);
		}
		if (*password == passwordAnswer && !*isWon)
		{
			*isWon = true;
			node->Rotate(30, glm::vec3(0.0f, 1.0f, 0.0f));
			node->Translate(glm::vec3(-74.0f, 0.0f, 30.0f));
		}
	}
};
