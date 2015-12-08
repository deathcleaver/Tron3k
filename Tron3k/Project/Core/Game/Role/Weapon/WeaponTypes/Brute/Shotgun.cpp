#include "Shotgun.h"

Shotgun::Shotgun()
{

}

Shotgun::~Shotgun()
{

}

void Shotgun::init()
{
	weaponType = WEAPON_TYPE::SHOTGUN;

	maxClipSize = 5;
	currentClipAmmo = maxClipSize;

	currentBulletId = 0;

	firingSpeed = 2.0f;
	firingSpeedCurrentDelay = firingSpeed;

	reloadTime = 2.5f;
	rldTimer = reloadTime;
}

int Shotgun::update(float deltaTime)
{
	return 0;
}