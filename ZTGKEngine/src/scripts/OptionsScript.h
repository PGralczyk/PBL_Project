#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

void setGlobalVolume(float v);
float getGlobalVolume();

class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class OptionsScript : public RealtimeScript {

private:
	string buttonId;
	bool isNotHover;
	GraphNode* choosen;

	SoundSource speaker;

public:
	//Constructor, here assign all the fields from the private section
	OptionsScript(GraphNode* nodePointer, string _buttonId, bool _isNotHover = false, GraphNode* _choosen = new GraphNode()) : RealtimeScript(nodePointer)
	{
		this->buttonId = _buttonId;
		this->isNotHover = _isNotHover;
		this->choosen = _choosen;
	}

	void Update()
	{
		if (isNotHover)
		{
			node->isHoverable = true;
			choosen->SetActive(false);

			if (ApTime::instance().isEasyMode)
			{
				if (buttonId == "easy")
				{
					node->isHoverable = false;
					choosen->SetActive(true);
				}
			}
			else
			{
				if (buttonId == "medium")
				{
					node->isHoverable = false;
					choosen->SetActive(true);
				}
			}
			if (buttonId == "leftArr")
			{
				if (getGlobalVolume() < 0.1)
					node->isHoverable = false;
			}
			if (buttonId == "rightArr")
			{
				if (getGlobalVolume() > 0.9)
					node->isHoverable = false;
			}
		}
	}

	void OnMouseClicked()
	{
		if (buttonId == "rightArr")
		{
			setGlobalVolume(getGlobalVolume() + 0.2f);
			if (!isNotHover && getGlobalVolume() > 0.9)
				node->SetActive(false);
		}
		else if (buttonId == "leftArr")
		{
			setGlobalVolume(getGlobalVolume() - 0.2f);
			if (!isNotHover && getGlobalVolume() < 0.1)
				node->SetActive(false);
		}
		else if (buttonId == "easy")
		{
			ApTime::instance().isEasyMode = true;
			if (!isNotHover)
				node->SetActive(false);
		}
		else if (buttonId == "medium")
		{
			ApTime::instance().isEasyMode = false;
			if (!isNotHover)
				node->SetActive(false);
		}

		speaker.Play(SoundBuffer::get()->getSound("click"));
	}

	~OptionsScript() = default;
};
