#ifndef RAYZAPPER_GAME_H
#define RAYZAPPER_GAME_H

#include "GameBehavior.h"

class Camera;

class Game
{
public:
	Game();
	~Game();
	void Run();
private:
	void Update(float dt, sf::Vector2i mouseScreenPosition);
	void Render();
	void Initialize(int windowWidth, int windowHeight);
	sf::RenderWindow *m_Window;
	Camera *m_Camera;
	GameBehavior *m_GameBehavior;
};

#endif