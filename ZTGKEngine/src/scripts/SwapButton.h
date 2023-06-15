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
	bool* forceSwap;

	//Constructor, here assign all the fields from the private section
	SwapButton(GraphNode* nodePointer, bool* _forceSwap) : RealtimeScript(nodePointer)
	{
		forceSwap = _forceSwap;
	}

	~SwapButton() = default;

	void OnMouseClicked()
	{
		*forceSwap = true;
	}

	void GreatReset()
	{
		*forceSwap = false;
	}
};
