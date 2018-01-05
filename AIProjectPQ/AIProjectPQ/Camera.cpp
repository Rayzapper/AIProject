#include "Camera.h"
#include <cassert>

static const int cameraMoveSpeed = 6 * 60;
static sf::Vector2i mouseLastScreenPosition;

Camera::Camera(sf::RenderWindow *window)
	: m_Window(window)
{
	int windowWidth = (int)window->getSize().x, windowHeight = (int)window->getSize().y;
	m_MainView = sf::View(sf::Vector2f(0, 0), sf::Vector2f((float)windowWidth, (float)windowHeight));
	m_GUIView = sf::View(sf::Vector2f((float)windowWidth / 2, (float)windowHeight / 2), sf::Vector2f((float)windowWidth, (float)windowHeight));
}

Camera::~Camera()
{

}

void Camera::Update(float dt, sf::Vector2i mouseScreenPosition)
{
	if (m_Zoom != m_TargetZoom)
	{
		float zoomDifference = m_TargetZoom - m_Zoom;
		m_Zoom += zoomDifference / 3;
		m_MainView.setSize(m_GUIView.getSize() * m_Zoom);
	}

	sf::Vector2f viewCenter = m_MainView.getCenter(), screenMovement(0, 0);

	m_MainView.setCenter(viewCenter + (screenMovement * m_Zoom));

	mouseLastScreenPosition = mouseScreenPosition;

	sf::Vector2f mouseWorldVector = m_Window->mapPixelToCoords(mouseScreenPosition, m_MainView);
	m_MouseWorldPosition.x = mouseWorldVector.x;
	m_MouseWorldPosition.y = mouseWorldVector.y;
}

void Camera::UpdateWindowSize(sf::Vector2i windowSize)
{
	m_MainView.setSize((float)windowSize.x * m_Zoom, (float)windowSize.y * m_Zoom);
	m_GUIView.setSize((float)windowSize.x, (float)windowSize.y);
}

void Camera::SetView(ViewType type)
{
	if (type == MAINVIEW) m_Window->setView(m_MainView);
	else m_Window->setView(m_GUIView);
}

void Camera::SetZoom(float zoom)
{
	m_Zoom = zoom;
	m_TargetZoom = zoom;
}

void Camera::SetTargetZoom(float zoom)
{
	m_TargetZoom = zoom;
}

sf::Vector2f Camera::GetMouseWorldPosition() const
{
	return m_MouseWorldPosition;
}

float Camera::GetZoom() const
{
	return m_Zoom;
}

float Camera::GetTargetZoom() const
{
	return m_TargetZoom;
}