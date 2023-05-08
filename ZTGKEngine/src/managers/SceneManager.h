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
#include "../scripts/TestRealtimeScript.h"
#include "../scripts/OtherTestRealtimeScript.h"
#include "ApTime.h"
#include "RoomSwapManager.h"
#include "ClickPicker.h"
#include "../UI/ApRectangle.h"
#include "../UI/Text.h"
#include "../scripts/ChessBoardPuzzle.h";
#include "../scripts/TileScript.h";
#include "../scripts/OneTimeActivatorScript.h";
#include "../scripts/InventoryItemScript.h"
#include "../scripts/ChandelierScript.h";

class SceneManager
{
private:
	GraphNode* UI;
	GLFWwindow* window;
	bool* isBright;
	int objectId = 1;
	unsigned int* SCR_HEIGHT;
	unsigned int* SCR_WIDTH;

	unsigned int framebuffer;
	unsigned int textureColorbuffer;
	unsigned int rbo;

public:
	GraphNode* world;
	Shader *lightShader;
	Shader *defaultShader;
	Shader* textureShader;
	Shader* outlineShader;

	SceneManager() {};
	~SceneManager()
	{
		delete(world);
		delete(UI);
	}

	void Setup(GLFWwindow* givenWindow, bool *brightReference, unsigned int* SCR_WIDTH, unsigned int* SCR_HEIGHT, Shader * otherShaders ...)
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
		Loading("res/models/everest.jpg");
		Scene1Setup(&otherShaders);
		std::cout << "----------------------------------------------" << std::endl;
		std::cout << "-----------------LOADING-DONE-----------------" << std::endl;
		std::cout << "----------------------------------------------" << std::endl;
		ExecuteStartScripts();
	}

	void Update(int currentlyPicked, bool singleMouse, bool isHoldingMouseButton)
	{
		world->Update(currentlyPicked, singleMouse, window);
		UI->Update(currentlyPicked, singleMouse, window);
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

	void RenderWithShader(Shader& choiceShader, int mode) {
		world->choiceDraw(choiceShader, mode);
	}

	void Render(unsigned int currentlyPicked)
	{
		world->Draw(currentlyPicked);
		glDepthFunc(GL_ALWAYS);
		UI->Draw(currentlyPicked);
		glDepthFunc(GL_LESS);
	}

	void Scene1Setup(Shader* additionalShaders[] = nullptr)
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
		GraphNode* DrawerMainObject = new GraphNode();
		ChessMainObject->AddChild(DrawerMainObject);
		std::cout << "LOADING: chess board base" << std::endl;
		GraphNode* ChessBoardBottom = CreateNode("res/models/szachownica_spod.fbx", defaultShader);
		DrawerMainObject->AddChild(ChessBoardBottom);
		std::cout << "LOADING: chess drawer" << std::endl;
		Model* drawerBase = new Model("res/models/szuflada.fbx");
		std::cout << "LOADING: chess drawer frame" << std::endl;
		Model* drawerFrame = new Model("res/models/szuflada_obudowa.fbx");
		drawerBase->SetShader(defaultShader);
		drawerFrame->SetShader(defaultShader);

		GraphNode* drawer1 = new GraphNode(drawerFrame);
		GraphNode* drawer1MovableSegment = new GraphNode(drawerBase, objectId++);
		drawer1->AddChild(drawer1MovableSegment);

		GraphNode* drawer2 = new GraphNode(drawerFrame);
		GraphNode* drawer2MovableSegment = new GraphNode(drawerBase, objectId++);
		drawer2->AddChild(drawer2MovableSegment);

		GraphNode* drawer3 = new GraphNode(drawerFrame);
		GraphNode* drawer3MovableSegment = new GraphNode(drawerBase, objectId++);
		drawer3->AddChild(drawer3MovableSegment);

		DrawerMainObject->AddChild(drawer1);
		DrawerMainObject->AddChild(drawer2);
		DrawerMainObject->AddChild(drawer3);
		DrawerMainObject->Scale(3.0);
		DrawerMainObject->Translate(glm::vec3(900, -600, 800));
		drawer2->Translate(glm::vec3(0, -240, 0));
		drawer3->Translate(glm::vec3(0, -480, 0));
		ChessMainObject->Translate(glm::vec3(0, 20, 0));
		

		//--------------------------Setting-chess-fields--------------------------
		GraphNode* ChessTiles = new GraphNode();
		ChessMainObject->AddChild(ChessTiles);
		GraphNode* chessTiles[64];
		int distance = 260;
		std::cout << "LOADING: white chess tile" << std::endl;
		Model* model_white = new Model("res/models/pole_biale.fbx");
		std::cout << "LOADING: black chess tile" << std::endl;
		Model* model_black = new Model("res/models/pole_czarne.fbx");
		model_white->SetShader(defaultShader);
		model_black->SetShader(defaultShader);
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (i % 2 == 0)
					chessTiles[i * 8 + 2 * j] = new GraphNode(model_white, objectId++);
				else
					chessTiles[i * 8 + 2 * j] = new GraphNode(model_black, objectId++);
				ChessTiles->AddChild(chessTiles[i * 8 + 2 * j]);
				chessTiles[i * 8 + 2 * j]->Translate(glm::vec3(distance * 2 * j, 0, distance * i));
				
				if (i % 2 == 0)
					chessTiles[i * 8 + 2 * j + 1] = new GraphNode(model_black, objectId++);
				else
					chessTiles[i * 8 + 2 * j + 1] = new GraphNode(model_white, objectId++);
				ChessTiles->AddChild(chessTiles[i * 8 + 2 * j + 1]);
				chessTiles[i * 8 + 2 * j + 1]->Translate(glm::vec3(distance * 2 * j + distance, 0, distance * i));
			}
		}
		//--------------------------Setting-chess-pieces--------------------------
		ChessPieceScript* pieces[10];
			//------------------------------------------------------------------------
			std::cout << "LOADING: whitePawn " << std::endl;
			GraphNode* whitePawn = CreateNode("res/models/pionek_bialy.fbx", defaultShader);
			pieces[0] = new ChessPieceScript(whitePawn, window);
			whitePawn->AddScript(pieces[0]);
			whitePawn->Translate(glm::vec3(100.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(whitePawn);
			//****************
			std::cout << "LOADING: whitePawnActivator " << std::endl;
			GraphNode* whitePawnActivator = CreateNode("res/models/pionek_bialy.fbx", defaultShader);
			whitePawnActivator->AddScript(new OneTimeActivatorScript(whitePawnActivator, whitePawn));
			whitePawn->SetActive(false);
			whitePawnActivator->Translate(glm::vec3(20.0f, 4.0f, -70.0f));
			whitePawnActivator->Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			whitePawnActivator->Scale(0.015f);
			Scene1Bright->AddChild(whitePawnActivator);
			whitePawnActivator->SetActive(false);
			//------------------------------------------------------------------------
			std::cout << "LOADING: blackPawn " << std::endl;
			GraphNode* blackPawn = CreateNode("res/models/pionek_czarny.fbx", defaultShader);
			pieces[1] = new ChessPieceScript(blackPawn, window);
			blackPawn->AddScript(pieces[1]);
			blackPawn->Translate(glm::vec3(300.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(blackPawn);
			//------------------------------------------------------------------------
			std::cout << "LOADING: whiteKing " << std::endl;
			GraphNode* whiteKing = CreateNode("res/models/krol_bialy.fbx", defaultShader);
			pieces[2] = new ChessPieceScript(whiteKing, window);
			whiteKing->AddScript(pieces[2]);
			whiteKing->Translate(glm::vec3(500.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(whiteKing);
			//------------------------------------------------------------------------
			std::cout << "LOADING: blackKing " << std::endl;
			GraphNode* blackKing = CreateNode("res/models/krol_czarny.fbx", defaultShader);
			pieces[3] = new ChessPieceScript(blackKing, window);
			blackKing->AddScript(pieces[3]);
			blackKing->Translate(glm::vec3(700.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(blackKing);
			//------------------------------------------------------------------------
			std::cout << "LOADING: whiteQueen " << std::endl;
			GraphNode* whiteQueen = CreateNode("res/models/krolowa_biala.fbx", defaultShader);
			pieces[4] = new ChessPieceScript(whiteQueen, window);
			whiteQueen->AddScript(pieces[4]);
			whiteQueen->Translate(glm::vec3(900.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(whiteQueen);
			//------------------------------------------------------------------------
			std::cout << "LOADING: blackQueen " << std::endl;
			GraphNode* blackQueen = CreateNode("res/models/krolowa_czarna.fbx", defaultShader);
			pieces[5] = new ChessPieceScript(blackQueen, window);
			blackQueen->AddScript(pieces[5]);
			blackQueen->Translate(glm::vec3(1100.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(blackQueen);
			//------------------------------------------------------------------------
			std::cout << "LOADING: whiteKnight " << std::endl;
			GraphNode* whiteKnight = CreateNode("res/models/konik_bialy.fbx", defaultShader);
			pieces[6] = new ChessPieceScript(whiteKnight, window);
			whiteKnight->AddScript(pieces[6]);
			whiteKnight->Translate(glm::vec3(1300.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(whiteKnight);
			//------------------------------------------------------------------------
			std::cout << "LOADING: blackKnight " << std::endl;
			GraphNode* blackKnight = CreateNode("res/models/konik_czarny.fbx", defaultShader);
			pieces[7] = new ChessPieceScript(blackKnight, window);
			blackKnight->AddScript(pieces[7]);
			blackKnight->Translate(glm::vec3(1500.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(blackKnight);
			//------------------------------------------------------------------------
			std::cout << "LOADING: whiteRook " << std::endl;
			GraphNode* whiteRook = CreateNode("res/models/wieza_biala.fbx", defaultShader);
			pieces[8] = new ChessPieceScript(whiteRook, window);
			whiteRook->AddScript(pieces[8]);
			whiteRook->Translate(glm::vec3(1700.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(whiteRook);
			//------------------------------------------------------------------------
			std::cout << "LOADING: blackRook " << std::endl;
			GraphNode* blackRook = CreateNode("res/models/wieza_czarna.fbx", defaultShader);
			pieces[9] = new ChessPieceScript(blackRook, window);
			blackRook->AddScript(pieces[9]);
			blackRook->Translate(glm::vec3(1900.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(blackRook);
		//-----------------------------Creating-Puzzle----------------------------
		ChessBoardPuzzle* puzzle = new ChessBoardPuzzle(ChessMainObject, chessTiles, pieces);
		ChessMainObject->AddScript(puzzle);
		//-----------------------------Binding-Scripts----------------------------
		for (int i = 0; i < 64; i++)
		{
			chessTiles[i]->AddScript(new TileScript(chessTiles[i], puzzle, i));
		}
		ChessMainObject->Scale(0.015);
		ChessMainObject->Translate(glm::vec3(0.0f, 20.0f, 0.0f));
		std::cout << "LOADING: mainScene1 object " << std::endl;
		GraphNode* SceneBackground = CreateNode("res/models/pokoj_export.fbx", defaultShader);
		GraphNode* SceneOutsideBright = CreateUiElement(0,0,200,150, "res/models/bright_forest.png", additionalShaders[0]);
		GraphNode* SceneOutsideDark = CreateUiElement(0, 0, 200, 150, "res/models/dark_forest.png", additionalShaders[0]);

		//SETTING_INHERITANCE
		world->AddChild(Scene1);
		Scene1->AddChild(Scene1Bright);
		Scene1->AddChild(Scene1Dark);
		Scene1Bright->AddChild(SceneOutsideBright);
		Scene1Dark->AddChild(SceneOutsideDark);
		world->AddChild(bulb);
		Scene1->AddChild(SceneBackground);
		SceneBackground->Scale(0.1f);

		Scene1->AddChild(ChessMainObject);

		Scene1->Scale(0.005f);
		bulb->Scale(0.01f);
		bulb->Translate(glm::vec3(0.4f, 0.5f, 0.0f));
		SceneOutsideBright->Translate(glm::vec3(-100.0f, 55.0f, 136.0f));
		SceneOutsideBright->Scale(0.35f);
		//SceneOutsideBright->Rotate(glm::vec3(0.4f, 0.5f, 0.0f));

		SceneOutsideDark->Translate(glm::vec3(-100.0f, 55.0f, 136.0f));
		SceneOutsideDark->Scale(0.35f);
		//SceneOutsideDark->Rotate(glm::vec3(0.4f, 0.5f, 0.0f));


		//--------------------------Chandelier-Puzzle--------------------------
		GraphNode* ChandelierBright = new GraphNode();
		Scene1Bright->AddChild(ChandelierBright);
		ChandelierBright->Translate(glm::vec3(0.0f, -10.0f, -40.0f));

		GraphNode* Chain = CreateNode("res/models/zyrandol_lancuch.fbx", defaultShader);
		Chain->Scale(0.1f);
		ChandelierBright->AddChild(Chain);

		GraphNode* ChandelierGlass = CreateNode("res/models/zyrandol_szklany.fbx", defaultShader);
		ChandelierGlass->Scale(0.1f);
		ChandelierBright->AddChild(ChandelierGlass);

		GraphNode* ChandelierShattered = CreateNode("res/models/zyrandol_stluczony.fbx", defaultShader);
		ChandelierShattered->Scale(0.1f);
		ChandelierShattered->Translate(glm::vec3(0.0f, 0.0f, -40.0f));
		Scene1->AddChild(ChandelierShattered);
		ChandelierShattered->SetActive(false);

		GraphNode* ChandelierNoGlass = CreateNode("res/models/zyrandol_szklany.fbx", defaultShader);
		ChandelierNoGlass->Scale(0.1f);
		ChandelierNoGlass->Translate(glm::vec3(0.0f, -10.0f, -40.0f));
		Scene1Dark->AddChild(ChandelierNoGlass);
		ChandelierNoGlass->AddScript(new ChandelierScript(ChandelierNoGlass, ChandelierGlass, ChandelierShattered, whitePawnActivator));
		objectId--;

		GraphNode* Line = CreateNode("res/models/zyrandol_lina.fbx", defaultShader);
		Line->Scale(0.1f);
		Line->Translate(glm::vec3(0.0f, -10.0f, -40.0f));
		Scene1Dark->AddChild(Line);

		Scene1->AddScript(new RoomSwapManager(Scene1, Scene1Bright, Scene1Dark, window, isBright));


		//--------------------------------UI-AND-INVENTORY--------------------------------
		GraphNode* bottomPanel = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/gui_panel.png", textureShader);
		UI->AddChild(bottomPanel);
		bottomPanel->AddScript(new OtherTestRealtimeScript(bottomPanel));

		GraphNode* scissors = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/nozyce.png", textureShader);
		UI->AddChild(scissors);
		scissors->Scale(0.75);
		scissors->Translate(glm::vec3(-50, -250, 0));
		scissors->AddScript(new InventoryItemScript(scissors, "scissoors", window));
		scissors->SetActive(false);

		GraphNode* rose2 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hopa_u_dzoszuly/wazon_z_roza.png", textureShader);
		rose2->Scale(0.5);
		rose2->Translate(glm::vec3(300, -200, 0));
		UI->AddChild(rose2);
		rose2->AddScript(new InventoryItemScript(rose2, "rose2", window));

		//Scripting for obtaining objects to inventory
		OneTimeActivatorScript* drawer1Script = new OneTimeActivatorScript(drawer1MovableSegment, scissors, false, true);
		drawer1Script->enabled = false;
		drawer1MovableSegment->AddScript(drawer1Script);
		drawer1MovableSegment->isHoverable = false;
		puzzle->SetPrizes(drawer1Script);
	}

	void PostProcessSetup()
	{
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *SCR_WIDTH, *SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, *SCR_WIDTH, *SCR_HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void EnableFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	}

	void DisableFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

private:
	GraphNode* CreateNode(string const& pathToModel, Shader* shader)
	{
		Model *model = new Model(pathToModel);
		model->SetShader(shader);
		return new GraphNode(model, objectId++);
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

		Model* model = new Model(mesh);
		model->SetShader(shader);
		return new GraphNode(model, objectId++);
	}

	void Loading(std::string path)
	{
		//std::cout << "Showing load screen...\n";
		glm::mat4 projectionPrimitive = glm::ortho(0.0f, float(*SCR_WIDTH), 0.0f, float(*SCR_HEIGHT));
		glm::mat4 viewPrimitive = glm::mat4(1.0);
		textureShader->use();
		textureShader->setMat4("projection", projectionPrimitive);
		textureShader->setMat4("view", viewPrimitive);
		ApRectangle loadScreen(0, 0, *SCR_WIDTH, *SCR_HEIGHT, path);
		loadScreen.SetShader(textureShader);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_ALWAYS);
		loadScreen.Draw();
		glDisable(GL_BLEND);
		glfwSwapBuffers(window);
		glDepthFunc(GL_LESS);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//std::cout << "Showing scene...\n";
	}
};



