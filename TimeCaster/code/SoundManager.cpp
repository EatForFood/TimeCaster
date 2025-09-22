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
	// Loads hurt sound file into buffer
	// hurtBuffer.loadFromFile("sound/hit.wav");
	// hurt.setBuffer(hurtBuffer);

	// Seeding randomiser
	srand(time(0));
} // End of SoundManager constructor

// Function for placing songs within the soundtrack list
void SoundManager::populateSoundtrack() {
	//soundtrack[0] = soundtrack1;
	//soundtrack[1] = soundtrack2;
} // End of populateSoundtrack function

// Function for checking whether soundtrack is currently playing or not
bool SoundManager::isSoundtrackPlaying() {
	for (int i = 0; i < sizeof(soundtrack) / sizeof(Sound); i++)
	{
		if (soundtrack[i].getStatus() == SoundSource::Status::Playing) {
			return true;
		}
	}
	return false;
} // End of isSoundtrackPlaying function

// Function to play a random song from the soundtrack
void SoundManager::playSoundtrack() {
	soundtrack[rand() % sizeof(soundtrack) / sizeof(Sound)].play();
} // End of playSoundtrack function