#pragma once
#include <sstream>
#include <fstream>
#include <SFML/Audio.hpp>
#include <iostream>
#include <list>

using namespace sf;

class SoundManager
{
private:
	// List to store soundtrack songs
	Sound soundtrack[2];

	// Prepare the hit sound
	//SoundBuffer hitBuffer;
	//Sound hit;

public:
	// SoundManager constructor
	SoundManager();

	// Populate soundtrack function
	void populateSoundtrack();

	// Checks whether soundtrack is currently playing or not
	bool isSoundtrackPlaying();

	// Play soundtrack function
	void playSoundtrack();
};