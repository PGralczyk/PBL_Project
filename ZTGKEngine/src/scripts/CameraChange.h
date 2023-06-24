#pragma once
#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class GraphNode;
class CameraChange : public RealtimeScript
{
private:
	bool* toBeChanged;
	GLFWwindow* window;
public:
	CameraChange(GraphNode* nodePointer, GLFWwindow* _window, bool* _toBeChanged) : RealtimeScript(nodePointer)
	{
		toBeChanged = _toBeChanged;
		window = _window;
	}
	~CameraChange() = default;

	void OnMouseClicked()
	{
		*toBeChanged = true;
	}

	void Update()
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) ||
			(ApTime::instance().isSwitching && &ApTime::instance().isPlantPosition == toBeChanged))
		{
			*toBeChanged = false;
		}

		if (*toBeChanged)
		{
			node->isHoverable = false;
		}
		else
		{
			node->isHoverable = true;
		}
	}
};

