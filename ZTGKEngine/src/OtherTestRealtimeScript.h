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

	void Update()
	{
		//With current deltaTime implementation it will rotate the object by 90* in one second
		//If you do something within update function, then if you multiply by deltaTime, it will do
		//it in one second, whatever it is
		node->Rotate(45.0f * ApTime::instance().deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
		//Uncomment below to get a nice slowdown effect!
		//ApTime::instance().SetTimeRatio(ApTime::instance().GetTimeRatio() * 0.9999f);
	}
};
