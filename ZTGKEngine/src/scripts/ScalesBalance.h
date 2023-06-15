#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class ScalesBalance : public RealtimeScript {

private:
	int* scalesPuzzleController;
	int rotateStatus;
	GraphNode* prize;

public:
	//Constructor, here assign all the fields from the private section
	ScalesBalance(GraphNode* nodePointer, int* _scalesPuzzleController, GraphNode* _prize) : RealtimeScript(nodePointer)
	{
		this->scalesPuzzleController = _scalesPuzzleController;
		this->rotateStatus = 0;
		this->prize = _prize;
		node->isHoverable = false;
	}

	void Update()
	{
		if (*scalesPuzzleController > 0)
		{
			if (rotateStatus == 0)
			{
				node->Rotate(-10, glm::vec3(1, 0, 0));
			}
			else if (rotateStatus == -1)
			{
				node->Rotate(-20, glm::vec3(1, 0, 0));
			}
			rotateStatus = 1;
		}
		else if (*scalesPuzzleController == 0)
		{
			if (rotateStatus == -1)
			{
				node->Rotate(-10, glm::vec3(1, 0, 0));
			}
			else if (rotateStatus == 1)
			{
				node->Rotate(10, glm::vec3(1, 0, 0));

			}
			rotateStatus = 0;
			prize->SetActive(true);
			enabled = false;
		}
		else
		{
			if (rotateStatus == 0)
			{
				node->Rotate(10, glm::vec3(1, 0, 0));
			}
			else if (rotateStatus == 1)
			{
				node->Rotate(20, glm::vec3(1, 0, 0));

			}
			rotateStatus = -1;
		}
	}

	~ScalesBalance() = default;

	void GreatReset()
	{
		this->rotateStatus = 0;
		enabled = true;
		*scalesPuzzleController = 3;
	}
};
