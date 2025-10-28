#include <SFML/Graphics.hpp>
#include "Engine.h"
#include "Entity.h"
#include "Chunk.h"
#include "NavBox.h"
#include "Cell.h"
#include <random>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include "CollisionDetection.h"

using namespace std;
using namespace sf;

random_device rd;
mt19937 gen(rd());

Chunk::Chunk(String type, Vector2f chunk, bool load)
{

	m_Type = type;
	m_LoadChunk = load;
	m_Chunk = chunk;

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

	for (int x = 0; x < 50; ++x)
		for (int y = 0; y < 50; ++y)
			blockedTiles[x][y] = false;

	if (m_LoadChunk)
	{
		loadChunk();

		if (m_Type == "spawn")
		{
			for (int x = 10; x < 35; x++) // create road with pavestones
			{
				for (int y = 15; y < 18; y++)
				{
					int chance = rand() % 5;

					if (chance <= 2)
					{
						placeTile(x, y, 8, 1, false, false);
					}
					else
					{
						placeTile(x, y, 9, 1, false, false);
					}
				}
			}

			/*
			placeHouse4(13, 11);
			placeHouse3(23, 10);
			placeHouse2(13, 23);
			placeHouse1(23, 26);
			placeCastle(10, 35);
			*/
		}
	}
	else
	{
		if (m_Type == "spawn") { // create spawn chunk

			// Grass 
			for (int x = 0; x < 50; x++)
			{
				for (int y = 0; y < 50; y++)
				{
					if (x > 9 && x < 35 && y > 4 && y < 35) // create mud inside castle
					{
						int chance = rand() % 5;

						if (chance <= 2)
						{
							placeTile(x, y, 7, 0, false, true);
						}
						else
						{
							placeTile(x, y, 8, 0, false, true);
						}
					}
					else if (x > 7 && x < 38 && y > 2 && y < 37) // create grass that trees cant spawn on 
					{

						int chance = rand() % 5;

						if (chance <= 2)
						{
							placeTile(x, y, 4, 2, false, true);
						}
						else
						{
							placeTile(x, y, 5, 2, false, true);
						}
					}
					else
					{
						int chance = rand() % 2500; // use random grass tiles to give ground some texture

						if (chance <= 1200)
						{
							placeTile(x, y, 0, 0, false, true);
						}
						else if (chance <= 2400)
						{
							placeTile(x, y, 1, 0, false, true);
						}
						else if (chance <= 2500)
						{
							placeTile(x, y, 6, 0, false, true);
						}
					}
				}
			}

			for (int x = 10; x < 35; x++) // create road with pavestones
			{
				for (int y = 15; y < 18; y++)
				{
					int chance = rand() % 5;

					if (chance <= 2)
					{
						placeTile(x, y, 8, 1, false, false);
					}
					else
					{
						placeTile(x, y, 9, 1, false, false);
					}
				}
			}

			// Forground empty
			for (int x = 0; x < 50; x++)
			{
				for (int y = 0; y < 50; y++)
				{

					placeTile(x, y, 6, 2, true, true);

				}
			}

			placeHouse4(13, 11);
			placeHouse3(23, 10);
			placeHouse2(13, 23);
			placeHouse1(23, 26);
			placeCastle(10, 35);


			createForest(); // create a forest
		}

		if (m_Type == "grass") {

			// Grass 
			for (int x = 0; x < 50; x++)
			{
				for (int y = 0; y < 50; y++)
				{

					int chance = rand() % 2500;

					if (chance <= 1200)
					{
						placeTile(x, y, 0, 0, false, true);
					}
					else if (chance <= 2400)
					{
						placeTile(x, y, 64, 0, false, true);
					}
					else if (chance <= 2500)
					{
						placeTile(x, y, 384, 0, false, true);
					}

				}
			}

			// Forground empty
			for (int x = 0; x < 50; x++)
			{
				for (int y = 0; y < 50; y++)
				{

					placeTile(x, y, 6, 2, true, true);

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

					if (chance <= 1200)
					{
						placeTile(x, y, 0, 0, false, true);
					}
					else if (chance <= 2400)
					{
						placeTile(x, y, 1, 0, false, true);
					}
					else if (chance <= 2500)
					{
						placeTile(x, y, 6, 0, false, true);
					}
				}
			}

			// Forground empty
			for (int x = 0; x < 50; x++)
			{
				for (int y = 0; y < 50; y++)
				{

					placeTile(x, y, 6, 2, true, true);

				}
			}

			createForest(); // create a forest
		}

		if (m_Type == "goblinVillage") {

			// Grass 
			for (int x = 0; x < 50; x++)
			{
				for (int y = 0; y < 50; y++)
				{
					int chance = rand() % 2500;

					if (chance <= 1200)
					{
						placeTile(x, y, 0, 0, false, true);
					}
					else if (chance <= 2400)
					{
						placeTile(x, y, 1, 0, false, true);
					}
					else if (chance <= 2500)
					{
						placeTile(x, y, 6, 0, false, true);
					}
				}
			}

			// Forground empty
			for (int x = 0; x < 50; x++)
			{
				for (int y = 0; y < 50; y++)
				{

					placeTile(x, y, 6, 2, true, true);

				}
			}

			for (int x = 15; x < 35; x += 6)
			{
				for (int y = 15; y < 35; y += 6)
				{
					int chance = rand() % 15;

					if (chance < 3) // if spawning a house create grass that trees cant spawn on around house
					{
						for (int i = x - 1; i < x + 5; i++)
						{
							for (int j = y - 4; j < y + 1; j++)
							{
								int chance2 = rand() % 5;

								if (chance2 <= 2)
								{
									placeTile(i, j, 4, 2, false, true);
								}
								else
								{
									placeTile(i, j, 5, 2, false, true);
								}
							}
						}
					}

					if (chance == 0)
					{
						placeHouse2(x, y);
					}
					else if (chance == 1)
					{
						placeHouse3(x, y);
					}
					else if (chance == 2)
					{
						placeHouse4(x, y);
					}
				}
			}

			createForest(); // create a forest
		}

		if (m_Type == "scorchedForest") {

			// Grass 
			for (int x = 0; x < 50; x++)
			{
				for (int y = 0; y < 50; y++)
				{

					placeTile(x, y, 0, 0, false, true);

				}
			}

			// Forground empty
			for (int x = 0; x < 50; x++)
			{
				for (int y = 0; y < 50; y++)
				{

					placeTile(x, y, 6, 2, true, true);

				}
			}

			createBurntForest();
		}

		saveChunk();
	}
}


// Function to place isometric tiles and map textures to the tiles (x and y is position on isometric grid, not coordinates)
// texX and texY is texture location on sprite sheet
void Chunk::placeTile(int x, int y, int texX, int texY, bool forGround, bool save)
{
	// Save tile type 
	if (!m_LoadChunk && save)
	{
		if (forGround)
		{
			m_TileTypeFGround[x][y] = Vector2i(texX, texY);
		}
		else
		{
			m_TileType[x][y] = Vector2i(texX, texY);
		}
	}
	texX *= TILE_SIZE;
	texY *= TILE_SIZE;

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
			placeTile(x, sy, 5, 14, false, false); // door
		}
		else
		{
			placeTile(x, sy, 1, 14, false, false);
		}

	}

	// House east wall 
	for (int y = sy - 3; y < sy + 1; y++)
	{
		placeTile(sx + 4, y, 1, 14, false, false);
	}

	// House 2nd floor east wall
	for (int y = sy - 3; y < sy - 1; y++)
	{
		placeTile(sx + 3, y, 8, 19, true, false);
	}

	// House east roof
	placeTile(sx + 2, sy - 4, 7, 13, true, false);
	placeTile(sx + 3, sy - 4, 7, 13, true, false);

	// House west roof 1
	for (int x = sx - 1; x < sx + 4; x++)
	{
		placeTile(x, sy - 1, 1, 13, true, false);
	}

	// House west roof 2
	for (int x = sx - 2; x < sx + 3; x++)
	{
		placeTile(x, sy - 3, 1, 13, true, false);
	}

	Structure structure;
	structure.position = Vector2i(sx, sy);
	structure.type = "house1";
	structures.push_back(structure);

	sx += offset.x;
	sy += offset.y;
	NavBox navbox(sx + 1, sy - 3, 5, 4);
	navBoxes.push_back(navbox);
	
}

void Chunk::placeHouse2(int sx, int sy) { // sx 15, sy 18

	// House west wall
	for (int x = sx; x < sx + 2; x++)
	{
		placeTile(x, sy, 1, 14, false, false);
	}

	// House east wall 
	for (int y = sy - 2; y < sy + 1; y++)
	{
		if (y == sy - 1)
		{
			placeTile(sx + 1, y, 4, 14, false, false);
		}
		else
		{
			placeTile(sx + 1, y, 1, 14, false, false);
		}
	}
	// roof
	placeTile(sx - 1, sy - 1, 5, 13, true, false);

	for (int y = sy - 3; y < sy; y++)
	{
		placeTile(sx, y, 2, 13, true, false);
	}

	Structure structure;
	structure.position = Vector2i(sx, sy);
	structure.type = "house2";
	structures.push_back(structure);

	sx += offset.x;
	sy += offset.y;
	NavBox navbox(sx + 1, sy - 2, 2, 3);
	navBoxes.push_back(navbox);

	cells.emplace_back("house2", Vector2f(sx, sy), m_Chunk);
}

void Chunk::placeHouse3(int sx, int sy) { // sx 15, sy 18

	// House west wall
	for (int x = sx; x < sx + 3; x++)
	{
		if (x == sx + 1)
		{
			placeTile(x, sy, 5, 14, false, false); // door
		}
		else
		{
			placeTile(x, sy, 1, 14, false, false);
		}
	}

	// House east wall 
	for (int y = sy - 1; y < sy + 1; y++)
	{
		placeTile(sx + 2, y, 1, 14, false, false);
	}

	placeTile(sx + 1, sy - 2, 7, 13, true, false);

	// roof
	for (int x = sx - 1; x < sx + 2; x++)
	{
		placeTile(x, sy - 1, 1, 13, true, false);
	}

	Structure structure;
	structure.position = Vector2i(sx, sy);
	structure.type = "house3";
	structures.push_back(structure);

	sx += offset.x;
	sy += offset.y;
	NavBox navbox(sx + 1, sy - 1, 3, 2);
	navBoxes.push_back(navbox);

}

void Chunk::placeHouse4(int sx, int sy) { // sx 15, sy 18

	// House west wall
	for (int x = sx; x < sx + 2; x++)
	{
		placeTile(x, sy, 1, 14, false, false);
	}

	// House east wall 
	for (int y = sy - 2; y < sy + 1; y++)
	{
		if (y == sy - 1)
		{
			placeTile(sx + 1, y, 4, 14, false, false);
		}
		else
		{
			placeTile(sx + 1, y, 1, 14, false, false);
		}
	}

	// House west wall 2
	for (int x = sx + 2; x < sx + 5; x++)
	{
		if (x == sx + 3)
		{
			placeTile(x, sy - 2, 7, 14, false, false);
		}
		else
		{
			placeTile(x, sy - 2, 1, 14, false, false);
		}
	}

	// House east wall  2
	for (int y = sy - 3; y < sy - 1; y++)
	{
		placeTile(sx + 4, y, 1, 14, false, false); // 64/64 = 1, 896/64 = 14
	}

	placeTile(sx + 3, sy - 4, 7, 13, true, false); // 448/64 = 7, 832/64 = 13

	// roof
	placeTile(sx - 1, sy - 1, 5, 13, true, false); // 320/64 = 5, 832/64 = 13

	placeTile(sx, sy - 3, 9, 13, true, false);     // 576/64 = 9, 832/64 = 13 (corner piece)

	for (int y = sy - 2; y < sy; y++)
	{
		placeTile(sx, y, 2, 13, true, false);      // 128/64 = 2, 832/64 = 13
	}

	// roof
	for (int x = sx + 1; x < sx + 4; x++)
	{
		placeTile(x, sy - 3, 1, 13, true, false);  // 64/64 = 1, 832/64 = 13
	}

	Structure structure;
	structure.position = Vector2i(sx, sy);
	structure.type = "house4";
	structures.push_back(structure);

	sx += offset.x;
	sy += offset.y;
	NavBox navbox1(sx + 1, sy - 2, 2, 3);
	navBoxes.push_back(navbox1);
	markNavBoxAsBlocked(navbox1);

	NavBox navbox2(sx + 3, sy - 3, 3, 2);
	navBoxes.push_back(navbox2);
	markNavBoxAsBlocked(navbox2);
}

void Chunk::placeCastle(int sx, int sy) { // sx 15, sy 18

	// FIRST LEVEL

// Castle west wall 
	for (int y = sy - 30; y < sy; y++)
	{
		if (y != 15 && y != 16 && y != 17)
		{
			placeTile(sx, y, 0, 12, false, false);
		}
	}

	// Castle north wall
	for (int x = sx + 1; x < sx + 25; x++)
	{
		placeTile(x, sy - 30, 0, 12, false, false);
	}

	// Castle east wall 
	for (int y = sy - 30; y < sy; y++)
	{
		if (y != 15 && y != 16 && y != 17)
		{
			placeTile(sx + 24, y, 0, 12, false, false);
		}
	}

	// Castle south wall
	for (int x = sx; x < sx + 25; x++)
	{
		placeTile(x, sy, 0, 12, false, false);
	}

	// SECOND LEVEL

	// Castle west wall 
	for (int y = sy - 31; y < sy; y++)
	{
		if (y != 14 && y != 15 && y != 16)
		{
			placeTile(sx - 1, y, 0, 12, true, false);
		}
	}

	// Castle north wall
	for (int x = sx; x < sx + 24; x++)
	{
		placeTile(x, sy - 31, 0, 12, true, false);
	}

	// Castle east wall 
	for (int y = sy - 31; y < sy; y++)
	{
		if (y != 14 && y != 15 && y != 16)
		{
			placeTile(sx + 23, y, 0, 12, true, false);
		}
	}

	// Castle south wall
	for (int x = sx - 1; x < sx + 24; x++)
	{
		placeTile(x, sy - 1, 0, 12, true, false);
	}

	// THIRD LEVEL

	// Castle west wall 
	for (int y = sy - 32; y < sy - 1; y++)
	{
		placeTile(sx - 2, y, 0, 12, true, false);
	}

	// Castle north wall
	for (int x = sx - 1; x < sx + 23; x++)
	{
		placeTile(x, sy - 32, 0, 12, true, false);
	}

	// Castle east wall 
	for (int y = sy - 32; y < sy - 1; y++)
	{
		placeTile(sx + 22, y, 0, 12, true, false);
	}

	// Castle south wall
	for (int x = sx - 2; x < sx + 23; x++)
	{
		placeTile(x, sy - 2, 0, 12, true, false);
	}

	// FOURTH LEVEL

	// Castle west wall 
	for (int y = sy - 33; y < sy - 1; y++)
	{
		if (y % 2 == 0) {
			placeTile(sx - 3, y, 0, 13, true, false); // 832 → 13
		}
	}

	// Castle north wall
	for (int x = sx - 1; x < sx + 22; x++)
	{
		if (x % 2 == 1) {
			placeTile(x, sy - 33, 0, 13, true, false);
		}
	}

	// Castle east wall 
	for (int y = sy - 32; y < sy - 1; y++)
	{
		if (y % 2 == 0) {
			placeTile(sx + 21, y, 0, 13, true, false);
		}
	}

	// Castle south wall
	for (int x = sx - 2; x < sx + 23; x++)
	{
		if (x % 2 == 1) {
			placeTile(x, sy - 3, 0, 13, true, false);
		}
	}

	Structure structure;
	structure.position = Vector2i(sx, sy);
	structure.type = "castle";
	structures.push_back(structure);

	sx += offset.x;
	sy += offset.y;

	// create navBoxes for castle walls
	NavBox navboxW1(sx + 1, sy, 1, -17);
	navBoxes.push_back(navboxW1);

	NavBox navboxW2(sx + 1, sy - 20, 1, -10);
	navBoxes.push_back(navboxW2);

	NavBox navboxN(sx + 1, sy - 30, 25, 1);
	navBoxes.push_back(navboxN);

	NavBox navboxE1(sx + 25, sy, 1, -17);
	navBoxes.push_back(navboxE1);

	NavBox navboxE2(sx + 25, sy - 20, 1, -10);
	navBoxes.push_back(navboxE2);

	NavBox navboxS(sx + 1, sy, 25, 1);
	navBoxes.push_back(navboxS);
}

void Chunk::CreateEntity(String type, int x, int y) {

	if (!m_LoadChunk)
	{
		m_TileEntity[x][y] = type;
	}

	Entity entity;
	x += offset.x;
	y += offset.y;
	float ix = (x - y) * (TILE_SIZE / 2);
	float iy = (x + y) * (TILE_SIZE / 4);
	entity.spawn(type, ix, iy);

	if (type == "tree1" || type == "tree2" || type == "tree3" || type == "tree4" || type == "tree6" || type == "tree7") // if entity requires a navBox for isometric collisions
	{
		NavBox nav(x, y, 1, 1);
		nav.NavTree();
		navBoxes.push_back(nav);
		markNavBoxAsBlocked(nav);
	}
	else if (type == "tree5" || type == "tree8")
	{
		NavBox nav(x, y, 1, 1);
		nav.NavTreeLarge();
		navBoxes.push_back(nav);
		markNavBoxAsBlocked(nav);
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

vector<Cell> Chunk::getCells() {

	return cells;
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

void Chunk::createForest() // create a forest from random tree entities
{
	for (int i = 0; i < CHUNK_SIZE - 2; i++)
	{
		for (int j = 0; j < CHUNK_SIZE - 2; j++)
		{
			if (m_TileType[i][j] == Vector2i(0, 0) || m_TileType[i][j] == Vector2i(1, 0)) // if ground is grass place trees + stuff
			{
				int chance = rand() % 2500;

				if (chance <= 200) // 10% chance to spawn tree
				{
					int type = 1 + (gen() % 2);
					string entity = "tree" + to_string(type);
					CreateEntity(entity, i, j);
				}
				else if (chance <= 400) // 10% chance to spawn bush
				{
					int type = 1 + (gen() % 5);
					string entity = "bush" + to_string(type);
					CreateEntity(entity, i, j);
				}
				else if (chance <= 410) // 10% chance to spawn log
				{
					int type = 1 + (gen() % 2);
					string entity = "log" + to_string(type);
					CreateEntity(entity, i, j);
				}
				else if (chance == 500) // 10% chance to spawn big tree
				{
					CreateEntity("tree5", i, j);
				}
			}
		}
	}
}

void Chunk::createBurntForest() // create a forest from random tree entities
{
	for (int i = 0; i < CHUNK_SIZE - 2; i++)
	{
		for (int j = 0; j < CHUNK_SIZE - 2; j++)
		{
			int chance = rand() % 2500;

			if (chance <= 200) // 10% chance to spawn tree
			{
				int type = 6 + (gen() % 2);
				string entity = "tree" + to_string(type);
				CreateEntity(entity, i, j);
			}
			else if (chance <= 400) // 10% chance to spawn bush
			{
				int type = 6;
				string entity = "bush" + to_string(type);
				CreateEntity(entity, i, j);
			}
			else if (chance <= 410) // 10% chance to spawn log
			{
				int type = 1 + (gen() % 2);
				string entity = "log" + to_string(type);
				CreateEntity(entity, i, j);
			}
			else if (chance == 500) // 10% chance to spawn big tree
			{
				CreateEntity("tree8", i, j);
			}
		}
	}
}

void Chunk::saveChunk()
{
	// Create a filename based on the chunk's coordinates
	string path = "gamedata/chunks/chunk_" +
		to_string((int)m_Chunk.x) + "_" +
		to_string((int)m_Chunk.y) + ".txt";

	ofstream outFile(path);
	if (!outFile.is_open()) {
		cerr << "Failed to open file for saving: " << path << endl;
		return;
	}

	// --- Save tiles ---
	for (int y = 0; y < 50; y++) {
		for (int x = 0; x < 50; x++) {
			outFile << m_TileType[x][y].x << "," << m_TileType[x][y].y << "  "
				<< m_TileTypeFGround[x][y].x << "," << m_TileTypeFGround[x][y].y << "  "
				<< (m_TileEntity[x][y].isEmpty() ? "none" : m_TileEntity[x][y].toAnsiString()) << "\n";
		}
	}

	// --- Save structures ---
	outFile << "STRUCTURES_BEGIN\n";
	outFile << structures.size() << "\n";
	for (const auto& structure : structures) {
		outFile << structure.type << " " << structure.position.x << " " << structure.position.y << "\n";
	}
	outFile << "STRUCTURES_END\n";

	outFile.close();
}

void Chunk::loadChunk()
{
	string path = "gamedata/chunks/chunk_" +
		to_string((int)m_Chunk.x) + "_" +
		to_string((int)m_Chunk.y) + ".txt";

	ifstream inFile(path);
	if (!inFile.is_open()) {
		cerr << "Failed to open file for loading: " << path << endl;
		return;
	}

	// --- Load tiles ---
	for (int y = 0; y < 50; y++) {
		for (int x = 0; x < 50; x++) {
			string bgPair, fgPair, entityName;
			inFile >> bgPair >> fgPair >> entityName;

			int bgx, bgy, fgx, fgy;
			sscanf_s(bgPair.c_str(), "%d,%d", &bgx, &bgy);
			sscanf_s(fgPair.c_str(), "%d,%d", &fgx, &fgy);

			m_TileType[x][y] = { bgx, bgy };
			m_TileTypeFGround[x][y] = { fgx, fgy };

			if (entityName != "none") {
				m_TileEntity[x][y] = entityName;
				CreateEntity(entityName, x, y);
			}
			else {
				m_TileEntity[x][y] = "";
			}

			placeTile(x, y, bgx, bgy, false, true);
			placeTile(x, y, fgx, fgy, true, true);
		}
	}

	// --- Load structures ---
	string line;
	structures.clear();

	while (std::getline(inFile, line)) {
		if (line == "STRUCTURES_BEGIN") {
			size_t count;
			inFile >> count;

			for (size_t i = 0; i < count; ++i) {
				Structure s;
				inFile >> s.type >> s.position.x >> s.position.y;
				structures.push_back(s);

				// Optional: recreate or spawn structure in the world
				placeStructure(s.type, s.position);
			}
		}
	}

	inFile.close();
}

String Chunk::getChunkType()
{
	return m_Type;
}

bool Chunk::isTileBlocked(int tileX, int tileY)
{
	// Convert tile coordinate to world coordinate
	float ix = (tileX - tileY) * (TILE_SIZE / 2);
	float iy = (tileX + tileY) * (TILE_SIZE / 4);
	Vector2f pos(ix, iy);

	// Check collision with any NavBox obstacle
	for (auto& nav : navBoxes)
	{
		ConvexShape shape = nav.getShape();
		if (collision.pointInShape(pos, shape))
			return true;
	}
	return false;
}

void Chunk::markNavBoxAsBlocked(const NavBox& nav)
{
	FloatRect bounds = nav.getShape().getGlobalBounds();

	for (int x = 0; x < 50; ++x)
	{
		for (int y = 0; y < 50; ++y)
		{
			float ix = (x - y) * (TILE_SIZE / 2) + TILE_SIZE / 2;
			float iy = (x + y) * (TILE_SIZE / 4) + TILE_SIZE / 2;
			Vector2f pos(ix, iy);

			if (bounds.contains(pos))
				blockedTiles[x][y] = true;
		}
	}
}

bool Chunk::blockEdges(int tileX, int tileY)
{
	if (tileX < 0 || tileY < 0 || tileX >= 50 || tileY >= 50)
		return true; // treat out-of-bounds as blocked

	return blockedTiles[tileX][tileY];
}

void Chunk::placeStructure(String type, Vector2i position)
{
	

	if (type == "house1")
	{
		placeHouse1(position.x, position.y);
	}

	if (type == "house2")
	{
		placeHouse2(position.x, position.y);
	}

	if (type == "house3")
	{
		placeHouse3(position.x, position.y);
	}

	if (type == "house4")
	{
		placeHouse4(position.x, position.y);
	}

	if (type == "castle")
	{
		placeCastle(position.x, position.y);
	}
}