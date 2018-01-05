#ifndef RAYZAPPER_GAMEOBJECT_H
#define RAYZAPPER_GAMEOBJECT_H

#include <SFML\Graphics.hpp>

class GameObject
{
public:
	virtual void Update() = 0;
	virtual void Render(sf::RenderWindow *window) = 0;
	sf::Vector2f GetPosition() const;
	virtual void SetPosition(sf::Vector2f position);
protected:
	GameObject();
	sf::Vector2f m_Position;
};

#endif
