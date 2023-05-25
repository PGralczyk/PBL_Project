#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class GrowPlantScript : public RealtimeScript {
private:
	int growthGoal;
	int growthState = 0;
	glm::vec3 translation;
	int* puzzleState;
	GraphNode* flowerReference[5];
public:
	//Constructor, here assign all the fields from the private section
	GrowPlantScript(GraphNode* nodePointer, int _growthGoal, int* _puzzleState, GraphNode* _flowerReference[]) : RealtimeScript(nodePointer)
	{
		growthGoal = _growthGoal;
		for (int i = 0; i < 5; i++)
		{
			flowerReference[i] = _flowerReference[i];
			flowerReference[i]->SetActive(false);
		}
		puzzleState = _puzzleState;
	}

	~GrowPlantScript() = default;

	void growPlant()
	{
		if (growthState < 5)
		{
			bool isRight = growthGoal == growthState;
			flowerReference[growthState]->SetActive(true);
			growthState++;
			if (!isRight && growthGoal == growthState)
			{
				*puzzleState += 1;
			}	
			else if (isRight && growthGoal != growthState)
			{
				*puzzleState -= 1;
			}
		}
		else
		{
			if (growthGoal == growthState)
				*puzzleState--;
			growthState = 0;
			for (GraphNode* flower : flowerReference)
			{
				flower->SetActive(false);
			}
		}
	}

	void OnMouseClicked()
	{
		if (ApTime::instance().pickedElementId == "scissoors")
		{
			if (growthGoal == growthState)
				*puzzleState -= 1;
			growthState = 0;
			for (GraphNode* flower : flowerReference)
			{
				flower->SetActive(false);
			}
		}
		else if (ApTime::instance().pickedElementId == "bucket")
		{
			growPlant();
		}
	}

	void Update()
	{
		if (*puzzleState == 3)
		{
			node->isHoverable = false;
			enabled = false;
		}

		if (ApTime::instance().currentPuzzleState == 5 && ApTime::instance().adviseWindow > 0 && growthState != growthGoal)
		{
			node->forceHover = true;
		}
	}
};
