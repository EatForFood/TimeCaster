#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "NavBox.h"
#include "CollisionDetection.h"
#include "Player.h"

using namespace std;
using namespace sf;

class Enemy : public Character
{
private:
	float START_SPEED = 25;
	float START_HEALTH = 100;
	string m_Type;

	CollisionDetection collision;
	vector<NavBox> navBoxes;

	FloatRect m_RenderArea;

	bool m_MoveLeft = false;
	bool m_MoveRight = false;
	bool m_MoveUp = false;
	bool m_MoveDown = false;
	bool m_IsMoving = false;

	int m_Chunk; // Enemy's current chunk

	Clock movementTimer;

	float m_Damage = 4; // Change this later to be set dynamically

	void move();
public:
	Enemy();

	void spawn(IntRect arena, Vector2f resolution, int tileSize, String type, int level);

	void update(float elapsedTime, vector<NavBox> navBox);

	float getCurrentHP();

	void healOverTime();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	void stopMoving();

	bool isEnemyMoving();

	void revertPosition();

	void setChunk(int chunk);

	int getChunk();

	FloatRect getRenderArea();

	float getDamage();
};