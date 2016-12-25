#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "tinyxml.h"

struct Object
{
    int GetPropertyInt(std::string const& objName);
    float GetPropertyFloat(std::string const& objName);
    std::string GetPropertyString(std::string const& objName);

    std::string name;
    std::string type;
    sf::Rect<float> rect;
    std::map<std::string, std::string> properties;

    sf::Sprite sprite;
};

struct Layer
{
    int opacity;
    std::vector<sf::Sprite> tiles;
};

class Level
{
public:
    bool LoadFromFile(std::string filename);
    Object GetObject(std::string name);
    std::vector<Object> GetObjects(std::string name);
    std::vector<Object> GetAllObjects();
    void Draw(sf::RenderWindow& window, sf::FloatRect const& viewportRect);
    sf::Vector2i GetTileSize();
	float Level::GetTilemapWidth();
	float Level::GetTilemapHeight();

private:
    int width, height, tileWidth, tileHeight;
    int firstTileID;
    sf::Rect<float> drawingBounds;
    sf::Texture tilesetImage;
    std::vector<Object> objects;
    std::vector<Layer> layers;
};
#endif