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
	GraphNode* flowerReference;
public:
	//Constructor, here assign all the fields from the private section
	GrowPlantScript(GraphNode* nodePointer, int _growthGoal, int* _puzzleState, GraphNode* _flowerReference) : RealtimeScript(nodePointer)
	{
		growthGoal = _growthGoal;
		flowerReference = _flowerReference;
		translation = flowerReference->getTranslation();
		puzzleState = _puzzleState;
	}

	~GrowPlantScript() = default;

	void growPlant()
	{
		if (growthState < 5)
		{
			bool isRight = growthGoal == growthState;
			growthState++;
			translation.y += 4.0f;
			flowerReference->setTranslate(translation);
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
			translation.y -= 4.0f * growthState;
			growthState = 0;
			flowerReference->setTranslate(translation);
		}
	}

	void OnMouseClicked()
	{
		if (ApTime::instance().pickedElementId == "scissoors")
		{
			if (growthGoal == growthState)
				*puzzleState -= 1;
			translation.y -= 200.0f * growthState;
			growthState = 0;
			flowerReference->setTranslate(translation);
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
