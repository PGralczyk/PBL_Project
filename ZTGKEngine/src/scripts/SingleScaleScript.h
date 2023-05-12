#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class SingleScaleScript : public RealtimeScript {
private:
	int* scalesPuzzleController;
	bool isLeft;
	glm::vec3 startPosition;
	GraphNode* weightsTab[9];

public:
	//Constructor, here assign all the fields from the private section
	SingleScaleScript(GraphNode* nodePointer, int* _scalesPuzzleController, bool _isLeft, GraphNode* _weightsTab[9]) : RealtimeScript(nodePointer)
	{
		this->scalesPuzzleController = _scalesPuzzleController;
		this->isLeft = _isLeft;
		startPosition = node->getTranslation();
		for (int i = 0; i < 9; i++)
		{
			this->weightsTab[i] = _weightsTab[i];
		}
	}

	void Update()
	{
		if (*scalesPuzzleController > 0)
		{
			if (isLeft)
			{
				node->setTranslate(startPosition + glm::vec3(0, 20, 0));
			}
			else
			{
				node->setTranslate(startPosition + glm::vec3(0, -20, 0));
			}
		}
		else if (*scalesPuzzleController == 0)
		{
			node->setTranslate(startPosition);
			enabled = false;
			node->isHoverable = false;
		}
		else
		{
			if (isLeft)
			{
				node->setTranslate(startPosition + glm::vec3(0, -20, 0));
			}
			else
			{
				node->setTranslate(startPosition + glm::vec3(0, 20, 0));
			}
		}
	}

	~SingleScaleScript() = default;

	void OnMouseClicked()
	{
		if (node->isHoverable)
		{
			if (ApTime::instance().pickedElementId == "weight3")
			{
				if (isLeft)
				{
					*scalesPuzzleController -= 3;
					weightsTab[0]->SetActive(true);
					weightsTab[6]->SetActive(false);
				}
				else
				{
					*scalesPuzzleController += 3;
					weightsTab[3]->SetActive(true);
					weightsTab[6]->SetActive(false);
				}
			}
			else if (ApTime::instance().pickedElementId == "weight2")
			{
				if (isLeft)
				{
					*scalesPuzzleController -= 2;
					weightsTab[1]->SetActive(true);
					weightsTab[7]->SetActive(false);
				}
				else
				{
					*scalesPuzzleController += 2;
					weightsTab[4]->SetActive(true);
					weightsTab[7]->SetActive(false);
				}
			}
			else if (ApTime::instance().pickedElementId == "weight5")
			{
				if (isLeft)
				{
					*scalesPuzzleController -= 5;
					weightsTab[2]->SetActive(true);
					weightsTab[8]->SetActive(false);
				}
				else
				{
					*scalesPuzzleController += 5;
					weightsTab[5]->SetActive(true);
					weightsTab[8]->SetActive(false);
				}
			}
			
			if (weightsTab[6]->GetActive() && weightsTab[7]->GetActive() && weightsTab[8]->GetActive())
			{
				for (int i = 0; i < 6; i++)
				{
					weightsTab[i]->SetActive(false);
				}
				weightsTab[6]->SetActive(true);
				weightsTab[7]->SetActive(true);
				weightsTab[8]->SetActive(true);
			}
		}
	}
};

