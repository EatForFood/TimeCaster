#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Particle
{
private:
	// Where is the Particle?
	Vector2f m_Position;

	// What each Particle looks like
	RectangleShape m_ParticleShape;

	// Is this Particle currently whizzing through the air
	bool m_InFlight = false;

	// How much mana does the Particle cost?
	float m_ManaCost;

	// Particle animation stuff
	Sprite m_Sprite;
	Texture m_Texture;
	Vector2i sheetCoordinate;
	Vector2i spriteSize;
	int m_Animation_It_Limit; //Max animation iterations
	bool m_Horizontal{ true };
	int m_Ani_Counter{};
	int m_Counter{};
	float m_TimeElapsed;
	float m_AnimationTimer = 0;


public:
	// The constructor
	Particle();

	// Stop the Particle
	void stop();

	// Returns the value of m_InFlight
	bool isInFlight();

	// Launch a new Particle
	void shoot(float startX, float startY,
		float xTarget, float yTarget, float damage);

	// Tell the calling code where the Particle is in the world
	FloatRect getPosition();

	// Return the actual shape (for drawing)
	RectangleShape getShape();

	// Update the Particle each frame
	void update(float elapsedTime, const std::vector<NavBox>& navBox);

	float bGetX();
	float bGetY();

	void setSpriteFromSheet(IntRect textureBox, int tileSize);
	void moveTextureRect();

	FloatRect getHitbox();

	Sprite getSprite();

	// Combat related functions
	float getParticleDamage();
	bool piercesEnemy();
	bool piercesObject();
};