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

	// Prepare the button click sound
	SoundBuffer clickBuffer;
	Sound buttonClick;

	// Prepare the boss intro sound
	SoundBuffer bossIntroBuffer;
	Sound bossIntro;

	// Prepare the story intro sound
	SoundBuffer storyIntroBuffer;
	Sound storyIntro;

	SoundBuffer swordSwingBuffer;
	Sound swordSwing;

	SoundBuffer goldBuffer;
	Sound gold;
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

	// Function to play the time stop active sound
	void playTimeStopActiveSound();

	// Function to end the time stop active sound
	void stopTimeStopActiveSound();

	// Function to play the time stop end sound
	void playTimeStopEndSound();

	// Function to play the heal sound
	void playHealSound();

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
};