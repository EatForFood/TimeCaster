#include <SFML/Graphics.hpp>
#include "TimeCaster.h"
#include "World.h"
#include "Chunk.h"


using namespace std;
using namespace sf;

World::World()
{
	
}


void World::newWorld()
{
	chunks.emplace_back("grass", Vector2f(-1, -1));
	chunks.emplace_back("grass", Vector2f(-1, 0));
	chunks.emplace_back("grass", Vector2f(-1, +1));
	chunks.emplace_back("grass", Vector2f(0, -1));
	chunks.emplace_back("spawn", Vector2f(0, 0));
	chunks.emplace_back("grass", Vector2f(0, +1));
	chunks.emplace_back("grass", Vector2f(+1, -1));
	chunks.emplace_back("grass", Vector2f(+1, 0));
	chunks.emplace_back("grass", Vector2f(+1, +1));

}

vector<Chunk> World::getChunks()
{
	return chunks;
}

vector<NavBox> World::getNavBoxes(int i)
{
	return chunks[i].getNavBoxes();
}

vector<Entity> World::getEntities(int i)
{
	return chunks[i].getEntities();
}

void World::clearWorld()
{
	for (size_t i = 0; i < chunks.size(); i++)
	{
		chunks[i].clearChunk();
	}
	chunks.clear();
}

int World::getWorldSize()
{
	return chunks.size();
}

VertexArray World::getBackground(int i) {

	return chunks[i].getBackground();
}

VertexArray World::getForground(int i) {

	return chunks[i].getForground();
}