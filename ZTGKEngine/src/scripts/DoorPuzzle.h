#pragma once

#include <glm/glm.hpp>
#include "../GraphNode.h"
#include "../ApTime.h"
#include "RealtimeScript.h""

//for debug ------------------
#include "../SoundBuffer.h"
#include "../SoundSource.h"
#include "../UI/Text.h"
//-----------------------------

class GraphNode;
class OtherTestRealtimeScript;
class ChessPieceScript;
class OneTimeActivatorScript;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class DoorPuzzle : public RealtimeScript {

private:
	GraphNode* puzzle;
	GLFWwindow* window;
	GraphNode* finalScreen;
	string* password;
	string passwordAnswer = "24121834";
	bool* isWon;
	Text* text;
	Shader* textShader;

	SoundSource speaker;
	SoundSource doorSpeaker;

	glm::vec3 initialPosition;

public:

	//Constructor, here assign all the fields from the private section
	DoorPuzzle(GraphNode* nodePointer, GraphNode* _puzzle, GraphNode* _finalScreen, GLFWwindow* _window,
		string* _password, bool* _isWon, Text* _text, Shader* _textShader) : RealtimeScript(nodePointer)
	{
		puzzle = _puzzle;
		window = _window;
		password = _password;
		isWon = _isWon;
		finalScreen = _finalScreen;
		text = _text;
		textShader = _textShader;
	}

	void Start()
	{
		initialPosition = node->getTranslation();
	}

	~DoorPuzzle() = default;

	void OnMouseClicked()
	{
		if (*isWon)
		{
			doorSpeaker.Play(SoundBuffer::get()->getSound("tuptup"));
			finalScreen->SetActive(true);

			ApTime::instance().isBuzzzing = !ApTime::instance().isBuzzzing;
			ApTime::instance().isGiantDoor = true;
		}
		else
		{
			puzzle->SetActive(true);
			ApTime::instance().isGiantDoor = true;
		}
	}

	void Update()
	{
		if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_RIGHT))
		{
			ApTime::instance().isGiantDoor = false;
			puzzle->SetActive(false);
		}
		if (*password == passwordAnswer && !*isWon)
		{
			//ApTime::instance().gameMusic["pianoEmotional"]->Stop();
			ApTime::instance().mainMusicSpeaker->Stop();

			speaker.Play(SoundBuffer::get()->getSound("correctCode"));
			ALint state = AL_PLAYING;
			while (state == AL_PLAYING && (alGetError() == AL_NO_ERROR || alGetError() == AL_INVALID_NAME))
			{
				state = speaker.GetState();
			}
			doorSpeaker.Play(SoundBuffer::get()->getSound("vaultDoor"));
			ApTime::instance().isGiantDoor = false;
			*isWon = true;
			puzzle->SetActive(false);
			node->Rotate(30, glm::vec3(0.0f, 1.0f, 0.0f));
			node->Translate(glm::vec3(-74.0f, 0.0f, 30.0f));

			
			
			while (state == AL_PLAYING && (alGetError() == AL_NO_ERROR || alGetError() == AL_INVALID_NAME))
			{
				state = doorSpeaker.GetState();
			}
			//ApTime::instance().gameMusic["pianoEmotional"]->Replay();
			while (ApTime::instance().mainMusicSpeaker->GetState() == AL_PLAYING && (alGetError() == AL_NO_ERROR || alGetError() == AL_INVALID_NAME))
			{
			}
			ApTime::instance().mainMusicSpeaker->Play(SoundBuffer::get()->getSound("finalMusic"));
		}
		if (ApTime::instance().currentPuzzleState == 7 && ApTime::instance().adviseWindow > 0)
		{
			node->forceHover = true;
		}
	}

	void GreatReset()
	{
		*password = "";
		*isWon = false;
	}
};
