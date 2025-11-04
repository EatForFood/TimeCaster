#pragma once
#include <SFML/Graphics.hpp>


using namespace sf;

class Spell
{
private:
	// Where is the spell?
	Vector2f m_Position;

	// What each spell looks like
	RectangleShape m_SpellShape;

	// Is this spell currently whizzing through the air
	bool m_InFlight = false;

	// How fast does a spell travel?
	float m_SpellSpeed = 300;

	// What fraction of 1 pixel does the spell travel, 
	// Horizontally and vertically each frame?
	// These values will be derived from m_SpellSpeed
	float m_SpellDistanceX;
	float m_SpellDistanceY;

	// Where is this spell headed to?
	float m_XTarget;
	float m_YTarget;

	// Some boundaries so the spell doesn't fly forever
	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;

	// How much Damage will the spell do? Set to 0 if it's non-damaging spell
	float m_SpellDamage;

	// Will the spell pierce through enemies or objects?
	bool m_PierceEnemy;
	bool m_PierceObject;

	// spell animation stuff
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

// Public function prototypes go here
public:
	// The constructor
	Spell();

	// Stop the spell
	void stop();

	// Returns the value of m_InFlight
	bool isInFlight();

	// Launch a new spell
	void shoot(float startX, float startY,
		float xTarget, float yTarget);

	// Tell the calling code where the spell is in the world
	FloatRect getPosition();

	// Return the actual shape (for drawing)
	RectangleShape getShape();

	// Update the spell each frame
	void update(float elapsedTime);

	float bGetX();
	float bGetY();

	void setSpriteFromSheet(IntRect textureBox, int tileSize);
	void moveTextureRect();

	

	Sprite getSprite();

	// Combat related functions
	float getSpellDamage();
	bool piercesEnemy();
	bool piercesObject();
};