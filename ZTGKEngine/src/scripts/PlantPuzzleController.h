#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class PlantPuzzleController : public RealtimeScript {
private:
	int* puzzleState;
	GraphNode* prizes;
public:
	//Constructor, here assign all the fields from the private section
	PlantPuzzleController(GraphNode* nodePointer, int* _puzzleState, GraphNode* _prizes) : RealtimeScript(nodePointer)
	{
		puzzleState = _puzzleState;
		prizes = _prizes;
	}

	~PlantPuzzleController() = default;

	void Update()
	{
		if (*puzzleState == 3)
		{
			node->Translate(glm::vec3(0.0f, 600.0f, 0.0f));
			prizes->SetActive(true);
			enabled = false;
		}
	}

};
