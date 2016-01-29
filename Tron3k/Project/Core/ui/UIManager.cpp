#include "UIManager.h"

UIManager::UIManager()
{
	fileNamesListFirstGroup = nullptr;
	fileNamesListSecondGroup = nullptr;
	menus = nullptr;
	nrOfFileNamesFirstGroup = 0;
	nrOfFileNamesSecondGroup = 0;
	nrOfMenus = 0;
	maxMenus = 5;
	textureRes = new std::vector<glm::vec2>;
	currentMenu = -1;
}
UIManager::~UIManager() 
{
	if (renderPipe != nullptr)
	{
		renderPipe->ui_textureRelease(uiTextureIds);
	}
	if (fileNamesListFirstGroup != nullptr )
		delete [] fileNamesListFirstGroup;
	if (fileNamesListSecondGroup != nullptr)
		delete[] fileNamesListSecondGroup;
	if ( menus != nullptr )
		delete[] menus;
	console = nullptr;
	if (textureRes)
		delete textureRes;
}

//Start menu
void UIManager::init(Console* console)
{
	std::string fileNameFirstGroup = "GameFiles/UIFiles/menuFileNames.txt";
	std::string fileNameSecondGroup = "GameFiles/UIFiles/menuFileNames2.txt";
	this->console = console;

	//Texture Paths
	texturePaths.push_back("GameFiles/Textures/UITextures/testmap.png");
	texturePaths.push_back("GameFiles/Textures/UITextures/multiplayer.png");
	texturePaths.push_back("GameFiles/Textures/UITextures/settings.png");
	texturePaths.push_back("GameFiles/Textures/UITextures/exit.png");

	loadInTexture();

	//Open tthe first groups file
	std::ifstream myfile;
	myfile.open(fileNameFirstGroup);

	if (myfile.is_open())
	{
		int counter = 0;
		std::string inputString;

		//Number of menu files.
		getline(myfile, inputString);
		nrOfFileNamesFirstGroup = std::stoi(inputString);

		fileNamesListFirstGroup = new std::string[nrOfFileNamesFirstGroup];

		while (!myfile.eof()) //Looping through the file until there is nothing left to read.
		{
			//Get the next file name
			getline(myfile, inputString);

			//Store the name
			fileNamesListFirstGroup[counter] = inputString;
			counter++;
		}
	}
	else
		console->printMsg("Error: UIManager.init could not open the first groups file.", "System", 'S');
	myfile.close();

	//Open the second groups file
	myfile.open(fileNameSecondGroup);

	if (myfile.is_open())
	{
		int counter = 0;
		std::string inputString;

		//Number of menu files.
		getline(myfile, inputString);
		nrOfFileNamesSecondGroup = std::stoi(inputString);

		fileNamesListSecondGroup = new std::string[nrOfFileNamesSecondGroup];

		while (!myfile.eof()) //Looping through the file until there is nothing left to read.
		{
			//Get the next file name
			getline(myfile, inputString);

			//Store the name
			fileNamesListSecondGroup[counter] = inputString;
			counter++;
		}
	}
	else
		console->printMsg("Error: UIManager.init could not open the second groups file.", "System", 'S');
	myfile.close();

	currentGroup = 1;

	LoadNextSet(0); //Load the first set of menus.
	setMenu(0); //Set start menu as the current menu
}

void UIManager::loadInTexture()
{
	uiTextureIds.clear();
	textureRes->clear();
	GLuint temp;
	for (int i = 0; i < texturePaths.size(); i++)
	{
		temp = 0; 
		int xres, yres;
		renderPipe->ui_loadTexture(&temp, (char*)(texturePaths[i].c_str()), &xres, &yres);
		textureRes[0].push_back(glm::vec2());
		textureRes[0][i].x = xres;
		textureRes[0][i].y = yres;
		uiTextureIds.push_back(temp);
	}
	texturePaths.clear();
}

void UIManager::menuRender()
{
	renderPipe->ui_initRender();
	//
	////test render
	//glm::mat4 worldtest;
	//// position
	//worldtest[0].w;
	//worldtest[1].w;
	//worldtest[2].w;
	////sacle
	//worldtest[0].x = 0.25f;
	//worldtest[1].y = 0.25f;
	//worldtest[2].z;
	//
	//renderPipe->ui_renderQuad(&worldtest[0][0], uiTextureIds[0], 1.0f);

	menus[currentMenu].render(uiTextureIds);
}

void UIManager::inGameRender()
{
	renderPipe->ui_InGameRenderInit();

	menus[currentMenu].render(uiTextureIds);
}


//Add and remove menus
void UIManager::setMenu(int menuId)
{
	if (menuId > -1 && menuId < nrOfMenus)
		currentMenu = menuId;
	else
		console->printMsg("Error: invalid menuId in function setMenu.", "System", 'S');
}
void UIManager::removeAllMenus() 
{
	nrOfMenus = 0;
	currentMenu = -1;
	if(menus != nullptr)
		delete[] menus;
	menus = nullptr;
}

int UIManager::collisionCheck(glm::vec2 pos)
{
	return menus[currentMenu].mouseCollission(pos);
}
void UIManager::hoverCheck(glm::vec2 pos)
{
	menus[currentMenu].mouseHover(pos);
}


void UIManager::changeTex(int objId, int whichTex)
{
	menus[currentMenu].changeTex(objId, whichTex);
}

bool UIManager::LoadNextSet(int whichMenuGroup)
{
	removeAllMenus();

	if (currentGroup == 0)
		currentGroup = 1;
	else
		currentGroup = 0;

	//Menu init has to take the GLuint texture id!

	switch (currentGroup)
	{
	case 0: //First Group
	{
		menus = new UI[nrOfFileNamesFirstGroup];
		for (int i = 0; i < nrOfFileNamesFirstGroup; i++)
		{
			menus[i].init(fileNamesListFirstGroup[i], console, renderPipe, textureRes);
			menus[i].setTextureId(uiTextureIds);
			nrOfMenus++;
		}
		break;
	}
	case 1: //Second Group
	{
		menus = new UI[nrOfFileNamesSecondGroup];
		for (int i = 0; i < nrOfFileNamesSecondGroup; i++)
		{
			menus[i].init(fileNamesListSecondGroup[i], console, renderPipe, textureRes);
			menus[i].setTextureId(uiTextureIds);
			nrOfMenus++;
		}
		break;
	}
	default:
		break;
	}

	return true;
}