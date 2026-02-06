#pragma once
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

namespace TextHelper 
{
	inline Text makeText(const Font& font, const string& str, int size, Vector2f pos, Color color)
	{
		Text text;
		text.setFont(font);
		text.setString(str);
		text.setCharacterSize(size);
		text.setFillColor(color);
		FloatRect bounds = text.getLocalBounds();
		text.setOrigin(
			bounds.left + bounds.width / 2.f,
			bounds.top
		);
		text.setPosition(pos);
		return text;
	}
};

