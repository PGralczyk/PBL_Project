#pragma once

#include "Model.h"
#include "RealtimeScript.h"

class GraphNode
{
protected:
	GraphNode* parent;
	Model* model;
	glm::mat4* worldTransform;
	glm::mat4* transform;

	glm::mat4 staticTransform;
	glm::vec3 translation = glm::vec3(0.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	float zRotationAngle = 0.0f;
	float staticRotateX = 0.0f;
	float staticRotateZ = 0.0f;
	float m_scale = 1.0f;

	glm::mat4* transformOnStart;
	std::vector<GraphNode*> children;
	float dir;
	float x;
	float y;
	float z;

	std::vector<RealtimeScript*> realtimeScripts;

	bool isActive = true;

public:
	GraphNode(Model* m = NULL)
	{
		this->model = m;
		parent = NULL;
		transform = new glm::mat4(1);
		worldTransform = new glm::mat4(1);
		transformOnStart = new glm::mat4(1);
	}
	~GraphNode(void)
	{
		delete transform;
		delete worldTransform;
		for (unsigned int i = 0; i < children.size(); ++i) {
			delete children[i];
		}
		for (unsigned int i = 0; i < realtimeScripts.size(); ++i) {
			delete realtimeScripts[i];
		}
	}

	glm::mat4* GetTransform() { return transform; }

	glm::mat4* GetWorldTransform() { return worldTransform; }

	Model* GetModel() { return model; }

	void SetModel(Model* m) { model = m; }

	void AddChild(GraphNode* node)
	{
		children.push_back(node);
		node->parent = this;
	}

	void Update(unsigned int currentlyPicked, bool singleClick, GLFWwindow* window)
	{
		if (parent)
		{
			*worldTransform = *parent->worldTransform * (*transform);
		}
		else
		{
			*worldTransform = *transform;
		}
		if (model)
		{
			if (!parent) //Failsafe for being DUMB and not adding root node, may change for the sake of... stuff?
			{
				model->setTransform(transformOnStart);
			}
			else {
				model->setTransform(worldTransform);
			}
		}
		
		if (isActive)
		{
			//With every call to Update in node we also execute updates in scripts
			for (RealtimeScript* script : realtimeScripts)
			{
				script->Update();
			}

			bool getIds = false;
			if (model && currentlyPicked == model->objectID)
			{
				if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
				{
					if (singleClick)
					{
						for (RealtimeScript* script : realtimeScripts)
						{
							script->OnMouseClicked();
						}
					}

					for (RealtimeScript* script : realtimeScripts)
					{
						script->OnMouseDragged();
					}
				}
				else
				{
					for (RealtimeScript* script : realtimeScripts)
					{
						script->OnMouseHover();
					}
				}
			}


			for (GraphNode* node : children)
			{
				node->Update(currentlyPicked, singleClick, window);
			}
		}
	}
	void Draw()
	{
		if (isActive)
		{
			if (model) {
				model->Draw();
			}

			for (GraphNode* node : children)
			{
				node->Draw();
			}
		}
	}

	void nPickDraw(Shader& pickShader) {
		if (isActive)
		{
			if (model) {
				model->pickDraw(pickShader);
			}

			for (GraphNode* node : children)
			{
				node->nPickDraw(pickShader);
			}
		}
	}

	void Rotate(float angle, glm::vec3 axis) {
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angle), axis);
		RenderTransform();
	}

	void RenderTransform() {
		*transform = glm::translate(glm::mat4(1.0f), translation) *
			rotationMatrix *
			glm::scale(glm::mat4(1.0f), glm::vec3(m_scale));
	}

	void setStaticRotateZ(float staticAngle) {
		staticRotateZ = staticAngle;
	}

	void setStaticRotateX(float staticAngle) {
		staticRotateX = staticAngle;
	}


	void Translate(glm::vec3 translate) {
		translation = translation + translate;
		RenderTransform();
	}

	void setTranslate(glm::vec3* translate) {
		translation = *translate;
		RenderTransform();
	}

	glm::vec3 getTranslation() {
		return translation;
	}

	void Scale(float scale) {
		m_scale = scale;
		RenderTransform();
	}

	//We add scripts to object represented by this node(it's added only to this particular node)
	void AddScript(RealtimeScript* script)
	{
		realtimeScripts.push_back(script);
	}

	//We execute start functions in all scripts(should be done right after finishing adding scripts)
	void ExecuteStartScripts()
	{
		for (unsigned int i = 0; i < realtimeScripts.size(); ++i) {
			realtimeScripts[i]->Start();
		}
		//We execute those start functions in children as well
		for (GraphNode* node : children)
		{
			node->ExecuteStartScripts();
		}
	}

	void SetActive(bool active)
	{
		isActive = active;
	}

	bool GetActive()
	{
		return isActive;
	}
};