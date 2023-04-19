#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"
#include "RealtimeScript.h"
#include "ApTime.h"
#include "OtherTestRealtimeScript.h"

class GraphNode;
class OtherTestRealtimeScript;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class ChessBoardPuzzle : public RealtimeScript {

private:
	GraphNode* pieces[64];
	int tileState[64];

public:
	bool isMovingChess = true;

	//Constructor, here assign all the fields from the private section
	ChessBoardPuzzle(GraphNode* nodePointer, GraphNode* givenPieces[64]) : RealtimeScript(nodePointer)
	{
		for (int i = 0; i < 64; i++)
		{
			pieces[i] = givenPieces[i];
		}
		for (int tile : tileState)
		{
			tile = 0;
		}
		
	}

	~ChessBoardPuzzle() = default;

	void PlaceChessPiece(int tileID)
	{
		std::cout << "PickedTile: " << tileID << std::endl;
	}


};
