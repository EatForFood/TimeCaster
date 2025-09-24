#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

using namespace sf;

class CreateBackground
{
private:

	const int TILE_SIZE = 64;
	const int VERTS_IN_QUAD = 4;
	int currentVertexBG = 0;
	int currentVertexFG = 0;
	VertexArray rVABG; // Vertex array for background tiles
	VertexArray rVAFG; // Vertex array for forground tiles

	bool forGround = false;

	bool debug;
	Font debugFont;
	std::vector<Text> debugText;

	std::vector<Entity> entities;

	bool setUp = true;

public:

	CreateBackground();

	void placeTile(int x, int y, int texX, int texY, bool forGround);

	VertexArray getBackground();
	VertexArray getForground();

	std::vector<Text> getDebugText();

	void CreateEntity(String type, int x, int y);

	std::vector<Entity> getEntities();

	int createLandscape();

	void placeHouse1(int x, int y);

	void placeTree1(int x, int y);
	void placeTree2(int x, int y);
	void placeTree3(int x, int y);
	void placeTree4(int x, int y);
	void placeTree5(int x, int y);
	void placeTree6(int x, int y);
	void placeTree7(int x, int y);
	void placeTree8(int x, int y);

};
