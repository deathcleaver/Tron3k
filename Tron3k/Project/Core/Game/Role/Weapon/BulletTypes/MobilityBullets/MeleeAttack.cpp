#include "MeleeAttack.h"


MeleeAttack::MeleeAttack(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID, Player* player)
{
	initValues(position, direction, pID, bID, tID);
	vel = glm::vec3(25.0f);
	damage = 70;
	ttl = 0.15f;
	target = player;
	type = BULLET_TYPE::MELEE_ATTACK;
}

MeleeAttack::~MeleeAttack()
{}

int MeleeAttack::update(float dt)
{
	if (attackTimer >= 0)
		attackTimer -= dt;
	else
	{
		pos += dir * vel * dt;

		updateWorldMat();

		ttl -= dt;
		if (ttl <= 0)	//Bullet is no longer alive and should be removed
			return 1;
	}

	return 0;
}