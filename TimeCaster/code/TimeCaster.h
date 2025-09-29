#pragma once

using namespace sf;

int createBackground(VertexArray& rVA, IntRect arena);

// The games difficulty will always be in one of these states
enum class Difficulty { Easy, Medium, Hard };

// The game will always be in one of these states
enum class State { MAIN_MENU, OPTIONS_MENU, STORY_INTRO, PLAYING, PAUSED, LEVELING_UP, GAME_OVER };