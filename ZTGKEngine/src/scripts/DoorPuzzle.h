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
	string* password;
	string passwordAnswer = "24161834";
	bool* isWon;

public:

	//Constructor, here assign all the fields from the private section
	DoorPuzzle(GraphNode* nodePointer, GraphNode* _puzzle, GLFWwindow* _window, string* _password, bool* _isWon) : RealtimeScript(nodePointer)
	{
		puzzle = _puzzle;
		window = _window;
		password = _password;
		isWon = _isWon;
	}

	~DoorPuzzle() = default;

	void OnMouseClicked()
	{
		if (*isWon)
		{

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
		if (*password == passwordAnswer)
		{
			*isWon = true;
		}
	}
};
