#include <sstream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Engine.h"
#include "Player.h"
#include "TextureHolder.h"
#include "Spell.h"
#include "Decal.h"
#include "Windows.h"
#include "World.h"
#include "SoundManager.h"
#include "CollisionDetection.h"
#include "Item.h"
#include "Weapon.h"
#include "Equipment.h"
#include "Enemy.h"
#include <string>
#include <thread>
#include "DragonBoss.h"
#include "TextHelper.hpp"

using namespace std;
using namespace sf;

Engine::Engine() : m_EquippedWeapons(player.getEquippedWeapons()), m_EquippedArmour(player.getEquippedArmour()), m_StoredItems(player.getStoredItems())
{
	sound.playMainMenuSound();
	player.loadConfigFile();

	difficulty = stringToDifficulty(player.getdifficultyString());
	windowedMode = player.getWindowedMode();
	vSync = player.getVSync();
	displayFps = player.getDisplayFps();
	Listener::setGlobalVolume(player.getVolume());
	fpsLimit = player.getFpsLimit();

	// set resolution
	resolution.x = 1920;
	resolution.y = 1080;
	
	if (windowedMode == true)
	{
		window.create(VideoMode(resolution.x, resolution.y), "TimeCaster", Style::Default);
		window.setFramerateLimit(fpsLimit);
	}
	else {
		window.create(VideoMode(resolution.x, resolution.y), "TimeCaster", Style::Fullscreen);
		window.setFramerateLimit(fpsLimit);
	}

	window.setVerticalSyncEnabled(vSync);
	window.setFramerateLimit(fpsLimit);

	filter.setSize(resolution);
	
	if (!timeFrozen) {
		filter.setFillColor(defaultFilter);
	}

	player.loadConfigFile();

	difficulty = stringToDifficulty(player.getdifficultyString());

	// Create the background
	// VertexArray background;
	// Load the texture for our background vertex array
	textureBackground = TextureHolder::GetTexture("graphics/landscape.png");
	textureBackground2 = TextureHolder::GetTexture("graphics/landscape2.png");
	textureBackground3 = TextureHolder::GetTexture("graphics/landscape3.png");
	textureBackground4 = TextureHolder::GetTexture("graphics/landscape4.png");
}

// Function to convert difficulty state to string
string Engine::difficultyToString(Difficulty difficulty)
{
	switch (difficulty)
	{
	case Difficulty::Easy:   return "Easy";
	case Difficulty::Medium: return "Medium";
	case Difficulty::Hard:   return "Hard";
	}
	return "Unknown";
}

// Function to convert difficulty in string form to state
Engine::Difficulty Engine::stringToDifficulty(string str)
{
	if (str == "Easy") { return Difficulty::Easy; }
	else if (str == "Medium") { return Difficulty::Medium; }
	else if (str == "Hard") { return Difficulty::Hard; }
	else return Difficulty::Medium;
}

// Function to populate the chunk vector with chunks
void Engine::populateChunkVector()
{
	for (int i = 0; i < world.getWorldSize(); i++) {
		chunks.push_back(*world.getChunk(i));
	}
}

// Function to detect the current chunk an entity finds themselves in
Chunk* Engine::getCurrentChunk(float x, float y) {
	for (Chunk& chunk : chunks) {
		FloatRect area = chunk.getChunkArea().getShape().getGlobalBounds();
		if (collision.pointInShape(Vector2f(x, y), chunk.getChunkArea().getShape())) {
			return &chunk;
		}
	}
	return nullptr;
}

bool Engine::playerNearShop()
{
	// only check for shops in the chunk the player is in
	Chunk* currentChunk = getCurrentChunk(player.getPosition().x, player.getPosition().y);

	if (currentChunk && currentChunk->playerNearDoor(player.getPosition()))
	{
		return true;
	}
	return false;
}

void Engine::run()
{
	initializeUI();
	initializeInventory();
	initializeShop();
	// Posistion shop items
	for (int i = 0; i < shopItems.size(); i++) {
		if (!shopItems[i].isNull()) {
			shopItems[i].getIcon().setPosition(emptyFrames[i].getPosition());
		}
	}

	for (int i = 0; i < shopItems.size(); i++) {
		if (!shopItems[i].isNull()) {
			shopItems[i].getIcon().setPosition(shopFrames[i].getPosition());
		}
	}

	// The main game loop
	while (window.isOpen())
	{
		update(); // update game 
		draw();
	} // End of main game loop
}

// Function used to generate the world and inform player that world generation is in progress
void Engine::generateWorld()
{
	skipIntroText.setString("World is loading...");
	textBounds = skipIntroText.getLocalBounds();
	viewCentre = mainView.getCenter();
	skipIntroText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 1030);
	world.newWorld();
	populateChunkVector();
	spawnEnemies();
	if (state == State::LOADING) {
		state = State::PLAYING;
	}

	worldLoaded = true;
	skipIntroText.setString("--- Press space to skip ---");
	textBounds = skipIntroText.getLocalBounds(); 
	viewCentre = mainView.getCenter();
	skipIntroText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 1030);
}

void Engine::loadGameWorld()
{
	world.loadWorld();
	populateChunkVector();
	spawnEnemies();
	state = State::PLAYING;
}
// Sets the player's difficulty multiplier
void Engine::setDifficulty()
{
	if (difficulty == Difficulty::Easy) {
		player.setDifficultyMult(0.33f);
	}
	else if (difficulty == Difficulty::Medium) {
		player.setDifficultyMult(0.66f);
	}
	else if (difficulty == Difficulty::Hard) {
		player.setDifficultyMult(1.0f);
	}
}

// Function used to spawn enemies into the game world
void Engine::spawnEnemies()
{
	vector<string> worldEnemyTypes;
	vector<Vector2i> worldEnemyLocations;

	for (int i = 0; i < world.getWorldSize(); i++)
	{
		vector<string> chunkEnemyTypes = world.getChunk(i)->getEnemyTypes();
		vector<Vector2i> chunkEnemyLocations = world.getChunk(i)->getEnemyLocations();

		worldEnemyTypes.insert(worldEnemyTypes.end(), chunkEnemyTypes.begin(), chunkEnemyTypes.end());
		worldEnemyLocations.insert(worldEnemyLocations.end(), chunkEnemyLocations.begin(), chunkEnemyLocations.end());
	}

	for (int i = 0; i < worldEnemyTypes.size(); i++)
	{
		if (worldEnemyTypes[i] != "Dragon") {
			auto e = std::make_unique<Enemy>();
			e->spawn(worldEnemyTypes[i], worldEnemyLocations[i], player.getPlayerLevel());
			enemyArr.push_back(std::move(e));
		}
		else {
			auto db = std::make_unique<DragonBoss>();
			db->spawn(worldEnemyTypes[i], worldEnemyLocations[i], player.getPlayerLevel());
			enemyArr.push_back(std::move(db));

		}
	}
}

bool Engine::isEquipment(const string& name)
{
	static const vector<string> equipmentList = {
		"Steel_Plate_Helmet", "Steel_Plate_Chestplate", "Steel_Plate_Pants",
		"Steel_Plate_Boots", "Chain_Mail", "Chain_Hood", "Soldiers_Helmet",
		"Leather_Cap", "Leather_Chestplate", "Leather_Leggings", "Leather_Boots",
		"Family_Hood", "Family_Robe", "Robe_Leggings", "Basic_Shoes"
	};
	return find(equipmentList.begin(), equipmentList.end(), name) != equipmentList.end();
}

bool Engine::isWeapon(const string& name)
{
	static const vector<string> weaponList = {
		"Iron_Sword", "Rapier"
	};
	return find(weaponList.begin(), weaponList.end(), name) != weaponList.end();
}