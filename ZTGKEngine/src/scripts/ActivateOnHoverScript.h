#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class ActivateOnHoverScript;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class ActivateOnHoverScript : public RealtimeScript {
private:
	GraphNode* other;
	bool shouldWork;

public:
	//Constructor, here assign all the fields from the private section
	ActivateOnHoverScript(GraphNode* nodePointer, GraphNode* _other, bool _shouldWork = true) : RealtimeScript(nodePointer)
	{
		other = _other;
		shouldWork = _shouldWork;
	}

	~ActivateOnHoverScript() = default;

	void OnMouseHover()
	{
		if(shouldWork)
			other->SetActive(true);
	}
};
