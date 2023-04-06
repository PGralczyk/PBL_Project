#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"
#include "RealtimeScript.h"
#include "ApTime.h"
#include "OtherTestRealtimeScript.h"

class GraphNode;
class OtherTestRealtimeScript;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class TestRealtimeScript : public RealtimeScript {


public:
	//Constructor, here assign all the fields from the private section
	TestRealtimeScript(GraphNode* nodePointer) : RealtimeScript(nodePointer)
	{
		
	}

	~TestRealtimeScript() = default;

	void OnMouseDragged()
	{
		node->Rotate(180.0f * ApTime::instance().deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	void Start()
	{
		node->Translate(glm::vec3(-4.0f, 0.0f, 0.0f));
	}

	void Update()
	{
		//With current deltaTime implementation it will rotate the object by 90* in one second
		//If you do something within update function, then if you multiply by deltaTime, it will do
		//it in one second, whatever it is
		node->Rotate(90.0f , glm::vec3(1.0f, 0.0f, 0.0f));
		node->Translate(glm::vec3(1.0f, 0.0f, 0.0f) * ApTime::instance().deltaTime);
		//node->Scale(1.0f);
		
		
		this->enabled = false;
		//Uncomment below to get a nice slowdown effect!
		//ApTime::instance().SetTimeRatio(ApTime::instance().GetTimeRatio() * 0.9999f);
	}
};
