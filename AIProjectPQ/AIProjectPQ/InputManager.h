#ifndef RAYZAPPER_INPUTMANAGER_H
#define RAYZAPPER_INPUTMANAGER_H

#include <SFML\Graphics.hpp>

enum Input
{
	MOUSELEFT,
	MOUSELEFTCLICK,
	MOUSERIGHT,
	MOUSERIGHTCLICK,
	MOUSEMIDDLE,
	MOUSEMIDDLECLICK,
	RETURN,
	SPACE,
	BACKSPACE,
	SHIFT,
	KEYW,
	KEYA,
	KEYS,
	KEYD,
	KEYUP,
	KEYLEFT,
	KEYDOWN,
	KEYRIGHT,
	KEYO,
	KEYANY,
	ESC,
	ESCCLICK
};

class InputManager
{
public:
	static InputManager& GetInstance();
	void Update();
	void Clear();
	void SetMouseWheelValue(int value);
	int GetMouseWheelValue() const;
	bool GetInput(Input input) const;
private:
	InputManager();
	InputManager(const InputManager &inputManager);
	InputManager& operator=(const InputManager &inputManager);
	bool m_MouseLeft = false, m_MouseLeftClick = false, m_MouseRight = false, m_MouseRightClick = false, m_MouseMiddle = false, m_MouseMiddleClick = false,
		m_Return = false, m_Backspace = false, m_Shift = false,
		m_W = false, m_A = false, m_S = false, m_D = false,
		m_Up = false, m_Left = false, m_Down = false, m_Right = false,
		m_O = false,
		m_Space = false, m_Esc = false, m_EscClick = false,
		m_Any = false;
	bool m_Click = false;
	int m_MouseWheelValue = 0;
};

#endif