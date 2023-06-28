#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"
#include "../managers/RoomSwapManager.h"

class JournalScript;
class JournalScript : public RealtimeScript {
private:
	SoundSource speaker;

public:
	GraphNode* object;

	//Constructor, here assign all the fields from the private section
	JournalScript(GraphNode* nodePointer, GraphNode* _object) : RealtimeScript(nodePointer)
	{
		object = _object;
	}

	~JournalScript() = default;

	void OnMouseClicked()
	{
		if (!(ApTime::instance().isMenuOpen))
		{
			speaker.Play(SoundBuffer::get()->getSound("bookOpen"));
			object->SetActive(true);
		}
	}
};
