#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class ReturnInfo : public RealtimeScript {
private:
	GraphNode* tutorial;
public:

	//Constructor, here assign all the fields from the private section
	ReturnInfo(GraphNode* nodePointer, GraphNode* _tutorial) : RealtimeScript(nodePointer)
	{
		tutorial = _tutorial;
	}

	~ReturnInfo() = default;
	void Update()
	{
		if (ApTime::instance().isBookPosition || ApTime::instance().isChessPosition || (ApTime::instance().isDeskPosition
			&& ApTime::instance().canSwap))
		{
			tutorial->SetActive(true);
		}
		else
		{
			tutorial->SetActive(false);
		}
	}
};