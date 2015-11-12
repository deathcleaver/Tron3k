#ifndef SOUND_H
#define SOUND_H
#include<sfml/Audio.hpp>
#include<glm/glm.hpp>
#include<vector>
#include"SoundList.h"

using namespace std;

class SoundPlayer
{
private:
	bool soundEnabler;
	
	sf::SoundBuffer sounds[SOUNDS::nrOfSounds];
	string musicList[MUSIC::nrOfMusic];

	sf::Sound soundPlayer;	//Loads small sound files into the memory
	sf::Music musicPlayer;	//Streams big sound files

	sf::SoundBuffer soundBuffer;

public:
	SoundPlayer();
	~SoundPlayer();

	void enableSounds(bool sound);

	void setVolumeMusic(int volume);
	void setVolumeSound(int volume);

	int playMusic(int music);

	int SoundPlayer::playExternalSound(int sound, sf::Vector3f soundOrigin);
	int SoundPlayer::playUserGeneratedSound(int sound);

	void SoundPlayer::rotate(float deltaTime);
};

#endif