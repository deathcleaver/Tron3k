#include "SoundPlayer.h"
int SoundPlayer::nrOfSoundsPlaying = 0;
float SoundPlayer::soundVolume = 50.0f;
float SoundPlayer::musicVolume = 50.0f;
bool SoundPlayer::soundEnabler = false;
bool SoundPlayer::initialized = false;
#include <iostream>

//#include <vld.h>

SoundPlayer* SoundPlayer::singleton = nullptr;

void SoundPlayer::release()
{
	if (singleton)
	{
		delete singleton;
	}
}

void SoundPlayer::init(SoundPlayer* sound, int activateSound)
{
	singleton = sound;
	soundEnabler = activateSound;

	if (soundEnabler)
	{
		singleton->soundList[SOUNDS::soundEffectEnergyBoost].loadFromFile("GameFiles/Sound/soundEffectEnergyBoost.ogg");
		singleton->soundList[SOUNDS::soundEffectBulletPlayerHit].loadFromFile("GameFiles/Sound/soundEffectBulletPlayerHit.ogg");
		singleton->soundList[SOUNDS::soundEffectPusleRifleShot].loadFromFile("GameFiles/Sound/soundEffectPusleRifleShot.ogg");
		singleton->soundList[SOUNDS::firstBlood].loadFromFile("GameFiles/Sound/voiceFirstBlood.ogg");
		singleton->musicList[MUSIC::mainMenu] = "GameFiles/Sound/musicMainMenu.ogg";
		singleton->soundList[SOUNDS::soundEffectGrenadeLauncher].loadFromFile("GameFiles/Sound/soundEffectGrenadeLauncher.ogg");
		singleton->soundList[SOUNDS::soundEffectShotGun].loadFromFile("GameFiles/Sound/soundEffectShotGun.ogg");
		singleton->soundList[SOUNDS::soundEffectDiscGun].loadFromFile("GameFiles/Sound/soundEffectDiscGun.ogg");
		singleton->soundList[SOUNDS::soundEffectLinkGun].loadFromFile("GameFiles/Sound/soundEffectLinkGun.ogg");
		singleton->soundList[SOUNDS::soundEffectMelee].loadFromFile("GameFiles/Sound/soundEffectMelee.ogg");
		singleton->soundList[SOUNDS::soundFootSteps].loadFromFile("GameFiles/Sound/soundFootSteps.ogg");
		singleton->soundList[SOUNDS::soundStepsManipulator].loadFromFile("GameFiles/Sound/SoundStepsManipulator.ogg");
		singleton->soundList[SOUNDS::soundEffectTrapperReload].loadFromFile("GameFiles/Sound/soundEffectTrapperReload2.ogg");
		singleton->soundList[SOUNDS::soundEffectShield].loadFromFile("GameFiles/Sound/soundShield.ogg");
		singleton->soundList[SOUNDS::soundEffectDiscBounce].loadFromFile("GameFiles/Sound/soundEffectDiscBounce.ogg");
		singleton->soundList[SOUNDS::soundEffectBruteSteps].loadFromFile("GameFiles/Sound/soundEffectBruteSteps2.ogg");
		singleton->soundList[SOUNDS::soundEffectShankerSteps].loadFromFile("GameFiles/Sound/soundEffectShankerSteps.ogg");
		singleton->soundList[SOUNDS::soundEffectDestroyerSteps].loadFromFile("GameFiles/Sound/soundEffectDestroyerSteps.ogg");
		singleton->soundList[SOUNDS::soundEffectClusterGrenade].loadFromFile("GameFiles/Sound/soundEffectClusterGrenadeExplosion.ogg");
		singleton->soundList[SOUNDS::soundEffectDestroyerStop].loadFromFile("GameFiles/Sound/soundEffectDestroyerStop.ogg");
		singleton->soundList[SOUNDS::soundEffectDestroyerStart].loadFromFile("GameFiles/Sound/soundEffectDestroyerStart.ogg");
		singleton->soundList[SOUNDS::soundEffectVacuumGrenade].loadFromFile("GameFiles/Sound/soundEffectVacuumGrenade.ogg");
		singleton->soundList[SOUNDS::soundAids].loadFromFile("GameFiles/Sound/soundAids.ogg");
		singleton->musicList[MUSIC::cantinaSong] = "GameFiles/Sound/cantinaSong.ogg";
		singleton->soundList[SOUNDS::power].loadFromFile("GameFiles/Sound/Power.ogg");
		singleton->soundList[SOUNDS::soundEffectGrenadeBounce].loadFromFile("GameFiles/Sound/soundEffectGrenadeBounce.ogg");
		singleton->soundList[SOUNDS::soundEffectHackingDart].loadFromFile("GameFiles/Sound/soundEffectHackingDart.ogg");
		singleton->soundList[SOUNDS::soundEffectLightSpeed].loadFromFile("GameFiles/Sound/soundEffectLightSpeed2.ogg");
		singleton->soundList[SOUNDS::soundEffectTrapperJump].loadFromFile("GameFiles/Sound/soundEffectTrapperJump.ogg");
		singleton->soundList[SOUNDS::soundEffectTrapperLand].loadFromFile("GameFiles/Sound/soundEffectTrapperLand.ogg");
		singleton->soundList[SOUNDS::soundEffectTrapperMultiJump ].loadFromFile("GameFiles/Sound/soundEffectTrapperMultiJump.ogg");
		singleton->soundList[SOUNDS::soundEffectLightWall].loadFromFile("GameFiles/Sound/soundEffectLightWall.ogg");
		singleton->soundList[SOUNDS::soundEffectYouDied].loadFromFile("GameFiles/Sound/soundEffectYouDied.ogg");
		singleton->soundList[SOUNDS::soundEffectHunterJump].loadFromFile("GameFiles/Sound/soundEffectHunterJump.ogg");
		singleton->soundList[SOUNDS::soundEffectAlarm].loadFromFile("GameFiles/Sound/soundEffectAlarm.ogg");
		singleton->soundList[SOUNDS::soundEffectFlies].loadFromFile("GameFiles/Sound/soundEffectFlies.ogg");
		singleton->soundList[SOUNDS::soundEffectFrogs].loadFromFile("GameFiles/Sound/soundEffectFrogs.ogg");
		singleton->soundList[SOUNDS::soundEffectNeonSign].loadFromFile("GameFiles/Sound/soundEffectNeonSign.ogg");
		singleton->soundList[SOUNDS::soundEffectClusterlingExplosion].loadFromFile("GameFiles/Sound/soundEffectClusterlingExplosion.ogg");
		singleton->soundList[SOUNDS::soundEffectCrows].loadFromFile("GameFiles/Sound/soundEffectCrows.ogg");
		singleton->soundList[SOUNDS::BreakingOutBass].loadFromFile("GameFiles/Sound/BreakingOutBass.ogg");
		singleton->soundList[SOUNDS::soundEffectBruteLand].loadFromFile("GameFiles/Sound/soundEffectBruteLand.ogg");
		singleton->soundList[SOUNDS::soundEffectBruteJump].loadFromFile("GameFiles/Sound/soundEffectBruteJump.ogg");
		singleton->soundList[SOUNDS::soundEffectGrenadeLauncherBounce].loadFromFile("GameFiles/Sound/soundEffectGrenadeLauncherBounce.ogg");
		singleton->soundList[SOUNDS::soundEffectStalkerReload].loadFromFile("GameFiles/Sound/soundEffectStalkerReload.ogg");
		singleton->soundList[SOUNDS::YouWin].loadFromFile("GameFiles/Sound/YouWin.ogg");
		singleton->soundList[SOUNDS::YouLose].loadFromFile("GameFiles/Sound/YouLose.ogg");
		singleton->soundList[SOUNDS::SoundForOvertime].loadFromFile("GameFiles/Sound/SoundForOvertime.ogg");
		singleton->soundList[SOUNDS::announcer15Seconds].loadFromFile("GameFiles/Sound/announcer15Seconds.ogg");
		singleton->soundList[SOUNDS::announcer5Seconds].loadFromFile("GameFiles/Sound/announcer5Seconds.ogg");
		singleton->soundList[SOUNDS::announcer5Tokens].loadFromFile("GameFiles/Sound/announcer5Tokens.ogg");
		singleton->soundList[SOUNDS::announcerCaptureZoneBreached].loadFromFile("GameFiles/Sound/announcerCaptureZone.ogg");
		singleton->soundList[SOUNDS::announcerCommence].loadFromFile("GameFiles/Sound/announcerCommenceOvertime.ogg");
		singleton->soundList[SOUNDS::announcerImpressive].loadFromFile("GameFiles/Sound/announcerImpressive.ogg");
		singleton->soundList[SOUNDS::announcerKillingSpree].loadFromFile("GameFiles/Sound/announcerKillingSpree.ogg");
		singleton->soundList[SOUNDS::announcerRound1].loadFromFile("GameFiles/Sound/announcerRound1.ogg");
		singleton->soundList[SOUNDS::announcerRound2].loadFromFile("GameFiles/Sound/announcerRound2.ogg");
		singleton->soundList[SOUNDS::announcerRound3].loadFromFile("GameFiles/Sound/announcerRound3.ogg");
		singleton->soundList[SOUNDS::soundEffectCaptureScored].loadFromFile("GameFiles/Sound/soundEffectCaptureScored.ogg");
		singleton->soundList[SOUNDS::soundEffectBruteDash].loadFromFile("GameFiles/Sound/soundEffectBruteDash.ogg");
		singleton->soundList[SOUNDS::soundEffectThermiteGrenade].loadFromFile("GameFiles/Sound/soundEffectThermiteGrenade.ogg");
		singleton->soundList[SOUNDS::announcerYouLose].loadFromFile("GameFiles/Sound/announcerYouLose.ogg");
		singleton->soundList[SOUNDS::announcerYouWin].loadFromFile("GameFiles/Sound/announcerYouWin.ogg");
		singleton->soundList[SOUNDS::soundEffectPunisherReload].loadFromFile("GameFiles/Sound/soundEffectPunisherReload.ogg");
		singleton->soundList[SOUNDS::announcerCleanup].loadFromFile("GameFiles/Sound/announcerCleanup.ogg");
		singleton->soundList[SOUNDS::soundEffectHP].loadFromFile("GameFiles/Sound/soundEffectHP.ogg");
		singleton->soundList[SOUNDS::soundEffectRespawn].loadFromFile("GameFiles/Sound/soundEffectRespawn.ogg");
		singleton->soundList[SOUNDS::soundEffectBulletPlayerHitSelf].loadFromFile("GameFiles/Sound/soundEffectBulletPlayerHitSelf.ogg");
		singleton->soundList[SOUNDS::soundEffectBruteStepsStereo].loadFromFile("GameFiles/Sound/soundEffectBruteSteps2Stereo.ogg");
		singleton->soundList[SOUNDS::soundEffectDiscGunStereo].loadFromFile("GameFiles/Sound/soundEffectDiscGunStereo.ogg");
		singleton->soundList[SOUNDS::soundEffectEnergyBoostStereo].loadFromFile("GameFiles/Sound/soundEffectEnergyBoostStereo.ogg");
		singleton->soundList[SOUNDS::soundEffectGrenadeLauncherStereo].loadFromFile("GameFiles/Sound/soundEffectGrenadeLauncherStereo.ogg");
		singleton->soundList[SOUNDS::soundEffectHunterJumpStereo].loadFromFile("GameFiles/Sound/soundEffectHunterJumpStereo.ogg");
		singleton->soundList[SOUNDS::soundEffectLightSpeedStereo].loadFromFile("GameFiles/Sound/soundEffectLightSpeedStereo.ogg");
		singleton->soundList[SOUNDS::soundEffectLightWallStereo].loadFromFile("GameFiles/Sound/soundEffectLightWallStereo.ogg");
		singleton->soundList[SOUNDS::soundEffectMeleeStereo].loadFromFile("GameFiles/Sound/soundEffectMeleeStereo.ogg");
		singleton->soundList[SOUNDS::soundEffectPulseRifleShotStereo].loadFromFile("GameFiles/Sound/soundEffectPusleRifleShotStereo.ogg");
		singleton->soundList[SOUNDS::soundEffectShankerStepsStereo].loadFromFile("GameFiles/Sound/soundEffectShankerStepsStereo.ogg");
		singleton->soundList[SOUNDS::soundEffectShotGunStereo].loadFromFile("GameFiles/Sound/soundEffectShotGunStereo.ogg");
		singleton->soundList[SOUNDS::soundEffectTrapperJumpStereo].loadFromFile("GameFiles/Sound/soundEffectTrapperJumpStereo.ogg");
		singleton->soundList[SOUNDS::soundEffectTrapperLandStereo].loadFromFile("GameFiles/Sound/soundEffectTrapperLandStereo.ogg");
		singleton->soundList[SOUNDS::soundEffectTrapperMultiJumpStereo].loadFromFile("GameFiles/Sound/soundEffectTrapperMultiJumpStereo.ogg");
		singleton->soundList[SOUNDS::soundFootStepsStereo].loadFromFile("GameFiles/Sound/soundFootStepsStereo.ogg");
		singleton->soundList[SOUNDS::soundEffectBruteDashStereo].loadFromFile("GameFiles/Sound/soundEffectBruteDashStereo.ogg");

		initialized = true;
	}
}

SoundPlayer* SoundPlayer::getSound()
{
	return singleton;
}

SoundPlayer::SoundPlayer()
{

}

SoundPlayer::~SoundPlayer()
{
	
}

void SoundPlayer::playMelee(float x, float y, float z)
{
	for (int i = 0; i < 10; i++)
	{
	if (melee[i].getStatus() != 2)
		{
			melee[i].setBuffer(soundList[SOUNDS::soundEffectMelee]);
			melee[i].setPosition(x, y, z);
			melee[i].setVolume(50);
			melee[i].setAttenuation(80);
			melee[i].setMinDistance(10.0f);
			melee[i].play();
		}
	}
}

void SoundPlayer::playMeleeStereo()
{
	for (int i = 0; i < 10; i++)
	{
		if (melee[i].getStatus() != 2)
		{
			melee[i].setBuffer(soundList[SOUNDS::soundEffectMeleeStereo]);
			melee[i].setVolume(50);
			melee[i].setAttenuation(80);
			melee[i].setMinDistance(10.0f);
			melee[i].play();
			return;
		}
	}
}

float SoundPlayer::getVolumeMusic()
{
	return musicVolume;
}

void SoundPlayer::stopMusic()
{
	fading = true;
	//musicPlayer.stop();
}

int SoundPlayer::playJump(int role, float x, float y, float z)
{
	if (role == 0)
	{
			GetSound()->playExternalSound(SOUNDS::soundEffectTrapperJump, x, y, z);
	}

	else if (role == 2)
	{
		GetSound()->playExternalSound(SOUNDS::soundEffectHunterJump, x, y, z);
	}

	else if (role == 3)
	{
		//GetSound()->playExternalSound(SOUNDS::soundEffectBruteJump, x, y, z);
		//stopBrute();
	}
	else
	{
		return 0;
	}
}

int SoundPlayer::playLand(int role, float x, float y, float z)
{
	if (role == 0)
	{
		GetSound()->playExternalSound(SOUNDS::soundEffectTrapperLand, x, y, z);
	}

	else if (role == 2)
	{
		GetSound()->playExternalSound(SOUNDS::soundEffectTrapperLand, x, y, z);
	}

	else if (role == 3)
	{
		GetSound()->playExternalSound(SOUNDS::soundEffectBruteLand, x, y, z);
	}
	else
	{
		return 0;
	}
}


int SoundPlayer::playMapSounds()
{
	{
		if (soundEnabler == 1 && initialized == 1)
		{

			/*if (musicPlayer.getStatus() == sf::Sound::Playing)
			{
				theCantinaSong.stop();
			}*/

			if (!theCantinaSong.openFromFile(musicList[cantinaSong]))
			{
				return -1;
			}
			theCantinaSong.setPosition(45, 1.55, 95);
			theCantinaSong.setAttenuation(80);
			theCantinaSong.setVolume(20);
			theCantinaSong.play();
			theCantinaSong.setLoop(true);
			theCantinaSong.setMinDistance(9.5);
			
			/*mapSounds[0].setBuffer(soundList[SOUNDS::soundAids]);
			mapSounds[0].setPosition(-10, 5, -10);
			mapSounds[0].setVolume(30);
			mapSounds[0].setAttenuation(10);
			mapSounds[0].setMinDistance(5.0f);*/
			mapSounds[1].setBuffer(soundList[SOUNDS::BreakingOutBass]);
			mapSounds[1].setPosition(44, 1.55, 100);
			mapSounds[1].setVolume(50);
			mapSounds[1].setAttenuation(80);
			mapSounds[1].setMinDistance(25.0f);
			mapSounds[2].setBuffer(soundList[SOUNDS::soundEffectAlarm]);
			mapSounds[2].setPosition(73, 1.55, 4);
			mapSounds[2].setVolume(20);
			mapSounds[2].setAttenuation(20);
			mapSounds[2].setMinDistance(50.0f);
			mapSounds[3].setBuffer(soundList[SOUNDS::soundEffectFlies]);
			mapSounds[3].setPosition(51, 1.55, 108);
			mapSounds[3].setVolume(30);
			mapSounds[3].setAttenuation(3);
			mapSounds[3].setMinDistance(9.0f);
			mapSounds[4].setBuffer(soundList[SOUNDS::soundEffectFlies]);
			mapSounds[4].setPosition(-74, 1.55, 30);
			mapSounds[4].setVolume(30);
			mapSounds[4].setAttenuation(3);
			mapSounds[4].setMinDistance(9.0f);
			mapSounds[5].setBuffer(soundList[SOUNDS::soundEffectFrogs]);
			mapSounds[5].setPosition(-48, 1.55, 72);
			mapSounds[5].setVolume(10);
			mapSounds[5].setAttenuation(10);
			mapSounds[5].setMinDistance(4.0f);
			mapSounds[6].setBuffer(soundList[SOUNDS::soundEffectCrows]);
			mapSounds[6].setPosition(35, 1.55, 15);
			mapSounds[6].setVolume(20);
			mapSounds[6].setAttenuation(20);
			mapSounds[6].setMinDistance(40.0f);
			mapSounds[7].setBuffer(soundList[SOUNDS::soundEffectFlies]);
			mapSounds[7].setPosition(-32, 1.55, 47);
			mapSounds[7].setVolume(30);
			mapSounds[7].setAttenuation(3);
			mapSounds[7].setMinDistance(9.0f);
			mapSounds[8].setBuffer(soundList[SOUNDS::soundEffectNeonSign]);
			mapSounds[8].setPosition(24, 1.45, 59.8);
			mapSounds[8].setVolume(20);
			mapSounds[8].setAttenuation(5);
			mapSounds[8].setMinDistance(2.0f);
			mapSounds[9].setBuffer(soundList[SOUNDS::soundEffectNeonSign]);
			mapSounds[9].setPosition(27.8, 1.45, 59.8);
			mapSounds[9].setVolume(20);
			mapSounds[9].setAttenuation(5);
			mapSounds[9].setMinDistance(2.0f);
			mapSounds[10].setBuffer(soundList[SOUNDS::soundEffectNeonSign]);
			mapSounds[10].setPosition(-18.2, 1.45, 38.94);
			mapSounds[10].setVolume(20);
			mapSounds[10].setAttenuation(5);
			mapSounds[10].setMinDistance(2.0f);
			mapSounds[11].setBuffer(soundList[SOUNDS::soundEffectNeonSign]);
			mapSounds[11].setPosition(-50.29, 1.45, 60);
			mapSounds[11].setVolume(20);
			mapSounds[11].setAttenuation(5);
			mapSounds[11].setMinDistance(2.0f);
			mapSounds[12].setBuffer(soundList[SOUNDS::soundEffectNeonSign]);
			mapSounds[12].setPosition(-10.3, 5.6, 63.2);
			mapSounds[12].setVolume(20);
			mapSounds[12].setAttenuation(5);
			mapSounds[12].setMinDistance(2.0f);
			

			for (int i = 0; i < 13; i++)
			{
				mapSounds[i].setLoop(true);
				mapSounds[i].play();
			}
		}

		return 0;
	}

}

void SoundPlayer::enableSounds()
{
	if (!soundEnabler)
	{
		for (int i = 0; i < nrOfSoundsPlaying; i++)
		{
			sounds[i].setVolume(soundVolume);
		}
		soundEnabler = true;
		//playMusic(mainMenu);
		//musicPlayer.setVolume(musicVolume);
	}
	else
	{
		for (int i = 0; i < nrOfSoundsPlaying; i++)
		{
			sounds[i].stop();
		}
		musicPlayer.stop();
		soundEnabler = false;
	}
}
bool SoundPlayer::getSoundEnabler()
{
	return soundEnabler;
}

bool SoundPlayer::getInitialized()
{
	return initialized;
}

void SoundPlayer::setVolumeMusic(float volume)
{
	musicPlayer.setVolume(volume);
}

void SoundPlayer::setVolumeSound(float volume)
{
	this->soundVolume = volume;
}

int SoundPlayer::playUserGeneratedSound(int sound)
{
	if (soundEnabler && initialized == 1)
	{
		sounds[nrOfSoundsPlaying].setBuffer(soundList[sound]);
		sounds[nrOfSoundsPlaying].setRelativeToListener(true);
		sounds[nrOfSoundsPlaying].setPosition(0, 0, 0);
		sounds[nrOfSoundsPlaying].play();
		nrOfSoundsPlaying++;
		nrOfSoundsPlaying %= MAXSOUNDS;
	}

	return 0;
}

int SoundPlayer::playBrute(float x, float y, float z)
{
	if (sf::Listener::getPosition().x > x - 0.1 && sf::Listener::getPosition().x < x + 0.1)
	{
		if (sf::Listener::getPosition().z > z - 0.1 && sf::Listener::getPosition().z < z + 0.1)
		{
			bruteSteps.setRelativeToListener(true);
			bruteSteps.setPosition(0, 0, 0);

		}
	}

	else
	{
		bruteSteps.setRelativeToListener(false);
		bruteSteps.setPosition(x, y, z);			//Set the sound's position in the world. Could be passed in through a parameter.
	}

	//std::cout << "x: " << sf::Listener::getPosition().x << " y: " << sf::Listener::getPosition().y << " z: " << sf::Listener::getPosition().z << endl;
	//std::cout << "x: " << x << " y: " << y << " z: " << z << endl;
	//sounds[nrOfSoundsPlaying].isRelativeToListener();
	bruteSteps.setMinDistance(10.0f);		//Set the sound's distance it travels before it starts to attenuate. Could be passed in through a parameter.

	bruteSteps.setBuffer(soundList[SOUNDS::soundEffectBruteSteps]);
	bruteSteps.setVolume(50);
	bruteSteps.setLoop(false);
	bruteSteps.play();


return 0;
}


int SoundPlayer::playDestroyer(float x, float y, float z)
{
	
	if (soundEnabler && initialized == 1)
	{
		playDestroyerStart(x, y, z);
		if (sf::Listener::getPosition().x > x - 0.1 && sf::Listener::getPosition().x < x + 0.1)
		{
			if (sf::Listener::getPosition().z > z - 0.1 && sf::Listener::getPosition().z < z + 0.1)
			{
				destroyerSteps.setRelativeToListener(true);
				destroyerSteps.setPosition(0, 0, 0);

			}
		}

		else
		{
			destroyerSteps.setRelativeToListener(false);
			destroyerSteps.setPosition(x, y, z);			//Set the sound's position in the world. Could be passed in through a parameter.
		}

		//std::cout << "x: " << sf::Listener::getPosition().x << " y: " << sf::Listener::getPosition().y << " z: " << sf::Listener::getPosition().z << endl;
		//std::cout << "x: " << x << " y: " << y << " z: " << z << endl;
		//sounds[nrOfSoundsPlaying].isRelativeToListener();
		destroyerSteps.setMinDistance(10.0f);		//Set the sound's distance it travels before it starts to attenuate. Could be passed in through a parameter.

		destroyerSteps.setBuffer(soundList[SOUNDS::soundEffectDestroyerSteps]);
		destroyerSteps.setVolume(50);
		destroyerSteps.setLoop(true);
		destroyerSteps.play();
	}

	return 0;
}

int SoundPlayer::playDestroyerStop(float x, float y, float z)
{

	if (soundEnabler && initialized == 1)
	{
		if (sf::Listener::getPosition().x > x - 0.1 && sf::Listener::getPosition().x < x + 0.1)
		{
			if (sf::Listener::getPosition().z > z - 0.1 && sf::Listener::getPosition().z < z + 0.1)
			{
				destroyerStop.setRelativeToListener(true);
				destroyerStop.setPosition(0, 0, 0);

			}
		}

		else
		{
			destroyerStop.setRelativeToListener(false);
			destroyerStop.setPosition(x, y, z);			//Set the sound's position in the world. Could be passed in through a parameter.
		}

		//std::cout << "x: " << sf::Listener::getPosition().x << " y: " << sf::Listener::getPosition().y << " z: " << sf::Listener::getPosition().z << endl;
		//std::cout << "x: " << x << " y: " << y << " z: " << z << endl;
		//sounds[nrOfSoundsPlaying].isRelativeToListener();
		destroyerStop.setMinDistance(10.0f);		//Set the sound's distance it travels before it starts to attenuate. Could be passed in through a parameter.

		destroyerStop.setBuffer(soundList[SOUNDS::soundEffectDestroyerStop]);
		destroyerStop.setVolume(50);
		destroyerStop.setLoop(false);
		destroyerStop.play();
	}

	return 0;
}

int SoundPlayer::playDestroyerStart(float x, float y, float z)
{

	if (soundEnabler && initialized == 1)
	{
		if (sf::Listener::getPosition().x > x - 0.1 && sf::Listener::getPosition().x < x + 0.1)
		{
			if (sf::Listener::getPosition().z > z - 0.1 && sf::Listener::getPosition().z < z + 0.1)
			{
				destroyerStart.setRelativeToListener(true);
				destroyerStart.setPosition(0, 0, 0);

			}
		}

		else
		{
			destroyerStart.setRelativeToListener(false);
			destroyerStart.setPosition(x, y, z);			//Set the sound's position in the world. Could be passed in through a parameter.
		}

		//std::cout << "x: " << sf::Listener::getPosition().x << " y: " << sf::Listener::getPosition().y << " z: " << sf::Listener::getPosition().z << endl;
		//std::cout << "x: " << x << " y: " << y << " z: " << z << endl;
		//sounds[nrOfSoundsPlaying].isRelativeToListener();
		destroyerStop.setMinDistance(10.0f);		//Set the sound's distance it travels before it starts to attenuate. Could be passed in through a parameter.

		destroyerStart.setBuffer(soundList[SOUNDS::soundEffectDestroyerStart]);
		destroyerStart.setVolume(50);
		destroyerStart.setLoop(false);
		destroyerStart.play();
	}

	return 0;
}

int SoundPlayer::playExternalSound(int sound, float x, float y, float z)
{
	if (soundEnabler && initialized == 1)
	{
		if (sf::Listener::getPosition().x > x - 0.1 && sf::Listener::getPosition().x < x + 0.1)
		{
			if (sf::Listener::getPosition().z > z - 0.1 && sf::Listener::getPosition().z < z + 0.1)
			{
				sounds[nrOfSoundsPlaying].setRelativeToListener(true);
				sounds[nrOfSoundsPlaying].setPosition(0, 0, 0);

			}
		}

		else
		{
			sounds[nrOfSoundsPlaying].setRelativeToListener(false);
			sounds[nrOfSoundsPlaying].setPosition(x, y, z);			//Set the sound's position in the world. Could be passed in through a parameter.
		}

		//std::cout << "x: " << sf::Listener::getPosition().x << " y: " << sf::Listener::getPosition().y << " z: " << sf::Listener::getPosition().z << endl;
		//std::cout << "x: " << x << " y: " << y << " z: " << z << endl;
		//sounds[nrOfSoundsPlaying].isRelativeToListener();
		sounds[nrOfSoundsPlaying].setMinDistance(10.0f);		//Set the sound's distance it travels before it starts to attenuate. Could be passed in through a parameter.
		sounds[nrOfSoundsPlaying].setBuffer(soundList[sound]);
		sounds[nrOfSoundsPlaying].setVolume(soundVolume);
		sounds[nrOfSoundsPlaying].play();
		nrOfSoundsPlaying++;
		nrOfSoundsPlaying %= MAXSOUNDS;
	}

	return 0;
}


void SoundPlayer::setLocalPlayerDir(glm::vec3 playerDir)
{
	sf::Listener::setDirection(playerDir.x, 0.0, playerDir.z);
}

void SoundPlayer::setLocalPlayerPos(glm::vec3 playerPos)
{
	sf::Listener::setPosition(playerPos.x, playerPos.y, playerPos.z);
}

int SoundPlayer::playMusic(int music)
{
	if (soundEnabler == 1 && initialized == 1)
	{

		if (musicPlayer.getStatus() == sf::Sound::Playing)
		{
			musicPlayer.stop();
		}

		if (!musicPlayer.openFromFile(musicList[music]))
		{
			return -1;
		}

		musicPlayer.play();
		musicPlayer.setLoop(true);
	}

	return 0;
}

void SoundPlayer::rotate(float deltaTime)
{
	sf::Listener::setDirection(cos(deltaTime), 0.0f, sin(deltaTime));
}

SoundPlayer* CreateSound()
{
	return new SoundPlayer();
}

SoundPlayer* GetSound()
{
	return SoundPlayer::getSound();
}

bool GetSoundActivated()
{
	return SoundPlayer::getSoundEnabler();
}

bool GetInitialized()
{
	return SoundPlayer::getInitialized();
}

void InitSound(SoundPlayer* sound, int activateSound)
{
	SoundPlayer::init(sound, activateSound);
}

void ReleaseSound()
{
	SoundPlayer::release();
}

void SoundPlayer::playFootsteps(int role, float posX, float posY, float posZ)
{
	
		
		if (role == 0)
		{
			playExternalSound(SOUNDS::soundFootSteps, posX, posY, posZ);
		}

		if (role == 1)
		{
			playDestroyer(posX, posY, posZ);
		}

		if (role == 2)
		{
			playExternalSound(SOUNDS::soundEffectShankerSteps, posX, posY, posZ);
		}

		if (role == 3)
		{
			playExternalSound(SOUNDS::soundEffectBruteSteps, posX, posY, posZ);
			//playUserGeneratedSound(SOUNDS::soundEffectBruteSteps);
		}

		if (role == 4)
		{
			playExternalSound(SOUNDS::soundStepsManipulator, posX, posY, posZ);
		}
	
	
}

void SoundPlayer::PlayStereoFootsteps(int role)
{
	if (role == 0)
	{
		playUserGeneratedSound(SOUNDS::soundFootStepsStereo);
	}

	if (role == 2)
	{
		playUserGeneratedSound(SOUNDS::soundEffectShankerStepsStereo);
	}

	if (role == 3)
	{
		playUserGeneratedSound(SOUNDS::soundEffectBruteStepsStereo);
	}

}

