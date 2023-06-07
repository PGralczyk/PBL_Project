#pragma once

#include "Model.h"
#include "ApTime.h"
#include "./scripts/RealtimeScript.h"

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
	float dir;
	float x;
	float y;
	float z;

	unsigned int objectId;

	std::vector<RealtimeScript*> realtimeScripts;

	bool isActive = true;

public:
	std::vector<GraphNode*> children;
	bool isHoverable = true;
	bool forceHover = false;

	GraphNode(Model* m = NULL, unsigned int givenId = 0)
	{
		this->model = m;
		parent = NULL;
		transform = new glm::mat4(1);
		worldTransform = new glm::mat4(1);
		transformOnStart = new glm::mat4(1);
		objectId = givenId;
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
		if (forceHover)
		{
			if (ApTime::instance().adviseWindow <= 0)
			{
				forceHover = false;
			}
		}
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
				if (script->enabled)
					script->Update();
			}

			bool getIds = false;
			if (model && this->realtimeScripts.size() > 0 && currentlyPicked == objectId)
			{
				if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
				{
					if (singleClick)
					{
						for (RealtimeScript* script : realtimeScripts)
						{
							if (script->enabled)
								script->OnMouseClicked();
						}
					}

					for (RealtimeScript* script : realtimeScripts)
					{
						if (script->enabled)
							script->OnMouseDragged();
					}
				}
				if(isHoverable)
				{
					for (RealtimeScript* script : realtimeScripts)
					{
						if (script->enabled)
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
	void Draw(unsigned int currentlyPicked)
	{
		if (isActive)
		{
			if (model) {
				model->setTransform(worldTransform);
				model->Draw(currentlyPicked, objectId, isHoverable, forceHover);
			}

			for (GraphNode* node : children)
			{
				node->Draw(currentlyPicked);
			}
		}
	}

	void choiceDraw(Shader& pickShader, int mode) {
		if (isActive)
		{
			if (model) {
				model->shaderDraw(pickShader, mode);
			}

			for (GraphNode* node : children)
			{
				node->choiceDraw(pickShader, mode);
			}
		}
	}

	void nPickDraw(Shader& pickShader) {
		if (isActive)
		{
			if (model && this->realtimeScripts.size() > 0) {
				model->setTransform(worldTransform);
				model->pickDraw(pickShader, objectId);
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

	void setTranslate(glm::vec3 translate) {
		translation = translate;
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

	unsigned int GetObjectId()
	{
		return this->objectId;
	}

	void GiveAdvice()
	{
		this->forceHover = true;
		ApTime::instance().adviseWindow = 5.0f;
	}
};