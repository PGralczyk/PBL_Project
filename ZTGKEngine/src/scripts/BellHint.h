#pragma once

#include <glm/glm.hpp>
#include "../GraphNode.h"
#include "../ApTime.h"
#include "RealtimeScript.h""


class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class BellHint : public RealtimeScript {

private:

public:

	//Constructor, here assign all the fields from the private section
	BellHint(GraphNode* nodePointer) : RealtimeScript(nodePointer)
	{

	}

	~BellHint() = default;


	void Update()
	{
		if (ApTime::instance().adviseWindow > 0)
		{
			if ((ApTime::instance().currentPuzzleState == 3 && ApTime::instance().brightWorld) ||
				ApTime::instance().currentPuzzleState == 4 && ApTime::instance().brightWorld ||
				ApTime::instance().currentPuzzleState == 5 && !ApTime::instance().brightWorld ||
				ApTime::instance().currentPuzzleState == 2 && !ApTime::instance().brightWorld ||
				ApTime::instance().currentPuzzleState == 0 && !ApTime::instance().brightWorld
				)
			{
				node->forceHover = true;
			}
		}

	}
};
