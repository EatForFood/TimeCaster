#pragma once
#include <SFML/Graphics.hpp>
#include "Chunk.h"

using namespace sf;
using namespace std;

class World
{
private:

	vector<Chunk> chunks;

public:

	World();

	void newWorld();

	vector<Chunk> getChunks();

	vector<NavBox> getNavBoxes(int i);

	vector<Entity> getEntities(int i);

	void clearWorld();

	int getWorldSize();

	VertexArray getBackground(int i);

	VertexArray getForground(int i);

};
