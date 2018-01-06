#include "UIButton.h"

UIButton::UIButton(sf::IntRect hitbox, sf::String buttonString, sf::Font *font)
	: GameObject()
{
	m_Hitbox = hitbox;
	m_ButtonString = buttonString;
	m_ButtonText.setString(buttonString);
	m_ButtonText.setFont(*font);
}

UIButton::UIButton(sf::IntRect hitbox, sf::String buttonString, sf::Font *font, sf::Shape *drawShape)
	: GameObject(drawShape)
{
	m_Hitbox = hitbox;
	m_ButtonString = buttonString;
	m_ButtonText.setString(buttonString);
	m_ButtonText.setFont(*font);
	m_TextPosition = sf::Vector2f(10.f, 40.f);
	m_ButtonText.setCharacterSize(30);
	m_ButtonText.setOrigin(0.f, 20.f);
}

UIButton::~UIButton()
{
	
}

void UIButton::RenderSprite(sf::RenderWindow *window)
{
	window->draw(m_Sprite);
	window->draw(m_ButtonText);
}

void UIButton::RenderShape(sf::RenderWindow *window)
{
	window->draw(*m_Shape);
	window->draw(m_ButtonText);
}

bool UIButton::GetMouseover(sf::Vector2i mouseScreenPosition)
{
	return m_Hitbox.contains(mouseScreenPosition);
}

sf::IntRect UIButton::GetHitbox() const
{
	return m_Hitbox;
}

sf::String UIButton::GetString() const
{
	return m_ButtonString;
}

void UIButton::SetPosition(sf::Vector2f position)
{
	m_Position = position;
	m_Sprite.setPosition(position);
	if (m_Shape != nullptr) m_Shape->setPosition(position);
	m_ButtonText.setPosition(position + m_TextPosition);
	m_Hitbox.left = (int)position.x;
	m_Hitbox.top = (int)position.y;
}

void UIButton::SetHitbox(sf::IntRect hitbox)
{
	m_Hitbox = hitbox;
}

void UIButton::SetString(sf::String buttonString)
{
	m_ButtonString = buttonString;
	m_ButtonText.setString(buttonString);
}