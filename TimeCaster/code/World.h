#pragma once
#include <SFML/Graphics.hpp>
#include "Chunk.h"

using namespace sf;
using namespace std;

class World
{
private:

	const int WORLD_SIZE = 9;
	int GRID_SIZE = sqrt(WORLD_SIZE);
	vector<Chunk> chunks;

	Vector2i m_TileTypeF[50][50];
	String m_TileEntityF[50][50];
	Vector2i m_TileTypeFGroundF[50][50];

public:

	World();

	void newWorld();

	void loadWorld();

	vector<Chunk> getChunks();

	Chunk getChunk(int i);

	vector<NavBox> getNavBoxes(int i);

	vector<Entity> getEntities(int i);

	void clearWorld();

	int getWorldSize();

	NavBox getChunkArea(int i);

	Vector2f getChunkCenter(int i);

	VertexArray getBackground(int i);

	VertexArray getForground(int i);

};
