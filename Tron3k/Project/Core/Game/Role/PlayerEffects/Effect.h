#ifndef EFFECT_H
#define EFFECT_H

#include <glm/glm.hpp>
#include <vector>

#include "../../GameDataIndex.h"

class Effect
{
protected:
	glm::vec3 pos;

	int playerId;
	int effectId;
	EFFECT_TYPE type;

	std::vector<int> playersHitByMe;
public:
	glm::vec3 getPos() { return pos; };
	void setPos(glm::vec3 p) { pos = p; };

	virtual void init(int pid, int eid, glm::vec3 position) = 0;
	void getId(int &pid, int &eid) { pid = playerId; eid = effectId; };
	virtual int update(float dt) = 0;

	virtual int getDamage() { return 0; };
	virtual void setDamage(int dmg) {};
	virtual void setInterestingVariable(float f) {};
	virtual float getInterestingVariable() { return -1; };

	void thisPlayerHit(int conid) { playersHitByMe.push_back(conid); };
	bool thisPlayerHasBeenHitByMe(int conid);

	EFFECT_TYPE getType() { return type; };
};
#endif