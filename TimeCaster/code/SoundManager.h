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

	// Stop soundtrack function
	void stopSoundtrack();

	void playStartGameSound();

	void playHitSound();

	void playButtonClickSound();
};