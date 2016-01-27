#include "ThunderDomeEffect.h"

ThunderDomeEffect::ThunderDomeEffect(Player* p)
{
	myPlayer = p;
}

void ThunderDomeEffect::init(int pid, int eid, glm::vec3 position)
{
	type = EFFECT_TYPE::THUNDER_DOME;
	playerId = pid; effectId = eid;

	lifeTime = 10.0f;
}

int ThunderDomeEffect::update(float dt)
{
	lifeTime -= dt;
	if (lifeTime < FLT_EPSILON)
		return 1;
	return 0;
}