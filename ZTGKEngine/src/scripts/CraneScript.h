#pragma once

#include <glm/glm.hpp>
#include "../GraphNode.h"
#include "../ApTime.h"
#include "RealtimeScript.h""

//for debug ------------------
#include "../SoundBuffer.h"
#include "../SoundSource.h"
//-----------------------------

class GraphNode;
class OtherTestRealtimeScript;
class ChessPieceScript;
class OneTimeActivatorScript;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class CraneScript : public RealtimeScript {

private:
	GraphNode* toBeDisabled;
	GraphNode* toBeEnabled;
	//sound testing
	SoundSource* speaker;
	SoundSource testSpeaker;

public:

	//Constructor, here assign all the fields from the private section
	CraneScript(GraphNode* nodePointer, GraphNode* _toBeDisabled, GraphNode* _toBeEnabled, SoundSource* _speaker) : RealtimeScript(nodePointer)
	{
		toBeDisabled = _toBeDisabled;
		toBeEnabled = _toBeEnabled;
		speaker = _speaker;
	}

	~CraneScript() = default;

	void OnMouseClicked()
	{
		// for testing sounds ------------------------------
		testSpeaker.Play(SoundBuffer::get()->getSound("test"));
		//speaker->Play(SoundBuffer::get()->getSound("test"));
		//(*speaker).Play(SoundBuffer::get()->getSound("test"));
		//---------------------------------------------------

		if (ApTime::instance().pickedElementId == "bucketEmpty")
		{
			toBeDisabled->SetActive(false);
			toBeEnabled->SetActive(true);
			enabled = false;
			ApTime::instance().currentPuzzleState = 5;
		}
	}

	void Update()
	{
		if (ApTime::instance().currentPuzzleState == 4 && ApTime::instance().adviseWindow > 0)
		{
			node->forceHover = true;
		}
	}

};
