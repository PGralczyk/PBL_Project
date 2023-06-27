#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"
#include "../managers/RoomSwapManager.h"

class JournalOnTableScript : public RealtimeScript {
private:
	SoundSource speaker;

public:
	GraphNode* openJournal;

	//Constructor, here assign all the fields from the private section
	JournalOnTableScript(GraphNode* nodePointer, GraphNode* _openJournal) : RealtimeScript(nodePointer)
	{
		openJournal = _openJournal;
	}

	~JournalOnTableScript() = default;

	void OnMouseClicked()
	{
		speaker.Play(SoundBuffer::get()->getSound("bookOpen"));
		ApTime::instance().canUseJournal = true;

		node->SetActive(false);
		openJournal->SetActive(true);
	}

};
