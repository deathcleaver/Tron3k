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

	currentMenu = -1;
}
UIManager::~UIManager() 
{
	if (fileNamesListFirstGroup != nullptr )
		delete [] fileNamesListFirstGroup;
	if (fileNamesListSecondGroup != nullptr)
		delete[] fileNamesListSecondGroup;
	if ( menus != nullptr )
		delete[] menus;
	console = nullptr;
}

//Start menu
void UIManager::init(std::string fileNameFirstGroup, std::string fileNameSecondGroup, Console* console)
{
	this->console = console;

	//Texture Paths
	texturePaths.push_back("../../../Debug/GameFiles/Textures/UITextures/testmap.png");
	texturePaths.push_back("../../../Debug/GameFiles/Textures/UITextures/multiplayer.png");
	texturePaths.push_back("../../../Debug/GameFiles/Textures/UITextures/settings.png");
	texturePaths.push_back("../../../Debug/GameFiles/Textures/UITextures/exit.png");

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

	currentGroup = 0;

	LoadNextSet(0); //Load the first set of menus.
	setMenu(0); //Set start menu as the current menu
}

void UIManager::loadInTexture()
{
	//uiTextureIds.clear();
	//texturePaths.clear();
	//GLuint temp;
	//
	//for (int i = 0; i < texturePaths.size(); i++)
	//{
	//	temp = uiLoadTexture(texturePaths[i]);
	//	uiTextureIds.push_back(temp);
	//}
}

void UIManager::render()
{
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
	for (int i = 0; i < nrOfMenus; i++)
	{
		menus[i].clean();
	}
	nrOfMenus = 0;
	currentMenu = -1;
	delete menus;
	menus = nullptr;
}

int UIManager::collisionCheck(glm::vec2 pos)
{
	return menus[currentMenu].mouseCollission(pos);
}

void UIManager::changeTex(int objId)
{
	menus[currentMenu].changeTex(objId);
}

bool UIManager::LoadNextSet(int whichMenuGroup)
{
	removeAllMenus();

	if (currentGroup == 0)
		currentGroup = 1;
	else
		currentGroup = 0;

	switch (currentGroup)
	{
	case 0: //First Group
	{
		menus = new UI[nrOfFileNamesFirstGroup];
		for (int i = 0; i < nrOfFileNamesFirstGroup; i++)
			menus[i].init(fileNamesListFirstGroup[i], console);
		break;
	}
	case 1: //Second Group
	{
		menus = new UI[nrOfFileNamesSecondGroup];
		for (int i = 0; i < nrOfFileNamesSecondGroup; i++)
			menus[i].init(fileNamesListSecondGroup[i], console);
		break;
	}
	default:
		break;
	}

	return true;
}