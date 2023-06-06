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
class DoorButton : public RealtimeScript {

private:
	string* password;
	string passwordAnswer = "24161834";
	bool* isWon;
	string number;

	SoundSource speaker;

public:

	//Constructor, here assign all the fields from the private section
	DoorButton(GraphNode* nodePointer, string* _password, bool* _isWon, string _number) : RealtimeScript(nodePointer)
	{
		password = _password;
		isWon = _isWon;
		number = _number;
	}

	~DoorButton() = default;

	void OnMouseClicked()
	{
		if(!*isWon)
		{
			speaker.Play(SoundBuffer::get()->getSound("typeCode"));

			if (password->length() >= 8)
			{
				*password = "";
			}
			else
			{
				password->append(number);
			}
		}
	}

	void Update()
	{
		if (*isWon)
		{
			enabled = false;
		}
	}
};
