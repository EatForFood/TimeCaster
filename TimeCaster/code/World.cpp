#include <SFML/Graphics.hpp>
#include "Engine.h"
#include "World.h"
#include "Chunk.h"
#include <random>
#include <fstream>
#include <sstream>

using namespace std;
using namespace sf;


World::World()
{
    WORLD_SIZE = 3;
}

void World::newWorld()
{
    GRID_SIZE = WORLD_SIZE;
    int half = GRID_SIZE / 2;

    ofstream out("gamedata/TCWorld.txt");

    // Possible chunk types (excluding "spawn")
    vector<string> chunkTypes = {
        "forest", "forest", "forest",
        "goblinVillage", "skeletonRuins",
    };

    // Set up random generator (mercene twister)
    mt19937 rng(static_cast<unsigned int>(time(nullptr)));
    uniform_int_distribution<int> dist(0, static_cast<int>(chunkTypes.size()) - 1);

    // Loop over extended bounds: add 1 layer for worldBorder
    for (int y = -half - 1; y <= half + 1; ++y)
    {
        for (int x = -half - 1; x <= half + 1; ++x)
        {
            string type;

            // Center chunk is always spawn
            if (x == 0 && y == 0)
            {
                type = "spawn";
            }
            else if (x == 0 && y == -1)
            {
                type = "bossArena";
            }
            // Border layer
            else if (x == -half - 1 || x == half + 1 || y == -half - 1 || y == half + 1)
            {
                type = "worldBorder";
            }
            // Inner world chunks
            else
            {
                type = chunkTypes[dist(rng)];
            }

            out << type << " " << x << " " << y << "\n";
            chunks.emplace_back(type, Vector2f(x, y), false);
        }
    }

    out.close();
}

void World::loadWorld() // load world from file
{
    ifstream in("gamedata/TCWorld.txt");
    if (!in.is_open())
        return;

    chunks.clear();

    string line;
    while (getline(in, line))
    {
        istringstream ss(line);
        string type;
        float x, y;

        ss >> type >> x >> y;

        // Recreate chunk, mark as loaded
        chunks.emplace_back(type, Vector2f(x, y), true);
    }

    in.close();
}

bool World::worldFileExists()
{
    ifstream in("gamedata/TCWorld.txt");
    if (!in.is_open())
    {
        return false;
    }
    else 
    {
		in.close();
		return true;
    }
     
}

Chunk* World::getChunk(int i)
{
	return &chunks[i];
}

const vector<NavBox>& World::getNavBoxes(int i) const
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

void World::setWorldSize(int size) 
{
    WORLD_SIZE = size;
}