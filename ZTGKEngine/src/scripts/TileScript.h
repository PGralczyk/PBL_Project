#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"
#include "RealtimeScript.h"
#include "ApTime.h"
#include "ChessBoardPuzzle.h";

class GraphNode;
class OtherTestRealtimeScript;
class ChessBoardPuzzle;

class TileScript : public RealtimeScript {

private:
	int id;
	ChessBoardPuzzle* boardReference;


public:
	//Constructor, here assign all the fields from the private section
	TileScript(GraphNode* nodePointer, ChessBoardPuzzle* givenBoardReference, int ID) : RealtimeScript(nodePointer)
	{
		boardReference = givenBoardReference;
		id = ID;
	}

	~TileScript() = default;

	void OnMouseClicked()
	{
		if (ApTime::instance().isChessPosition)
			boardReference->PlaceChessPiece(id);
		else
			ApTime::instance().isChessPosition = true;
	}

	void Update()
	{
		if (ApTime::instance().currentPuzzleState == 2 && ApTime::instance().adviseWindow > 0 && boardReference->shouldBeEnlightened())
		{
			node->forceHover = true;
		}
	}

};
