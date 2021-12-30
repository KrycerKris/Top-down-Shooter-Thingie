#include "Utils.h"

float nmUtils::AngleBetween(sf::Vector2f &point1, sf::Vector2f &point2) {
	return atan2(point2.y - point1.y, point2.x - point1.x) * 180 / PI;
}

sf::Vector2f nmUtils::NormaliseVector2f(sf::Vector2f vector)
{
	double length = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
	return sf::Vector2f(vector.x / length, vector.y / length);
}

float nmUtils::DistanceBetween(sf::Vector2f point1, sf::Vector2f point2)
{
	return sqrt(pow(point1.x - point2.x,2) + pow(point1.y - point2.y,2));
}
