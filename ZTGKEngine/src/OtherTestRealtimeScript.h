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
		std::cout << "Clicked" << std::endl;
		node->Rotate(45.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	}

};
