#ifndef EXPLOSIONEFFECT_H
#define EXPLOSIONEFFECT_H

#include "../Effect.h"
#include "../../../Player.h"

class Explosion : public Effect
{
private:
	float lifeTime;
	float exploRadius;
	float damage;
public:
	Explosion();
	void init(int pid, int eid, glm::vec3 position);
	void setInterestingVariable(float f) { exploRadius = f; }
	float getInterestingVariable() { return exploRadius; }
	int update(float dt);

	int getDamage() { return damage; }; //WOOO DAMAGE YO
	void setDamage(int dmg) { damage = dmg; };
};
#endif