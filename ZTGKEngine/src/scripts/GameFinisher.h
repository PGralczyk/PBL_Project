#pragma once

#include <glm/glm.hpp>
#include "../GraphNode.h"
#include "../ApTime.h"
#include "RealtimeScript.h""


class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class GameFinisher : public RealtimeScript {

private:
	GraphNode* tutorial;
	GraphNode* thankYouForPlaying;
	GLFWwindow* window;
	bool pressed = false;

public:

	//Constructor, here assign all the fields from the private section
	GameFinisher(GraphNode* nodePointer, GraphNode* _tutorial, GraphNode* _thankYouForPlaying, GLFWwindow* _window) : RealtimeScript(nodePointer)
	{
		tutorial = _tutorial;
		window = _window;
		thankYouForPlaying = _thankYouForPlaying;
		node->isHoverable = false;
	}

	~GameFinisher() = default;


	void Update()
	{
		tutorial->SetActive(true);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
		{
			pressed = true;
		}
		else if (pressed)
		{
			thankYouForPlaying->SetActive(true);
			ApTime::instance().forceReset = true;
			node->SetActive(false);
		}
	}

	void GreatReset()
	{
		pressed = false;
	}
};
