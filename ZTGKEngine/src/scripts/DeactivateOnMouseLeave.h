#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class DeactivateOnMouseLeave;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class DeactivateOnMouseLeave : public RealtimeScript {

public:
	int deactivate = 0;

	//Constructor, here assign all the fields from the private section
	DeactivateOnMouseLeave(GraphNode* nodePointer) : RealtimeScript(nodePointer)
	{

	}

	~DeactivateOnMouseLeave() = default;

	void OnMouseHover()
	{
		deactivate = 0;
	}

	void Update()
	{
		if (deactivate == 2)
		{
			deactivate = 0;
			node->SetActive(false);
		}
		deactivate++;
	}
};
