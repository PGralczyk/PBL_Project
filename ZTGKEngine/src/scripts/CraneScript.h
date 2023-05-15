#pragma once

#include <glm/glm.hpp>
#include "../GraphNode.h"
#include "../ApTime.h"
#include "RealtimeScript.h""

class GraphNode;
class OtherTestRealtimeScript;
class ChessPieceScript;
class OneTimeActivatorScript;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class CraneScript : public RealtimeScript {

private:
	GraphNode* toBeDisabled;
	GraphNode* toBeEnabled;

public:

	//Constructor, here assign all the fields from the private section
	CraneScript(GraphNode* nodePointer, GraphNode* _toBeDisabled, GraphNode* _toBeEnabled) : RealtimeScript(nodePointer)
	{
		toBeDisabled = _toBeDisabled;
		toBeEnabled = _toBeEnabled;
	}

	~CraneScript() = default;

	void OnMouseClicked()
	{
		if (ApTime::instance().pickedElementId == "bucketEmpty")
		{
			toBeDisabled->SetActive(false);
			toBeEnabled->SetActive(true);
			enabled = false;
		}
	}


};
