#pragma once
#include <SFML/Graphics.hpp>
#include "Chunk.h"

class World
{
private:

	int m_WorldSize = 3;
	int GRID_SIZE;
	vector<Chunk> m_Chunks;
	bool m_BossArenaPlaced = false;

	Vector2i m_BossArenaPos;
	vector<Vector2i> m_BossCandidates;

	Vector2i m_TileTypeF[50][50];
	String m_TileEntityF[50][50];
	Vector2i m_TileTypeFGroundF[50][50];

public:

	World();

	void newWorld();

	void loadWorld();

	bool worldFileExists();

	Chunk* getChunk(int i);

	const vector<NavBox>& getNavBoxes(int i) const;

	vector<Entity> getEntities(int i);

	void clearWorld();

	int getWorldSize();

	NavBox getChunkArea(int i);

	Vector2f getChunkCenter(int i);

	VertexArray getBackground(int i);

	VertexArray getForground(int i);

	void setWorldSize(int size);
};
