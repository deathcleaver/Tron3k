#ifndef MELEEATTACK_H
#define MELEEATTACK_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class MeleeAttack : public Bullet
{
private:
public:
	MeleeAttack(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID, int type);
	~MeleeAttack();

	int update(float dt);

};
#endif