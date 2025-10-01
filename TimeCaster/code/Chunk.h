#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "NavBox.h"

using namespace sf;
using namespace std;

class Chunk
{
protected:

	const int TILE_SIZE = 64;
	const int CHUNK_SIZE = 50;
	const int VERTS_IN_QUAD = 4;

	int offsetX; // amount to offset position of chunk by
	int offsetY;

	Vector2f offset;
	Vector2f chunkCenter;


	int currentVertexBG = 0;
	int currentVertexFG = 0;
	VertexArray rVABG; // Vertex array for background tiles
	VertexArray rVAFG; // Vertex array for forground tiles

	bool forGround = false;

	String m_Type; // type of chunk to generate

	bool debug;
	Font debugFont;
	vector<Text> debugText;

	vector<Entity> entities;

	NavBox chunkArea;
	vector<NavBox> navBoxes;


public:

	Chunk(String type, Vector2f chunk);

	void placeTile(int x, int y, int texX, int texY, bool forGround);

	VertexArray getBackground();
	VertexArray getForground();

	vector<Text> getDebugText();

	void CreateEntity(String type, int x, int y);

	vector<Entity> getEntities();

	vector<NavBox> getNavBoxes();

	NavBox getChunkArea();
	Vector2f getChunkCenter();

	int createLandscape();

	void clearChunk();

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
