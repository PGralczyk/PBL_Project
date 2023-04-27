#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"
#include "RealtimeScript.h"
#include "ApTime.h"

class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class OtherTestRealtimeScript : public RealtimeScript {


public:

	//Constructor, here assign all the fields from the private section
	OtherTestRealtimeScript(GraphNode* nodePointer) : RealtimeScript(nodePointer)
	{

	}

	~OtherTestRealtimeScript() = default;

	void OnMouseClicked()
	{

	}

};
