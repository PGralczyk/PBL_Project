#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class HintButton;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class HintButton : public RealtimeScript {

public:
	int deactivate = 0;

	//Constructor, here assign all the fields from the private section
	HintButton(GraphNode* nodePointer) : RealtimeScript(nodePointer)
	{

	}

	~HintButton() = default;

	void OnMouseClicked()
	{
		ApTime::instance().adviseWindow = 3.0f;
	}
};
