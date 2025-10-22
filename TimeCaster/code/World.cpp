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
	
}

void World::newWorld()
{
    int half = GRID_SIZE / 2;

    ofstream out("gamedata/TCWorld.txt");

    // Possible chunk types (excluding "spawn")
    vector<string> chunkTypes = {
        "forest", "forest", "forest",
        "goblinVillage",
    };

    // Set up random generator
    mt19937 rng(static_cast<unsigned int>(time(nullptr)));
    uniform_int_distribution<int> dist(0, static_cast<int>(chunkTypes.size()) - 1);

    for (int y = -half; y <= half; ++y)
    {
        for (int x = -half; x <= half; ++x)
        {
            if (x == 0 && y == 0)
            {
                // Center chunk is always spawn
                string type = "spawn";
                out << type << " " << x << " " << y << "\n";
                chunks.emplace_back(type, Vector2f(x, y), false);
            }
            else
            {
                // Choose a random type for other chunks
                string type = chunkTypes[dist(rng)];
                out << type << " " << x << " " << y << "\n";
                chunks.emplace_back(type, Vector2f(x, y), false);
            }
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

Chunk* World::getChunk(int i)
{
	return &chunks[i];
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