#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"
#include "RealtimeScript.h"
#include "ApTime.h"
#include "OtherTestRealtimeScript.h"
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
		if (boardReference->isMovingChess)
		{
			boardReference->PlaceChessPiece(id);
		}
	}
};
