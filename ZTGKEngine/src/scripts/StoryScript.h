#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class StoryScript : public RealtimeScript {

private:
	GLFWwindow* window;
	GraphNode* uno;
	GraphNode* segundo;
	GraphNode* tercero;
	GraphNode* cuarto;
	GraphNode* quinto;
	int counter = 1;
	bool keyPressed = false;

public:
	//Constructor, here assign all the fields from the private section
	StoryScript(GraphNode* nodePointer, GLFWwindow* _window,
		GraphNode* _uno,
		GraphNode* _segundo,
		GraphNode* _tercero,
		GraphNode* _cuarto,
		GraphNode* _quinto) : RealtimeScript(nodePointer)
	{
		window = _window;
		uno = _uno;
		segundo = _segundo;
		tercero = _tercero;
		cuarto = _cuarto;
		quinto = _quinto;
	}

	void Update()
	{
		if (!ApTime::instance().isFirstTime)
		{
			uno->SetActive(false);
			segundo->SetActive(false);
			tercero->SetActive(false);
			cuarto->SetActive(false);
			quinto->SetActive(false);

			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
			{
				if (!keyPressed)
				{
					counter++;
					keyPressed = true;
				}
			}
			else
			{
				keyPressed = false;
			}

			switch (counter)
			{
			case 1:
				uno->SetActive(true);
				break;
			case 2:
				segundo->SetActive(true);
				break;
			case 3:
				tercero->SetActive(true);
				break;
			case 4:
				cuarto->SetActive(true);
				break;
			case 5:
				quinto->SetActive(true);
				break;
			default:
				node->SetActive(false);
				break;
			}
		}
	}

	void GreatReset()
	{
		uno->SetActive(false);
		segundo->SetActive(false);
		tercero->SetActive(false);
		cuarto->SetActive(false);
		quinto->SetActive(false);
		counter = 1;
		keyPressed = false;
	}

	~StoryScript() = default;
};