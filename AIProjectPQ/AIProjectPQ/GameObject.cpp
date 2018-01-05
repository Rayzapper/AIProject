#include "GameObject.h"

sf::Vector2f GameObject::GetPosition() const
{
	return m_Position;
}

void GameObject::SetPosition(sf::Vector2f position)
{
	m_Position = position;
}

GameObject::GameObject()
{

}