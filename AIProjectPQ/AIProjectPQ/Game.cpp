#include "Game.h"
#include "Camera.h"

Game::Game()
{

}

Game::~Game()
{
	delete m_Window;
	delete m_Camera;
}

void Game::Run()
{
	int windowWidth = 900, windowHeight = 900;
	sf::Vector2i mouseScreenPosition;

	// Window creation
	Initialize(windowWidth, windowHeight);

	m_Camera = new Camera(m_Window);

	sf::Clock deltaClock;

	float dt;

	bool running = true, focus = true;

	while (running)
	{
		dt = deltaClock.restart().asSeconds();

		sf::Event event;
		while (m_Window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				running = false;
			if (event.type == sf::Event::GainedFocus)
				focus = true;
			if (event.type == sf::Event::LostFocus)
				focus = false;
			if (event.type == sf::Event::Resized)
			{
				int newWidth = event.size.width, newHeight = event.size.height;
				m_Camera->UpdateWindowSize(sf::Vector2i(newWidth, newHeight));
			}
		}

		mouseScreenPosition = sf::Mouse::getPosition(*m_Window);

		// --------------------Update--------------------

		Update(dt, mouseScreenPosition);

		// --------------------Render--------------------
		m_Window->clear();
		Render();

		// End frame
		m_Window->display();
		if (!focus)
		{
			sf::Time time = sf::milliseconds(100);
			sf::sleep(time);
		}
	}
}

void Game::Update(float dt, sf::Vector2i mouseScreenPosition)
{
	m_Camera->Update(dt, mouseScreenPosition);
	sf::Vector2f mouseWorldPosition = m_Camera->GetMouseWorldPosition();

	m_Camera->SetView(MAINVIEW);

	m_Camera->SetView(GUIVIEW);

}

void Game::Render()
{

}

void Game::Initialize(int windowWidth, int windowHeight)
{
	sf::String programName = "TopDownAIProject";

	m_Window = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), programName, sf::Style::Default);
	m_Window->setFramerateLimit(60);
	m_Window->setVerticalSyncEnabled(true);
}