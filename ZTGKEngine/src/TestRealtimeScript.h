#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"
#include "RealtimeScript.h"
#include "ApTime.h"

class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class TestRealtimeScript : public RealtimeScript {


public:
	//Constructor, here assign all the fields from the private section
	TestRealtimeScript(GraphNode* nodePointer) : RealtimeScript(nodePointer)
	{

	}

	~TestRealtimeScript() = default;

	void Start()
	{
		node->Translate(glm::vec3(-4.0f, 0.0f, 0.0f));
	}

	void Update()
	{
		node->Rotate(0.1f * ApTime::instance().deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
		//Uncomment below to get a nice slowdown effect!
		//ApTime::instance().SetTimeRatio(ApTime::instance().GetTimeRatio() * 0.9999f);
	}
};
