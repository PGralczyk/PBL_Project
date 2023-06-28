#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class HintButton;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class HintButton : public RealtimeScript {
private:
	SoundSource speaker;
	GraphNode* hintTutorial;

public:
	int deactivate = 0;

	//Constructor, here assign all the fields from the private section
	HintButton(GraphNode* nodePointer, GraphNode* _hintTutorial = nullptr) : RealtimeScript(nodePointer)
	{
		hintTutorial = _hintTutorial;
	}

	~HintButton() = default;

	void SetTutorial(GraphNode* tutorial)
	{
		hintTutorial = tutorial;
	}

	void OnMouseClicked()
	{
		if (!(ApTime::instance().isMenuOpen) && !ApTime::instance().isGiantDoor)
		{
			speaker.Play(SoundBuffer::get()->getSound("bell"));
			ApTime::instance().adviseWindow = 3.0f;

			if (hintTutorial) hintTutorial->SetActive(false);
		}
	}
};
