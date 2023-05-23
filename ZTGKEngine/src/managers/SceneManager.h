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
#include "FadeOut.h"
#include "../UI/ApRectangle.h"
#include "../UI/Text.h"
#include "../scripts/ChessBoardPuzzle.h";
#include "../scripts/TileScript.h";
#include "../scripts/OneTimeActivatorScript.h";
#include "../scripts/InventoryItemScript.h"
#include "../scripts/ChandelierScript.h";
#include "../scripts/GrowPlantScript.h";
#include "../scripts/PlantPuzzleController.h";
#include "../scripts/SingleScaleScript.h";
#include "../scripts/ScalesBalance.h";
#include "../scripts/PlantPuzzleController.h";
#include "../scripts/GrowPlantScript.h";
#include "../scripts/CraneScript.h";

class SceneManager
{
private:
	GraphNode* UI;
	GLFWwindow* window;
	FadeOut* fade;
	bool* isBright;
	int objectId = 1;
	unsigned int* SCR_HEIGHT;
	unsigned int* SCR_WIDTH;

	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	float timeCounter;

	bool engageSwap;

public:
	GraphNode* world;
	GraphNode* Scene1 = new GraphNode();
	GraphNode* Scene2 = new GraphNode();
	Shader *lightShader;
	Shader *defaultShader;
	Shader* textureShader;
	Shader* outlineShader;
	Shader* blurShader;
	Shader* mixShader;
	Shader* fadeShader;

	unsigned int frameBuffers[2];
	unsigned int textureBuffers[2];
	unsigned int rbo;
	bool* singleClick;

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
		engageSwap = false;
		this->SCR_HEIGHT = SCR_HEIGHT;
		this->SCR_WIDTH = SCR_WIDTH;
		isBright = brightReference;
		world = new GraphNode();
		UI = new GraphNode();
		timeCounter = 0.0f;
		fade = new FadeOut("res/models/particle.png", SCR_WIDTH, SCR_HEIGHT, fadeShader);
		Loading("res/models/everest.jpg");
		PostProcessSetup();
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
		
		if (engageSwap) {
			BlurRender(currentlyPicked, timeCounter);
			//count time
			timeCounter += ApTime::instance().deltaTime;
			if (timeCounter > 1.0f)
			{
				engageSwap = false;
				timeCounter = 0.0f;
			}
				
		}
		glDepthFunc(GL_ALWAYS);
		UI->Draw(currentlyPicked);
		glDepthFunc(GL_LESS);
	}

	void Scene1Setup(Shader* additionalShaders[] = nullptr)
	{
		std::cout << "***Scene1***" << std::endl;

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
			Model* _whitePawn = new Model("res/models/pionek_bialy.fbx");
			_whitePawn->SetShader(defaultShader);
			GraphNode* whitePawn = new GraphNode(_whitePawn, objectId++);
			pieces[0] = new ChessPieceScript(whitePawn, window);
			whitePawn->AddScript(pieces[0]);
			whitePawn->Translate(glm::vec3(100.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(whitePawn);
			//****************
			GraphNode* whitePawnActivator = new GraphNode(_whitePawn, objectId++);
			whitePawnActivator->AddScript(new OneTimeActivatorScript(whitePawnActivator, whitePawn));
			whitePawn->SetActive(false);
			whitePawnActivator->Translate(glm::vec3(20.0f, 4.0f, -70.0f));
			whitePawnActivator->Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			whitePawnActivator->Scale(0.015f);;
			//------------------------------------------------------------------------
			std::cout << "LOADING: blackPawn " << std::endl;
			Model* _blackPawn = new Model("res/models/pionek_czarny.fbx");
			_blackPawn->SetShader(defaultShader);
			GraphNode* blackPawn = new GraphNode(_blackPawn, objectId++);
			pieces[1] = new ChessPieceScript(blackPawn, window);
			blackPawn->AddScript(pieces[1]);
			blackPawn->Translate(glm::vec3(300.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(blackPawn);
			//****************
			GraphNode* blackPawnActivator = new GraphNode(_blackPawn, objectId++);
			blackPawnActivator->AddScript(new OneTimeActivatorScript(blackPawnActivator, blackPawn));
			blackPawn->SetActive(false);
			blackPawnActivator->Translate(glm::vec3(14.0f, 4.0f, -64.0f));
			blackPawnActivator->Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			blackPawnActivator->Scale(0.015f);
			//Below is a parent for two pawn activators, so that they can be activated together
			GraphNode* chandelierPrizes = new GraphNode();
			Scene1Bright->AddChild(chandelierPrizes);
			if(ApTime::instance().isEasyMode == false)
				chandelierPrizes->AddChild(blackPawnActivator);
			chandelierPrizes->AddChild(whitePawnActivator);
			chandelierPrizes->SetActive(false);
			chandelierPrizes->Translate(glm::vec3(18.0f, 0.0f, 0.0f));
			//------------------------------------------------------------------------
			std::cout << "LOADING: whiteKing " << std::endl;
			Model* _whiteKing = new Model("res/models/krol_bialy.fbx");
			_whiteKing->SetShader(defaultShader);
			GraphNode* whiteKing = new GraphNode(_whiteKing, objectId++);
			pieces[2] = new ChessPieceScript(whiteKing, window);
			whiteKing->AddScript(pieces[2]);
			whiteKing->Translate(glm::vec3(500.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(whiteKing);
			//****************
			GraphNode* whiteKingActivator = new GraphNode(_whiteKing, objectId++);
			whiteKingActivator->AddScript(new OneTimeActivatorScript(whiteKingActivator, whiteKing));
			whiteKing->SetActive(false);
			whiteKingActivator->Scale(0.015f);
			whiteKingActivator->Translate(glm::vec3(20.0f, 73.0f, -123.0f));
			//------------------------------------------------------------------------
			std::cout << "LOADING: blackKing " << std::endl;
			Model* _blackKing = new Model("res/models/krol_czarny.fbx");
			_blackKing->SetShader(defaultShader);
			GraphNode* blackKing = CreateNode("res/models/krol_czarny.fbx", defaultShader);
			pieces[3] = new ChessPieceScript(blackKing, window);
			blackKing->AddScript(pieces[3]);
			blackKing->Translate(glm::vec3(700.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(blackKing);
			//****************
			GraphNode* blackKingActivator = new GraphNode(_blackKing, objectId++);
			blackKingActivator->AddScript(new OneTimeActivatorScript(blackKingActivator, blackKing));
			blackKing->SetActive(false);
			blackKingActivator->Scale(0.015f);
			blackKingActivator->Translate(glm::vec3(20.0f, 73.0f, -126.0f));
			//Parent for both king activators so that they can be activated at the same time
			GraphNode* plantPuzzlePrizes = new GraphNode();
			plantPuzzlePrizes->AddChild(whiteKingActivator);
			if (ApTime::instance().isEasyMode == false)
				plantPuzzlePrizes->AddChild(blackKingActivator);
			plantPuzzlePrizes->SetActive(false);
			//------------------------------------------------------------------------
			std::cout << "LOADING: whiteQueen " << std::endl;
			Model* _whiteQueen = new Model("res/models/krolowa_biala.fbx");
			_whiteQueen->SetShader(defaultShader);
			GraphNode* whiteQueen = new GraphNode(_whiteQueen, objectId++);
			pieces[4] = new ChessPieceScript(whiteQueen, window);
			whiteQueen->AddScript(pieces[4]);
			whiteQueen->Translate(glm::vec3(900.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(whiteQueen);
			//------------------------------------------------------------------------
			std::cout << "LOADING: blackQueen " << std::endl;
			Model* _blackQueen = new Model("res/models/krolowa_czarna.fbx");
			_blackQueen->SetShader(defaultShader);
			GraphNode* blackQueen = new GraphNode(_blackQueen, objectId++);
			pieces[5] = new ChessPieceScript(blackQueen, window);
			blackQueen->AddScript(pieces[5]);
			blackQueen->Translate(glm::vec3(1100.0f, 100.0f, -230.0f));
			if (ApTime::instance().isEasyMode == false)
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
			if (ApTime::instance().isEasyMode == false)
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
			if (ApTime::instance().isEasyMode == false)
				ChessMainObject->AddChild(blackRook);
		//-----------------------------Creating-Puzzle----------------------------
		ChessBoardPuzzle* puzzle = new ChessBoardPuzzle(ChessMainObject, chessTiles, pieces, window);
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
		ChandelierBright->Translate(glm::vec3(20.0f, -10.0f, -10.0f));

		GraphNode* Chain = CreateNode("res/models/zyrandol_lancuch.fbx", defaultShader);
		Chain->Scale(0.1f);
		ChandelierBright->AddChild(Chain);

		GraphNode* ChandelierGlass = CreateNode("res/models/zyrandol_szklany.fbx", defaultShader);
		ChandelierGlass->Scale(0.1f);
		ChandelierBright->AddChild(ChandelierGlass);

		GraphNode* ChandelierShattered = CreateNode("res/models/zyrandol_stluczony.fbx", defaultShader);
		ChandelierShattered->Scale(0.1f);
		ChandelierShattered->Translate(glm::vec3(20.0f, 0.0f, -10.0f));
		Scene1->AddChild(ChandelierShattered);
		ChandelierShattered->SetActive(false);

		GraphNode* ChandelierNoGlass = CreateNode("res/models/zyrandol_szklany.fbx", defaultShader);
		ChandelierNoGlass->Scale(0.1f);
		ChandelierNoGlass->Translate(glm::vec3(20.0f, -10.0f, -10.0f));
		Scene1Dark->AddChild(ChandelierNoGlass);
		ChandelierNoGlass->AddScript(new ChandelierScript(ChandelierNoGlass, ChandelierGlass, ChandelierShattered, chandelierPrizes));
		objectId--;

		GraphNode* Line = CreateNode("res/models/zyrandol_lina.fbx", defaultShader);
		Line->Scale(0.1f);
		Line->Translate(glm::vec3(20.0f, -10.0f, -10.0f));
		Scene1Dark->AddChild(Line);

		//--------------------------------FLOWERS-AND-THE-REST----------------------------
		int* puzzleState = new int;
		*puzzleState = 0;

		GraphNode* RedFlower = CreateNode("res/models/zagadka_kwiaty/kwiatek_czerwony.fbx", defaultShader);
		RedFlower->Scale(0.1f);
		RedFlower->Translate(glm::vec3(-10.0f, -20.0f, -5.0f));
		Scene1Dark->AddChild(RedFlower);

		GraphNode* RedPot = CreateNode("res/models/zagadka_kwiaty/donica_czerwona.fbx", defaultShader);
		RedPot->Scale(0.1f);
		RedPot->Translate(glm::vec3(-10.0f, 0.0f, -5.0f));
		Scene1Bright->AddChild(RedPot);
		RedPot->AddScript(new GrowPlantScript(RedPot, 3, puzzleState, RedFlower));

		GraphNode* RedStripes = CreateNode("res/models/zagadka_kwiaty/donica_paski_czerwona.fbx", defaultShader);
		RedStripes->Scale(0.1f);
		RedStripes->Translate(glm::vec3(-10.0f, 0.0f, -5.0f));
		Scene1Dark->AddChild(RedStripes);


		GraphNode* BlueFlower = CreateNode("res/models/zagadka_kwiaty/kwiatek_niebieski.fbx", defaultShader);
		BlueFlower->Scale(0.1f);
		BlueFlower->Translate(glm::vec3(45.0f, -20.0f, 0.0f));
		Scene1Dark->AddChild(BlueFlower);

		GraphNode* BluePot = CreateNode("res/models/zagadka_kwiaty/donica_niebieska.fbx", defaultShader);
		BluePot->Scale(0.1f);
		BluePot->Translate(glm::vec3(45.0f, 0.0f, 0.0f));
		Scene1Bright->AddChild(BluePot);
		BluePot->AddScript(new GrowPlantScript(BluePot, 4, puzzleState, BlueFlower));

		GraphNode* BlueStripes = CreateNode("res/models/zagadka_kwiaty/donica_paski_niebieska.fbx", defaultShader);
		BlueStripes->Scale(0.1f);
		BlueStripes->Translate(glm::vec3(45.0f, 0.0f, 0.0f));
		Scene1Dark->AddChild(BlueStripes);


		GraphNode* GreenFlower = CreateNode("res/models/zagadka_kwiaty/kwiatek_zielony.fbx", defaultShader);
		GreenFlower->Scale(0.1f);
		GreenFlower->Translate(glm::vec3(10.0f, -20.0f, -5.0f));
		Scene1Dark->AddChild(GreenFlower);

		GraphNode* GreenPot = CreateNode("res/models/zagadka_kwiaty/donica_zielona.fbx", defaultShader);
		GreenPot->Scale(0.1f);
		GreenPot->Translate(glm::vec3(10.0f, 0.0f, -5.0f));
		Scene1Bright->AddChild(GreenPot);
		GreenPot->AddScript(new GrowPlantScript(GreenPot, 2, puzzleState, GreenFlower));

		GraphNode* GreenStripes = CreateNode("res/models/zagadka_kwiaty/donica_paski_zielona.fbx", defaultShader);
		GreenStripes->Scale(0.1f);
		GreenStripes->Translate(glm::vec3(10.0f, 0.0f, -5.0f));
		Scene1Dark->AddChild(GreenStripes);


		GraphNode* WhiteFlower = CreateNode("res/models/zagadka_kwiaty/kwiatek_bialy.fbx", defaultShader);
		WhiteFlower->Scale(0.1f);
		WhiteFlower->Translate(glm::vec3(10.0f, 0.0f, -5.0f));
		plantPuzzlePrizes->AddChild(WhiteFlower);

		GraphNode* WhitePot = CreateNode("res/models/zagadka_kwiaty/donica_kolorowaa.fbx", defaultShader);
		WhitePot->Scale(0.1f);
		WhitePot->Translate(glm::vec3(10.0f, 0.0f, -5.0f));
		Scene1Dark->AddChild(WhitePot);
		WhitePot->AddScript(new PlantPuzzleController(WhitePot, puzzleState, plantPuzzlePrizes));
		Scene1Dark->AddChild(plantPuzzlePrizes);


		GraphNode* Tap = CreateNode("res/models/zagadka_kwiaty/kranik.fbx", defaultShader);
		Tap->Scale(0.1f);
		Tap->Translate(glm::vec3(10.0f, 0.0f, -5.0f));
		Scene1Dark->AddChild(Tap);

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
		scissors->AddScript(new InventoryItemScript(scissors, "scissoors", window, singleClick));
		scissors->SetActive(false);

		GraphNode* bucket = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/bucket_water.png", textureShader);
		bucket->Scale(0.75);
		bucket->Translate(glm::vec3(70, 40, 0));
		UI->AddChild(bucket);
		bucket->AddScript(new InventoryItemScript(bucket, "bucket", window, singleClick));
		bucket->SetActive(false);

		GraphNode* bucketEmpty = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/bucket_empty.png", textureShader);
		bucketEmpty->Scale(0.75);
		bucketEmpty->Translate(glm::vec3(70, 40, 0));
		UI->AddChild(bucketEmpty);
		bucketEmpty->AddScript(new InventoryItemScript(bucketEmpty, "bucketEmpty", window, singleClick));
		bucketEmpty->SetActive(false);

		Tap->AddScript(new CraneScript(Tap, bucketEmpty, bucket));

		//Scripting for obtaining objects to inventory
		OneTimeActivatorScript* drawer1Script = new OneTimeActivatorScript(drawer1MovableSegment, scissors, false, true);
		drawer1Script->enabled = false;
		drawer1MovableSegment->AddScript(drawer1Script);
		drawer1MovableSegment->isHoverable = false;

		OneTimeActivatorScript* drawer2Script = new OneTimeActivatorScript(drawer2MovableSegment, bucketEmpty, false, true);
		drawer2Script->enabled = false;
		drawer2MovableSegment->AddScript(drawer2Script);
		drawer2MovableSegment->isHoverable = false;

		puzzle->SetPrizes(drawer1Script, drawer2Script);

		GraphNode* door = CreateNode("res/models/drzwi.fbx", defaultShader);
		door->Scale(0.15f);
		door->Rotate(70, glm::vec3(0.0, 1.0, 0.0));
		door->Translate(glm::vec3(40.0, 0.0, 120.0));
		Scene1->AddChild(door);
		door->AddScript(new RoomSwapManager(door, Scene1Bright, Scene1Dark, window, Scene1, Scene2, isBright, &engageSwap));

		//SCALES PUZZLE
		GraphNode* scalesPlantLeft = CreateNode("", defaultShader);
		Scene1Dark->AddChild(scalesPlantLeft);
		GraphNode* scalesPlantRight = CreateNode("", defaultShader);
		Scene1Dark->AddChild(scalesPlantRight);

		GraphNode* ScalesPuzzle = new GraphNode();
		ScalesPuzzle->Scale(0.1);
		ScalesPuzzle->Translate(glm::vec3(50.0f, 0.0f, 0.0f));
		Scene1Bright->AddChild(ScalesPuzzle);

		GraphNode* staticScales = CreateNode("res/models/stojak.fbx", defaultShader);
		ScalesPuzzle->AddChild(staticScales);
		GraphNode* rotatingScales = CreateNode("res/models/bujak.fbx", defaultShader);
		//rotatingScales->Translate(glm::vec3(-40.0f, 0.0f, 0.0f));
		ScalesPuzzle->AddChild(rotatingScales);
		GraphNode* leftScales = CreateNode("res/models/szalka.fbx", defaultShader);
		ScalesPuzzle->AddChild(leftScales);
		GraphNode* rightScales = CreateNode("res/models/szalka.fbx", defaultShader);
		ScalesPuzzle->AddChild(rightScales);
		rightScales->Translate(glm::vec3(0.0f, 0.0f, -200.0f));

		Model* weight = new Model("");
		weight->SetShader(defaultShader);
		GraphNode* weight1l = new GraphNode(weight, objectId++);
		leftScales->AddChild(weight1l);
		GraphNode* weight2l = new GraphNode(weight, objectId++);
		leftScales->AddChild(weight2l);
		GraphNode* weight3l = new GraphNode(weight, objectId++);
		leftScales->AddChild(weight3l);
		GraphNode* weight1r = new GraphNode(weight, objectId++);
		rightScales->AddChild(weight1r);
		GraphNode* weight2r = new GraphNode(weight, objectId++);
		rightScales->AddChild(weight2r);
		GraphNode* weight3r = new GraphNode(weight, objectId++);
		rightScales->AddChild(weight3r);

		GraphNode* weight1 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,"res/models/hopa_u_dzoszuly/wazon_z_roza.png", textureShader);
		weight1->Scale(0.5);
		weight1->Translate(glm::vec3(450, -200, 0));
		UI->AddChild(weight1);
		weight1->AddScript(new InventoryItemScript(weight1, "weight3", window, singleClick));

		GraphNode* weight2 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hopa_u_dzoszuly/wazon_z_roza.png", textureShader);
		weight2->Scale(0.5);
		weight2->Translate(glm::vec3(600, -200, 0));
		UI->AddChild(weight2);
		weight2->AddScript(new InventoryItemScript(weight2, "weight2", window, singleClick));

		GraphNode* weight3 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hopa_u_dzoszuly/wazon_z_roza.png", textureShader);
		weight3->Scale(0.5);
		weight3->Translate(glm::vec3(750, -200, 0));
		UI->AddChild(weight3);
		weight3->AddScript(new InventoryItemScript(weight3, "weight5", window, singleClick));

		GraphNode* scalesTab[9];
		scalesTab[0] = weight1l;
		scalesTab[1] = weight2l;
		scalesTab[2] = weight3l;
		scalesTab[3] = weight1r;
		scalesTab[4] = weight2r;
		scalesTab[5] = weight3r;
		scalesTab[6] = weight1;
		scalesTab[7] = weight2;
		scalesTab[8] = weight3;

		GraphNode* scalesPuzzlePrizes = new GraphNode();

		int* scalesPuzzleController = new int;
		*scalesPuzzleController = 2;
		rotatingScales->AddScript(new ScalesBalance(rotatingScales, scalesPuzzleController, scalesPuzzlePrizes));
		leftScales->AddScript(new SingleScaleScript(leftScales, scalesPuzzleController, true, scalesTab));
		rightScales->AddScript(new SingleScaleScript(rightScales, scalesPuzzleController, false, scalesTab));
	}

	void PostProcessSetup()
	{
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, *SCR_WIDTH, *SCR_HEIGHT);

		//glGenFramebuffers(1, &framebuffer);
		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		//glGenTextures(1, &textureColorbuffer);
		//glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *SCR_WIDTH, *SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

		glGenFramebuffers(2, frameBuffers);
		glGenTextures(2, textureBuffers);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[i]);
			glBindTexture(GL_TEXTURE_2D, textureBuffers[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, *SCR_WIDTH, *SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffers[i], 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
			// also check if framebuffers are complete (no need for depth buffer)
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void renderQuad()
	{
		if (quadVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void BlurRender(unsigned int currentlyPicked, float scale) { // for later when we have a specific object to render

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[0]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[1]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[0]);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		fade->Draw(scale);
		bool horizontal = true;
		unsigned int amount = 10;
		blurShader->use();
		blurShader->setInt("image", 0);
		for (unsigned int i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[horizontal]);
			blurShader->setInt("horizontal", horizontal);
			glBindTexture(GL_TEXTURE_2D, textureBuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
			renderQuad();
			horizontal = !horizontal;
			glClear(GL_DEPTH_BUFFER_BIT);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[horizontal]);
		world->Draw(currentlyPicked);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		mixShader->use();
		mixShader->setInt("scene", 0);
		mixShader->setInt("bloomBlur", 1);
		mixShader->setInt("bloom", true);
		mixShader->setFloat("exposure", 1.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBuffers[horizontal]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureBuffers[!horizontal]);
		renderQuad();
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



