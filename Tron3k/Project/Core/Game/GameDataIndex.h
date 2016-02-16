#ifndef GAME_DATA_INDEX_H
#define GAME_DATA_INDEX_H

enum WEAPON_TYPE
{
	PULSE_RIFLE,
	ENERGY_BOOST,
	DISC_GUN,
	MELEE,
	BATTERYWPN_SLOW,
	BATTERYWPN_SPEED,
	LINK_GUN,
	ENERGY_SHIELD,
	PLASMA_AUTORIFLE,
	GRENADE_LAUNCHER,
	SHOTGUN,
	NROFWEAPONS
};

enum SPECIAL_TYPE
{
	LIGHTWALL,
	MULTIJUMP,
	WALLJUMP,
	SPRINTD,
	HACKINGDARTSPECIAL,
	DASH,
	THUNDERDOME,
	LIGHTSPEEDSPECIAL,
	NROFSPECIALS
};

enum BULLET_TYPE
{
	PULSE_SHOT,
	DISC_SHOT,
	BATTERY_SPEED_SHOT,
	BATTERY_SLOW_SHOT,
	LINK_SHOT,
	PLASMA_SHOT,
	GRENADE_SHOT,
	SHOTGUN_PELLET,
	THERMITE_GRENADE,
	CLUSTER_GRENADE,
	CLUSTERLING,
	CLEANSE_BOMB,
	HACKING_DART,
	VACUUM_GRENADE,
	MELEE_ATTACK,
	KILLYOURSELF,
	NROFBULLETS
};

enum CONSUMABLE_TYPE
{
	CLUSTERGRENADE,
	OVERCHARGE,
	LIGHTSPEED,
	THERMITEGRENADE,
	VACUUMGRENADE,
	HACKINGDART,
	NROFCONSUMABLES
};

enum MODIFIER_TYPE
{
	LIGHTWALLCONTROLLOCK,
	HACKINGDARTMODIFIER,
	LIGHTSPEEDMODIFIER,
	OVERCHARGEMODIFIER,
	SPRINTCONTROLLOCK,
	TRUEGRITMODIFIER,
	TRAPPERSHAREAMMO,
	NROFMODIFIERS
};

enum EFFECT_TYPE
{
	LIGHT_WALL,
	THUNDER_DOME,
	EXPLOSION,
	CLEANSEEXPLOSION,
	BATTERY_SLOW,
	BATTERY_SPEED,
	THERMITE_CLOUD,
	ZEROFRICTION,
	VACUUM,
	HEALTHPACK,
	NROFEFFECTS
};

#endif