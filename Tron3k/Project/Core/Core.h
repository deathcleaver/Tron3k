#ifndef CORE_H
#define CORE_H


#include "ui\UIManager.h"
#include "Input.h"
#include "../../SoundPlayer/sound/SoundPlayer.h"

#undef APIENTRY

#include "../Network/Client.h"
#include "../Network/Server.h"

#include "../../RenderPipeline/IRenderPipeline.h"
#include "Game\Game.h"

#include "Input.h"
#include "Console.h"

#include "CameraInput.h"

#include <fstream> //load settings
#include <sstream>

#define VALIDKEYS 11

class Core
{
private:
	bool settingsLoaded = false;
	int uitmpcounter;
	float slowmode = 1.0f;
	float slowdownFactor = 0.8f;
	float slowdownTimer = 0.0f;

	float timepass;	//temp
	Topology* top;
	Game* game;
	IRenderPipeline* renderPipe;
	Input* i;
	Console console;

	CameraInput* serverCam;

	UIManager* uiManager;
	bool renderUI;
	
	int* namePlates;
	float cursorBlink;

	// window
	GLFWwindow* win;
	bool cursorVisible;
	bool recreate;
	bool fullscreen;
	bool serverRender;
	int winX, winY;

	//gamestates
	Gamestate current;
	Gamestate last;
	int subState;
	int lastSubState;

	//network
	float tick_timer;

	//netlog
	bool client_record;
	bool client_playback;
	float playbackSpeed = 1.0f;

	//for culling
	vec3 lastCampos;
	vec3 lastPlayerPos;

	//gamestate update functions
	void upStart(float dt);
	void upMenu(float dt);
	void upRoam(float dt);
	void upClient(float dt);
	void upServer(float dt);

	
	void startHandleCmds(std::string com = "");
	void roamHandleCmds(std::string com = "");
	void clientHandleCmds(std::string com = "");
	void serverHandleCmds();

	void renderWorld(float dt);
	void inGameUIUpdate();
	void handleCulling();
	void createWindow(int x, int y, bool fullscreen);
	void removeWindow();

	void initPipeline();

	void disconnect();

	//load from file
	IpAddress _addrs;
	int _port;
	string _name;
	void loadSettings();
	void saveSettings();

	//Relays BoundingBoxes to Physics
	bool sendChunkBoxes(int chunkID);
	void sendCapPointBoxes();
	void sendRoomBoxes();

	void sendPlayerBox();
	void sendWorldBoxes();

	void showTeamSelect();
	//Menu activations
	bool startTeamSelect; //temp
	bool renderMenu;
	bool menuKeyListener;

	//Menu input
	void menuKeyInputUpdate();
	const int validKeyboardInputs[VALIDKEYS] = { GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9, GLFW_KEY_0, GLFW_KEY_PERIOD };

public:
	
	vec3 TEAMONECOLOR = vec3(1.0f, 0.5f, 0.0f);
	vec3 TEAMTWOCOLOR = vec3(0.0f, 0.5f, 0.0f);

	void init();
	void update(float dt);

	bool windowVisible() const;

	void setfps(int fps);

	~Core();
};

#endif