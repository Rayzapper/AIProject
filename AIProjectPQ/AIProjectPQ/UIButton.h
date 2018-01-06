#ifndef RAYZAPPER_UIBUTTON_H
#define RAYZAPPER_UIBUTTON_H

#include "GameObject.h"

class UIButton : public GameObject
{
public:
	UIButton(sf::IntRect hitbox, sf::String buttonString, sf::Font *font);
	UIButton(sf::IntRect hitbox, sf::String buttonString, sf::Font *font, sf::Shape *drawShape);
	~UIButton();
	virtual void RenderSprite(sf::RenderWindow *window);
	virtual void RenderShape(sf::RenderWindow *window);
	bool GetMouseover(sf::Vector2i mouseScreenPosition);
	sf::IntRect GetHitbox() const;
	sf::String GetString() const;
	virtual void SetPosition(sf::Vector2f position);
	void SetHitbox(sf::IntRect hitbox);
	void SetString(sf::String buttonString);
private:
	sf::IntRect m_Hitbox;
	sf::String m_ButtonString;
	sf::Text m_ButtonText;
	sf::Vector2f m_TextPosition = sf::Vector2f(0.f, 0.f);
};

#endif