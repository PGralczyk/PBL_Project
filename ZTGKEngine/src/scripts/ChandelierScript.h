#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class ChandelierScript : public RealtimeScript {
private:
	GraphNode* Glass;
	GraphNode* Shattered;
	GraphNode* prize;

public:
	//Constructor, here assign all the fields from the private section
	ChandelierScript(GraphNode* nodePointer, GraphNode* givenGlass, GraphNode* givenShattered, GraphNode* _prize) : RealtimeScript(nodePointer)
	{
		this->Glass = givenGlass;
		this->Shattered = givenShattered;
		prize = _prize;
	}

	~ChandelierScript() = default;

	void OnMouseClicked()
	{
		if (ApTime::instance().pickedElementId == "scissoors")
		{
			Glass->SetActive(false);
			Shattered->SetActive(true);
			node->SetActive(false);
			prize->SetActive(true);
			ApTime::instance().pickedElementId = "";
			ApTime::instance().currentPuzzleState = 2;
		}
	}

	void Update()
	{
		if (ApTime::instance().currentPuzzleState == 3 && ApTime::instance().adviseWindow > 0)
		{
			node->forceHover = true;
		}
	}
};
