#include <SFML/Graphics.hpp>
#include "TimeCaster.h"
#include "Entity.h"
#include "Chunk.h"
#include "NavBox.h"

using namespace std;
using namespace sf;

Chunk::Chunk(String type, Vector2f chunk)
{
	m_Type = type;

	offset.x = chunk.x * CHUNK_SIZE;
	offset.y = chunk.y * CHUNK_SIZE;

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

		CreateEntity("tree1", 5, 5);
		CreateEntity("tree2", 8, 5);
		CreateEntity("tree3", 5, 8);
		CreateEntity("tree4", 5, 14);
		CreateEntity("tree5", 14, 7);
		CreateEntity("tree6", 18, 7);
		CreateEntity("tree7", 22, 7);
		CreateEntity("tree8", 29, 7);


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

		CreateEntity("tree1", 4, 5);
		CreateEntity("tree2", 10, 5);
		CreateEntity("tree3", 22, 20);
		CreateEntity("tree4", 26, 14);
		CreateEntity("tree5", 5, 38);
		CreateEntity("tree6", 10, 23);
		CreateEntity("tree7", 30, 7);
		CreateEntity("tree8", 40, 15);
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

int Chunk::createLandscape()
{
	/*
	debug = false;
	debugFont.loadFromFile("fonts/dogica.ttf");
	*/

	// What type of primitive are we using?
	rVABG.setPrimitiveType(Quads);
	rVAFG.setPrimitiveType(Quads);

	// Set the size of the vertex array
	rVABG.resize(100 * 100 * VERTS_IN_QUAD);
	rVAFG.resize(200 * 200 * VERTS_IN_QUAD);

	// Start at the beginning of the vertex array

	// Grass and cliffs
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

	CreateEntity("tree1", 5, 5);
	CreateEntity("tree2", 8, 5);
	CreateEntity("tree3", 5, 8);
	CreateEntity("tree4", 5, 14);
	CreateEntity("tree5", 14, 7);
	CreateEntity("tree6", 18, 7);
	CreateEntity("tree7", 22, 7);
	CreateEntity("tree8", 29, 7);

	
	placeHouse1(15, 15);
	placeHouse1(23, 15);
	placeHouse1(15, 23);
	placeHouse1(23, 23);
	
	return TILE_SIZE;
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

// These trees can be placed in places the player cannot reach for background scenary
void Chunk::placeTree1(int x, int y) {

	placeTile(x, y, 128, 832, false);
	placeTile(x-2, y-2, 128, 768, false);
}

void Chunk::placeTree2(int x, int y) {

	placeTile(x, y, 192, 832, false);
	placeTile(x - 2, y - 2, 192, 768, false);
}

void Chunk::placeTree3(int x, int y) {

	placeTile(x, y, 0, 948, false);
	placeTile(x - 2, y - 2, 0, 884, false);
	placeTile(x - 4, y - 4, 0, 820, false);
}

void Chunk::placeTree4(int x, int y) {

	placeTile(x, y, 64, 948, false);
	placeTile(x - 2, y - 2, 64, 884, false);
}

void Chunk::placeTree5(int x, int y) {

	placeTile(x, y, 128, 948, false);
	placeTile(x - 2, y - 2, 128, 884, false);
}

void Chunk::placeTree6(int x, int y) {

	placeTile(x, y, 192, 948, false);
	placeTile(x - 2, y - 2, 192, 884, false);
}

void Chunk::placeTree7(int x, int y) {

	placeTile(x, y, 256, 948, false);
	placeTile(x - 2, y - 2, 256, 884, false);
	placeTile(x + 1, y - 1, 320, 948, false);
	placeTile(x - 1, y - 3, 320, 884, false);
	placeTile(x + 2, y - 2, 384, 948, false);
	placeTile(x, y - 4, 384, 884, false);
}

void Chunk::placeTree8(int x, int y) {

	placeTile(x, y, 448, 948, false);
	placeTile(x - 2, y - 2, 448, 884, false);
	placeTile(x + 1, y - 1, 512, 948, false);
	placeTile(x - 1, y - 3, 512, 884, false);
	placeTile(x + 2, y - 2, 576, 948, false);
	placeTile(x, y - 4, 576, 884, false);
}

void Chunk::CreateEntity(String type, int x, int y) {
	Entity entity;
	x += offset.x;
	y += offset.y;
	float ix = (x - y) * (TILE_SIZE / 2);
	float iy = (x + y) * (TILE_SIZE / 4);
	entity.spawn(type, ix, iy);

	NavBox nav(x,y,1,1);
	nav.NavTree();
	entities.push_back(entity);
	navBoxes.push_back(nav);
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

void Chunk::clearChunk() {
	rVABG.clear();
	rVAFG.clear();
	currentVertexBG = 0;
	currentVertexFG = 0;
	entities.clear();
	navBoxes.clear();

}