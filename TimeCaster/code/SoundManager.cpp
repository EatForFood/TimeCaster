#include <sstream>
#include <fstream>
#include <SFML/Audio.hpp>
#include "SoundManager.h"
#include <iostream>
#include <list>

using namespace std;
using namespace sf;

// SoundManager constructor
SoundManager::SoundManager() {
	// Loads hit sound file into buffer
	//hitBuffer.loadFromFile("sound/hit.wav");
	//hit.setBuffer(hitBuffer);

	// Seeding randomiser
	srand(time(0));
}

// Function for placing songs within the soundtrack list
void SoundManager::populateSoundtrack() {
	//soundtrack[0] = soundtrack1;
	//soundtrack[1] = soundtrack2;
}

// Function for checking whether soundtrack is currently playing or not
bool SoundManager::isSoundtrackPlaying() {
	for (int i = 0; i < sizeof(soundtrack) / sizeof(Sound); i++)
	{
		if (soundtrack[i].getStatus() == SoundSource::Status::Playing) {
			return true;
		}
	}
	return false;
}

// Function to play a random song from the soundtrack
void SoundManager::playSoundtrack() {
	soundtrack[rand() % sizeof(soundtrack) / sizeof(Sound)].play();
}