#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"
#include "RealtimeScript.h"
#include "ApTime.h"
#include "OtherTestRealtimeScript.h"
#include "ChessPieceScript.h"
#include "OneTimeActivatorScript.h"

class GraphNode;
class OtherTestRealtimeScript;
class ChessPieceScript;
class OneTimeActivatorScript;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class ChessBoardPuzzle : public RealtimeScript {

private:
	GraphNode* fields[64];
	int tileState[64];
	ChessPieceScript* pieces[10];
	unsigned int phase = 0;
	OneTimeActivatorScript* prize1;
	OneTimeActivatorScript* prize2;
	OneTimeActivatorScript* prize3;
	GLFWwindow* window;
	float solutionMultiplyer = 1;

	SoundSource speaker;

public:

	//Constructor, here assign all the fields from the private section
	ChessBoardPuzzle(GraphNode* nodePointer, GraphNode* givenFields[64], ChessPieceScript* givenPieces[10], GLFWwindow* _window) : RealtimeScript(nodePointer)
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
		window = _window;
	}

	void SetPrizes(OneTimeActivatorScript* givenPrize1, OneTimeActivatorScript* givenPrize2, 
		OneTimeActivatorScript* givenPrize3)
	{
		prize1 = givenPrize1;
		prize2 = givenPrize2;
		prize3 = givenPrize3;
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

		if (ApTime::instance().isEasyMode)
		{
			solutionMultiplyer /= 2;
		}
	}

	~ChessBoardPuzzle() = default;

	void PlaceChessPiece(int tileID)
	{
		for (ChessPieceScript* piece : pieces)
		{
			if (piece->isBeingMoved)
			{
				speaker.Play(SoundBuffer::get()->getSound("placePiece"));

				piece->isBeingMoved = false;
				if (tileState[tileID] == 0)
				{
					piece->GetNode()->setTranslate(glm::vec3(fields[tileID]->getTranslation().x,
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
		if (correctPieces == 6 * solutionMultiplyer && phase < 1)
		{
			phase++;
			ApTime::instance().currentPuzzleState = 3;
			if (prize1 != NULL)
			{
				prize1->GetNode()->Translate(glm::vec3(400.0f, 0.0f, 0.0f));
				prize1->GetNode()->isHoverable = true;
				prize1->enabled = true;
			}
		}
		else if (correctPieces == 8 * solutionMultiplyer && phase < 2)
		{
			phase++;
			ApTime::instance().currentPuzzleState = 4;
			if (prize2 != NULL)
			{
				prize2->GetNode()->Translate(glm::vec3(300.0f, 0.0f, 0.0f));
				prize2->GetNode()->isHoverable = true;
				prize2->enabled = true;
			}
		}
		else if (correctPieces == 10 * solutionMultiplyer && phase < 3)
		{
			phase++;
			ApTime::instance().currentPuzzleState = 6;
			if (prize3 != NULL)
			{
				prize3->GetNode()->Translate(glm::vec3(200.0f, 0.0f, 0.0f));
				prize3->GetNode()->isHoverable = true;
				prize3->enabled = true;
			}
		}
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

			speaker.Play(SoundBuffer::get()->getSound("placePiece"));
		}
		if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_RIGHT))
		{
			for (ChessPieceScript* piece : pieces)
			{
				if (piece->isBeingMoved)
				{
					piece->isBeingMoved = false;
					piece->GetNode()->Translate(glm::vec3(0.0f, -300.0f, 0.0f));
				}
			}
			ApTime::instance().isChessPosition = false; 
		}
	}

	bool shouldBeEnlightened()
	{
		unsigned int availablePieces = 0;
		for (ChessPieceScript* piece : pieces)
		{
			if (piece->GetNode()->GetActive())
			{
				availablePieces++;
			}
		}
		availablePieces -= 3;

		for (ChessPieceScript* piece : pieces)
		{
			if (piece->goalPosition == piece->brightWorldPosition)
			{
				availablePieces--;
			}
		}
		return (availablePieces > 0);
	}

};
