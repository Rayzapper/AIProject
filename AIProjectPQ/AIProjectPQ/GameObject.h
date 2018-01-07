#ifndef RAYZAPPER_GAMEOBJECT_H
#define RAYZAPPER_GAMEOBJECT_H

#include <SFML\Graphics.hpp>

#define PI (float)3.1415926

using namespace std;

class GameObject
{
public:
	GameObject();
	GameObject(sf::Shape *drawShape);
	~GameObject();
	void Render(sf::RenderWindow *window);
	virtual void RenderSprite(sf::RenderWindow *window);
	virtual void RenderShape(sf::RenderWindow *window);
	sf::Vector2f GetPosition() const;
	virtual void SetPosition(sf::Vector2f position);
	virtual void SetTexture(sf::Texture *texture);
	void SetShape(sf::Shape *drawShape);
	void OffsetPosition(sf::Vector2f offset);
protected:
	sf::Vector2f m_Position;
	sf::Sprite m_Sprite;
	sf::Shape *m_Shape = nullptr;
	bool m_RenderMode;
};

#endif
