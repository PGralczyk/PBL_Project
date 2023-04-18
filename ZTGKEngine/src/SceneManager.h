#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image/stb_image.h>
#include <assimp/Importer.hpp>

#include "Shader.h"
#include "Model.h"
#include "GraphNode.h"
#include "Camera.h"
#include "TestRealtimeScript.h"
#include "OtherTestRealtimeScript.h"
#include "ApTime.h"
#include "RoomSwapManager.h"
#include "ClickPicker.h"
#include "ApRectangle.h"
#include "Text.h"

class SceneManager
{
private:
	GraphNode* world;
	GLFWwindow* window;
	bool* isBright;
	int objectId = 1;

public:
	Shader *lightShader;
	Shader *defaultShader;

	SceneManager() {};
	~SceneManager()
	{
		delete(world);
	}

	void Setup(GLFWwindow* givenWindow, bool *brightReference)
	{
		window = givenWindow;
		isBright = brightReference;
		world = new GraphNode();
		Scene1Setup();
	}

	void Update(int currentlyPicked = 0, bool singleMouse = false)
	{
		world->Update(currentlyPicked, singleMouse);
	}

	void ExecuteStartScripts()
	{
		world->ExecuteStartScripts();
	}

	void RenderMousePicking(Shader& pickShader)
	{
		world->nPickDraw(pickShader);
	}

	void Render()
	{
		world->Draw();
	}

	void Scene1Setup()
	{
		GraphNode* Scene1 = new GraphNode();

		//BRIGHT_WORLD:
		GraphNode* Scene1Bright = new GraphNode();
		GraphNode* bulb =  CreateNode("res/models/House.obj", lightShader);
		GraphNode* chair1 = CreateNode("res/models/krzeselko.fbx", defaultShader);

		//DARK_WORLD:
		GraphNode* Scene1Dark = new GraphNode();
		GraphNode* chair3 = CreateNode("res/models/krzeselko.fbx", defaultShader);

		//SETTING_INHERITANCE
		world->AddChild(Scene1);
		Scene1->AddChild(Scene1Bright);
		Scene1->AddChild(Scene1Dark);
		Scene1->AddChild(bulb);

		Scene1Bright->AddChild(chair1);
		Scene1Dark->AddChild(chair3);

		Scene1Bright->Scale(0.005f);
		Scene1Dark->Scale(0.005f);
		bulb->Scale(0.1f);

		//SETTING_SCRIPTS
		chair1->AddScript(new TestRealtimeScript(chair1));
		chair3->AddScript(new OtherTestRealtimeScript(chair3));
		Scene1->AddScript(new RoomSwapManager(Scene1, Scene1Bright, Scene1Dark, window, isBright));
	}



private:
	GraphNode* CreateNode(string const& pathToModel, Shader* shader)
	{
		Model *model = new Model(pathToModel, objectId++);
		model->SetShader(shader);
		return new GraphNode(model);
	}
};

