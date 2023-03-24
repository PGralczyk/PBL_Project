#pragma once

#include "Model.h"

class GraphNode
{
protected:
	GraphNode* parent;
	Model* model;
	glm::mat4* worldTransform;
	glm::mat4* transform;

	glm::mat4 staticTransform;
	glm::vec3 translation = glm::vec3(0.0f);
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

	void Update()
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
		for (GraphNode* node : children)
		{
			node->Update();
		}
	}
	void Draw()
	{
		if (model) { 
			model->Draw(); 
		}

		for (GraphNode* node : children)
		{
			node->Draw();
		}
	}

	void Rotate(float angle, glm::vec3 axis) {
		*transform = glm::rotate(*transform, glm::radians(angle), axis);
	}

	void RenderTransform() {
		*transform = glm::translate(glm::mat4(1.0f), translation) *
			glm::rotate(glm::mat4(1.0f), glm::radians(staticRotateX), glm::vec3(-1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(staticRotateZ), glm::vec3(0, -1, 0)) *
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

	void Scale(float scale) {
		m_scale = scale;
		RenderTransform();
	}
};