#include "PulseRifle.h"

PulseRifle::PulseRifle()
{

}

PulseRifle::~PulseRifle()
{

}

void PulseRifle::init()
{
	weaponType = WEAPON_TYPE::PULSE_RIFLE;

	maxClipSize = 12;
	currentClipAmmo = 12;

	currentBulletId = 0;

	firingSpeed = 0.3f;
	firingSpeedCurrentDelay = 0.0f;

	reloadTime = 1.0f;
	rldTimer = 0.0f;
}

int PulseRifle::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}

void PulseRifle::setAmmo(int newAmmo)
{
	currentClipAmmo = newAmmo;
}