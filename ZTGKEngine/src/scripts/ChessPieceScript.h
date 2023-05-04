#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"
#include "RealtimeScript.h"
#include "ApTime.h"
#include "OtherTestRealtimeScript.h"
#include "ChessBoardPuzzle.h"

class GraphNode;
class OtherTestRealtimeScript;
class ChessBoardPuzzle;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class ChessPieceScript : public RealtimeScript {

private:

public:
	bool isBeingMoved = false;
	int tileId = 100;
	glm::vec3 goalPosition;
	glm::vec3 brightWorldPosition;
	GLFWwindow* window;
	bool brightWorld = true;

	//Constructor, here assign all the fields from the private section
	ChessPieceScript(GraphNode* nodePointer, GLFWwindow* givenWindow) : RealtimeScript(nodePointer)
	{
		window = givenWindow;
	}

	void Start()
	{
		brightWorldPosition = node->getTranslation();
	}

	~ChessPieceScript() = default;

	void OnMouseClicked()
	{
		if (ApTime::instance().brightWorld)
		{
			if (!isBeingMoved)
			{
				node->Translate(glm::vec3(0.0f, 300.0f, 0.0f));
				isBeingMoved = true;
			}

			if (ApTime::instance().pickedElementId == "rose")
			{
				ApTime::instance().pickedElementId = "";
				std::cout << "Used rose on chess piece\n";
			}

			if (ApTime::instance().pickedElementId == "rose2")
			{
				ApTime::instance().pickedElementId = "";
				std::cout << "Used rose2 on chess piece\n";
			}
		}
	}

	void SavePosition()
	{
		brightWorldPosition = glm::vec3(node->getTranslation().x, node->getTranslation().y,
			node->getTranslation().z);
	}

	void Update()
	{
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			isBeingMoved = false;
			if (ApTime::instance().brightWorld)
			{
				node->setTranslate(new glm::vec3(brightWorldPosition.x,
					brightWorldPosition.y, brightWorldPosition.z));
			}
			else
			{
				node->setTranslate(new glm::vec3(goalPosition.x,
					goalPosition.y, goalPosition.z));
			}
			brightWorld = !brightWorld;
		}
	}
};
