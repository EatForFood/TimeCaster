#include <SFML/Graphics.hpp>
#include "TimeCaster.h"
#include "Entity.h"
#include "Chunk.h"
#include "NavBox.h"
#include <random>

using namespace std;
using namespace sf;

random_device rd;
mt19937 gen(rd());

Chunk::Chunk(String type, Vector2f chunk)
{
	m_Type = type;

	offset.x = chunk.x * CHUNK_SIZE;
	offset.y = chunk.y * CHUNK_SIZE;

	// calcualte center point of chunk
	int tileX = 25 + offset.x;
	int tileY = 25 + offset.y;

	float ix = (tileX - tileY) * (TILE_SIZE / 2);
	float iy = (tileX + tileY) * (TILE_SIZE / 4);

	chunkCenter = Vector2f(ix, iy); // find center point in world coords of chunk

	chunkArea.SetNavBox(offset.x, offset.y, 50, 50); // create a navBox around the chunk 



	rVABG.setPrimitiveType(Quads);
	rVAFG.setPrimitiveType(Quads);

	// Set the size of the vertex array
	rVABG.resize(100 * 100 * VERTS_IN_QUAD);
	rVAFG.resize(200 * 200 * VERTS_IN_QUAD);

	if (m_Type == "spawn") { // create spawn chunk

		/*
	debug = false;
	debugFont.loadFromFile("fonts/dogica.ttf");
	*/

		// Grass 
		for (int x = 0; x < 50; x++)
		{
			for (int y = 0; y < 50; y++)
			{
				int chance = rand() % 2500;

				if (chance <= 1200) // 10% chance to spawn tree
				{
					placeTile(x, y, 0, 0, false);
				}
				else if (chance <= 2400) // 10% chance to spawn bush
				{
					placeTile(x, y, 64, 0, false);
				}
				else if (chance <= 2500) // 10% chance to spawn big tree
				{
					placeTile(x, y, 384, 0, false);
				}

				currentVertexBG += VERTS_IN_QUAD;
			}
		}

		// Forground empty
		for (int x = 0; x < 50; x++)
		{
			for (int y = 0; y < 50; y++)
			{

				placeTile(x, y, 256, 128, true);

				currentVertexFG += VERTS_IN_QUAD;
			}
		}

		CreateEntity("tree1", 5, 5, true);
		CreateEntity("tree2", 8, 5, true);
		CreateEntity("tree3", 5, 8, true);
		CreateEntity("tree4", 5, 14, true);
		CreateEntity("tree5", 14, 7, true);
		CreateEntity("tree6", 18, 7, true);
		CreateEntity("tree7", 22, 7, true);
		CreateEntity("tree8", 29, 7, true);


		placeHouse1(15, 15);
		placeHouse1(23, 15);
		placeHouse1(15, 23);
		placeHouse1(23, 23);

	}

	if (m_Type == "grass") {

		// Grass 
		for (int x = 0; x < 50; x++)
		{
			for (int y = 0; y < 50; y++)
			{

				int chance = rand() % 2500;

				if (chance <= 1200) // 10% chance to spawn tree
				{
					placeTile(x, y, 0, 0, false);
				}
				else if (chance <= 2400) // 10% chance to spawn bush
				{
					placeTile(x, y, 64, 0, false);
				}
				else if (chance <= 2500) // 10% chance to spawn big tree
				{
					placeTile(x, y, 384, 0, false);
				}

				currentVertexBG += VERTS_IN_QUAD;
			}
		}

		// Forground empty
		for (int x = 0; x < 50; x++)
		{
			for (int y = 0; y < 50; y++)
			{

				placeTile(x, y, 256, 128, true);

				currentVertexFG += VERTS_IN_QUAD;
			}
		}
	}

	if (m_Type == "forest") {

		// Grass 
		for (int x = 0; x < 50; x++)
		{
			for (int y = 0; y < 50; y++)
			{
				int chance = rand() % 2500;

				if (chance <= 1200) // 10% chance to spawn tree
				{
					placeTile(x, y, 0, 0, false);
				}
				else if (chance <= 2400) // 10% chance to spawn bush
				{
					placeTile(x, y, 64, 0, false);
				}
				else if (chance <= 2500) // 10% chance to spawn big tree
				{
					placeTile(x, y, 384, 0, false);
				}

				currentVertexBG += VERTS_IN_QUAD;
			}
		}

		// Forground empty
		for (int x = 0; x < 50; x++)
		{
			for (int y = 0; y < 50; y++)
			{

				placeTile(x, y, 256, 128, true);

				currentVertexFG += VERTS_IN_QUAD;
			}
		}

		createForest(300,50,50);
	}

	if (m_Type == "scorchedForest") {

		// Grass 
		for (int x = 0; x < 50; x++)
		{
			for (int y = 0; y < 50; y++)
			{

				placeTile(x, y, 0, 0, false);


				currentVertexBG += VERTS_IN_QUAD;
			}
		}

		// Forground empty
		for (int x = 0; x < 50; x++)
		{
			for (int y = 0; y < 50; y++)
			{

				placeTile(x, y, 256, 128, true);

				currentVertexFG += VERTS_IN_QUAD;
			}
		}

		createBurntForest(300, 50, 50);
	}
}

// Function to place isometric tiles and map textures to the tiles (x and y is position on isometric grid, not coordinates)
// texX and texY is texture location on sprite sheet
void Chunk::placeTile(int x, int y, int texX, int texY, bool forGround)
{
	// Cartesian to isometric
	x += offset.x;
	y += offset.y;
	float ix = (x - y) * (TILE_SIZE / 2);
	float iy = (x + y) * (TILE_SIZE / 4);

	if (forGround) // place tiles on forground layer
	{
		rVAFG[currentVertexFG + 0].position = sf::Vector2f(ix, iy);
		rVAFG[currentVertexFG + 1].position = sf::Vector2f(ix + TILE_SIZE, iy);
		rVAFG[currentVertexFG + 2].position = sf::Vector2f(ix + TILE_SIZE, iy + TILE_SIZE);
		rVAFG[currentVertexFG + 3].position = sf::Vector2f(ix, iy + TILE_SIZE);

		// Texture mapping
		rVAFG[currentVertexFG + 0].texCoords = sf::Vector2f(texX, texY);
		rVAFG[currentVertexFG + 1].texCoords = sf::Vector2f(texX + TILE_SIZE, texY);
		rVAFG[currentVertexFG + 2].texCoords = sf::Vector2f(texX + TILE_SIZE, texY + TILE_SIZE);
		rVAFG[currentVertexFG + 3].texCoords = sf::Vector2f(texX, texY + TILE_SIZE);

		currentVertexFG += VERTS_IN_QUAD;
	}
	else // place tiles on background layer
	{
		rVABG[currentVertexBG + 0].position = sf::Vector2f(ix, iy);
		rVABG[currentVertexBG + 1].position = sf::Vector2f(ix + TILE_SIZE, iy);
		rVABG[currentVertexBG + 2].position = sf::Vector2f(ix + TILE_SIZE, iy + TILE_SIZE);
		rVABG[currentVertexBG + 3].position = sf::Vector2f(ix, iy + TILE_SIZE);

		// Texture mapping
		rVABG[currentVertexBG + 0].texCoords = sf::Vector2f(texX, texY);
		rVABG[currentVertexBG + 1].texCoords = sf::Vector2f(texX + TILE_SIZE, texY);
		rVABG[currentVertexBG + 2].texCoords = sf::Vector2f(texX + TILE_SIZE, texY + TILE_SIZE);
		rVABG[currentVertexBG + 3].texCoords = sf::Vector2f(texX, texY + TILE_SIZE);

		currentVertexBG += VERTS_IN_QUAD;
	}
	
	// show x y position for tile in game for debug
	if (debug == true && x <= 20 && y <= 20)
	{
		Text label;
		label.setFont(debugFont);
		label.setString(std::to_string(x) + "," + std::to_string(y));
		label.setCharacterSize(8);       // small size so it fits
		label.setFillColor(sf::Color::White);

		// position at tile center
		label.setPosition(ix + TILE_SIZE / 4, iy + TILE_SIZE / 4);

		// store it in vector
		debugText.push_back(label);
	}
}

void Chunk::placeHouse1(int sx, int sy) { // sx 15, sy 18

	// House west wall
	for (int x = sx; x < sx + 5; x++)
	{
		if (x == sx + 2)
		{
			placeTile(x, sy, 320, 1140, false); // door
		}
		else
		{
			placeTile(x, sy, 64, 1140, false);
		}

	}

	// House east wall 
	for (int y = sy-3; y < sy+1; y++)
	{
		placeTile(sx+4, y, 64, 1140, false);
	}

	// House east roof
	placeTile(sx+2, sy-4, 448, 1076, true);
	placeTile(sx+3, sy-4, 448, 1076, true);

	// House 2nd floor east wall
	for (int y = sy-3; y < sy-1; y++)
	{
		placeTile(sx+3, y, 512, 1460, true);
	}

	// House west roof 1
	for (int x = sx-1; x < sx+4; x++)
	{
		placeTile(x, sy-1, 64, 1076, true);
	}

	// House west roof 2
	for (int x = sx-2; x < sx+3; x++)
	{
		placeTile(x, sy-3, 64, 1076, true);
	}

	NavBox navbox(sx+1, sy-3, 5, 4);
	navBoxes.push_back(navbox);
}

void Chunk::CreateEntity(String type, int x, int y, bool navBox) {
	Entity entity;
	x += offset.x;
	y += offset.y;
	float ix = (x - y) * (TILE_SIZE / 2);
	float iy = (x + y) * (TILE_SIZE / 4);
	entity.spawn(type, ix, iy);

	if (navBox) // if entity requires a navBox for isometric collisions
	{
		NavBox nav(x, y, 1, 1);
		nav.NavTree();
		navBoxes.push_back(nav);
	}
	else if (type == "tree5" || type == "tree8")
	{
		NavBox nav(x, y, 1, 1);
		nav.NavTreeLarge();
		navBoxes.push_back(nav);
	}
	else
	{
		NavBox nav(x, y, 1, 1);
		nav.NavEmpty();
		navBoxes.push_back(nav);
	}

	entities.push_back(entity);
}

// return background Vertex Array
VertexArray Chunk::getBackground() {
	
	return rVABG;
}

// return forground Vertex Array
VertexArray Chunk::getForground() {

	return rVAFG;
}

vector<Text> Chunk::getDebugText() {

	return debugText;
}

vector<Entity> Chunk::getEntities() {

	return entities;
}

vector<NavBox> Chunk::getNavBoxes() {

	return navBoxes;
}

NavBox Chunk::getChunkArea() {

	return chunkArea;
}

Vector2f Chunk::getChunkCenter() {

	return chunkCenter;
}

void Chunk::clearChunk() {
	rVABG.clear();
	rVAFG.clear();
	currentVertexBG = 0;
	currentVertexFG = 0;
	entities.clear();
	navBoxes.clear();

}

void Chunk::createForest(int numTrees, int chunkWidth, int chunkHeight) // create a forest from random tree entities
{
	for (int i = 0; i < chunkWidth - 2; i++)
	{
		for (int j = 0; j < chunkHeight - 2; j++)
		{
			int chance = rand() % 2500;

			if (chance <= 200) // 10% chance to spawn tree
			{
				int type = 1 + (gen() % 2);
				string entity = "tree" + to_string(type);
				CreateEntity(entity, i, j, true);
			}
			else if (chance <= 400) // 10% chance to spawn bush
			{
				int type = 1 + (gen() % 5);
				string entity = "bush" + to_string(type);
				CreateEntity(entity, i, j, false);
			}
			else if (chance <= 410) // 10% chance to spawn log
			{
				int type = 1 + (gen() % 2);
				string entity = "log" + to_string(type);
				CreateEntity(entity, i, j, false);
			}
			else if (chance == 500) // 10% chance to spawn big tree
			{
				CreateEntity("tree5", i, j, false);
			}
		}
	}
}

void Chunk::createBurntForest(int numTrees, int chunkWidth, int chunkHeight) // create a forest from random tree entities
{
	for (int i = 0; i < chunkWidth - 2; i++)
	{
		for (int j = 0; j < chunkHeight - 2; j++)
		{
			int chance = rand() % 2500;

			if (chance <= 200) // 10% chance to spawn tree
			{
				int type = 6 + (gen() % 2);
				string entity = "tree" + to_string(type);
				CreateEntity(entity, i, j, true);
			}
			else if (chance <= 400) // 10% chance to spawn bush
			{
				int type = 6;
				string entity = "bush" + to_string(type);
				CreateEntity(entity, i, j, false);
			}
			else if (chance <= 410) // 10% chance to spawn log
			{
				int type = 1 + (gen() % 2);
				string entity = "log" + to_string(type);
				CreateEntity(entity, i, j, false);
			}
			else if (chance == 500) // 10% chance to spawn big tree
			{
				CreateEntity("tree8", i, j, false);
			}
		}
	}
}