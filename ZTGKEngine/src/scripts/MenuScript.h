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
	GraphNode* piece1;
	GraphNode* piece2;
	GraphNode* piece3;
	GraphNode* piece4;
	GraphNode* piece5;

	SoundSource speaker;

public:
	//Constructor, here assign all the fields from the private section
	MenuScript(GraphNode* nodePointer, GraphNode* _other, GLFWwindow* _window, string _buttonId, 
		GraphNode* _firstTime = new GraphNode(), 
		GraphNode* _piece1 = new GraphNode(),
		GraphNode* _piece2 = new GraphNode(),
		GraphNode* _piece3 = new GraphNode(),
		GraphNode* _piece4 = new GraphNode(),
		GraphNode* _piece5 = new GraphNode()
	) : RealtimeScript(nodePointer)
	{
		this->window = _window;
		this->buttonId = _buttonId;
		this->other = _other;
		this->firstTime = _firstTime;
		this->piece1 = _piece1;
		this->piece2 = _piece2;
		this->piece3 = _piece3;
		this->piece4 = _piece4;
		this->piece5 = _piece5;
	}

	void Update()
	{
		if (ApTime::instance().isFirstTime)
		{
			if (ApTime::instance().isEasyMode)
			{
				this->piece1->SetActive(false);
				this->piece2->SetActive(false);
				this->piece3->SetActive(false);
				this->piece4->SetActive(false);
				this->piece5->SetActive(false);

			}
			else			
			{
				this->piece1->SetActive(true);
				this->piece2->SetActive(true);
				this->piece3->SetActive(true);
				this->piece4->SetActive(true);
				this->piece5->SetActive(true);
			}
		}
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
			if (!ApTime::instance().isFirstTime)
				other->SetActive(false);
			else
				firstTime->SetActive(true);
		}
		else if (buttonId == "exit")
			glfwSetWindowShouldClose(window, true);
		else if (buttonId == "startGame")
		{
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
