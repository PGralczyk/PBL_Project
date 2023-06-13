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
#include "../scripts/MenuScript.h";
#include "../scripts/MenuButtonScript.h";
#include "../scripts/ActivateOnHoverScript.h";
#include "../scripts/DeactivateOnMouseLeave.h";
#include "../scripts/HintButton.h";
#include "../scripts/SwapButton.h";
#include "../scripts/Papers.h";
#include "../scripts/DoorPuzzle.h";
#include "../scripts/DoorButton.h";
#include "../scripts/JournalScript.h";
#include "../scripts/StartingDesk.h";
#include "../scripts/OptionsScript.h";
#include "../scripts/ShowMusicVolume.h";
#include "../scripts/CameraChange.h";
#include "../scripts/ReturnInfo.h";

class SceneManager
{
private:
	GraphNode* UI;
	GraphNode* UIBright = new GraphNode();
	GraphNode* UIDark = new GraphNode();
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
	bool phase;
	bool poof; //phase change peak checker
	bool forceSwap = false;
	string* password = new string;
	bool isFirstBuffer = true;

public:
	GraphNode* world;
	GraphNode* Scene1 = new GraphNode();
	GraphNode* Scene2 = new GraphNode();
	GraphNode* menu = new GraphNode();
	Shader* lightShader;
	Shader* defaultShader;
	Shader* textureShader;
	Shader* outlineShader;
	Shader* blurShader;
	Shader* mixShader;
	Shader* fadeShader;
	Shader* brightShader;
	Shader* bloomMixShader;
	Shader* textShader;
	Text* text;
	GraphNode* DoorPuzzleObject;


	unsigned int frameBuffers[2];
	unsigned int textureBuffers[2];
	unsigned int intermediateFBuffer;
	unsigned int intermediateTBuffer;
	unsigned int rbo;
	bool* singleClick;

	GraphNode* gameModePieces[5];

	SceneManager() {};
	~SceneManager()
	{
		delete(world);
		delete(UI);
	}

	void MenuSetup()
	{
		//menu->SetActive(false);
		GraphNode* credits = new GraphNode();
		GraphNode* options = new GraphNode();
		GraphNode* firstTimeGameMode = new GraphNode();

		GraphNode* backg = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/menu/menu_bg.png", textureShader);
		menu->AddChild(backg);

		GraphNode* playBtn = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/menu/menu_play_button.png", textureShader);
		GraphNode* playHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/menu/menu_play_button_hover.png", textureShader);
		menu->AddChild(playBtn);
		menu->AddChild(playHover);
		playHover->SetActive(false);
		playBtn->AddScript(new ActivateOnHoverScript(playBtn, playHover));
		playHover->AddScript(new DeactivateOnMouseLeave(playHover));
		playHover->AddScript(new MenuScript(playHover, menu, window, "play", 
			firstTimeGameMode,
			gameModePieces[0],
			gameModePieces[1],
			gameModePieces[2],
			gameModePieces[3],
			gameModePieces[4]
		));

		GraphNode* creditsBtn = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/menu/menu_credits_button.png", textureShader);
		GraphNode* creditsHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/menu/menu_credits_button_hover.png", textureShader);
		menu->AddChild(creditsBtn);
		menu->AddChild(creditsHover);
		creditsHover->SetActive(false);
		creditsBtn->AddScript(new ActivateOnHoverScript(creditsBtn, creditsHover));
		creditsHover->AddScript(new DeactivateOnMouseLeave(creditsHover));
		creditsHover->AddScript(new MenuScript(creditsHover, credits, window, "goNext"));

		GraphNode* optionsBtn = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/menu/menu_options_button.png", textureShader);
		GraphNode* optionsHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/menu/menu_options_button_hover.png", textureShader);
		menu->AddChild(optionsBtn);
		menu->AddChild(optionsHover);
		optionsHover->SetActive(false);
		optionsBtn->AddScript(new ActivateOnHoverScript(optionsBtn, optionsHover));
		optionsHover->AddScript(new DeactivateOnMouseLeave(optionsHover));
		optionsHover->AddScript(new MenuScript(optionsHover, options, window, "goNext"));

		GraphNode* exitBtn = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/menu/menu_exit_button.png", textureShader);
		GraphNode* exitHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/menu/menu_exit_button_hover.png", textureShader);
		menu->AddChild(exitBtn);
		menu->AddChild(exitHover);
		exitHover->SetActive(false);
		exitBtn->AddScript(new ActivateOnHoverScript(exitBtn, exitHover));
		exitHover->AddScript(new DeactivateOnMouseLeave(exitHover));
		exitHover->AddScript(new MenuScript(exitHover, menu, window, "exit"));

#pragma region Credits
		menu->AddChild(credits);
		credits->SetActive(false);

		GraphNode* creditsPNG = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/awesome.jpg", textureShader);
		credits->AddChild(creditsPNG);

		GraphNode* backCredBtn = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/menu/menu_exit_button.png", textureShader);
		GraphNode* backCredHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/menu/menu_exit_button_hover.png", textureShader);
		backCredBtn->Translate(glm::vec3(1200, 0, 0));
		backCredHover->Translate(glm::vec3(1200, 0, 0));
		credits->AddChild(backCredBtn);
		credits->AddChild(backCredHover);
		backCredHover->SetActive(false);
		backCredBtn->AddScript(new ActivateOnHoverScript(backCredBtn, backCredHover));
		backCredHover->AddScript(new DeactivateOnMouseLeave(backCredHover));
		backCredHover->AddScript(new MenuScript(backCredHover, credits, window, "goBack"));

#pragma endregion 

#pragma region Options
		menu->AddChild(options);
		options->SetActive(false);

		GraphNode* optionsBG = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/options_sounds.png", textureShader);
		options->AddChild(optionsBG);

		GraphNode* backOptBtn = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/menu/menu_exit_button.png", textureShader);
		GraphNode* backOptHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/menu/menu_exit_button_hover.png", textureShader);
		options->AddChild(backOptBtn);
		options->AddChild(backOptHover);
		backOptHover->SetActive(false);
		backOptBtn->AddScript(new ActivateOnHoverScript(backOptBtn, backOptHover));
		backOptHover->AddScript(new DeactivateOnMouseLeave(backOptHover));
		backOptHover->AddScript(new MenuScript(backOptHover, options, window, "goBack"));

		GraphNode* leftArrBtn = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/sound_arrow_leftV2.png", textureShader);
		GraphNode* leftArrHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/sound_arrow_left_hoverV2.png", textureShader);
		options->AddChild(leftArrBtn);
		options->AddChild(leftArrHover);
		leftArrHover->SetActive(false);
		leftArrBtn->AddScript(new ActivateOnHoverScript(leftArrBtn, leftArrHover));
		leftArrHover->AddScript(new DeactivateOnMouseLeave(leftArrHover));
		leftArrBtn->AddScript(new OptionsScript(leftArrBtn, "leftArr", true));
		leftArrHover->AddScript(new OptionsScript(leftArrHover, "leftArr"));

		GraphNode* rightArrBtn = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/sound_arrow_rightV2.png", textureShader);
		GraphNode* rightArrHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/sound_arrow_right_hoverV2.png", textureShader);
		options->AddChild(rightArrBtn);
		options->AddChild(rightArrHover);
		rightArrHover->SetActive(false);
		rightArrBtn->AddScript(new ActivateOnHoverScript(rightArrBtn, rightArrHover));
		rightArrHover->AddScript(new DeactivateOnMouseLeave(rightArrHover));
		rightArrBtn->AddScript(new OptionsScript(rightArrBtn, "rightArr", true));
		rightArrHover->AddScript(new OptionsScript(rightArrHover, "rightArr"));

		GraphNode* dificultyLabel = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/difficulty.png", textureShader);
		options->AddChild(dificultyLabel);

		GraphNode* easyModeBtn = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/easyBtn.png", textureShader);
		GraphNode* easyModeHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/easyHover.png", textureShader);
		//easyModeBtn->Translate(glm::vec3(0, 400, 0));
		//easyModeHover->Translate(glm::vec3(0, 400, 0));
		options->AddChild(easyModeBtn);
		options->AddChild(easyModeHover);
		easyModeHover->SetActive(false);
		easyModeBtn->AddScript(new ActivateOnHoverScript(easyModeBtn, easyModeHover));
		easyModeHover->AddScript(new DeactivateOnMouseLeave(easyModeHover));
		easyModeBtn->AddScript(new OptionsScript(easyModeBtn, "easy", true));
		easyModeHover->AddScript(new OptionsScript(easyModeHover, "easy"));

		GraphNode* mediumModeBtn = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/mediumBtn.png", textureShader);
		GraphNode* mediumModeHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/mediumHover.png", textureShader);
		//mediumModeBtn->Translate(glm::vec3(400, 400, 0));
		//mediumModeHover->Translate(glm::vec3(400, 400, 0));
		options->AddChild(mediumModeBtn);
		options->AddChild(mediumModeHover);
		mediumModeHover->SetActive(false);
		mediumModeBtn->AddScript(new ActivateOnHoverScript(mediumModeBtn, mediumModeHover));
		mediumModeHover->AddScript(new DeactivateOnMouseLeave(mediumModeHover));
		mediumModeBtn->AddScript(new OptionsScript(mediumModeBtn, "medium", true));
		mediumModeHover->AddScript(new OptionsScript(mediumModeHover, "medium"));

#pragma endregion 		

#pragma region Music_Bars

		GraphNode* volume1 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/sound_bar_1_off.jpg", textureShader);
		options->AddChild(volume1);

		GraphNode* volume2 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/sound_bar_2_off.jpg", textureShader);
		options->AddChild(volume2);

		GraphNode* volume3 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/sound_bar_3_off.jpg", textureShader);
		options->AddChild(volume3);

		GraphNode* volume4 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/sound_bar_4_off.jpg", textureShader);
		options->AddChild(volume4);

		GraphNode* volume5 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/sound_bar_5_off.jpg", textureShader);
		options->AddChild(volume5);

		GraphNode* volume6 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/sound_bar_6_off.jpg", textureShader);
		options->AddChild(volume6);

		optionsBG->isHoverable = false;
		optionsBG->AddScript(new ShowMusicVolume(optionsBG,
			volume1,
			volume2,
			volume3,
			volume4,
			volume5,
			volume6
		));

#pragma endregion 

#pragma region First Time Choose Game Mode
		menu->AddChild(firstTimeGameMode);
		firstTimeGameMode->SetActive(false);

		GraphNode* windowPNG = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/everest.png", textureShader);
		firstTimeGameMode->AddChild(windowPNG);

		GraphNode* infoPNG = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/difficulty.png", textureShader);
		firstTimeGameMode->AddChild(infoPNG);
		infoPNG->Translate(glm::vec3(1000, 200, 0));

		GraphNode* backGmBtn = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/menu/menu_exit_button.png", textureShader);
		GraphNode* backGmHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/menu/menu_exit_button_hover.png", textureShader);
		backGmBtn->Translate(glm::vec3(1000, 0, 0));
		backGmHover->Translate(glm::vec3(1000, 0, 0));
		firstTimeGameMode->AddChild(backGmBtn);
		firstTimeGameMode->AddChild(backGmHover);
		backGmHover->SetActive(false);
		backGmBtn->AddScript(new ActivateOnHoverScript(backGmBtn, backGmHover));
		backGmHover->AddScript(new DeactivateOnMouseLeave(backGmHover));
		backGmHover->AddScript(new MenuScript(backGmHover, menu, window, "startGame",
			firstTimeGameMode,
			gameModePieces[0],
			gameModePieces[1],
			gameModePieces[2],
			gameModePieces[3],
			gameModePieces[4]
		));

		GraphNode* easyGmBtn = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/easyBtn.png", textureShader);
		GraphNode* easyGmHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/easyHover.png", textureShader);
		easyGmBtn->Translate(glm::vec3(1000, 200, 0));
		easyGmHover->Translate(glm::vec3(1000, 200, 0));
		firstTimeGameMode->AddChild(easyGmBtn);
		firstTimeGameMode->AddChild(easyGmHover);
		easyGmHover->SetActive(false);
		easyGmBtn->AddScript(new ActivateOnHoverScript(easyGmBtn, easyGmHover));
		easyGmHover->AddScript(new DeactivateOnMouseLeave(easyGmHover));
		easyGmBtn->AddScript(new OptionsScript(easyGmBtn, "easy", true));
		easyGmHover->AddScript(new OptionsScript(easyGmHover, "easy"));

		GraphNode* mediumGmBtn = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/mediumBtn.png", textureShader);
		GraphNode* mediumGmHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/optionsy/mediumHover.png", textureShader);
		mediumGmBtn->Translate(glm::vec3(1000, 200, 0));
		mediumGmHover->Translate(glm::vec3(1000, 200, 0));
		firstTimeGameMode->AddChild(mediumGmBtn);
		firstTimeGameMode->AddChild(mediumGmHover);
		mediumGmHover->SetActive(false);
		mediumGmBtn->AddScript(new ActivateOnHoverScript(mediumGmBtn, mediumGmHover));
		mediumGmHover->AddScript(new DeactivateOnMouseLeave(mediumGmHover));
		mediumGmBtn->AddScript(new OptionsScript(mediumGmBtn, "medium", true));
		mediumGmHover->AddScript(new OptionsScript(mediumGmHover, "medium"));

#pragma endregion 

	}

	void Setup(GLFWwindow* givenWindow, bool* brightReference, unsigned int* SCR_WIDTH, unsigned int* SCR_HEIGHT, Shader* otherShaders ...)
	{
		window = givenWindow;
		engageSwap = false;
		this->SCR_HEIGHT = SCR_HEIGHT;
		this->SCR_WIDTH = SCR_WIDTH;
		isBright = brightReference;
		world = new GraphNode();
		UI = new GraphNode();
		UI->AddChild(UIBright);
		UI->AddChild(UIDark);
		UIDark->SetActive(false);
		timeCounter = 0.0f;
		phase = true;
		poof = false;
		fade = new FadeOut("res/models/particle.png", SCR_WIDTH, SCR_HEIGHT, fadeShader);
		Loading("res/models/everest.png");
		PostProcessSetup();
		Scene1Setup(&otherShaders);
		Scene2Setup(&otherShaders);
		this->MenuSetup();
		UI->AddChild(menu);
		ExecuteStartScripts();
	}

	void Update(int currentlyPicked, bool singleMouse, bool isHoldingMouseButton)
	{
		world->Update(currentlyPicked, singleMouse, window);
		UI->Update(currentlyPicked, singleMouse, window);
		if (poof)
			poof = false;
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
		//world->Draw(currentlyPicked);


		if (engageSwap) {
			BlurRender(currentlyPicked, timeCounter);
			//count time

			if (timeCounter >= 0.5f) {
				phase = false;
				timeCounter = 0.5f;
				poof = true;
			}
			if (phase)
				timeCounter += ApTime::instance().deltaTime;
			else
				timeCounter -= ApTime::instance().deltaTime;

			if (timeCounter <= 0.0f)
			{
				engageSwap = false;
				timeCounter = 0.0f;
				phase = true;
			}
		}
		else {
			BloomRender(currentlyPicked, 0);
			glDepthFunc(GL_ALWAYS);
			UI->Draw(currentlyPicked);
			glDepthFunc(GL_LESS);
		}
		//For door puzzle(Don't mind it)
		if (DoorPuzzleObject->GetActive())
		{
			text->RenderText(*textShader, *password, 968, 770, 2, glm::vec3(1.0f, 0.0f, 0.0f));
		}
	}

	void Scene1Setup(Shader* additionalShaders[] = nullptr)
	{
		//BRIGHT_WORLD:
		GraphNode* Scene1Bright = new GraphNode();
		//DARK_WORLD:
		GraphNode* Scene1Dark = new GraphNode();
		//BULB MARKER
		GraphNode* bulb = CreateNode("res/models/House.obj", lightShader);

#pragma region Chess Puzzle

#pragma region Chess Main Object
		GraphNode* ChessMainObject = new GraphNode();
		GraphNode* DrawerMainObject = new GraphNode();
		ChessMainObject->AddChild(DrawerMainObject);
		GraphNode* ChessBoardBottom = CreateNode("res/models/szachownica_spod.fbx", defaultShader);
		DrawerMainObject->AddChild(ChessBoardBottom);
		Model* drawerBase = new Model("res/models/szuflada.fbx");
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
#pragma endregion

#pragma region Chess Fields
		//--------------------------Setting-chess-fields--------------------------
		GraphNode* ChessTiles = new GraphNode();
		ChessMainObject->AddChild(ChessTiles);
		GraphNode* chessTiles[64];
		int distance = 260;
		Model* model_white = new Model("res/models/pole_biale.fbx");
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
#pragma endregion

#pragma region Chess Pieces
		//--------------------------Setting-chess-pieces--------------------------
		ChessPieceScript* pieces[10];

			//------------------------------------------------------------------------
			Model* _whitePawn = new Model("res/models/pionek_bialy.fbx");
			_whitePawn->SetShader(defaultShader);
			GraphNode* whitePawn = new GraphNode(_whitePawn, objectId++);
			pieces[0] = new ChessPieceScript(whitePawn, window, &poof);
			whitePawn->AddScript(pieces[0]);
			whitePawn->Translate(glm::vec3(100.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(whitePawn);

			//****************
			GraphNode* whitePawnActivator = new GraphNode(_whitePawn, objectId++);
			whitePawnActivator->AddScript(new OneTimeActivatorScript(whitePawnActivator, whitePawn));
			whitePawn->SetActive(false);
			whitePawnActivator->Translate(glm::vec3(20.0f, 4.0f, -70.0f));
			whitePawnActivator->Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			whitePawnActivator->Scale(0.015f);

			//------------------------------------------------------------------------
			Model* _blackPawn = new Model("res/models/pionek_czarny.fbx");
			_blackPawn->SetShader(defaultShader);
			GraphNode* blackPawn = new GraphNode(_blackPawn, objectId++);
			pieces[1] = new ChessPieceScript(blackPawn, window, &poof);
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
			gameModePieces[0] = blackPawnActivator;
			chandelierPrizes->AddChild(blackPawnActivator);
			chandelierPrizes->AddChild(whitePawnActivator);
			chandelierPrizes->SetActive(false);
			chandelierPrizes->Translate(glm::vec3(18.0f, 0.0f, 40.0f));
			whitePawnActivator->Scale(0.02f);
			blackPawnActivator->Scale(0.02f);

			//------------------------------------------------------------------------
			Model* _whiteKing = new Model("res/models/krol_bialy.fbx");
			_whiteKing->SetShader(defaultShader);
			GraphNode* whiteKing = new GraphNode(_whiteKing, objectId++);
			pieces[2] = new ChessPieceScript(whiteKing, window, &poof);
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
			Model* _blackKing = new Model("res/models/krol_czarny.fbx");
			_blackKing->SetShader(defaultShader);
			GraphNode* blackKing = CreateNode("res/models/krol_czarny.fbx", defaultShader);
			pieces[3] = new ChessPieceScript(blackKing, window, &poof);
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
			gameModePieces[1] = blackKingActivator;
			plantPuzzlePrizes->AddChild(blackKingActivator);
			plantPuzzlePrizes->SetActive(false);

			//------------------------------------------------------------------------
			Model* _whiteQueen = new Model("res/models/krolowa_biala.fbx");
			_whiteQueen->SetShader(defaultShader);
			GraphNode* whiteQueen = new GraphNode(_whiteQueen, objectId++);
			pieces[4] = new ChessPieceScript(whiteQueen, window, &poof);
			whiteQueen->AddScript(pieces[4]);
			whiteQueen->Translate(glm::vec3(900.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(whiteQueen);

			//------------------------------------------------------------------------
			Model* _blackQueen = new Model("res/models/krolowa_czarna.fbx");
			_blackQueen->SetShader(defaultShader);
			GraphNode* blackQueen = new GraphNode(_blackQueen, objectId++);
			pieces[5] = new ChessPieceScript(blackQueen, window, &poof);
			blackQueen->AddScript(pieces[5]);
			blackQueen->Translate(glm::vec3(1100.0f, 100.0f, -230.0f));
			gameModePieces[2] = blackQueen;
			ChessMainObject->AddChild(blackQueen);

			//------------------------------------------------------------------------
			GraphNode* whiteKnight = CreateNode("res/models/konik_bialy.fbx", defaultShader);
			pieces[6] = new ChessPieceScript(whiteKnight, window, &poof);
			whiteKnight->AddScript(pieces[6]);
			whiteKnight->Translate(glm::vec3(1300.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(whiteKnight);

			//------------------------------------------------------------------------
			GraphNode* blackKnight = CreateNode("res/models/konik_czarny.fbx", defaultShader);
			pieces[7] = new ChessPieceScript(blackKnight, window, &poof);
			blackKnight->AddScript(pieces[7]);
			blackKnight->Translate(glm::vec3(1500.0f, 100.0f, -230.0f));
			gameModePieces[3] = blackKnight;
			ChessMainObject->AddChild(blackKnight);

			//------------------------------------------------------------------------
			GraphNode* whiteRook = CreateNode("res/models/wieza_biala.fbx", defaultShader);
			pieces[8] = new ChessPieceScript(whiteRook, window, &poof);
			whiteRook->AddScript(pieces[8]);
			whiteRook->Translate(glm::vec3(1700.0f, 100.0f, -230.0f));
			ChessMainObject->AddChild(whiteRook);

			//------------------------------------------------------------------------
			GraphNode* blackRook = CreateNode("res/models/wieza_czarna.fbx", defaultShader);
			pieces[9] = new ChessPieceScript(blackRook, window, &poof);
			blackRook->AddScript(pieces[9]);
			blackRook->Translate(glm::vec3(1900.0f, 100.0f, -230.0f));
			gameModePieces[4] = blackRook;
			ChessMainObject->AddChild(blackRook);
			
#pragma endregion

#pragma region Chess Puzzle Scripting
		//-----------------------------Creating-Puzzle----------------------------
		ChessBoardPuzzle* puzzle = new ChessBoardPuzzle(ChessMainObject, chessTiles, pieces, window);
		ChessMainObject->AddScript(puzzle);
		//-----------------------------Binding-Scripts----------------------------
		for (int i = 0; i < 64; i++)
		{
			chessTiles[i]->AddScript(new TileScript(chessTiles[i], puzzle, i));
		}
		ChessMainObject->Scale(0.015);
		ChessMainObject->Translate(glm::vec3(-100.0f, 20.0f, 0.0f));
#pragma endregion
#pragma endregion

#pragma region World Configuration
		GraphNode* Scene1MainObject = CreateNode("res/models/pokoj_export.fbx", defaultShader);
		GraphNode* Scene1MainObjectDark = CreateNode("res/models/pokoj_export_dark.fbx", defaultShader);
		GraphNode* SceneOutsideBright = CreateUiElement(0,0,200,150, "res/models/bright_forest.png", additionalShaders[0]);
		GraphNode* SceneOutsideDark = CreateUiElement(0, 0, 200, 150, "res/models/dark_forest.png", additionalShaders[0]);

		//SETTING_INHERITANCE
		world->AddChild(Scene1);
		Scene1->AddChild(Scene1Bright);
		Scene1->AddChild(Scene1Dark);
		Scene1Bright->AddChild(SceneOutsideBright);
		Scene1Dark->AddChild(SceneOutsideDark);
		world->AddChild(bulb);
		Scene1Bright->AddChild(Scene1MainObject);
		Scene1Dark->AddChild(Scene1MainObjectDark);
		Scene1MainObject->Scale(0.1f);
		Scene1MainObjectDark->Scale(0.1f);

		GraphNode* bookShelfBright = CreateNode("res/models/pokoj_ksiazki.fbx", defaultShader);
		GraphNode* bookShelfDark = CreateNode("res/models/pokoj_ksiazki_dark.fbx", defaultShader);
		bookShelfBright->AddScript(new CameraChange(bookShelfBright, window, &ApTime::instance().isBookPosition));
		bookShelfDark->AddScript(new CameraChange(bookShelfDark, window, &ApTime::instance().isBookPosition));
		bookShelfBright->Scale(0.1f);
		bookShelfDark->Scale(0.1f);
		Scene1Bright->AddChild(bookShelfBright);
		Scene1Dark->AddChild(bookShelfDark);

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
#pragma endregion

#pragma region Chandelier Puzzle
		//--------------------------Chandelier-Puzzle--------------------------
		GraphNode* ChandelierBright = new GraphNode();
		Scene1Bright->AddChild(ChandelierBright);
		ChandelierBright->Translate(glm::vec3(20.0f, -10.0f, 30.0f));

		GraphNode* Chain = CreateNode("res/models/zyrandol_lancuch.fbx", defaultShader);
		Chain->Scale(0.1f);
		ChandelierBright->AddChild(Chain);

		GraphNode* ChandelierGlass = CreateNode("res/models/zyrandol_szklany.fbx", defaultShader);
		ChandelierGlass->Scale(0.1f);
		ChandelierBright->AddChild(ChandelierGlass);

		GraphNode* ChandelierShattered = CreateNode("res/models/zyrandol_stluczony.fbx", defaultShader);
		ChandelierShattered->Scale(0.1f);
		ChandelierShattered->Translate(glm::vec3(20.0f, 0.0f, 30.0f));
		Scene1->AddChild(ChandelierShattered);
		ChandelierShattered->SetActive(false);

		GraphNode* ChandelierNoGlass = CreateNode("res/models/zyrandol_szklany.fbx", defaultShader);
		ChandelierNoGlass->Scale(0.1f);
		ChandelierNoGlass->Translate(glm::vec3(20.0f, -10.0f, 30.0f));
		Scene1Dark->AddChild(ChandelierNoGlass);
		ChandelierNoGlass->AddScript(new ChandelierScript(ChandelierNoGlass, ChandelierGlass, ChandelierShattered, chandelierPrizes));
		objectId--;

		GraphNode* Line = CreateNode("res/models/zyrandol_lina.fbx", defaultShader);
		Line->Scale(0.1f);
		Line->Translate(glm::vec3(20.0f, -10.0f, 30.0f));
		Scene1Dark->AddChild(Line);
#pragma endregion

#pragma region Flowers Puzzle
		int* puzzleState = new int;
		*puzzleState = 0;

		Model* redFlower = new Model("res/models/zagadka_kwiaty/kwiatek_czerwony.fbx");
		redFlower->SetShader(defaultShader);
		Model* blueFlower = new Model("res/models/zagadka_kwiaty/kwiatek_niebieski.fbx");
		blueFlower->SetShader(defaultShader);
		Model* greenFlower = new Model("res/models/zagadka_kwiaty/kwiatek_zielony.fbx");
		greenFlower->SetShader(defaultShader);

		GraphNode* emptyCan = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/can_empty.png", textureShader);
		emptyCan->SetActive(false);
		emptyCan->AddScript(new DeactivateAfterTime(emptyCan, 5.0f));
		UI->AddChild(emptyCan);

#pragma region Red Flowers
		GraphNode* RedFlowers[5];

		GraphNode* RedFlower1 = new GraphNode(redFlower, objectId);
		RedFlower1->Scale(0.1f);
		RedFlower1->Translate(glm::vec3(0.0f, -5.0f, -20.0f));
		Scene1Dark->AddChild(RedFlower1);
		RedFlowers[0] = RedFlower1;

		GraphNode* RedFlower2 = new GraphNode(redFlower, objectId);
		RedFlower2->Scale(0.1f);
		RedFlower2->Translate(glm::vec3(0.0f, 5.0f, -20.0f));
		Scene1Dark->AddChild(RedFlower2);
		RedFlowers[1] = RedFlower2;

		GraphNode* RedFlower3 = new GraphNode(redFlower, objectId);
		RedFlower3->Scale(0.1f);
		RedFlower3->Translate(glm::vec3(0.0f, 15.0f, -20.0f));
		Scene1Dark->AddChild(RedFlower3);
		RedFlowers[2] = RedFlower3;

		GraphNode* RedFlower4 = new GraphNode(redFlower, objectId);
		RedFlower4->Scale(0.1f);
		RedFlower4->Translate(glm::vec3(0.0f, 25.0f, -20.0f));
		Scene1Dark->AddChild(RedFlower4);
		RedFlowers[3] = RedFlower4;

		GraphNode* RedFlower5 = new GraphNode(redFlower, objectId);
		RedFlower5->Scale(0.1f);
		RedFlower5->Translate(glm::vec3(0.0f, 35.0f, -20.0f));
		Scene1Dark->AddChild(RedFlower5);
		RedFlowers[4] = RedFlower5;
#pragma endregion

		GraphNode* RedPot = CreateNode("res/models/zagadka_kwiaty/donica_czerwona.fbx", defaultShader);
		RedPot->Scale(0.1f);
		RedPot->Translate(glm::vec3(0.0f, 0.0f, -20.0f));
		Scene1Bright->AddChild(RedPot);
		RedPot->AddScript(new GrowPlantScript(RedPot, 3, puzzleState, RedFlowers, emptyCan));

		GraphNode* RedStripes = CreateNode("res/models/zagadka_kwiaty/donica_paski_czerwona.fbx", defaultShader);
		RedStripes->Scale(0.1f);
		RedStripes->Translate(glm::vec3(0.0f, 0.0f, -20.0f));
		Scene1Dark->AddChild(RedStripes);

#pragma region Blue Flowers
		GraphNode* BlueFlowers[5];

		GraphNode* BlueFlower1 = new GraphNode(blueFlower, objectId);
		BlueFlower1->Scale(0.1f);
		BlueFlower1->Translate(glm::vec3(0.0f, -5.0f, 0.0f));
		Scene1Dark->AddChild(BlueFlower1);
		BlueFlowers[0] = BlueFlower1;

		GraphNode* BlueFlower2 = new GraphNode(blueFlower, objectId);
		BlueFlower2->Scale(0.1f);
		BlueFlower2->Translate(glm::vec3(0.0f, 5.0f, 0.0f));
		Scene1Dark->AddChild(BlueFlower2);
		BlueFlowers[1] = BlueFlower2;

		GraphNode* BlueFlower3 = new GraphNode(blueFlower, objectId);
		BlueFlower3->Scale(0.1f);
		BlueFlower3->Translate(glm::vec3(0.0f, 15.0f, 0.0f));
		Scene1Dark->AddChild(BlueFlower3);
		BlueFlowers[2] = BlueFlower3;

		GraphNode* BlueFlower4 = new GraphNode(blueFlower, objectId);
		BlueFlower4->Scale(0.1f);
		BlueFlower4->Translate(glm::vec3(0.0f, 25.0f, 0.0f));
		Scene1Dark->AddChild(BlueFlower4);
		BlueFlowers[3] = BlueFlower4;

		GraphNode* BlueFlower5 = new GraphNode(blueFlower, objectId);
		BlueFlower5->Scale(0.1f);
		BlueFlower5->Translate(glm::vec3(0.0f, 35.0f, 0.0f));
		Scene1Dark->AddChild(BlueFlower5);
		BlueFlowers[4] = BlueFlower5;
#pragma endregion

		GraphNode* BluePot = CreateNode("res/models/zagadka_kwiaty/donica_niebieska.fbx", defaultShader);
		BluePot->Scale(0.1f);
		//BluePot->Translate(glm::vec3(45.0f, 0.0f, -80.0f));
		Scene1Bright->AddChild(BluePot);
		BluePot->AddScript(new GrowPlantScript(BluePot, 2, puzzleState, BlueFlowers, emptyCan));

		GraphNode* BlueStripes = CreateNode("res/models/zagadka_kwiaty/donica_paski_niebieska.fbx", defaultShader);
		BlueStripes->Scale(0.1f);
		//BlueStripes->Translate(glm::vec3(45.0f, 0.0f, -80.0f));
		Scene1Dark->AddChild(BlueStripes);

#pragma region Green Flowers
		GraphNode* GreenFlowers[5];

		GraphNode* GreenFlower1 = new GraphNode(greenFlower, objectId);
		GreenFlower1->Scale(0.1f);
		GreenFlower1->Translate(glm::vec3(0.0f, -5.0f, 0.0f));
		Scene1Dark->AddChild(GreenFlower1);
		GreenFlowers[0] = GreenFlower1;

		GraphNode* GreenFlower2 = new GraphNode(greenFlower, objectId);
		GreenFlower2->Scale(0.1f);
		GreenFlower2->Translate(glm::vec3(0.0f, 5.0f, 0.0f));
		Scene1Dark->AddChild(GreenFlower2);
		GreenFlowers[1] = GreenFlower2;

		GraphNode* GreenFlower3 = new GraphNode(greenFlower, objectId);
		GreenFlower3->Scale(0.1f);
		GreenFlower3->Translate(glm::vec3(0.0f, 15.0f, 0.0f));
		Scene1Dark->AddChild(GreenFlower3);
		GreenFlowers[2] = GreenFlower3;

		GraphNode* GreenFlower4 = new GraphNode(greenFlower, objectId);
		GreenFlower4->Scale(0.1f);
		GreenFlower4->Translate(glm::vec3(0.0f, 25.0f, 0.0f));
		Scene1Dark->AddChild(GreenFlower4);
		GreenFlowers[3] = GreenFlower4;

		GraphNode* GreenFlower5 = new GraphNode(greenFlower, objectId);
		GreenFlower5->Scale(0.1f);
		GreenFlower5->Translate(glm::vec3(0.0f, 35.0f, 0.0f));
		Scene1Dark->AddChild(GreenFlower5);
		GreenFlowers[4] = GreenFlower5;
#pragma endregion

		GraphNode* GreenPot = CreateNode("res/models/zagadka_kwiaty/donica_zielona.fbx", defaultShader);
		GreenPot->Scale(0.1f);
		//GreenPot->Translate(glm::vec3(10.0f, 0.0f, -5.0f));
		Scene1Bright->AddChild(GreenPot);
		GreenPot->AddScript(new GrowPlantScript(GreenPot, 4, puzzleState, GreenFlowers, emptyCan));

		GraphNode* GreenStripes = CreateNode("res/models/zagadka_kwiaty/donica_paski_zielona.fbx", defaultShader);
		GreenStripes->Scale(0.1f);
		//GreenStripes->Translate(glm::vec3(10.0f, 0.0f, -5.0f));
		Scene1Dark->AddChild(GreenStripes);


		GraphNode* WhiteFlower = CreateNode("res/models/zagadka_kwiaty/kwiatek_bialy.fbx", defaultShader);
		WhiteFlower->Scale(0.1f);
		WhiteFlower->Translate(glm::vec3(10.0f, 0.0f, -5.0f));
		plantPuzzlePrizes->AddChild(WhiteFlower);

		GraphNode* WhitePot = CreateNode("res/models/zagadka_kwiaty/donica_kolorowaa.fbx", defaultShader);
		WhitePot->Scale(0.1f);
		WhitePot->Translate(glm::vec3(10.0f, 0.0f, -5.0f));
		Scene1Bright->AddChild(WhitePot);
		WhitePot->AddScript(new PlantPuzzleController(WhitePot, puzzleState, plantPuzzlePrizes));
		Scene1Bright->AddChild(plantPuzzlePrizes);

		GraphNode* Tap = CreateNode("res/models/zagadka_kwiaty/kranik.fbx", defaultShader);
		Tap->Scale(0.1f);
		Tap->Translate(glm::vec3(90.0f, 0.0f, 10.0f));
		Tap->Rotate(180 ,glm::vec3(0.0f, 1.0f, 0.0f));
		Scene1Dark->AddChild(Tap);
#pragma endregion

#pragma region UI Tap Door Drawers
		//--------------------------------UI-AND-INVENTORY--------------------------------
		GraphNode* door = CreateNode("res/models/drzwi.fbx", defaultShader);
		door->Scale(0.15f);
		//door->Rotate(-150, glm::vec3(0.0, 1.0, 0.0));
		door->Translate(glm::vec3(55.0, 0.0, 250.0));
		Scene1->AddChild(door);

		GraphNode* returnTutorial = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/return_info.png", textureShader);
		returnTutorial->SetActive(false);
		UI->AddChild(returnTutorial);
		UI->AddScript(new ReturnInfo(UI, returnTutorial));

		RoomSwapManager* manager1 = new RoomSwapManager(door, Scene1Bright, Scene1Dark, UIBright, UIDark,
			window, Scene1, Scene2, isBright, singleClick, &forceSwap, &engageSwap, &poof);
		door->AddScript(manager1);

#pragma region Bright HUD
		GraphNode* bottomPanelBright = new GraphNode();
		UIBright->AddChild(bottomPanelBright);

		GraphNode* brightBackground = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/normal_world/hud_bg_s1.png", textureShader);
		bottomPanelBright->AddChild(brightBackground);

		GraphNode* brightLeaves1 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/normal_world/hud_leaves1_s1.png", textureShader);
		bottomPanelBright->AddChild(brightLeaves1);

		GraphNode* brightJournal = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/normal_world/hud_journal_s1.png", textureShader);
		bottomPanelBright->AddChild(brightJournal);

		GraphNode* journal1 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/journal1.png", textureShader);
		UI->AddChild(journal1);
		journal1->SetActive(false);
		journal1->AddScript(new DeactivateRMB(journal1, window, "bookClose"));

		GraphNode* brightJournalHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/normal_world/hud_journal_hover_s1.png", textureShader);
		bottomPanelBright->AddChild(brightJournalHover);
		brightJournalHover->SetActive(false);
		brightJournal->AddScript(new ActivateOnHoverScript(brightJournal, brightJournalHover));
		brightJournalHover->AddScript(new DeactivateOnMouseLeave(brightJournalHover));
		brightJournalHover->AddScript(new JournalScript(brightJournalHover, journal1));

		GraphNode* brightPlant = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/normal_world/hud_plant_s1.png", textureShader);
		bottomPanelBright->AddChild(brightPlant);
		brightPlant->SetActive(false);
		UI->AddScript(new ActivateOnBool(UI, brightPlant));

		GraphNode* brightPlantHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/normal_world/hud_plant__hover_s1.png", textureShader);
		bottomPanelBright->AddChild(brightPlantHover);
		brightPlantHover->SetActive(false);
		brightPlant->AddScript(new ActivateOnHoverScript(brightPlant, brightPlantHover));
		brightPlantHover->AddScript(new DeactivateOnMouseLeave(brightPlantHover));
		brightPlantHover->AddScript(new SwapButton(brightPlantHover, &forceSwap));

		GraphNode* brightHint = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/normal_world/hud_hint_s1.png", textureShader);
		bottomPanelBright->AddChild(brightHint);

		GraphNode* brightHintHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/normal_world/hud_hint_hover_s1.png", textureShader);
		bottomPanelBright->AddChild(brightHintHover);
		brightHintHover->SetActive(false);
		brightHint->AddScript(new ActivateOnHoverScript(brightHint, brightHintHover, &ApTime::instance().isEasyMode));
		brightHintHover->AddScript(new DeactivateOnMouseLeave(brightHintHover));
		brightHintHover->AddScript(new HintButton(brightHintHover));

		GraphNode* brightMenu = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/normal_world/hud_menu_s1.png", textureShader);
		bottomPanelBright->AddChild(brightMenu);

		GraphNode* brightMenuHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/normal_world/hud_menu_hover_s1.png", textureShader);
		bottomPanelBright->AddChild(brightMenuHover);
		brightMenuHover->SetActive(false);
		brightMenu->AddScript(new ActivateOnHoverScript(brightMenu, brightMenuHover));
		brightMenu->AddScript(new MenuButtonScript(brightMenu, menu, window, true));
		brightMenuHover->AddScript(new DeactivateOnMouseLeave(brightMenuHover));
		brightMenuHover->AddScript(new MenuButtonScript(brightMenuHover, menu, window));

		GraphNode* brightLeaves2 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/normal_world/hud_leaves2_s1.png", textureShader);
		bottomPanelBright->AddChild(brightLeaves2);
#pragma endregion

#pragma region Dark HUD
		GraphNode* bottomPanelDark = new GraphNode();
		UIDark->AddChild(bottomPanelDark);

		GraphNode* darkBackground = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/fked_up_world/hud_bg_s2.png", textureShader);
		bottomPanelDark->AddChild(darkBackground);

		GraphNode* darktLeaves1 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/fked_up_world/hud_leaves1_s2.png", textureShader);
		bottomPanelDark->AddChild(darktLeaves1);

		GraphNode* darkJournal = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/fked_up_world/hud_journal_s2.png", textureShader);
		bottomPanelDark->AddChild(darkJournal);

		GraphNode* darkJournalHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/fked_up_world/hud_journal_hover_s2.png", textureShader);
		bottomPanelDark->AddChild(darkJournalHover);
		darkJournalHover->SetActive(false);
		darkJournal->AddScript(new ActivateOnHoverScript(darkJournal, darkJournalHover));
		darkJournalHover->AddScript(new DeactivateOnMouseLeave(darkJournalHover));
		darkJournalHover->AddScript(new JournalScript(darkJournalHover, journal1));

		GraphNode* darkPlant = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/fked_up_world/hud_plant_s2.png", textureShader);
		bottomPanelDark->AddChild(darkPlant);

		GraphNode* darkPlantHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/fked_up_world/hud_plant_hover_s2.png", textureShader);
		bottomPanelDark->AddChild(darkPlantHover);
		darkPlantHover->SetActive(false);
		darkPlant->AddScript(new ActivateOnHoverScript(darkPlant, darkPlantHover));
		darkPlantHover->AddScript(new DeactivateOnMouseLeave(darkPlantHover));
		darkPlantHover->AddScript(new SwapButton(darkPlantHover, &forceSwap));

		GraphNode* darkHint = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/fked_up_world/hud_hint_s2.png", textureShader);
		bottomPanelDark->AddChild(darkHint);

		GraphNode* darkHintHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/fked_up_world/hud_hint_hover_s2.png", textureShader);
		bottomPanelDark->AddChild(darkHintHover);
		darkHintHover->SetActive(false);
		darkHint->AddScript(new ActivateOnHoverScript(darkHint, darkHintHover, &ApTime::instance().isEasyMode));
		darkHintHover->AddScript(new DeactivateOnMouseLeave(darkHintHover));
		darkHintHover->AddScript(new HintButton(darkHintHover));

		GraphNode* darkMenu = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/fked_up_world/hud_menu_s2.png", textureShader);
		bottomPanelDark->AddChild(darkMenu);

		GraphNode* darkMenuHover = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/fked_up_world/hud_menu_hover_s2.png", textureShader);
		bottomPanelDark->AddChild(darkMenuHover);
		darkMenuHover->SetActive(false);
		darkMenu->AddScript(new ActivateOnHoverScript(darkMenu, darkMenuHover));
		darkMenu->AddScript(new MenuButtonScript(darkMenu, menu, window, true));
		darkMenuHover->AddScript(new DeactivateOnMouseLeave(darkMenuHover));
		darkMenuHover->AddScript(new MenuButtonScript(darkMenuHover, menu, window));

		GraphNode* darkLeaves2 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hud/fked_up_world/hud_leaves2_s2.png", textureShader);
		bottomPanelDark->AddChild(darkLeaves2);
#pragma endregion

		//---------------------------------------------------------------------------
		GraphNode* scissors = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/nozyce.png", textureShader);
		UI->AddChild(scissors);
		scissors->Scale(0.75);
		scissors->Translate(glm::vec3(-50, -280, 0));
		scissors->AddScript(new InventoryItemScript(scissors, "scissoors", window, singleClick));
		scissors->SetActive(false);

		GraphNode* bucket = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/bucket_water.png", textureShader);
		bucket->Scale(0.75);
		bucket->Translate(glm::vec3(70, 10, 0));
		UI->AddChild(bucket);
		bucket->AddScript(new InventoryItemScript(bucket, "bucket", window, singleClick));
		bucket->SetActive(false);

		GraphNode* bucketEmpty = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/bucket_empty.png", textureShader);
		bucketEmpty->Scale(0.75);
		bucketEmpty->Translate(glm::vec3(70, 10, 0));
		UI->AddChild(bucketEmpty);
		bucketEmpty->AddScript(new InventoryItemScript(bucketEmpty, "bucketEmpty", window, singleClick));
		bucketEmpty->SetActive(false);

		GraphNode* pieceOfPaper = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/kartka_inventory.png", textureShader);
		pieceOfPaper->Scale(0.75);
		pieceOfPaper->Translate(glm::vec3(400, 10, 0));
		UI->AddChild(pieceOfPaper);
		pieceOfPaper->AddScript(new InventoryItemScript(pieceOfPaper, "paper", window, singleClick, false));
		pieceOfPaper->SetActive(false);

		GraphNode* PaperBright = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/Kartka_Z_Kodem1.png", textureShader);
		UIBright->AddChild(PaperBright);
		PaperBright->SetActive(false);
		PaperBright->Translate(glm::vec3(0.0f, 80.0f, 0.0f));

		GraphNode* PaperDark = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/Kartka_Z_Kodem2.png", textureShader);
		UIDark->AddChild(PaperDark);
		PaperDark->SetActive(false);
		PaperDark->Translate(glm::vec3(0.0f, 80.0f, 0.0f));

		pieceOfPaper->AddScript(new Papers(pieceOfPaper, PaperBright, PaperDark));

		Tap->AddScript(new CraneScript(Tap, bucketEmpty, bucket));

		//Scripting for obtaining objects to inventory
		OneTimeActivatorScript* drawer3Script = new OneTimeActivatorScript(drawer3MovableSegment, scissors, "", false, true);
		drawer3Script->enabled = false;
		drawer3MovableSegment->AddScript(drawer3Script);
		drawer3MovableSegment->isHoverable = false;

		OneTimeActivatorScript* drawer2Script = new OneTimeActivatorScript(drawer2MovableSegment, bucketEmpty, "", false, true);
		drawer2Script->enabled = false;
		drawer2MovableSegment->AddScript(drawer2Script);
		drawer2MovableSegment->isHoverable = false;

		OneTimeActivatorScript* drawer1Script = new OneTimeActivatorScript(drawer1MovableSegment, pieceOfPaper, "", false, true);
		drawer1Script->enabled = false;
		drawer1MovableSegment->AddScript(drawer1Script);
		drawer1MovableSegment->isHoverable = false;

		puzzle->SetPrizes(drawer3Script, drawer2Script, drawer1Script);

#pragma endregion
	}

	void Scene2Setup(Shader* additionalShaders[] = nullptr)
	{
#pragma region World Setup and Room Swap
		world->AddChild(Scene2);
		Scene2->Scale(0.005f);
		Scene1->SetActive(false);
		Scene2->Translate(glm::vec3(-0.5f, -0.15f, 0.0f));

		//BRIGHT_WORLD:
		GraphNode* Scene2Bright = new GraphNode();
		Scene2->AddChild(Scene2Bright);

		//DARK_WORLD:
		GraphNode* Scene2Dark = new GraphNode();
		Scene2->AddChild(Scene2Dark);

		GraphNode* Scene2MainObject = CreateNode("res/models/lab.fbx", defaultShader);
		Scene2MainObject->Scale(0.2f);
		Scene2Bright->AddChild(Scene2MainObject);

		GraphNode* Scene2MainObjectDark = CreateNode("res/models/lab_another_dimension.fbx", defaultShader);
		Scene2MainObjectDark->Scale(0.2f);
		Scene2Dark->AddChild(Scene2MainObjectDark);

		GraphNode* door2 = CreateNode("res/models/drzwi.fbx", defaultShader);
		door2->Scale(0.2f);
		door2->Translate(glm::vec3(5.0f, 0.0f, 1.0f));
		Scene2->AddChild(door2);

#pragma endregion

#pragma region plant and desk
		GraphNode* labDesk = CreateNode("res/models/stol_lab.fbx", defaultShader);
		labDesk->Scale(0.2f);
		Scene2Bright->AddChild(labDesk);
		labDesk->AddScript(new StartingDesk(labDesk, window));

		GraphNode* labDeskDark = CreateNode("res/models/stol_lab_another_dimension.fbx", defaultShader);
		labDeskDark->Scale(0.2f);
		Scene2Dark->AddChild(labDeskDark);
		labDeskDark->AddScript(new StartingDesk(labDeskDark, window, false));

		GraphNode* SwapTutorial = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/swap_tutorial.png", textureShader);
		SwapTutorial->SetActive(false);
		UI->AddChild(SwapTutorial);

		GraphNode* labDeskPlant = CreateNode("res/models/sadzonka_lab.fbx", defaultShader);
		labDeskPlant->Scale(0.2f);
		Scene2->AddChild(labDeskPlant);
		labDeskPlant->AddScript(new MagicalPlant(labDeskPlant, SwapTutorial));

		GraphNode* HintTutorial = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/hint_tutorial.png", textureShader);
		UI->AddChild(HintTutorial);
		HintTutorial->SetActive(false);
		HintTutorial->AddScript(new DeactivateAfterTime(HintTutorial, 8.0f));

		RoomSwapManager* manager2 = new RoomSwapManager(door2, Scene2Bright, Scene2Dark, UIBright, UIDark,
			window, Scene2, Scene1, isBright, singleClick, &forceSwap, &engageSwap, &poof, SwapTutorial, HintTutorial, false);
		door2->AddScript(manager2);
#pragma endregion

#pragma region Scales Puzzle
		GraphNode* deskUnderTheWall = CreateNode("res/models/stol_po_prawej.fbx", defaultShader);
		deskUnderTheWall->Scale(0.2f);
		Scene2Bright->AddChild(deskUnderTheWall);

		//SCALES PUZZLE
		GraphNode* scalesPlant = CreateNode("res/models/pomidory.fbx", defaultShader);
		scalesPlant->Scale(0.2f);
		Scene2Dark->AddChild(scalesPlant);
		//scalesPlantLeft->Translate(glm::vec3(100.0f, 0.0f, 0.0f));
		//scalesPlantLeft->Rotate(-20.0f, glm::vec3(0.0f, 1.0f, 0.0f));


		GraphNode* ScalesPuzzle = new GraphNode();
		ScalesPuzzle->Scale(0.1);
		ScalesPuzzle->Translate(glm::vec3(130.0f, 50.0f, -120.0f));
		Scene2Bright->AddChild(ScalesPuzzle);
		ScalesPuzzle->Rotate(-40, glm::vec3(0.0f, 1.0f, 0.0f));

		GraphNode* staticScales = CreateNode("res/models/stojak.fbx", defaultShader);
		ScalesPuzzle->AddChild(staticScales);
		GraphNode* rotatingScales = CreateNode("res/models/bujak.fbx", defaultShader);
		rotatingScales->Translate(glm::vec3(0.0f, 188.0f, 0.0f));
		ScalesPuzzle->AddChild(rotatingScales);
		GraphNode* leftScales = CreateNode("res/models/szalka.fbx", defaultShader);
		ScalesPuzzle->AddChild(leftScales);
		GraphNode* rightScales = CreateNode("res/models/szalka.fbx", defaultShader);
		ScalesPuzzle->AddChild(rightScales);
		rightScales->Translate(glm::vec3(0.0f, 0.0f, -236.0f));

#pragma region Weights
		Model* weight = new Model("res/models/ciezarek.fbx");
		weight->SetShader(defaultShader);
		GraphNode* weight1l = new GraphNode(weight, objectId++);
		leftScales->AddChild(weight1l);
		weight1l->Translate(glm::vec3(0.0f, 50.0f, 150.0f));
		weight1l->Scale(0.75);
		weight1l->SetActive(false);

		GraphNode* weight2l = new GraphNode(weight, objectId++);
		leftScales->AddChild(weight2l);
		weight2l->Translate(glm::vec3(0.0f, 50.0f, 120.0f));
		weight2l->Scale(0.75);
		weight2l->SetActive(false);

		GraphNode* weight3l = new GraphNode(weight, objectId++);
		leftScales->AddChild(weight3l);
		weight3l->Translate(glm::vec3(0.0f, 50.0f, 90.0f));
		weight3l->Scale(0.75);
		weight3l->SetActive(false);

		GraphNode* weight1r = new GraphNode(weight, objectId++);
		rightScales->AddChild(weight1r);
		weight1r->Translate(glm::vec3(0.0f, 50.0f, 150.0f));
		weight1r->Scale(0.75);
		//weight1r->SetActive(false);

		GraphNode* weight2r = new GraphNode(weight, objectId++);
		rightScales->AddChild(weight2r);
		weight2r->Translate(glm::vec3(0.0f, 50.0f, 120.0f));
		weight2r->Scale(0.75);
		//weight2r->SetActive(false);

		GraphNode* weight3r = new GraphNode(weight, objectId++);
		rightScales->AddChild(weight3r);
		weight3r->Translate(glm::vec3(0.0f, 50.0f, 90.0f));
		weight3r->Scale(0.75);
		//weight3r->SetActive(false);

		GraphNode* weight1 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT, "res/models/odwaznik_4.png", textureShader);
		//weight1->Scale(0.5);
		weight1->Translate(glm::vec3(-150, 0, 0));
		UI->AddChild(weight1);
		weight1->AddScript(new InventoryItemScript(weight1, "weight4", window, singleClick));
		weight1->SetActive(false);

		GraphNode* weight2 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/odwaznik_5.png", textureShader);
		//weight2->Scale(0.5);
		weight2->Translate(glm::vec3(-120, 0, 0));
		UI->AddChild(weight2);
		weight2->AddScript(new InventoryItemScript(weight2, "weight5", window, singleClick));
		weight2->SetActive(false);

		GraphNode* weight3 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/odwaznik_6.png", textureShader);
		//weight3->Scale(0.5);
		weight3->Translate(glm::vec3(-90, 0, 0));
		UI->AddChild(weight3);
		weight3->AddScript(new InventoryItemScript(weight3, "weight6", window, singleClick));
		weight3->SetActive(false);

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
#pragma endregion

		GraphNode* scalesPuzzlePrizes = new GraphNode();

		int* scalesPuzzleController = new int;
		*scalesPuzzleController = 3;
		rotatingScales->AddScript(new ScalesBalance(rotatingScales, scalesPuzzleController, scalesPuzzlePrizes));
		leftScales->AddScript(new SingleScaleScript(leftScales, scalesPuzzleController, true, scalesTab, manager2));
		rightScales->AddScript(new SingleScaleScript(rightScales, scalesPuzzleController, false, scalesTab, manager2));

#pragma endregion

#pragma region Door Puzzle
		GraphNode* GiantDoor = CreateNode("res/models/drzwi_sejf.fbx", defaultShader);
		Scene2->AddChild(GiantDoor);
		GiantDoor->Translate(glm::vec3(5.0f, 0.0f, -2.0f));
		GiantDoor->Scale(0.2f);

		GraphNode* finalScreen = new GraphNode();
		finalScreen->SetActive(false);
		UI->AddChild(finalScreen);
		GraphNode* Sanctum = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/Sanctum.png", textureShader);
		finalScreen->AddChild(Sanctum);

		GraphNode* paperSanctum = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/kartkaSanctum.png", textureShader);
		finalScreen->AddChild(paperSanctum);

		GraphNode* journal2 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/journal2.png", textureShader);
		UI->AddChild(journal2);
		journal2->SetActive(false);

		paperSanctum->AddScript(new OneTimeActivatorScript(paperSanctum, journal2, "bookOpen", false));

		*password = "";
		bool* isWon = new bool;
		*isWon = false;
		DoorPuzzleObject = new GraphNode();
		UI->AddChild(DoorPuzzleObject);
		DoorPuzzleObject->SetActive(false);
		GiantDoor->AddScript(new DoorPuzzle(GiantDoor, DoorPuzzleObject, finalScreen, window, password, isWon,
			text, textShader));

		GraphNode* MainDoor = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/drzwi_kodowe/drzwi_kodowe.png", textureShader);
		DoorPuzzleObject->AddChild(MainDoor);

		GraphNode* Number1 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/drzwi_kodowe/1.png", textureShader);
		DoorPuzzleObject->AddChild(Number1);
		Number1->AddScript(new DoorButton(Number1, password, isWon, "1"));

		GraphNode* Number2 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/drzwi_kodowe/2.png", textureShader);
		DoorPuzzleObject->AddChild(Number2);
		Number2->AddScript(new DoorButton(Number1, password, isWon, "2"));

		GraphNode* Number3 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/drzwi_kodowe/3.png", textureShader);
		DoorPuzzleObject->AddChild(Number3);
		Number3->AddScript(new DoorButton(Number1, password, isWon, "3"));

		GraphNode* Number4 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/drzwi_kodowe/4.png", textureShader);
		DoorPuzzleObject->AddChild(Number4);
		Number4->AddScript(new DoorButton(Number1, password, isWon, "4"));

		GraphNode* Number5 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/drzwi_kodowe/5.png", textureShader);
		DoorPuzzleObject->AddChild(Number5);
		Number5->AddScript(new DoorButton(Number1, password, isWon, "5"));

		GraphNode* Number6 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/drzwi_kodowe/6.png", textureShader);
		DoorPuzzleObject->AddChild(Number6);
		Number6->AddScript(new DoorButton(Number1, password, isWon, "6"));

		GraphNode* Number7 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/drzwi_kodowe/7.png", textureShader);
		DoorPuzzleObject->AddChild(Number7);
		Number7->AddScript(new DoorButton(Number1, password, isWon, "7"));

		GraphNode* Number8 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/drzwi_kodowe/8.png", textureShader);
		DoorPuzzleObject->AddChild(Number8);
		Number8->AddScript(new DoorButton(Number1, password, isWon, "8"));

		GraphNode* Number9 = CreateUiElement(0, 0, *SCR_WIDTH, *SCR_HEIGHT,
			"res/models/drzwi_kodowe/9.png", textureShader);
		DoorPuzzleObject->AddChild(Number9);
		Number9->AddScript(new DoorButton(Number1, password, isWon, "9"));

		Number1->Translate(glm::vec3(320.0f, 0.0f, 0.0f));
		Number2->Translate(glm::vec3(320.0f, 0.0f, 0.0f));
		Number3->Translate(glm::vec3(320.0f, 0.0f, 0.0f));
		Number4->Translate(glm::vec3(320.0f, 0.0f, 0.0f));
		Number5->Translate(glm::vec3(320.0f, 0.0f, 0.0f));
		Number6->Translate(glm::vec3(320.0f, 0.0f, 0.0f));
		Number7->Translate(glm::vec3(320.0f, 0.0f, 0.0f));
		Number8->Translate(glm::vec3(320.0f, 0.0f, 0.0f));
		Number9->Translate(glm::vec3(320.0f, 0.0f, 0.0f));

#pragma endregion
	}

	void PostProcessSetup()
	{
		//glDeleteBuffers(1, &rbo);
		//glDeleteBuffers(2, &intermediateFBuffer);
		//glDeleteBuffers(2, frameBuffers);
		//glDeleteTextures(1, &intermediateTBuffer);
		//glDeleteTextures(2, textureBuffers);
		if (isFirstBuffer)
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
			glGenFramebuffers(2, &intermediateFBuffer);
			glGenTextures(1, &intermediateTBuffer);
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
			glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBuffer);
			glBindTexture(GL_TEXTURE_2D, intermediateTBuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, *SCR_WIDTH, *SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, intermediateTBuffer, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
			// also check if framebuffers are complete (no need for depth buffer)
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			isFirstBuffer = false;
		}
		else
		{
			//glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, *SCR_WIDTH, *SCR_HEIGHT);

			//glGenFramebuffers(2, &intermediateFBuffer);
			//glGenTextures(1, &intermediateTBuffer);
			//glGenFramebuffers(2, frameBuffers);
			//glGenTextures(2, textureBuffers);
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
			glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBuffer);
			glBindTexture(GL_TEXTURE_2D, intermediateTBuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, *SCR_WIDTH, *SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, intermediateTBuffer, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
			// also check if framebuffers are complete (no need for depth buffer)
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

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
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}

	void BlurRender(unsigned int currentlyPicked, float scale) { // for later when we have a specific object to render
		BloomRender(currentlyPicked, intermediateFBuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[0]);
		glClearColor(1.0f, 1.0f, 1.0f, 0.00f);
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

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		mixShader->use();
		mixShader->setInt("scene", 0);
		mixShader->setInt("bloomBlur", 1);
		mixShader->setInt("bloom", true);
		mixShader->setFloat("exposure", 1.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, intermediateTBuffer);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureBuffers[!horizontal]);
		renderQuad();
		glClearColor(0.0f, 0.0f, 0.0f, 0.00f);
	}

	void BloomRender(unsigned int currentlyPicked, unsigned int bufferNumber)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[0]);
		glClearColor(1.0f, 1.0f, 1.0f, 0.00f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[1]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[0]);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		world->Draw(currentlyPicked);

		brightShader->use();
		brightShader->setInt("scene", 0);
		glBindTexture(GL_TEXTURE_2D, textureBuffers[0]);
		renderQuad();
		glClear(GL_DEPTH_BUFFER_BIT);

		bool horizontal = true;
		unsigned int amount = 1;
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
		glBindFramebuffer(GL_FRAMEBUFFER, bufferNumber);
		bloomMixShader->use();
		bloomMixShader->setInt("scene", 0);
		bloomMixShader->setInt("highlightParts", 1);
		bloomMixShader->setFloat("strength", 0.6);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBuffers[horizontal]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureBuffers[!horizontal]);
		renderQuad();
		if (bufferNumber != 0) {
			glDepthFunc(GL_ALWAYS);
			UI->Draw(currentlyPicked);
			glDepthFunc(GL_LESS);
		}
	}

private:
	GraphNode* CreateNode(string const& pathToModel, Shader* shader)
	{
		Model* model = new Model(pathToModel);
		model->SetShader(shader);
		return new GraphNode(model, objectId++);
	}

	GraphNode* CreateUiElement(int xPos, int yPos, int width, int height, string path, Shader* shader)
	{
		//glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos, 0));
		glm::mat4* Transform = new glm::mat4(1.0);
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