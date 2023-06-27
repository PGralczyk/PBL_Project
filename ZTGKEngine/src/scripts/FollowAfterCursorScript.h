#pragma once

#include <glm/glm.hpp>
#include "RealtimeScript.h"
#include "../GraphNode.h"
#include "../ApTime.h"

class FollowAfterCursorScript : public RealtimeScript {
private:
	double mouseX = 0.0;
	double mouseY = 0.0;
	unsigned int* SCR_WIDTH;
	unsigned int* SCR_HEIGHT;
	double aspectX, aspectY;
	GLFWwindow* window;
	
public:
	//Constructor, here assign all the fields from the private section
	FollowAfterCursorScript(GraphNode* nodePointer, GLFWwindow* _window, unsigned int* _SCR_WIDTH, unsigned int* _SCR_HEIGHT ) : RealtimeScript(nodePointer)
	{
		window = _window;
		SCR_WIDTH = _SCR_WIDTH;
		SCR_HEIGHT = _SCR_HEIGHT;
	}

	~FollowAfterCursorScript() = default;

	void Update()
	{
		mouseX = ApTime::instance().withinWindowCursorPosX;
		mouseY = ApTime::instance().withinWindowCursorPosY;

		aspectX = float(*SCR_WIDTH) / 1920.0f;
		aspectY = float(*SCR_HEIGHT) / 1009.0f;
		node->setTranslate(glm::vec3(mouseX - 148 * aspectX, -mouseY + 924 * aspectY, node->getTranslation().z));
	}
};
