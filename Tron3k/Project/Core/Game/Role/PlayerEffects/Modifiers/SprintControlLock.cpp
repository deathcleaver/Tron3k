#include "SprintControlLock.h"
#include "../../../Player.h"

void SprintControlLock::init(Player* myTarget)
{
	type = MODIFIER_TYPE::SPRINTCONTROLLOCK;
	sprintSpeed = 2.5f;

	target = myTarget;
	lifeTime = 4.0f;

	glm::vec2 dir = glm::vec2(target->getDir().x, target->getDir().z);
	dir = normalize(dir);

	vel = glm::vec3(dir.x, 0, dir.y)*sprintSpeed;
	target->setVelocity(vel);
}


int SprintControlLock::getData(float dt)
{
 	bool kill = false;
	lifeTime -= dt;

	if (target->getVelocity().x != vel.x || target->getVelocity().z != vel.z)
		kill = true;
	if (lifeTime <= 0.0f)
		kill = true;

	if (kill)
		return 1;

	return 0;
}

int SprintControlLock::setData(float dt)
{
	vel += vec3(0.3, 0.0, 0.3) * vel * dt;
	vel.y = target->getVelocity().y;
	target->setVelocity(vel);

	return 0;
}