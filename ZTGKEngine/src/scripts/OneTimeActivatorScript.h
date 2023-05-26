#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"
#include "RealtimeScript.h"
#include "ApTime.h"

class GraphNode;

class OneTimeActivatorScript : public RealtimeScript {

private:
	GraphNode* other;
	GraphNode* otherOther;
	bool shouldDisappear, shouldDeactivate;

public:
	//Constructor, here assign all the fields from the private section
	OneTimeActivatorScript(GraphNode* nodePointer, GraphNode* givenOther, bool shouldDisappear = true, bool shouldDeactivate = true, GraphNode* otherGivenOther = NULL) : RealtimeScript(nodePointer)
	{
		other = givenOther;
		otherOther = otherGivenOther;
		this->shouldDeactivate = shouldDeactivate;
		this->shouldDisappear = shouldDisappear;
	}

	~OneTimeActivatorScript() = default;

	void OnMouseClicked()
	{
		other->SetActive(true);
		if (otherOther != NULL)
		{
			otherOther->SetActive(true);
		}
		if (shouldDisappear)
		{
			node->SetActive(false);
		}
		else if (shouldDeactivate)
		{
			enabled = false;
			node->isHoverable = false;
		}
	}

	void Update()
	{
		if (ApTime::instance().adviseWindow > 0)
		{
			node->forceHover = true;
		}
	}
};
