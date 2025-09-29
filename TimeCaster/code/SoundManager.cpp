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
	// Seeding randomiser
	srand(time(0));
	
	// Loads start game sound file into buffer
	startGameBuffer.loadFromFile("sound/startGame.wav");
	startGame.setBuffer(startGameBuffer);

	// Loads hit sound file into buffer
	hitBuffer.loadFromFile("sound/hit.wav");
	hit.setBuffer(hitBuffer);

	// Loads click sound from file into buffer
	clickBuffer.loadFromFile("sound/buttonClick.wav");
	buttonClick.setBuffer(clickBuffer);

	soundTrack1Buffer.loadFromFile("sound/soundtrack1.ogg");
	soundtrack1.setBuffer(soundTrack1Buffer);

} // End of SoundManager constructor

// Function for placing songs within the soundtrack list
void SoundManager::populateSoundtrack() 
{
	soundtrack.push_back(soundtrack1);
	//soundtrack.push_back(soundtrack2);
} // End of populateSoundtrack function

// Function for checking whether soundtrack is currently playing or not
bool SoundManager::isSoundtrackPlaying() 
{
	for (auto& s : soundtrack) 
	{
		if (s.getStatus() == SoundSource::Status::Playing) {
			return true;
		}
	}
	return false;
} // End of isSoundtrackPlaying function

// Function to play a random song from the soundtrack
void SoundManager::playSoundtrack() 
{
	if (!soundtrack.empty()) 
	{
		soundtrack[rand() % soundtrack.size()].play();
	}
} // End of playSoundtrack function

void SoundManager::stopSoundtrack()
{
	for (auto& s : soundtrack) 
	{
		s.stop();
	}
}

void SoundManager::playStartGameSound() {
	startGame.play();
}

void SoundManager::playHitSound() {
	hit.play();
}

void SoundManager::playButtonClickSound() {
	buttonClick.play();
}