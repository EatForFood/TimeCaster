#pragma once
#include <sstream>
#include <fstream>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace sf;

class SoundManager
{
private:
	// Vector to store soundtrack songs
	vector<Sound> soundtrack;

	/***************
	Soundtrack songs
	****************/

	SoundBuffer soundtrack1Buffer;
	Sound soundtrack1;

	SoundBuffer soundtrack2Buffer;
	Sound soundtrack2;

	SoundBuffer soundtrack3Buffer;
	Sound soundtrack3;

	SoundBuffer soundtrack4Buffer;
	Sound soundtrack4;
	
	/***********
	Other sounds
	************/
	
	// Prepare the game over sound
	SoundBuffer gameOverBuffer;
	Sound gameOver;

	SoundBuffer mainMenuBuffer;
	Sound mainMenu;

	// Prepare the victory sound
	SoundBuffer victoryBuffer;
	Sound victory;
	
	// Prepare the start game sound
	SoundBuffer startGameBuffer;
	Sound startGame;

	// Prepare the hit sound
	SoundBuffer hitBuffer;
	Sound hit;

	// Prepare the fireball sound
	SoundBuffer fireballBuffer;
	Sound fireball;

	// Prepare the time stop cast sound
	SoundBuffer timeStopCastBuffer;
	Sound timeStopCast;

	// Prepare the time stop active sound
	SoundBuffer timeStopActiveBuffer;
	Sound timeStopActive;

	// Prepare the time stop end sound
	SoundBuffer timeStopEndBuffer;
	Sound timeStopEnd;

	// Prepare the heal spell sound
	SoundBuffer healBuffer;
	Sound heal;

	// Prepare the phase cast sound
	SoundBuffer phaseCastBuffer;
	Sound phaseCast;

	// Prepare the phase end sound
	SoundBuffer phaseEndBuffer;
	Sound phaseEnd;

	// Prepare the button click sound
	SoundBuffer clickBuffer;
	Sound buttonClick;

	// Prepare the boss intro sound
	SoundBuffer bossIntroBuffer;
	Sound bossIntro;

	// Prepare the story intro sound
	SoundBuffer storyIntroBuffer;
	Sound storyIntro;

	// Prepare the sword swing sound
	SoundBuffer swordSwingBuffer;
	Sound swordSwing;

	// Prepare the gold sound
	SoundBuffer goldBuffer;
	Sound gold;

	// Prepare the dragon roar sound
	SoundBuffer dragonRoarBuffer;
	Sound dragonRoar;

	// Prepare the dragon bite sound
	SoundBuffer dragonBiteBuffer;
	Sound dragonBite;

	// Prepare collect sound
	SoundBuffer collectBuffer;
	Sound collect;
public:
	// SoundManager constructor
	SoundManager();

	// Function to populate soundtrack function
	void populateSoundtrack();

	// Function to check whether soundtrack is currently playing or not
	bool isSoundtrackPlaying();

	// Function to play a random song from the soundtrack
	void playSoundtrack();

	// Stop soundtrack function
	void stopSoundtrack();

	// Function to play the start game sound
	void playStartGameSound();

	// Function to play the hit sound
	void playHitSound();

	// Function to play the Fireball sound
	void playFireballSound();

	// Function to play the time stop cast sound
	void playTimeStopCastSound();

	// Function to play the game over sound
	void playGameOverSound();

	// Function to stop the game over sound
	void stopGameOverSound();

	// Function to play the victory sound
	void playVictorySound();

	// Function to stop the victory sound
	void stopVictorySound();

	// Function to play the main menu sound
	void playMainMenuSound();

	// Function to stop the main menu sound
	void stopMainMenuSound();

	// Function to play the time stop active sound
	void playTimeStopActiveSound();

	// Function to end the time stop active sound
	void stopTimeStopActiveSound();

	// Function to play the time stop end sound
	void playTimeStopEndSound();

	// Function to play the heal sound
	void playHealSound();

	// Function to play the phase cast sound
	void playPhaseCastSound();

	// Function to play the phase end sound
	void playPhaseEndSound();

	// Function to play the button click sound
	void playButtonClickSound();

	// Function to play the story intro sound
	void playStoryIntroSound();

	// Function to stop the story intro sound
	void stopStoryIntroSound();

	// Function to play the boss intro sound
	void playBossIntroSound();

	void playSwordSwing();

	void playGoldSound();

	void playDragonRoar();

	void playDragonBite();

	void playCollectSound();
};