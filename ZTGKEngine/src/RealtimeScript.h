#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"

class GraphNode;
//WHAT IS THIS CLASS?
//It's an abstract class that acts as template for every realtime script.
//We can then create a bunch of scripts that inherit from this script and override
//it's functions. The idea is to attach such scripts to the node of desired object.
class RealtimeScript {

protected:
	//Here we want to store all the things that we would want to experiment with.
	//For instance we might want to transform our object, hence the mandatory
	//reference to the graph node.
	GraphNode* node;

public:
	//This bool is used to check if the script is active before calling it's
	//Update() function each frame. It's public because we want to have an
	//easy access to enabling/disabling realtime scripts from other scripts ect.
	bool enabled;

	//Constructor, here assign all the fields from the private section
	RealtimeScript(GraphNode* nodePointer)
	{
		node = nodePointer;
		enabled = true;
	}

	virtual ~RealtimeScript() = default;

	virtual void Start()
	{
		
	}

	virtual void Update()
	{

	}
};
