#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"
#include "RealtimeScript.h"
#include "ApTime.h"
#include "OtherTestRealtimeScript.h"
#include "ChessBoardPuzzle.h";

class GraphNode;
class OtherTestRealtimeScript;
class ChessBoardPuzzle;

class OneTimeActivatorScript : public RealtimeScript {

private:
	GraphNode* other;

public:
	//Constructor, here assign all the fields from the private section
	OneTimeActivatorScript(GraphNode* nodePointer, GraphNode* givenOther) : RealtimeScript(nodePointer)
	{
		other = givenOther;
	}

	~OneTimeActivatorScript() = default;

	void OnMouseClicked()
	{
		other->SetActive(true);
		node->SetActive(false);
	}
};
