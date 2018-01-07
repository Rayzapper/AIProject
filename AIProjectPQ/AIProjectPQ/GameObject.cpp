#include "GameObject.h"

GameObject::GameObject()
{
	m_RenderMode = true;
}

GameObject::GameObject(sf::Shape *drawShape)
	: m_Shape(drawShape)
{
	m_RenderMode = false;
}

GameObject::~GameObject()
{
	delete m_Shape;
}

void GameObject::Render(sf::RenderWindow *window)
{
	if (m_RenderMode) RenderSprite(window);
	else RenderShape(window);
}

void GameObject::RenderSprite(sf::RenderWindow *window)
{
	window->draw(m_Sprite);
}

void GameObject::RenderShape(sf::RenderWindow *window)
{
	window->draw(*m_Shape);
}

sf::Vector2f GameObject::GetPosition() const
{
	return m_Position;
}

void GameObject::SetPosition(sf::Vector2f position)
{
	m_Position = position;
	m_Sprite.setPosition(position);
	if (m_Shape != nullptr) m_Shape->setPosition(position);
}

void GameObject::SetTexture(sf::Texture *texture)
{
	m_Sprite.setTexture(*texture, true);
}

void GameObject::SetShape(sf::Shape *drawShape)
{
	m_Shape = drawShape;
}

void GameObject::OffsetPosition(sf::Vector2f offset)
{
	sf::Vector2f newPos = m_Position;
	newPos += offset;
	SetPosition(newPos);
}