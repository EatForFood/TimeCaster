#include <SFML/Graphics.hpp>
#include "Engine.h"
#include "World.h"
#include "Chunk.h"
#include <random>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;
using namespace sf;


World::World()
{
    
}

void World::newWorld()
{
    cout << "new world" << endl;
    m_BossCandidates.clear();
    GRID_SIZE = m_WorldSize;
    int halfWorld = GRID_SIZE / 2;

    ofstream out("gamedata/TCWorld.txt");

    // all possible chunk types
    vector<string> chunkTypes = {
        "forest", "forest", "forest",
        "goblinVillage", "skeletonRuins",
    };

    for (int y = -halfWorld - 1; y <= halfWorld + 1; ++y)
    {
        for (int x = -halfWorld - 1; x <= halfWorld + 1; ++x)
        {
            int dist = abs(x) + abs(y);

            // all valid positions for boss arena
            if (dist >= halfWorld / 2 && dist <= halfWorld)
            {
                m_BossCandidates.emplace_back(Vector2i(x, y));
            }
        }
    }

    // randomise RNG
    srand(time(NULL));
    int chance = rand() % (m_BossCandidates.size());
    m_BossArenaPos = m_BossCandidates[chance];

    // loop over world size from keeping 0,0 in the middle + add 1 layer for worldBorder
    for (int y = -halfWorld - 1; y <= halfWorld + 1; ++y)
    {
        for (int x = -halfWorld - 1; x <= halfWorld + 1; ++x)
        {
            int chance = rand() % (chunkTypes.size());
            string type;

            // center chunk is always spawn (0,0)
            if (x == 0 && y == 0)
            {
                type = "spawn";
            }
            else
            {
                int dist = abs(x) + abs(y);

                if (x == m_BossArenaPos.x && y == m_BossArenaPos.y)
                {
                    type = "bossArena";
                    m_BossArenaPlaced = true;
                    cout << "boss at: (" << x << "," << y << ")" << endl;
                }
                // world border chunks
                else if (x == -halfWorld - 1 || x == halfWorld + 1 || y == -halfWorld - 1 || y == halfWorld + 1)
                {
                    type = "worldBorder";
                }
                // pick random chunk from chunk pool
                else
                {
                    type = chunkTypes[chance];
                }
            }

                out << type << " " << x << " " << y << "\n"; // save chunk type to TCWorld.txt

                m_Chunks.emplace_back(type, Vector2f(x, y), false);      
        }
    }

    out.close();
}


void World::loadWorld() // load world from file
{
    ifstream in("gamedata/TCWorld.txt");
    if (!in.is_open())
        return;

    m_Chunks.clear();

    string line;
    while (getline(in, line))
    {
        istringstream ss(line);
        string type;
        float x, y;

        ss >> type >> x >> y;

        // Recreate chunk, mark as loaded
        m_Chunks.emplace_back(type, Vector2f(x, y), true);
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
	return &m_Chunks[i];
}

const vector<NavBox>& World::getNavBoxes(int i) const
{
    return m_Chunks[i].getNavBoxes();
}

NavBox World::getChunkArea(int i)
{
	return m_Chunks[i].getChunkArea();
}

vector<Entity> World::getEntities(int i)
{
	return m_Chunks[i].getEntities();
}

void World::clearWorld()
{
	for (size_t i = 0; i < m_Chunks.size(); i++)
	{
		m_Chunks[i].clearChunk();
	}
    m_Chunks.clear();
}

int World::getWorldSize()
{
	return m_Chunks.size();
}

VertexArray World::getBackground(int i) {

	return m_Chunks[i].getBackground();
}

VertexArray World::getForground(int i) {

	return m_Chunks[i].getForground();
}

Vector2f World::getChunkCenter(int i)
{
	return m_Chunks[i].getChunkCenter();
}

void World::setWorldSize(int size) 
{
    m_WorldSize = size;
}