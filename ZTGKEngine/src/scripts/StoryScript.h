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
	GraphNode* bcG;
	GraphNode* uno;
	GraphNode* segundo;
	GraphNode* tercero;
	GraphNode* cuarto;
	GraphNode* quinto;
	GraphNode* sexto;
	GraphNode* septimo;
	int counter = 1;
	bool keyPressed = false;

public:
	//Constructor, here assign all the fields from the private section
	StoryScript(GraphNode* nodePointer, GLFWwindow* _window,
		GraphNode* _bcG,
		GraphNode* _uno,
		GraphNode* _segundo,
		GraphNode* _tercero,
		GraphNode* _cuarto,
		GraphNode* _quinto,
		GraphNode* _sexto,
		GraphNode* _septimo) : RealtimeScript(nodePointer)
	{
		window = _window;
		bcG = _bcG;
		uno = _uno;
		segundo = _segundo;
		tercero = _tercero;
		cuarto = _cuarto;
		quinto = _quinto;
		sexto = _sexto;
		septimo = _septimo;
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
			sexto->SetActive(false);
			septimo->SetActive(false);

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
				bcG->SetActive(true);
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
			case 6:
				sexto->SetActive(true);
				break;
			case 7:
				septimo->SetActive(true);
				break;
			default:
				node->SetActive(false);
				break;
			}

		}
	}

	~StoryScript() = default;
};
