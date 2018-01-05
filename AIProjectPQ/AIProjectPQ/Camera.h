#ifndef RAYZAPPER_CAMERA_H
#define RAYZAPPER_CAMERA_H

#include <SFML/Graphics.hpp>

enum ViewType
{
	MAINVIEW,
	GUIVIEW
};

class Camera
{
public:
	Camera(sf::RenderWindow *window);
	~Camera();
	void Update(float dt, sf::Vector2i mouseScreenPosition);
	void UpdateWindowSize(sf::Vector2i windowSize);

	void SetView(ViewType type);
	void SetZoom(float zoom);
	void SetTargetZoom(float zoom);

	sf::Vector2f GetMouseWorldPosition() const;
	float GetZoom() const;
	float GetTargetZoom() const;
private:
	sf::RenderWindow *m_Window;
	sf::View m_MainView, m_GUIView;
	sf::Vector2f m_MouseWorldPosition;
	float m_Zoom = 1.f, m_TargetZoom = 1.f;
};

#endif