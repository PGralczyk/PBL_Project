#pragma once

#include <glm/glm.hpp>
#include "../GraphNode.h"
#include "../ApTime.h"
#include "RealtimeScript.h""

//for debug ------------------
#include "../SoundBuffer.h"
#include "../SoundSource.h"
//-----------------------------

class GraphNode;
class OtherTestRealtimeScript;
class ChessPieceScript;
class OneTimeActivatorScript;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class Papers : public RealtimeScript {

private:
	GraphNode* paperBright;
	GraphNode* paperDark;

public:

	//Constructor, here assign all the fields from the private section
	Papers(GraphNode* nodePointer, GraphNode* _paperBright, GraphNode* _paperDark) : RealtimeScript(nodePointer)
	{
		paperBright = _paperBright;
		paperDark = _paperDark;
	}

	~Papers() = default;

	void Update()
	{
		if (ApTime::instance().pickedElementId == "paper")
		{
			paperBright->SetActive(true);
			paperDark->SetActive(true);
		}
		else
		{
			paperBright->SetActive(false);
			paperDark->SetActive(false);
		}
	}

};
