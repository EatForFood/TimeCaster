#pragma once
#include <sstream>
#include <fstream>
#include <SFML/Audio.hpp>
#include <iostream>
#include <list>

using namespace std;
using namespace sf;

class SoundManager
{
private:
	// List to store soundtrack songs
	Sound soundtrack[1];

	// Prepare the start game sound
	SoundBuffer startGameBuffer;
	Sound startGame;

	// Prepare the hit sound
	SoundBuffer hitBuffer;
	Sound hit;

	SoundBuffer clickBuffer;
	Sound buttonClick;

	SoundBuffer soundTrack1Buffer;
	Sound soundtrack1;

public:
	// SoundManager constructor
	SoundManager();

	// Populate soundtrack function
	void populateSoundtrack();

	// Checks whether soundtrack is currently playing or not
	bool isSoundtrackPlaying();

	// Play soundtrack function
	void playSoundtrack();

	void playStartGameSound();

	void playHitSound();

	void playButtonClickSound();
};