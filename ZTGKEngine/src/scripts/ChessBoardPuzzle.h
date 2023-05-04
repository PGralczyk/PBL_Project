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

	void Start()
	{
		srand(time(NULL));
		for (ChessPieceScript* piece : pieces)
		{	
			unsigned int index;
			do
			{
				index = rand() % 64;
			} 
			while (tileState[index]);
			tileState[index] = 1;
			piece->goalPosition = glm::vec3(fields[index]->getTranslation().x,
				piece->GetNode()->getTranslation().y, fields[index]->getTranslation().z);
		}
		for (int i = 0; i < 64; i++)
		{
			tileState[i] = 0;
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
					piece->SavePosition();
					tileState[tileID] = 1;
					if (piece->tileId != 100)
					{
						tileState[piece->tileId] = 0;
					}
					piece->tileId = tileID;
					CheckSolution();
				}
				else
				{
					piece->GetNode()->Translate(glm::vec3(0.0f, -300.0f, 0.0f));
				}
				//HERE YOU CAN CHECK I DEMANDED STATE IS ACHIEVED. IF SO, THEN YOU JUST PUSH THE REQUIRED DRAWER
			}
		}
	}

	void CheckSolution()
	{
		unsigned int correctPieces = 0;
		for (ChessPieceScript* piece : pieces)
		{
			if (piece->goalPosition == piece->brightWorldPosition)
			{
				correctPieces++;
			}
		}
		std::cout << correctPieces << std::endl;
	}

	void Update()
	{
		int pickedPieces = 0;
		for (ChessPieceScript* piece : pieces)
		{
			if (piece->isBeingMoved)
			{
				pickedPieces++;
			}
		}
		if (pickedPieces > 1)
		{
			for (ChessPieceScript* piece : pieces)
			{
				if (piece->isBeingMoved)
				{
					piece->isBeingMoved = false;
					piece->GetNode()->Translate(glm::vec3(0.0f, -300.0f, 0.0f));
				}
			}
		}

	}

};
