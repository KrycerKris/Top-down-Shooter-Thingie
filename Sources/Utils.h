#pragma once
#include "Defines.h"
#include <SFML/Graphics.hpp>
namespace nmUtils
{
	float AngleBetween(sf::Vector2f &point1, sf::Vector2f &point2);
	sf::Vector2f NormaliseVector2f(const sf::Vector2f &vector);
	float DistanceBetween(const sf::Vector2f &point1,const sf::Vector2f &point2);
	sf::Vector2f RandVector2f();
}