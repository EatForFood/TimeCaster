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
	int m_TileEntity[50][50];
	Vector2i m_TileTypeFGround[50][50];

	bool m_Walkable[50][50];


	bool debug;
	Font debugFont;
	vector<Text> debugText;

	vector<Entity> entities;

	NavBox chunkArea;
	vector<NavBox> navBoxes;

	vector<Cell> cells;

	struct Structure
	{
		int type;
		Vector2i position;
	};

	vector<Structure> structures;

	vector<Vector2i> enemyLocations;
	vector<string> enemyTypes;

	// Door locations for the shop
	vector<Vector2f> doorLocations;

	bool blockedTiles[50][50];
public:

	Chunk(String type, Vector2f chunk, bool load);
	
	void saveChunk();
	void loadChunk();

	void placeTile(int x, int y, int texX, int texY, bool forGround, bool save);

	void placeShopSign(int x, int y);

	VertexArray getBackground();
	VertexArray getForground();

	vector<Text> getDebugText();

	void CreateEntity(int type, int x, int y);

	vector<Entity> getEntities();

	const vector<NavBox>& getNavBoxes() const;

	vector<Cell> getCells();

	String getChunkType();

	NavBox getChunkArea();
	Vector2f getChunkCenter();


	bool playerNearDoor(Vector2f playerPos);

	void saveDoorLocation(Vector2f position);

	void clearChunk();

	void placeStructure(int type, Vector2i position);

	void placeHouse1(int x, int y);
	void placeHouse2(int x, int y);
	void placeHouse3(int x, int y);
	void placeHouse4(int x, int y);
	void placeCastle(int x, int y);
	void placeBossArena(int x, int y);

	void createForest();
	void createBurntForest();

	void createNodes();
	bool getNode(int x, int y);
	Vector2f getChunkLocation();

	void CreateEnemySpawn(string type, Vector2i position);

	vector<Vector2i>getEnemyLocations();
	vector<string>getEnemyTypes();


};
