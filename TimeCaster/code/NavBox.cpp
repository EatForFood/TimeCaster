#include "NavBox.h"
using namespace sf;

NavBox::NavBox()
{

}

NavBox::NavBox(float x, float y, float w, float h) // create navBox at position(x,y) with width (w) and height (h) in isometric tiles
{
    navArea.setPointCount(4);

    ix = (x - y) * (TILE_SIZE / 2);
    iy = (x + y) * (TILE_SIZE / 4);

    navArea.setPosition(ix, iy);
    m_Position = Vector2f(ix, iy);

    navArea.setPoint(0, Vector2f(0, 0));
    navArea.setPoint(1, Vector2f(w * (TILE_SIZE / 2), w * (TILE_SIZE / 4)));
    navArea.setPoint(2, Vector2f((w - h) * (TILE_SIZE / 2), (w + h) * (TILE_SIZE / 4)));
    navArea.setPoint(3, Vector2f(-h * (TILE_SIZE / 2), h * (TILE_SIZE / 4)));

    navArea.setFillColor(Color::Transparent);
    navArea.setOutlineColor(Color::Black);
    navArea.setOutlineThickness(1.f);
   
}

void::NavBox::SetNavBox(float x, float y, float w, float h) // create navBox at position(x,y) with width (w) and height (h) in isometric tiles
{
    navArea.setPointCount(4);

    ix = (x - y) * (TILE_SIZE / 2);
    iy = (x + y) * (TILE_SIZE / 4);

    navArea.setPosition(ix, iy);
    m_Position = Vector2f(ix, iy);

    navArea.setPoint(0, Vector2f(0, 0));
    navArea.setPoint(1, Vector2f(w * (TILE_SIZE / 2), w * (TILE_SIZE / 4)));
    navArea.setPoint(2, Vector2f((w - h) * (TILE_SIZE / 2), (w + h) * (TILE_SIZE / 4)));
    navArea.setPoint(3, Vector2f(-h * (TILE_SIZE / 2), h * (TILE_SIZE / 4)));

    navArea.setFillColor(Color::Transparent);
    navArea.setOutlineColor(Color::Black);
    navArea.setOutlineThickness(1.f);

}

void NavBox::NavTree() // reduce navBox size for trees
{
    navArea.setScale(0.5f,0.5f);
    navArea.setPosition(navArea.getPosition().x, navArea.getPosition().y + 55);
}

Vector2f NavBox::getPosition()
{
    return m_Position;
}

ConvexShape NavBox::getShape()
{
    return navArea;
}

void NavBox::NavEmpty() // empty navbox
{
    navArea.setScale(0.0f, 0.0f);
    navArea.setPosition(navArea.getPosition().x, navArea.getPosition().y);
}
