#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"
#include "RealtimeScript.h"
#include "ApTime.h"
#include "OtherTestRealtimeScript.h"
#include "ChessPieceScript.h"

class GraphNode;
class OtherTestRealtimeScript;
class ChessPieceScript;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class ChessBoardPuzzle : public RealtimeScript {

private:
	GraphNode* fields[64];
	int tileState[64];
	ChessPieceScript* pieces[10];

public:
	bool isMovingChess = true;

	//Constructor, here assign all the fields from the private section
	ChessBoardPuzzle(GraphNode* nodePointer, GraphNode* givenFields[64], ChessPieceScript* givenPieces[10]) : RealtimeScript(nodePointer)
	{
		for (int i = 0; i < 64; i++)
		{
			fields[i] = givenFields[i];
		}
		for (int i = 0; i < 10; i++)
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
		for (ChessPieceScript* piece : pieces)
		{
			if (piece->isBeingMoved)
			{
				piece->isBeingMoved = false;
				if (tileState[tileID] == 0)
				{
					piece->GetNode()->setTranslate(new glm::vec3(fields[tileID]->getTranslation().x,
						piece->GetNode()->getTranslation().y - 300.0f, fields[tileID]->getTranslation().z));
					tileState[tileID] = 1;
					if (piece->tileId != 100)
					{
						tileState[piece->tileId] = 0;
					}
					piece->tileId = tileID;
				}
				else
				{
					piece->GetNode()->Translate(glm::vec3(0.0f, -300.0f, 0.0f));
				}
				//HERE YOU CAN CHECK I DEMANDED STATE IS ACHIEVED. IF SO, THEN YOU JUST PUSH THE REQUIRED DRAWER
			}
		}
	}

};
