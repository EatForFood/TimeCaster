#include <SFML/Graphics.hpp>
#include "TimeCaster.h"
#include "World.h"
#include "Chunk.h"
#include <random>


using namespace std;
using namespace sf;

World::World()
{
	
}


void World::newWorld()
{
    int half = GRID_SIZE / 2; 

    for (int y = -half; y <= half; y++)
    {
        for (int x = -half; x <= half; x++)
        {
            if (x == 0 && y == 0) // center chunk is spawn
            {
                chunks.emplace_back("spawn", Vector2f(x, y));
            }
            else
            {
                chunks.emplace_back("forest", Vector2f(x, y));
            }
        }
    }
}

vector<Chunk> World::getChunks()
{
	return chunks;
}

Chunk World::getChunk(int i)
{
	return chunks[i];
}

vector<NavBox> World::getNavBoxes(int i)
{
	return chunks[i].getNavBoxes();
}

NavBox World::getChunkArea(int i)
{
	return chunks[i].getChunkArea();
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

Vector2f World::getChunkCenter(int i)
{
	return chunks[i].getChunkCenter();
}