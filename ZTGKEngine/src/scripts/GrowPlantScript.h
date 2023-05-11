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
public:
	//Constructor, here assign all the fields from the private section
	GrowPlantScript(GraphNode* nodePointer, int _growthGoal, int* _puzzleState) : RealtimeScript(nodePointer)
	{
		growthGoal = _growthGoal;
		translation = node->getTranslation();
		puzzleState = _puzzleState;
	}

	~GrowPlantScript() = default;

	void growPlant()
	{
		bool isRight = growthGoal == growthState;
		growthGoal++;
		translation.y += 200.0f;
		node->setTranslate(translation);
		if (!isRight && growthGoal == growthState)
			*puzzleState++;
		else if (isRight && growthGoal != growthState)
		{
			*puzzleState--;
		}
	}

	void onMouseClick()
	{
		if (ApTime::instance().pickedElementId == "scissors")
		{
			if (growthGoal == growthState)
				*puzzleState--;
			translation.y -= 200.0f * growthState;
			growthState = 0;
			node->setTranslate(translation);
			ApTime::instance().pickedElementId == "";
		}
		else if (ApTime::instance().pickedElementId == "water")
		{
			growPlant();
			ApTime::instance().pickedElementId == "";
		}
	}
};
