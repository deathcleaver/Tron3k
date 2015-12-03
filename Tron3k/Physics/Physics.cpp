#include "Physics.h"

//#include <vld.h>

Physics::Physics()
{

}

Physics::~Physics()
{

}

bool Physics::init()
{
	initBulletBox();
	return 1;
}

void Physics::initBulletBox()
{
	bulletBox.init();

	//TEMPORARY
	glm::vec3 size = glm::vec3(0.2f, 0.2f, 0.2f);

	//bulletBox.setSize(size);
}

bool Physics::release()
{
	delete this;
	return 1;
}

bool Physics::checkAABBCollision(Geometry* obj1, Geometry* obj2)
{
	//AABB collision mellan objekt
	if (obj1->getPos().x + obj1->getSize().x > obj2->getPos().x - obj2->getSize().x &&
		obj1->getPos().x - obj1->getSize().x < obj2->getPos().x + obj2->getSize().x)//x
	{
		if (obj1->getPos().y + obj1->getSize().y > obj2->getPos().y - obj2->getSize().y &&
			obj1->getPos().y - obj1->getSize().y < obj2->getPos().y + obj2->getSize().y)//y
		{
			if (obj1->getPos().z + obj1->getSize().z > obj2->getPos().z - obj2->getSize().z &&
				obj1->getPos().z - obj1->getSize().z < obj2->getPos().z + obj2->getSize().z)//z
			{
				return 1;
			}
		}
	}

	return 0;
}

bool Physics::checkAABBCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	AABB aabb1 = mesh1.getAABB();
	AABB aabb2 = mesh2.getAABB();

	glm::vec3 pos1 = glm::vec3(aabb1.pos);
	glm::vec3 pos2 = glm::vec3(aabb2.pos);

	glm::vec3 max1 = glm::vec3(aabb1.max);
	glm::vec3 max2 = glm::vec3(aabb2.max);

	glm::vec3 min1 = glm::vec3(aabb1.min);
	glm::vec3 min2 = glm::vec3(aabb2.min);

	//max1 = max1 - pos1;
	max2 = max2 - pos2;

	//min1 = min1 - pos1;
	min2 = min2 - pos2;




	if (pos1.x + max1.x > pos2.x + min2.x &&
		pos1.x + min1.x < pos2.x + max2.x)//x
	{
		if (pos1.y + max1.y > pos2.y + min2.y &&
			pos1.y + min1.y < pos2.y + max2.y)//y
		{
			if (pos1.z + max1.z > pos2.z + min2.z &&
				pos1.z + min1.z < pos2.z + max2.z)//z
			{
				return 1;
			}
		}
	}

	return 0;
}

bool Physics::checkCylindervAABBCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	if (mesh1.getCylinder().pos.y + mesh1.getCylinder().height > mesh2.getAABB().pos.y - mesh2.getAABB().min.y &&
		mesh1.getCylinder().pos.y - mesh1.getCylinder().height < mesh2.getAABB().pos.y + mesh2.getAABB().max.y)
	{
		//Collides in Y

		//http://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
		glm::vec2 dist = glm::vec2(abs(mesh1.getCylinder().pos.x - mesh2.getAABB().pos.x), abs(mesh1.getCylinder().pos.z - mesh2.getAABB().pos.z));

		if (dist.x > mesh2.getAABB().max.x + mesh1.getCylinder().radius)
			return 0;
		if (dist.y > mesh2.getAABB().max.z + mesh1.getCylinder().radius)
			return 0;

		if (dist.x <= mesh2.getAABB().max.x)
			return 1;
		if (dist.y <= mesh2.getAABB().max.z)
			return 1;

		float cDist = ((dist.x - mesh2.getAABB().min.x) * (dist.x - mesh2.getAABB().min.x)) + ((dist.y - mesh2.getAABB().min.z) * (dist.y - mesh2.getAABB().min.z));
		
		if (cDist <= mesh1.getCylinder().radius * mesh1.getCylinder().radius)
			return 1;
		return 0;

	}

	return 0;
	/*if (mesh1.getAABB().posX + mesh1.getAABB().sizeX > mesh2.getAABB().posX - mesh2.getAABB().sizeX &&
		mesh1.getAABB().posX - mesh1.getAABB().sizeX < mesh2.getAABB().posX + mesh2.getAABB().sizeX)//x
	{
		if (mesh1.getAABB().posY + mesh1.getAABB().sizeY > mesh2.getAABB().posY - mesh2.getAABB().sizeY &&
			mesh1.getAABB().posY - mesh1.getAABB().sizeY < mesh2.getAABB().posY + mesh2.getAABB().sizeY)//y
		{
			if (mesh1.getAABB().posZ + mesh1.getAABB().sizeZ > mesh2.getAABB().posZ - mesh2.getAABB().sizeZ &&
				mesh1.getAABB().posZ - mesh1.getAABB().sizeZ < mesh2.getAABB().posZ + mesh2.getAABB().sizeZ)//z
			{
				return 1;
			}
		}
	}

	return 0;*/
}

bool Physics::checkOBBCollision(Geometry* obj1, Geometry* obj2)
{
	return 1;
}

bool Physics::checkOBBCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	return 1;
}

bool Physics::checkPlayerVPlayerCollision(glm::vec3 playerPos1, glm::vec3 playerPos2)
{
	playerBox.setPos(playerPos1);
	CollideMesh p2;
	p2.init();
	p2.setPos(playerPos2);
	//p2.setSize(playerBox.getSize());


	bool collide = checkAABBCollision(playerBox, p2);

	//if (collide)
		//collide = checkOBBCollision(&obj1, &obj2);
	return collide;
}

bool Physics::checkPlayerVBulletCollision(glm::vec3 playerPos, glm::vec3 bulletPos)
{
	playerBox.setPos(playerPos);
	bulletBox.setPos(bulletPos);
	bool collide = false;// checkAABBCollision(playerBox, bulletBox);

	//if (collide)
		//collide = checkOBBCollision(&player, &bullet);
	return collide;
}


bool Physics::checkPlayerVWorldCollision(glm::vec3 playerPos)
{
	bool collides = false;
	playerBox.setPos(playerPos);

	for (int i = 0; i < worldBoxes.size(); i++)
	{
		for (int j = 0; j < worldBoxes[i].size(); j++)
		{
			if (checkAABBCollision(playerBox, worldBoxes[i][j]))
				//if (checkOBBCollision(playerPos, worldBoxes[i]))
				collides = true;
		}
	}
	return collides;
}

bool Physics::checkBulletVWorldCollision(glm::vec3 bulletPos)
{
	bool collides = false;
	bulletBox.setPos(bulletPos);

	for (int i = 0; i < worldBoxes.size(); i++)
	{
		//if (checkAABBCollision(bulletBox, worldBoxes[i][0]))
			//collides = true;
	}
	return collides;
}


void Physics::addGravity(glm::vec3 &vel, float dt)
{
	vel.y -= GRAVITY * dt;
}

void Physics::receiveChunkBoxes(int chunkID, void* cBoxes)
{
	std::vector<AABB>* cBox = (std::vector<AABB>*)cBoxes;
	std::vector<AABB> b = std::vector<AABB>(*cBox);

	storeChunkBox(chunkID, b);
}

void Physics::storeChunkBox(int chunkID, std::vector<AABB> cBoxes)
{
	//chunkID isn't used, but we send it here anyway, JUST IN CASE

	//creates a vector, so we know information is copied
	std::vector<CollideMesh> cMeshes;
	CollideMesh temp;

	for (int i = 0; i < cBoxes.size(); i++)
	{
		//stores all the info in a tempmesh
		temp.init();
		temp.setAABB(cBoxes[i]);

		//adds it to our vector
		cMeshes.push_back(temp);
	}

	//Adds the vector to the WorldBox
	//This vector is all the collisionboxes in a chunk
	//so Chunk[0] will have it's collisionBoxes in worldBoxes[0]
	worldBoxes.push_back(cMeshes);

}

void Physics::receivePlayerBox(std::vector<float> pBox)
{
	float xPos, yPos, zPos;
	float xSize, ySize, zSize;
	playerBox.init();

	xSize = abs(pBox[0] - pBox[1]) / 2;
	ySize = abs(pBox[2] - pBox[3]) / 2;
	zSize = abs(pBox[4] - pBox[5]) / 2;

	xPos = (pBox[0] + pBox[1]) / 2;
	yPos = (pBox[2] + pBox[3]) / 2;
	zPos = (pBox[4] + pBox[5]) / 2;

	playerBox.setAABB(glm::vec3(xPos, yPos, zPos), glm::vec3(xSize, ySize, zSize), glm::vec3(-xSize, -ySize, -zSize));
}

void Physics::receiveWorldBoxes(std::vector<std::vector<float>> wBoxes)
{
	float xPos, yPos, zPos;
	float xSize, ySize, zSize;
	CollideMesh temp;
	temp.init();
	for (int i = 0; i < wBoxes.size(); i++)
	{
		xSize = abs(wBoxes[i][0] - wBoxes[i][1]) / 2;
		ySize = abs(wBoxes[i][2] - wBoxes[i][3]) / 2;
		zSize = abs(wBoxes[i][4] - wBoxes[i][5]) / 2;

		xPos = (wBoxes[i][0] + wBoxes[i][1]) / 2;
		yPos = (wBoxes[i][2] + wBoxes[i][3]) / 2;
		zPos = (wBoxes[i][4] + wBoxes[i][5]) / 2;

		temp.setAABB(glm::vec3(xPos, yPos, zPos), glm::vec3(xSize, ySize, zSize), glm::vec3(-xSize, -ySize, -zSize));

		worldBoxes[0].push_back(temp);
	}
}

Physics* CreatePhysics()
{
	return new Physics();
}