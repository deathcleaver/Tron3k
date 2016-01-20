#include "BatteryFields.h"

BatteryFields::BatteryFields()
{

}

BatteryFields::~BatteryFields()
{

}

bool BatteryFields::shoot()
{
	bool ableToShoot = false;

	if (rldTimer < FLT_EPSILON)
	{
		if (firingSpeedCurrentDelay < FLT_EPSILON && currentClipAmmo > 0)
		{
			currentClipAmmo--;
			firingSpeedCurrentDelay = firingSpeed;
			ableToShoot = true;
			if (weaponType == WEAPON_TYPE::BATTERYFIELD_SLOW)
				weaponType = WEAPON_TYPE::BATTERYFIELD_SPEED;
			else if (weaponType == WEAPON_TYPE::BATTERYFIELD_SPEED)
				weaponType = WEAPON_TYPE::BATTERYFIELD_SLOW;
		}
	}

	return ableToShoot;
}

bool BatteryFields::reload()
{
	rldTimer = reloadTime;
	weaponType = WEAPON_TYPE::BATTERYFIELD_SPEED;
	return true;
}

void BatteryFields::init()
{
	weaponType = WEAPON_TYPE::BATTERYFIELD_SPEED;

	maxClipSize = 2;
	currentClipAmmo = maxClipSize;

	currentBulletId = 0;

	firingSpeed = 0.0f;
	firingSpeedCurrentDelay = 0;

	reloadTime = 3.5f;
	rldTimer = 0;
}

int BatteryFields::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}