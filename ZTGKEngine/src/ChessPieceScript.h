#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"
#include "RealtimeScript.h"
#include "ApTime.h"
#include "OtherTestRealtimeScript.h"
#include "ChessBoardPuzzle.h"

class GraphNode;
class OtherTestRealtimeScript;
class ChessBoardPuzzle;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class ChessPieceScript : public RealtimeScript {

private:

public:
	bool isBeingMoved = false;
	int tileId = 100;

	//Constructor, here assign all the fields from the private section
	ChessPieceScript(GraphNode* nodePointer) : RealtimeScript(nodePointer)
	{

	}

	~ChessPieceScript() = default;

	void OnMouseClicked()
	{
		if (!isBeingMoved)
		{
			node->Translate(glm::vec3(0.0f, 300.0f, 0.0f));
			isBeingMoved = true;
		}

		if (ApTime::instance().pickedElementId == "rose")
		{
			ApTime::instance().pickedElementId = "";
			std::cout << "Used rose on chess piece\n";
		}

		if (ApTime::instance().pickedElementId == "rose2")
		{
			ApTime::instance().pickedElementId = "";
			std::cout << "Used rose2 on chess piece\n";
		}
	}
};
