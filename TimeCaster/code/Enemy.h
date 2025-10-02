#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"

using namespace std;
using namespace sf;

class Enemy : public Character
{
private:
	float START_SPEED = 100;
	float START_HEALTH = 100;
	int m_level;
	String m_Type;

public:
	Enemy();

	void spawn(IntRect arena, Vector2f resolution, int tileSize, String type, int level);

	void update(float elapsedTime, Vector2i mousePosition);

	float getCurrentHP();

	void healOverTime();
};