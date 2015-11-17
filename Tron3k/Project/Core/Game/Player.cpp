#include "Player.h"

Player::Player()
{
	
}

void Player::init(std::string pName, glm::vec3 initPos, bool isLocal)
{
	name = pName;
	pos = initPos;

	isLocalPlayer = isLocal;

	i = Input::getInput();
	cam = CameraInput::getCam();

	if (isLocal)
		rotate(0, -3.141592654f, 0);
}

void Player::setName(std::string newName)
{
	name = newName;
}

void Player::setGoalPos(glm::vec3 newPos)
{
	goalpos = newPos;
	pos = newPos; //Temporary
}

void Player::setGoalDir(glm::vec3 newDir)
{
	goaldir = newDir;
	rotatePlayer(dir, goaldir);
	dir = newDir; //Temporary 
}

void Player::update(float dt)
{
	if (isLocalPlayer)
	{
		vec3 olddir = cam->getDir();
		cam->update(dt, false);
		dir = cam->getDir();

		if (i->getKeyInfo(GLFW_KEY_W))
			pos += dir * dt;

		if (i->getKeyInfo(GLFW_KEY_S))
			pos -= dir * dt;

		if (i->getKeyInfo(GLFW_KEY_A))
		{
			vec3 left = cross(vec3(0, 1, 0), dir);
			left = normalize(left);
			pos += left * dt;
		}
		if (i->getKeyInfo(GLFW_KEY_D))
		{
			vec3 left = cross(dir, vec3(0, 1, 0));
			left = normalize(left);
			pos += left * dt;
		}

		cam->setCam(pos, dir);
		if (olddir != dir)
			rotatePlayer(olddir, dir);
	}
	
	worldMat[0].w = pos.x;
	worldMat[1].w = pos.y - 1.5f;
	worldMat[2].w = pos.z;
}

void Player::rotatePlayer(vec3 olddir, vec3 newdir)
{
	//vec3 oldIgnoreY = vec3(olddir.x, 0, olddir.z);
	//vec3 newIgnoreY = vec3(newdir.x, 0, newdir.z);

	//float angle = (dot(oldIgnoreY, newIgnoreY)) / (oldIgnoreY.length() * newIgnoreY.length());
	float angle = atan2(newdir.x, newdir.z) - atan2(olddir.x, olddir.z);
	rotate(0, -angle, 0);
}