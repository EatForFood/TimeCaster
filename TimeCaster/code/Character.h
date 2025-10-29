#pragma once
#include <SFML/Graphics.hpp>


using namespace sf;
using namespace std;

class Character
{
protected:
	Sprite m_Sprite;
	Sprite m_SpriteSheet; // sprite sheet for base character sprite

	Sprite m_SpriteHead;
	Sprite m_SpriteSheetHead; // sprite sheet for head armour

	Sprite m_SpriteTorso;
	Sprite m_SpriteSheetTorso; // sprite sheet for torso armour

	Sprite m_SpritePants;
	Sprite m_SpriteSheetPants; // sprite sheet for pants armour

	Sprite m_SpriteShoes;
	Sprite m_SpriteSheetShoes; // sprite sheet for shoes

	vector<Sprite> m_Clothes;

	// How long does a jump last
	float m_JumpDuration;

	// Is character currently jumping or falling
	bool m_IsJumping;
	bool m_IsFalling;

	// Which directions is the character currently moving in
	bool m_LeftPressed;
	bool m_RightPressed;
	bool m_UpPressed;
	bool m_DownPressed;

	// How long has this jump lasted so far
	float m_TimeThisJump;

	// Has the character just initialted a jump
	bool m_JustJumped = false;

	//which direction is the character headed? used to see if we need to reset the animation to first frame as we have changed direction
	Vector2f direction{ 0, -1 };

	// How fast is the character
	float m_Speed;

	// How much health the character can have & does have
	float m_Health;
	float m_MaxHealth;

	// Character level
	int m_Level;

	// Where is & was the character
	Vector2f m_Position;
	Vector2f m_PositionLast;

	// what is the character called?
	String m_Name;

	// Where are the characters various body parts?
	FloatRect m_Feet;
	FloatRect m_Head;
	FloatRect m_Right;
	FloatRect m_Left;

	FloatRect m_CollisionBox;

	// And a texture
	Texture m_Texture;

	Vector2i sheetCoordinate; // Coordinate on spritesheet
	Vector2i spriteSize;
	bool m_Alive{ true };
	int m_Animation_It_Limit; //Max animation iterations
	bool m_Horizontal{ true };
	int m_Ani_Counter{};
	int m_Counter{};
	Clock m_Clock;
	float m_TimeElapsed;
	//50 ms for each frame

	float m_AnimationTimer = 0;

	IntRect m_Hitbox;

	// Where the bullet should spawn from
	Vector2f m_Bullet_Spawn;

	// What is the screen resolution
	Vector2f m_Resolution;

	// What size is the current arena
	IntRect m_Arena;

	// How big is each tile of the arena
	int m_TileSize;

public:

	//void virtual update(float elapsedTime);
	//setSprite to use correct animation cell
	void setSpriteFromSheet(IntRect textureBox, int tileSize);

	Sprite getPants();
	Sprite getHead();
	Sprite getTorso();
	Sprite getShoes();

	//move the rectangle to the next cell in the animation
	void moveTextureRect();

	// Send a copy of the sprite to main
	Sprite getSprite();
	Sprite getSpriteFromSheet();

	// Where is the character
	void setPosition(Vector2f newPosition);
	Vector2f getPosition();

	FloatRect getGlobalBounds();

	float getY();

	// Where is the center of the character
	Vector2f getCenter();

	// Where is the origin of the character
	Vector2f getOrigin();

	FloatRect getColBox();

	// Which angle is the character facing
	float getRotation();

	// How much health has the character currently got?
	float getHealth();
	float getMaxHealth();

	void resetAniCounter();
	int getAniCounter();

};

