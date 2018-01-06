#include "InputManager.h"

InputManager& InputManager::GetInstance()
{
	static InputManager inputManager;
	return inputManager;
}

void InputManager::Update()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		m_Any = true;
		m_MouseLeft = true;
		if (!m_Click)
			m_MouseLeftClick = true;
		m_Click = true;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		m_Any = true;
		m_MouseRight = true;
		if (!m_Click)
			m_MouseRightClick = true;
		m_Click = true;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
	{
		m_Any = true;
		m_MouseMiddle = true;
		if (!m_Click)
			m_MouseMiddleClick = true;
		m_Click = true;
	}
	if (!m_MouseLeft && !m_MouseRight && !m_MouseMiddle)
		m_Click = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return)) m_Return = true, m_Any = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) m_Space = true, m_Any = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::BackSpace)) m_Backspace = true, m_Any = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)) m_Shift = true, m_Any = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) m_W = true, m_Any = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) m_A = true, m_Any = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) m_S = true, m_Any = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) m_D = true, m_Any = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) m_Up = true, m_Any = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) m_Left = true, m_Any = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) m_Down = true, m_Any = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) m_Right = true, m_Any = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O)) m_O = true, m_Any = true;
}

void InputManager::Clear()
{
	m_MouseLeft = false;
	m_MouseLeftClick = false;
	m_MouseRight = false;
	m_MouseRightClick = false;
	m_MouseMiddle = false;
	m_MouseMiddleClick = false;
	m_Return = false;
	m_Space = false;
	m_Backspace = false;
	m_Shift = false;
	m_W = false;
	m_A = false;
	m_S = false;
	m_D = false;
	m_Up = false;
	m_Left = false;
	m_Down = false;
	m_Right = false;
	m_O = false;
	m_Any = false;
	m_MouseWheelValue = 0;
}

void InputManager::SetMouseWheelValue(int value)
{
	m_MouseWheelValue = value;
}

int InputManager::GetMouseWheelValue() const
{
	return m_MouseWheelValue;
}

bool InputManager::GetInput(Input input) const
{
	if (input == MOUSELEFT) return m_MouseLeft;
	else if (input == MOUSELEFTCLICK) return m_MouseLeftClick;
	else if (input == MOUSERIGHT) return m_MouseRight;
	else if (input == MOUSERIGHTCLICK) return m_MouseRightClick;
	else if (input == MOUSEMIDDLE) return m_MouseMiddle;
	else if (input == MOUSEMIDDLECLICK) return m_MouseMiddleClick;
	else if (input == RETURN) return m_Return;
	else if (input == SPACE) return m_Space;
	else if (input == BACKSPACE) return m_Backspace;
	else if (input == SHIFT) return m_Shift;
	else if (input == KEYW) return m_W;
	else if (input == KEYA) return m_A;
	else if (input == KEYS) return m_S;
	else if (input == KEYD) return m_D;
	else if (input == KEYUP) return m_Up;
	else if (input == KEYLEFT) return m_Left;
	else if (input == KEYDOWN) return m_Down;
	else if (input == KEYRIGHT) return m_Right;
	else if (input == KEYO) return m_O;
	else if (input == KEYANY) return m_Any;
	else return false;
}

InputManager::InputManager()
{

}