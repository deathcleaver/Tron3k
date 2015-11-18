#ifndef WEAPON_H
#define WEAPON_H

#include "Bullet.h"

class Weapon
{
	private:
		int maxClipSize;
		int currentClipAmmo;
		
		float firingSpeed;
		float currentDelay;

		int weaponType;
	public:
		Weapon();
		~Weapon();

		void init(int maxClipSize, int weaponType, float firingSpeed);
		bool shoot();
		bool reload();

		void update(float deltaTime);

		int getType() { return weaponType; };
};
#endif