#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

float getGlobalVolume();

class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class ShowMusicVolume : public RealtimeScript {

private:
	GraphNode* volume1;
	GraphNode* volume2;
	GraphNode* volume3;
	GraphNode* volume4;
	GraphNode* volume5;
	GraphNode* volume6;

public:
	//Constructor, here assign all the fields from the private section
	ShowMusicVolume(GraphNode* nodePointer, 
		GraphNode* _volume1,
		GraphNode* _volume2,
		GraphNode* _volume3,
		GraphNode* _volume4,
		GraphNode* _volume5,
		GraphNode* _volume6
	) : RealtimeScript(nodePointer)
	{
		this->volume1 = _volume1;
		this->volume2 = _volume2;
		this->volume3 = _volume3;
		this->volume4 = _volume4;
		this->volume5 = _volume5;
		this->volume6 = _volume6;
	}

	void Start()
	{
		node->isHoverable = false;
	}

	void Update()
	{
		volume1->SetActive(false);
		volume2->SetActive(false);
		volume3->SetActive(false);
		volume4->SetActive(false);
		volume5->SetActive(false);
		volume6->SetActive(false);

		if (getGlobalVolume() < 0.1f)
			volume1->SetActive(true);
		else if (getGlobalVolume() < 0.3f)
			volume2->SetActive(true);
		else if (getGlobalVolume() < 0.5f)
			volume3->SetActive(true);
		else if (getGlobalVolume() < 0.7f)
			volume4->SetActive(true);
		else if (getGlobalVolume() < 0.9f)
			volume5->SetActive(true);
		else if (getGlobalVolume() == 1)
			volume6->SetActive(true);
	}

	~ShowMusicVolume() = default;
};
