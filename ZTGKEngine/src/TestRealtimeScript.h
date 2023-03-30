#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"
#include "RealtimeScript.h"

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
		node->Rotate(0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
	}
};
