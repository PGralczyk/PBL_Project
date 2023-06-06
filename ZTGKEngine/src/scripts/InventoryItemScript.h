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
	glm::vec3 oryginalPosition;
	bool* singleClick;
	bool shouldMove;

	SoundSource speaker;

public:
	void Start()
	{
		oryginalPosition.x = node->getTranslation().x;
		oryginalPosition.y = node->getTranslation().y;
		oryginalPosition.z = node->getTranslation().z;
	}

	InventoryItemScript(GraphNode* nodePointer, string elementId_, GLFWwindow* windowNode, bool* _singleClick,
		bool _shouldMove = true) : RealtimeScript(nodePointer)
	{
		this->elementId = elementId_;
		this->window = windowNode;
		singleClick = _singleClick;
		shouldMove = _shouldMove;
	}

	void Update()
	{
		if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_RIGHT))
		{
			ApTime::instance().pickedElementId = "";
			node->setTranslate(oryginalPosition);
		}
		else if (ApTime::instance().pickedElementId != elementId) {
			node->setTranslate(oryginalPosition);
		}
		else if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_LEFT) && *singleClick && ApTime::instance().pickedElementId == elementId)
		{
			ApTime::instance().pickedElementId = "";
			node->setTranslate(oryginalPosition);
		}
	}

	~InventoryItemScript() = default;

	void OnMouseClicked()
	{
		if (!(ApTime::instance().pickedElementId == this->elementId))
		{
			ApTime::instance().pickedElementId = this->elementId;
			speaker.Play(SoundBuffer::get()->getSound("selectItem"));
			if(shouldMove)
				node->Translate(glm::vec3(0.0f, 100.0f, 0.0f));
		}
	}

	void OnMouseHover()
	{
		//Do something when hovered over an item
	}
};