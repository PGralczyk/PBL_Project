#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"
#include "../managers/RoomSwapManager.h"

class SwapButton;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class SwapButton : public RealtimeScript {

public:
	int deactivate = 0;
	RoomSwapManager* manager;

	//Constructor, here assign all the fields from the private section
	SwapButton(GraphNode* nodePointer, RoomSwapManager* _manager) : RealtimeScript(nodePointer)
	{
		manager = _manager;
	}

	~SwapButton() = default;

	void OnMouseClicked()
	{
		manager->SetForceSwap();
	}
};
