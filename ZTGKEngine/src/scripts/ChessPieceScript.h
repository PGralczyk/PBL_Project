#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"
#include "RealtimeScript.h"
#include "ApTime.h"
#include "ChessBoardPuzzle.h"

class GraphNode;
class OtherTestRealtimeScript;
class ChessBoardPuzzle;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class ChessPieceScript : public RealtimeScript {

private:

	SoundSource speaker;

public:
	bool isBeingMoved = false;
	int tileId = 100;
	glm::vec3 goalPosition;
	glm::vec3 brightWorldPosition;
	glm::vec3 originalPosition;
	GLFWwindow* window;
	bool* poof;

	//Constructor, here assign all the fields from the private section
	ChessPieceScript(GraphNode* nodePointer, GLFWwindow* givenWindow, bool* poof = nullptr) : RealtimeScript(nodePointer)
	{
		window = givenWindow;
		this->poof = poof;
	}

	void Start()
	{
		brightWorldPosition = node->getTranslation();
		originalPosition = node->getTranslation();
	}

	~ChessPieceScript() = default;

	void OnMouseClicked()
	{
		if (ApTime::instance().isChessPosition)
		{
			if (ApTime::instance().brightWorld)
			{
				if (!isBeingMoved)
				{
					node->Translate(glm::vec3(0.0f, 300.0f, 0.0f));
					isBeingMoved = true;
				}
			}
		}
		else
		{
			ApTime::instance().isChessPosition = true;
		}
	}

	void SavePosition()
	{
		brightWorldPosition = glm::vec3(node->getTranslation().x, node->getTranslation().y,
			node->getTranslation().z);
	}

	void Update()
	{
		if (*poof)
		{
			isBeingMoved = false;
			if (!ApTime::instance().brightWorld)
			{
				node->setTranslate(glm::vec3(brightWorldPosition.x,
					brightWorldPosition.y, brightWorldPosition.z));
				node->isHoverable = true;
			}
			else
			{
				node->setTranslate(glm::vec3(goalPosition.x,
					goalPosition.y, goalPosition.z));
				node->isHoverable = false;
			}
		}
	}

	void GreatReset()
	{
		isBeingMoved = false;
		tileId = 100;
		brightWorldPosition = originalPosition;

	}
};
