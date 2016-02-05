#include "BulletPulseShot.h"


PulseShot::PulseShot(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	initValues(position, direction, pID, bID, tID);
	vel = glm::vec3(90.0f);
	damage = 17;
}

PulseShot::~PulseShot()
{}

int PulseShot::update(float dt)
{
	pos += dir * vel * dt;

	updateWorldMat();
	
	ttl -= dt;
	if (ttl <= 0)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}