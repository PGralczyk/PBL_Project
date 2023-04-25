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
#include "ChessBoardPuzzle.h";
#include "TileScript.h";
#include "OneTimeActivatorScript.h";

class SceneManager
{
private:
	GraphNode* world;
	GraphNode* UI;
	GLFWwindow* window;
	bool* isBright;
	int objectId = 1;
	unsigned int* SCR_HEIGHT;
	unsigned int* SCR_WIDTH;

public:
	Shader *lightShader;
	Shader *defaultShader;
	Shader* textureShader;

	SceneManager() {};
	~SceneManager()
	{
		delete(world);
		delete(UI);
	}

	void Setup(GLFWwindow* givenWindow, bool *brightReference, unsigned int* SCR_WIDTH, unsigned int* SCR_HEIGHT)
	{
		std::cout << "----------------------------------------------" << std::endl;
		std::cout << "-----------------LOADING-GAME-----------------" << std::endl;
		std::cout << "----------------------------------------------" << std::endl;
		window = givenWindow;
		this->SCR_HEIGHT = SCR_HEIGHT;
		this->SCR_WIDTH = SCR_WIDTH;
		isBright = brightReference;
		world = new GraphNode();
		UI = new GraphNode();
		Scene1Setup();
		UiSetup();
		std::cout << "----------------------------------------------" << std::endl;
		std::cout << "-----------------LOADING-DONE-----------------" << std::endl;
		std::cout << "----------------------------------------------" << std::endl;
	}

	void Update(int currentlyPicked = 0, bool singleMouse = false)
	{
		world->Update(currentlyPicked, singleMouse);
		UI->Update(currentlyPicked, singleMouse);
	}

	void ExecuteStartScripts()
	{
		world->ExecuteStartScripts();
		UI->ExecuteStartScripts();
	}

	void RenderMousePicking(Shader& pickShader, Shader& texturePickShader)
	{
		world->nPickDraw(pickShader);
		glDepthFunc(GL_ALWAYS);
		//glm::mat4 projectionPrimitive = glm::ortho(0.0f, float(*SCR_WIDTH), 0.0f, float(*SCR_HEIGHT));
		//glm::mat4 viewPrimitive = glm::mat4(1.0);
		//pickShader.use();
		//pickShader.setMat4("projection", projectionPrimitive);
		//pickShader.setMat4("view", viewPrimitive);
		UI->nPickDraw(texturePickShader);
		glDepthFunc(GL_LESS);
	}

	void Render()
	{
		world->Draw();
		glDepthFunc(GL_ALWAYS);
		UI->Draw();
		glDepthFunc(GL_LESS);
	}

	void UiSetup()
	{
		//ApRectangle bottomPanel(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/gui_panel.png");
		GraphNode* bottomPanel = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, 
			"res/models/gui_panel.png", textureShader);
		UI->AddChild(bottomPanel);
		bottomPanel->AddScript(new OtherTestRealtimeScript(bottomPanel));

	}

	void Scene1Setup()
	{
		std::cout << "***Scene1***" << std::endl;
		GraphNode* Scene1 = new GraphNode();

		//BRIGHT_WORLD:
		std::cout << "LOADING: scene structure" << std::endl;
		GraphNode* Scene1Bright = new GraphNode();
		GraphNode* bulb =  CreateNode("res/models/House.obj", lightShader);

		//DARK_WORLD:
		GraphNode* Scene1Dark = new GraphNode();
		////CHESS_BOARD_LOADING
		std::cout << "LOADING: chess board" << std::endl;
		GraphNode* ChessMainObject = new GraphNode();
		//GraphNode* drawer1 = CreateNode("path/to/drawer", defaultShader);
		//GraphNode* drawer2 = CreateNode("path/to/drawer", defaultShader);
		//GraphNode* drawer3 = CreateNode("path/to/drawer", defaultShader);
		//GraphNode* chessBody = CreateNode("path/to/body", defaultShader);
		//ChessMainObject->AddChild(drawer1);
		//ChessMainObject->AddChild(drawer2);
		//ChessMainObject->AddChild(drawer3);
		//ChessMainObject->AddChild(chessBody);
		//--------------------------Setting-chess-fields--------------------------
		GraphNode* chessTiles[64];
		int distance = 260;
		string firstTilePath = "res/models/pole_biale.fbx";
		string secondTilePath = "res/models/pole_czarne.fbx";
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				//CHANGE KRZESELKO FOR WHITE AND BLACK BOARD PIECES!!!
				std::cout << "LOADING: chess tile nr. " << 8 * i + 2 * j << std::endl;
				chessTiles[i * 8 + 2 * j] = CreateNode(firstTilePath, defaultShader);
				ChessMainObject->AddChild(chessTiles[i * 8 + 2 * j]);
				chessTiles[i * 8 + 2 * j]->Translate(glm::vec3(distance * 2 * j, 0, distance * i));
				
				std::cout << "LOADING: chess tile nr. " << 8 * i + 2 * j + 1 << std::endl;
				chessTiles[i * 8 + 2 * j + 1] = CreateNode(secondTilePath, defaultShader);
				ChessMainObject->AddChild(chessTiles[i * 8 + 2 * j + 1]);
				chessTiles[i * 8 + 2 * j + 1]->Translate(glm::vec3(distance * 2 * j + distance, 0, distance * i));
			}
			string temp = firstTilePath;
			firstTilePath = secondTilePath;
			secondTilePath = temp;
		}
		//--------------------------Setting-chess-pieces--------------------------
		ChessPieceScript* pieces[10];
		{
			//------------------------------------------------------------------------
			std::cout << "LOADING: whitePawn " << std::endl;
			GraphNode* whitePawn = CreateNode("res/models/pionek_bialy.fbx", defaultShader);
			pieces[0] = new ChessPieceScript(whitePawn);
			whitePawn->AddScript(pieces[0]);
			whitePawn->Translate(glm::vec3(300.0f, 100.0f, -500.0f));
			ChessMainObject->AddChild(whitePawn);
			//****************
			std::cout << "LOADING: whitePawnActivator " << std::endl;
			GraphNode* whitePawnActivator = CreateNode("res/models/pionek_bialy.fbx", defaultShader);
			whitePawnActivator->AddScript(new OneTimeActivatorScript(whitePawnActivator, whitePawn));
			whitePawn->SetActive(false);
			whitePawnActivator->Translate(glm::vec3(300.0f, 1000.0f, -500.0f));
			ChessMainObject->AddChild(whitePawnActivator);
			//------------------------------------------------------------------------
			std::cout << "LOADING: blackPawn " << std::endl;
			GraphNode* blackPawn = CreateNode("res/models/pionek_czarny.fbx", defaultShader);
			pieces[1] = new ChessPieceScript(blackPawn);
			blackPawn->AddScript(pieces[1]);
			blackPawn->Translate(glm::vec3(600.0f, 100.0f, -500.0f));
			ChessMainObject->AddChild(blackPawn);
			//------------------------------------------------------------------------
			std::cout << "LOADING: whiteKing " << std::endl;
			GraphNode* whiteKing = CreateNode("res/models/krol_bialy.fbx", defaultShader);
			pieces[2] = new ChessPieceScript(whiteKing);
			whiteKing->AddScript(pieces[2]);
			whiteKing->Translate(glm::vec3(900.0f, 100.0f, -500.0f));
			ChessMainObject->AddChild(whiteKing);
			//------------------------------------------------------------------------
			std::cout << "LOADING: blackKing " << std::endl;
			GraphNode* blackKing = CreateNode("res/models/krol_czarny.fbx", defaultShader);
			pieces[3] = new ChessPieceScript(blackKing);
			blackKing->AddScript(pieces[3]);
			blackKing->Translate(glm::vec3(1200.0f, 100.0f, -500.0f));
			ChessMainObject->AddChild(blackKing);
			//------------------------------------------------------------------------
			std::cout << "LOADING: whiteQueen " << std::endl;
			GraphNode* whiteQueen = CreateNode("res/models/krolowa_biala.fbx", defaultShader);
			pieces[4] = new ChessPieceScript(whiteQueen);
			whiteQueen->AddScript(pieces[4]);
			whiteQueen->Translate(glm::vec3(1500.0f, 100.0f, -500.0f));
			ChessMainObject->AddChild(whiteQueen);
			//------------------------------------------------------------------------
			std::cout << "LOADING: blackQueen " << std::endl;
			GraphNode* blackQueen = CreateNode("res/models/krolowa_czarna.fbx", defaultShader);
			pieces[5] = new ChessPieceScript(blackQueen);
			blackQueen->AddScript(pieces[5]);
			blackQueen->Translate(glm::vec3(1800.0f, 100.0f, -500.0f));
			ChessMainObject->AddChild(blackQueen);
			//------------------------------------------------------------------------
			std::cout << "LOADING: whiteKnight " << std::endl;
			GraphNode* whiteKnight = CreateNode("res/models/konik_bialy.fbx", defaultShader);
			pieces[6] = new ChessPieceScript(whiteKnight);
			whiteKnight->AddScript(pieces[6]);
			whiteKnight->Translate(glm::vec3(2100.0f, 100.0f, -500.0f));
			ChessMainObject->AddChild(whiteKnight);
			//------------------------------------------------------------------------
			std::cout << "LOADING: blackKnight " << std::endl;
			GraphNode* blackKnight = CreateNode("res/models/konik_czarny.fbx", defaultShader);
			pieces[7] = new ChessPieceScript(blackKnight);
			blackKnight->AddScript(pieces[7]);
			blackKnight->Translate(glm::vec3(2400.0f, 100.0f, -500.0f));
			ChessMainObject->AddChild(blackKnight);
			//------------------------------------------------------------------------
			std::cout << "LOADING: whiteRook " << std::endl;
			GraphNode* whiteRook = CreateNode("res/models/wieza_biala.fbx", defaultShader);
			pieces[8] = new ChessPieceScript(whiteRook);
			whiteRook->AddScript(pieces[8]);
			whiteRook->Translate(glm::vec3(2700.0f, 100.0f, -500.0f));
			ChessMainObject->AddChild(whiteRook);
			//------------------------------------------------------------------------
			std::cout << "LOADING: blackRook " << std::endl;
			GraphNode* blackRook = CreateNode("res/models/wieza_czarna.fbx", defaultShader);
			pieces[9] = new ChessPieceScript(blackRook);
			blackRook->AddScript(pieces[9]);
			blackRook->Translate(glm::vec3(3000.0f, 100.0f, -500.0f));
			ChessMainObject->AddChild(blackRook);
		}
		//-----------------------------Creating-Puzzle----------------------------
		ChessBoardPuzzle* puzzle = new ChessBoardPuzzle(ChessMainObject, chessTiles, pieces);
		ChessMainObject->AddScript(puzzle);
		//-----------------------------Binding-Scripts----------------------------
		for (int i = 0; i < 64; i++)
		{
			chessTiles[i]->AddScript(new TileScript(chessTiles[i], puzzle, i));
		}
		for (int i = 0; i < 10; i++)
		{
			pieces[i]->puzzleReference = puzzle;
		}
		ChessMainObject->Scale(0.015);
		ChessMainObject->Translate(glm::vec3(0.0f, 20.0f, 0.0f));
		std::cout << "LOADING: mainScene1 object " << std::endl;
		GraphNode* SceneBackground = CreateNode("res/models/pokoj_export.fbx", defaultShader);

		//SETTING_INHERITANCE
		world->AddChild(Scene1);
		Scene1->AddChild(Scene1Bright);
		Scene1->AddChild(Scene1Dark);
		world->AddChild(bulb);
		Scene1->AddChild(SceneBackground);
		SceneBackground->Scale(0.1f);

		Scene1Bright->AddChild(ChessMainObject);

		Scene1->Scale(0.005f);
		bulb->Scale(0.01f);
		bulb->Translate(glm::vec3(0.4f, 0.5f, 0.0f));

		//SETTING_SCRIPTS
		Scene1->AddScript(new RoomSwapManager(Scene1, Scene1Bright, Scene1Dark, window, isBright));
	}



private:
	GraphNode* CreateNode(string const& pathToModel, Shader* shader)
	{
		Model *model = new Model(pathToModel, objectId++);
		model->SetShader(shader);
		return new GraphNode(model);
	}

	GraphNode* CreateUiElement(int xPos, int yPos, int width, int height, string path, Shader* shader)
	{
		//glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos, 0));
		glm::mat4 *Transform = new glm::mat4(1.0);
		glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

		/*vector<Vertex> vertices =
		{
			{{ -0.5f, -0.5f, -0.9f}, { 0, 0, 0}, { 0, 0}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
			{{ -0.5f, 0.5f, -0.9f}, { 0, 0, 0}, { 0, 1}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
			{{ 0.5f, 0.5f, -0.9f}, { 0, 0, 0}, { 1, 1}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
			{{0.5, -0.5, -0.9f}, {0, 0, 0}, {1, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} }
		};*/

		vector<Vertex> vertices =
		{
			{{ xPos, yPos, -0.999}, { 0, 0, 0}, { 0, 1}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
			{{ xPos, yPos + height, -0.999}, { 0, 0, 0}, { 0, 0}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
			{{ xPos + width, yPos + height, -0.999}, { 0, 0, 0}, { 1, 0}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
			{{xPos + width, yPos, -0.999}, {0, 0, 0}, {1, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} }
		};

		vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };

		Texture texture;
		texture.id = TextureFromFile(path.c_str());
		texture.isEmbedded = false;
		texture.type = "texture_diffuse";
		vector<Texture> textures;
		textures.push_back(texture);

		Mesh* mesh = new Mesh(vertices, indices, textures);

		Model* model = new Model(mesh, objectId);
		model->SetShader(shader);
		return new GraphNode(model);
	}
};

