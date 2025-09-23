#include <SFML/Graphics.hpp>
#include "TimeCaster.h"
#include "Entity.h"
#include "CreateBackground.h"

CreateBackground::CreateBackground()
{
	
}

// Function to place isometric tiles and map textures to the tiles (x and y is position on isometric grid, not coordinates)
// texX and texY is texture location on sprite sheet
void CreateBackground::placeTile(int x, int y, int texX, int texY)
{
	// Cartesian to isometric
	float ix = (x - y) * (TILE_SIZE / 2);
	float iy = (x + y) * (TILE_SIZE / 4);

	// Vertex positions
	rVA[currentVertex + 0].position = sf::Vector2f(ix, iy);
	rVA[currentVertex + 1].position = sf::Vector2f(ix + TILE_SIZE, iy);
	rVA[currentVertex + 2].position = sf::Vector2f(ix + TILE_SIZE, iy + TILE_SIZE);
	rVA[currentVertex + 3].position = sf::Vector2f(ix, iy + TILE_SIZE);

	// Texture mapping
	rVA[currentVertex + 0].texCoords = sf::Vector2f(texX, texY);
	rVA[currentVertex + 1].texCoords = sf::Vector2f(texX + TILE_SIZE, texY);
	rVA[currentVertex + 2].texCoords = sf::Vector2f(texX + TILE_SIZE, texY + TILE_SIZE);
	rVA[currentVertex + 3].texCoords = sf::Vector2f(texX, texY + TILE_SIZE);

	currentVertex += VERTS_IN_QUAD;

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

int CreateBackground::createLandscape()
{
	debug = false;
	debugFont.loadFromFile("fonts/dogica.ttf");

	// What type of primitive are we using?
	rVA.setPrimitiveType(Quads);

	// Set the size of the vertex array
	rVA.resize(100 * 100 * VERTS_IN_QUAD);

	// Start at the beginning of the vertex array

	// Grass and cliffs
	for (int x = 0; x < (rVA.getVertexCount() / 500) - 10; x++)
	{
		for (int y = 0; y < (rVA.getVertexCount() / 500) - 10; y++)
		{
			if (y <= 1 || x <= 1) // place cliffs at edges of map
			{
				placeTile(x, y, 256, 192);
			}
			else // place grass
			{
				placeTile(x, y, 0, 0);
			}

			currentVertex += VERTS_IN_QUAD;
		}
	}

	// House west wall
	for (int x = 15; x < 20; x++)
	{
		placeTile(x, 18, 64, 1140);
	}

	// House east wall
	for (int y = 15; y < 19; y++)
	{
		placeTile(19, y, 64, 1140);
	}

	// House east roof
	placeTile(17, 14, 448, 1076);
	placeTile(18, 14, 448, 1076);

	// House 2nd floor east wall
	for (int y = 15; y < 17; y++)
	{
		placeTile(18, y, 512, 1460);
	}

	// House west roof 1
	for (int x = 14; x < 19; x++)
	{
		placeTile(x, 17, 64, 1076);
	}

	// House west roof 2
	for (int x = 13; x < 18; x++)
	{
		placeTile(x, 15, 64, 1076);
	}

	if (setUp)
	{
		CreateEntity("tree1", 5, 5);
		CreateEntity("tree2", 8, 5);
		CreateEntity("tree3", 5, 8);
		CreateEntity("tree4", 5, 14);
		CreateEntity("tree5", 14, 7);
		CreateEntity("tree6", 18, 7);
		CreateEntity("tree7", 22, 7);
		CreateEntity("tree8", 29, 7);
		setUp = false;
	}

	return TILE_SIZE;
}

// These trees can be placed in places the player cannot reach for background scenary
void CreateBackground::placeTree1(int x, int y) {

	placeTile(x, y, 128, 832);
	placeTile(x-2, y-2, 128, 768);
}

void CreateBackground::placeTree2(int x, int y) {

	placeTile(x, y, 192, 832);
	placeTile(x - 2, y - 2, 192, 768);
}

void CreateBackground::placeTree3(int x, int y) {

	placeTile(x, y, 0, 948);
	placeTile(x - 2, y - 2, 0, 884);
	placeTile(x - 4, y - 4, 0, 820);
}

void CreateBackground::placeTree4(int x, int y) {

	placeTile(x, y, 64, 948);
	placeTile(x - 2, y - 2, 64, 884);
}

void CreateBackground::placeTree5(int x, int y) {

	placeTile(x, y, 128, 948);
	placeTile(x - 2, y - 2, 128, 884);
}

void CreateBackground::placeTree6(int x, int y) {

	placeTile(x, y, 192, 948);
	placeTile(x - 2, y - 2, 192, 884);
}

void CreateBackground::placeTree7(int x, int y) {

	placeTile(x, y, 256, 948);
	placeTile(x - 2, y - 2, 256, 884);
	placeTile(x + 1, y - 1, 320, 948);
	placeTile(x - 1, y - 3, 320, 884);
	placeTile(x + 2, y - 2, 384, 948);
	placeTile(x, y - 4, 384, 884);
}

void CreateBackground::placeTree8(int x, int y) {

	placeTile(x, y, 448, 948);
	placeTile(x - 2, y - 2, 448, 884);
	placeTile(x + 1, y - 1, 512, 948);
	placeTile(x - 1, y - 3, 512, 884);
	placeTile(x + 2, y - 2, 576, 948);
	placeTile(x, y - 4, 576, 884);
}

void CreateBackground::CreateEntity(String type, int x, int y) {
	Entity entity;
	float ix = (x - y) * (TILE_SIZE / 2);
	float iy = (x + y) * (TILE_SIZE / 4);
	entity.spawn(type, ix, iy);
	entities.push_back(entity);
}


VertexArray CreateBackground::getLandscape() {
	
	return rVA;
}

std::vector<Text> CreateBackground::getDebugText() {

	return debugText;
}

std::vector<Entity> CreateBackground::getEntities() {

	return entities;
}