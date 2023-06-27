#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"
#include "../Music.h"

class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class MenuScript : public RealtimeScript {

private:
	GLFWwindow* window;
	string buttonId;
	GraphNode* other;
	GraphNode* firstTime;

	SoundSource speaker;

public:
	//Constructor, here assign all the fields from the private section
	MenuScript(GraphNode* nodePointer, GraphNode* _other, GLFWwindow* _window, string _buttonId, 
		GraphNode* _firstTime = new GraphNode()) : RealtimeScript(nodePointer)
	{
		this->window = _window;
		this->buttonId = _buttonId;
		this->other = _other;
		this->firstTime = _firstTime;
	}

	void OnMouseClicked()
	{
		if (buttonId == "goBack")
		{
			other->SetActive(false);
		}
		else if (buttonId == "goNext")
			other->SetActive(true);
		else if (buttonId == "play")
		{
			ApTime::instance().isMenuOpen = false;

			if (!ApTime::instance().isFirstTime)
				other->SetActive(false);
			else
				firstTime->SetActive(true);
		}
		else if (buttonId == "exit")
			glfwSetWindowShouldClose(window, true);
		else if (buttonId == "startGame")
		{
			ApTime::instance().isMenuOpen = false;
			firstTime->SetActive(false);
			other->SetActive(false);
			ApTime::instance().isFirstTime = false;
			ApTime::instance().mainMusicSpeaker->Stop();
			//ApTime::instance().gameMusic["pianoEmotional"]->Play();
			ApTime::instance().mainMusicSpeaker->Play(SoundBuffer::get()->getSound("levelMusic"));
			ApTime::instance().mainAmbientSpeaker->Play(SoundBuffer::get()->getSound("lightBuzz"));
		}

		speaker.Play(SoundBuffer::get()->getSound("click"));
	}

	~MenuScript() = default;
};
