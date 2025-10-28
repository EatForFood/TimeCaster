#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "NavBox.h"
#include "Cell.h"
#include "CollisionDetection.h"

using namespace sf;
using namespace std;

class Chunk
{
protected:

	CollisionDetection collision;

	const int TILE_SIZE = 64;
	const int CHUNK_SIZE = 50;
	const int VERTS_IN_QUAD = 4;

	int offsetX; // amount to offset position of chunk by
	int offsetY;

	Vector2f m_Chunk;
	Vector2f offset;
	Vector2f chunkCenter;


	int currentVertexBG = 0;
	int currentVertexFG = 0;
	VertexArray rVABG; // Vertex array for background tiles
	VertexArray rVAFG; // Vertex array for forground tiles

	bool forGround = false;

	String m_Type; // type of chunk to generate

	bool m_LoadChunk = false;
	
	Vector2i m_TileType[50][50];
	String m_TileEntity[50][50];
	Vector2i m_TileTypeFGround[50][50];
	

	bool debug;
	Font debugFont;
	vector<Text> debugText;

	vector<Entity> entities;

	NavBox chunkArea;
	vector<NavBox> navBoxes;

	vector<Cell> cells;

	struct Structure
	{
		string type;
		Vector2i position;
	};

	vector<Structure> structures;

	bool blockedTiles[50][50];
public:

	Chunk(String type, Vector2f chunk, bool load);
	
	void saveChunk();
	void loadChunk();

	void placeTile(int x, int y, int texX, int texY, bool forGround, bool save);

	VertexArray getBackground();
	VertexArray getForground();

	vector<Text> getDebugText();

	void CreateEntity(String type, int x, int y);

	vector<Entity> getEntities();

	vector<NavBox> getNavBoxes();

	vector<Cell> getCells();

	String getChunkType();

	NavBox getChunkArea();
	Vector2f getChunkCenter();

	void clearChunk();

	void placeStructure(String type, Vector2i position);

	void placeHouse1(int x, int y);
	void placeHouse2(int x, int y);
	void placeHouse3(int x, int y);
	void placeHouse4(int x, int y);
	void placeCastle(int x, int y);

	void createForest();
	void createBurntForest();

	bool isTileBlocked(int tileX, int tileY);

	void markNavBoxAsBlocked(const NavBox& nav);

	bool blockEdges(int tileX, int tileY);
};
