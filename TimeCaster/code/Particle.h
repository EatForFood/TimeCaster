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

	



	// Tell the calling code where the Particle is in the world
	FloatRect getPosition();

	// Return the actual shape (for drawing)
	RectangleShape getShape();

	// Update the Particle each frame
	void update(float elapsedTime);

	float bGetX();
	float bGetY();

	void setSpriteFromSheet(IntRect textureBox, int tileSize);
	void moveTextureRect();

	FloatRect getHitbox();

	Sprite getSprite();
};