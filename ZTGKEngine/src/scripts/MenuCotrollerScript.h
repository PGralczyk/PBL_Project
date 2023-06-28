#pragma once


#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class MenuControllerScript : public RealtimeScript {

private:
	GraphNode* mainMenu;
	GraphNode* options;
	GraphNode* credits;
	GraphNode* firstTime;

public:
	//Constructor, here assign all the fields from the private section
	MenuControllerScript(GraphNode* nodePointer, GraphNode* _mainMenu, GraphNode* _options, GraphNode* _credits, GraphNode* _firstTime) : RealtimeScript(nodePointer)
	{
		mainMenu = _mainMenu;
		options = _options;
		credits = _credits;
		firstTime = _firstTime;
	}

	void Update()
	{
		mainMenu->SetActive(true);

		if (options->GetActive())
			mainMenu->SetActive(false);
		if (credits->GetActive())
			mainMenu->SetActive(false);
		if (firstTime->GetActive())
			mainMenu->SetActive(false);
	}

	~MenuControllerScript() = default;

};