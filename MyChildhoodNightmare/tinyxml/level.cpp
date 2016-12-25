#include "level.h"

int Object::GetPropertyInt(std::string const& objName)//aica?auaai iiia? naienoaa a iaoai nienea
{
    return atoi(properties[objName].c_str());
}

float Object::GetPropertyFloat(std::string const& objName)
{
    return (float)strtod(properties[objName].c_str(), NULL);
}

std::string Object::GetPropertyString(std::string const& objName)//iieo?eou eiy a aeaa no?iee.a?iaa iiiyoii
{
    return properties[objName];
}

bool Level::LoadFromFile(std::string filename)//aaiaoi?ey-ia?auaiea e iaoiaai eeanna aia eeanna 
{
	TiXmlDocument levelFile(filename.c_str());//caa?o?aai oaee a TiXmlDocument

	// caa?o?aai XML-ea?oo
	if (!levelFile.LoadFile())//anee ia oaaeinu caa?oceou ea?oo
	{
		std::cout << "Loading level \"" << filename << "\" failed." << std::endl;//auaaai ioeaeo
		return false;
	}

	// ?aaioaai n eiioaeia?ii map
	TiXmlElement *map;
	map = levelFile.FirstChildElement("map");

	// i?eia? ea?ou: <map version="1.0" orientation="orthogonal"
	// width="10" height="10" tilewidth="34" tileheight="34">
	width = atoi(map->Attribute("width"));//ecaeaeaai ec iaoae ea?ou aa naienoaa
	height = atoi(map->Attribute("height"));//oa naienoaa, eioi?ua caaaaaee i?e ?aaioa a 
	tileWidth = atoi(map->Attribute("tilewidth"));//oaeeiai ?aaaeoi?a
	tileHeight = atoi(map->Attribute("tileheight"));

	// Aa?ai iienaiea oaeenaoa e eaaioeoeeaoi? ia?aiai oaeea
	TiXmlElement *tilesetElement;
	tilesetElement = map->FirstChildElement("tileset");
	firstTileID = atoi(tilesetElement->Attribute("firstgid"));

	// source - ioou ai ea?oeiee a eiioaeia?a image
	TiXmlElement *image;
	image = tilesetElement->FirstChildElement("image");
	std::string imagepath = image->Attribute("source");

	// iuoaainy caa?oceou oaeenao
	sf::Image img;

	if (!img.loadFromFile(imagepath))
	{
		std::cout << "Failed to load tile sheet." << std::endl;//anee ia oaaeinu caa?oceou oaeenao-auaiaei ioeaeo a eiinieu
		return false;
	}


	img.createMaskFromColor(sf::Color(255, 255, 255));//aey ianee oaaoa.nae?an iao ianee
	tilesetImage.loadFromImage(img);
	tilesetImage.setSmooth(false);//naea?eaaiea

								  // iieo?aai eiee?anoai noieaoia e no?ie oaeenaoa
	int columns = tilesetImage.getSize().x / tileWidth;
	int rows = tilesetImage.getSize().y / tileHeight;

	// aaeoi? ec i?yiioaieuieeia ecia?a?aiee (TextureRect)
	std::vector<sf::Rect<int>> subRects;

	for (int y = 0; y < rows; y++)
		for (int x = 0; x < columns; x++)
		{
			sf::Rect<int> rect;

			rect.top = y * tileHeight;
			rect.height = tileHeight;
			rect.left = x * tileWidth;
			rect.width = tileWidth;

			subRects.push_back(rect);
		}

	// ?aaioa ni neiyie
	TiXmlElement *layerElement;
	layerElement = map->FirstChildElement("layer");
	while (layerElement)
	{
		Layer layer;

		// anee i?enoonoaoao opacity, oi caaaai i?ic?a?iinou neiy, eia?a ii iieiinou? iai?ic?a?ai
		if (layerElement->Attribute("opacity") != NULL)
		{
			float opacity = (float)strtod(layerElement->Attribute("opacity"), NULL);
			layer.opacity = (int)(255 * opacity);
		}
		else
		{
			layer.opacity = 255;
		}

		//  eiioaeia? <data> 
		TiXmlElement *layerDataElement;
		layerDataElement = layerElement->FirstChildElement("data");

		if (layerDataElement == NULL)
		{
			std::cout << "Bad map. No layer information found." << std::endl;
		}

		//  eiioaeia? <tile> - iienaiea oaeeia ea?aiai neiy
		TiXmlElement *tileElement;
		tileElement = layerDataElement->FirstChildElement("tile");

		if (tileElement == NULL)
		{
			std::cout << "Bad map. No tile information found." << std::endl;
			return false;
		}

		int x = 0;
		int y = 0;

		while (tileElement)
		{
			int tileGID = atoi(tileElement->Attribute("gid"));
			int subRectToUse = tileGID - firstTileID;

			// Onoaiaaeeaaai TextureRect ea?aiai oaeea
			if (subRectToUse >= 0)
			{
				sf::Sprite sprite;
				sprite.setTexture(tilesetImage);
				sprite.setTextureRect(subRects[subRectToUse]);
				sprite.setPosition((float)(x * tileWidth), (float)(y * tileHeight));
				sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)layer.opacity));

				layer.tiles.push_back(sprite);//caeeauaaai a neie ni?aeou oaeeia
			}

			tileElement = tileElement->NextSiblingElement("tile");

			x++;
			if (x >= width)
			{
				x = 0;
				y++;
				if (y >= height)
					y = 0;
			}
		}

		layers.push_back(layer);

		layerElement = layerElement->NextSiblingElement("layer");
	}

	// ?aaioa n iauaeoaie
	TiXmlElement *objectGroupElement;

	// anee anou neie iauaeoia
	if (map->FirstChildElement("objectgroup") != NULL)
	{
		objectGroupElement = map->FirstChildElement("objectgroup");
		while (objectGroupElement)
		{
			//  eiioaeia? <object>
			TiXmlElement *objectElement;
			objectElement = objectGroupElement->FirstChildElement("object");

			while (objectElement)
			{
				// iieo?aai ana aaiiua - oei, eiy, iiceoey, e oa
				std::string objectType;
				if (objectElement->Attribute("type") != NULL)
				{
					objectType = objectElement->Attribute("type");
				}
				std::string objectName;
				if (objectElement->Attribute("name") != NULL)
				{
					objectName = objectElement->Attribute("name");
				}
				int x = atoi(objectElement->Attribute("x"));
				int y = atoi(objectElement->Attribute("y"));

				int objWidth, objHeight;

				sf::Sprite sprite;
				sprite.setTexture(tilesetImage);
				sprite.setTextureRect(sf::Rect<int>(0, 0, 0, 0));
				sprite.setPosition((float)x, (float)y);

				if (objectElement->Attribute("width") != NULL)
				{
					objWidth = atoi(objectElement->Attribute("width"));
					objHeight = atoi(objectElement->Attribute("height"));
				}
				else
				{
					objWidth = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].width;
					objHeight = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].height;
					sprite.setTextureRect(subRects[atoi(objectElement->Attribute("gid")) - firstTileID]);
				}

				// yecaiiey? iauaeoa
				Object object;
				object.name = objectName;
				object.type = objectType;
				object.sprite = sprite;

				sf::Rect <float> objectRect;
				objectRect.top = (float)y;
				objectRect.left = (float)x;
				objectRect.height = (float)objHeight;
				objectRect.width = (float)objWidth;
				object.rect = objectRect;

				// "ia?aiaiiua" iauaeoa
				TiXmlElement *properties;
				properties = objectElement->FirstChildElement("properties");
				if (properties != NULL)
				{
					TiXmlElement *prop;
					prop = properties->FirstChildElement("property");
					if (prop != NULL)
					{
						while (prop)
						{
							std::string propertyName = prop->Attribute("name");
							std::string propertyValue = prop->Attribute("value");

							object.properties[propertyName] = propertyValue;

							prop = prop->NextSiblingElement("property");
						}
					}
				}


				objects.push_back(object);

				objectElement = objectElement->NextSiblingElement("object");
			}
			objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
		}
	}
	else
	{
		std::cout << "No object layers found..." << std::endl;
	}

	return true;
}
Object Level::GetObject(std::string name)
{
    // oieuei ia?aue iauaeo n caaaiiui eiaiai
    for (size_t i = 0; i < objects.size(); i++)
        if (objects[i].name == name)
            return objects[i];

	return objects[0];
}

std::vector<Object> Level::GetObjects(std::string name)
{
    // ana iauaeou n caaaiiui eiaiai
    std::vector<Object> vec;
    for (size_t i = 0; i < objects.size(); i++)
        if (objects[i].name == name)
            vec.push_back(objects[i]);

    return vec;
}


std::vector<Object> Level::GetAllObjects()
{
    return objects;
};


sf::Vector2i Level::GetTileSize()
{
    return sf::Vector2i(tileWidth, tileHeight);
}

void Level::Draw(sf::RenderWindow& window, sf::FloatRect const& viewportRect)
{
	for (const auto &layer : layers)
	{
		for (const auto &tile : layer.tiles)
		{
			if (viewportRect.intersects(tile.getGlobalBounds()))
			{
				window.draw(tile);
			}
		}
	}
}

float Level::GetTilemapWidth()
{
	// return width of tilemap in pixels
	return static_cast<float>(width * tileWidth);
}

float Level::GetTilemapHeight()
{
	// return height of tilemap in pixels
	return static_cast<float>(height * tileHeight);
}