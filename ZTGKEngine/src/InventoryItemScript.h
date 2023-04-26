#pragma once

#include <glm/glm.hpp>
#include "GraphNode.h"
#include "RealtimeScript.h"
#include "ApTime.h"

class GraphNode;
//WHAT IS THIS CLASS?
//A test of implementing realtime script
class InventoryItemScript : public RealtimeScript {

private:
	string elementId = "";
	GLFWwindow* window;	

public:


	InventoryItemScript(GraphNode* nodePointer, string elementId_, GLFWwindow* windowNode) : RealtimeScript(nodePointer)
	{
		this->elementId = elementId_;
		this->window = windowNode;
	}

	void Update()
	{
		if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_RIGHT))
		{
			ApTime::instance().pickedElementId = "";
			node->Scale(0.5);
			//node->setTranslate(&this->startPosition);
		}
		else if (ApTime::instance().pickedElementId != elementId) {
			node->Scale(0.5);
		}
	}

	~InventoryItemScript() = default;

	void OnMouseClicked()
	{
		ApTime::instance().pickedElementId = this->elementId;
		node->Scale(0.6);
	}
};