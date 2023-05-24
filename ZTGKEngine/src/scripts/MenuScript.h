#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class MenuScript : public RealtimeScript {

private:
	bool* gameMode;
	string gameModeId;
	bool* choosen;

public:
	//Constructor, here assign all the fields from the private section
	MenuScript(GraphNode* nodePointer, string _gameModeId, bool* _gameMode, bool* _choosen) : RealtimeScript(nodePointer)
	{
		this->gameMode = _gameMode;
		this->gameModeId = _gameModeId;
		this->choosen = _choosen;
	}

	void Update()
	{
		if (choosen)
		{
			node->SetActive(false);
		}
	}

	void OnMouseClicked()
	{
		if (gameModeId == "easy")
		{
			*choosen = true;
			*gameMode = true;
			node->SetActive(false);
		}
		else if (gameModeId == "medium")
		{
			*choosen = true;
			*gameMode = false;
			node->SetActive(false);
		}
	}

	~MenuScript() = default;
};
