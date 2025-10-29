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
	
	/***************
	Soundtrack songs
	****************/

	soundtrack1Buffer.loadFromFile("sound/soundtrack1.ogg");
	soundtrack1.setBuffer(soundtrack1Buffer);

	soundtrack2Buffer.loadFromFile("sound/soundtrack2.ogg");
	soundtrack2.setBuffer(soundtrack2Buffer);

	soundtrack3Buffer.loadFromFile("sound/soundtrack3.ogg");
	soundtrack3.setBuffer(soundtrack3Buffer);

	soundtrack4Buffer.loadFromFile("sound/soundtrack4.ogg");
	soundtrack4.setBuffer(soundtrack4Buffer);

	/***********
	Other sounds/music
	************/
	
	// Loads start game sound file into buffer
	startGameBuffer.loadFromFile("sound/startGame.wav");
	startGame.setBuffer(startGameBuffer);

	// Loads hit sound file into buffer
	hitBuffer.loadFromFile("sound/hit.wav");
	hit.setBuffer(hitBuffer);

	// Loads click sound from file into buffer
	clickBuffer.loadFromFile("sound/buttonClick.wav");
	buttonClick.setBuffer(clickBuffer);

	// Loads story intro sound from file into buffer
	storyIntroBuffer.loadFromFile("sound/storyIntro.wav");
	storyIntro.setBuffer(storyIntroBuffer);

	// Loads boss intro sound from file into buffer
	bossIntroBuffer.loadFromFile("sound/bossIntro.wav");
	bossIntro.setBuffer(bossIntroBuffer);

} // End of SoundManager constructor

// Function for placing songs within the soundtrack list
void SoundManager::populateSoundtrack() 
{
	soundtrack.push_back(soundtrack1);
	soundtrack.push_back(soundtrack2);
	soundtrack.push_back(soundtrack3);
	soundtrack.push_back(soundtrack4);
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

// Function to stop current playing soundtrack song
void SoundManager::stopSoundtrack()
{
	for (auto& s : soundtrack) 
	{
		s.stop();
	}
}

// Function to play the start game sound
void SoundManager::playStartGameSound() {
	startGame.play();
}

// Function to play the hit sound
void SoundManager::playHitSound() {
	hit.play();
}

// Function to play the button click sound
void SoundManager::playButtonClickSound() {
	buttonClick.play();
}

// Function to play the story intro sound
void SoundManager::playStoryIntroSound() {
	storyIntro.play();
}

// Function to stop the story intro sound
void SoundManager::stopStoryIntroSound() {
	storyIntro.stop();
}

// Function to play the boss intro sound
void SoundManager::playBossIntroSound() {
	bossIntro.play();
}