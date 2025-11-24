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

	/*****************
	Other sounds/music
	*****************/
	
	// Loads start game sound file into buffer
	startGameBuffer.loadFromFile("sound/startGame.wav");
	startGame.setBuffer(startGameBuffer);

	// Loads hit sound file into buffer
	hitBuffer.loadFromFile("sound/hit.wav");
	hit.setBuffer(hitBuffer);

	// Loads fireball sound 
	fireballBuffer.loadFromFile("sound/fireball.wav");
	fireball.setBuffer(fireballBuffer);

	// Loads time stop cast sound into buffer
	timeStopCastBuffer.loadFromFile("sound/timeStopCast.wav");
	timeStopCast.setBuffer(timeStopCastBuffer);

	// Loads time stop active sound into buffer
	timeStopActiveBuffer.loadFromFile("sound/timeStopActive.wav");
	timeStopActive.setBuffer(timeStopActiveBuffer);

	// Loads time stop end sound into buffer
	timeStopEndBuffer.loadFromFile("sound/timeStopEnd.wav");
	timeStopEnd.setBuffer(timeStopEndBuffer);

	// Loads the phase cast sound into buffer
	phaseCastBuffer.loadFromFile("sound/phaseCast.wav");
	phaseCast.setBuffer(phaseCastBuffer);

	// Loads the phase end sound into buffer
	phaseEndBuffer.loadFromFile("sound/phaseEnd.wav");
	phaseEnd.setBuffer(phaseEndBuffer);

	// Loads heal spell sound into buffer
	healBuffer.loadFromFile("sound/heal.mp3");
	heal.setBuffer(healBuffer);

	// Loads game over sound from file into buffer
	gameOverBuffer.loadFromFile("sound/gameOver.mp3");
	gameOver.setBuffer(gameOverBuffer);

	// Loads victory sound from file into buffer
	victoryBuffer.loadFromFile("sound/victory.mp3");
	victory.setBuffer(victoryBuffer);

	// Loads click sound from file into buffer
	clickBuffer.loadFromFile("sound/buttonClick.wav");
	buttonClick.setBuffer(clickBuffer);

	// Loads story intro sound from file into buffer
	storyIntroBuffer.loadFromFile("sound/storyIntro.wav");
	storyIntro.setBuffer(storyIntroBuffer);
	 
	// Loads boss intro sound from file into buffer
	bossIntroBuffer.loadFromFile("sound/bossIntro.wav");
	bossIntro.setBuffer(bossIntroBuffer);

	// Loads main menu sound from file into buffer
	mainMenuBuffer.loadFromFile("sound/mainMenu.mp3");
	mainMenu.setBuffer(mainMenuBuffer);

	swordSwingBuffer.loadFromFile("sound/swordSwing.wav");
	swordSwing.setBuffer(swordSwingBuffer);

	goldBuffer.loadFromFile("sound/coin.mp3");
	gold.setBuffer(goldBuffer);

	dragonRoarBuffer.loadFromFile("sound/dragonRoar.wav");
	dragonRoar.setBuffer(dragonRoarBuffer);

	dragonBiteBuffer.loadFromFile("sound/dragonBite.wav");
	dragonBite.setBuffer(dragonBiteBuffer);
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

// Function to play the Fireball sound
void SoundManager::playFireballSound() {
	fireball.play();
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

void SoundManager::playSwordSwing() {
	if (swordSwing.getStatus() != SoundSource::Status::Playing)
	swordSwing.play();
}

void SoundManager::playTimeStopCastSound() {
	timeStopCast.play();
}

void SoundManager::playTimeStopActiveSound() {
	if (timeStopActive.getStatus() != SoundSource::Status::Playing)
		timeStopActive.play();
}

void SoundManager::stopTimeStopActiveSound() {
	timeStopActive.stop();
}

void SoundManager::playTimeStopEndSound() {
	timeStopEnd.play();
}

void SoundManager::playHealSound() {
	if (heal.getStatus() != SoundSource::Status::Playing)
		heal.play();
}

void SoundManager::playPhaseCastSound() {
	phaseCast.play();
}

void SoundManager::playPhaseEndSound() {
	phaseEnd.play();
}

void SoundManager::playGoldSound() {
	gold.play();
}

void SoundManager::playGameOverSound() {
	gameOver.play();
}

void SoundManager::stopGameOverSound() {
	gameOver.stop();
}

void SoundManager::playDragonRoar() {
	dragonRoar.play();
}

void SoundManager::playDragonBite() {
	dragonBite.play();
}

void SoundManager::playVictorySound() {
	victory.play();
}

void SoundManager::stopVictorySound() {
	victory.stop();
}