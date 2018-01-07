#include "GameBehavior.h"
#include "InputManager.h"
#include <iostream>
#include "PieceBehavior.h"

GameBehavior::GameBehavior()
{
	m_GameState = new GameState(nullptr, this);
}

GameBehavior::~GameBehavior()
{
	m_GameState->Exit(true);
	delete m_GameState;
	delete m_Window;
}

void GameBehavior::Run()
{
	int windowWidth = 1200, windowHeight = 900;

	// Window creation
	Initialize(windowWidth, windowHeight);

	sf::Clock deltaClock;

	float dt;

	bool focus = true;

	while (m_Running)
	{
		dt = deltaClock.restart().asSeconds();

		sf::Event event;
		while (m_Window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				ExitGame();
			if (event.type == sf::Event::GainedFocus)
				focus = true;
			if (event.type == sf::Event::LostFocus)
				focus = false;
			if (event.type == sf::Event::Resized)
			{
				int newWidth = event.size.width, newHeight = event.size.height;
			}
		}

		m_MouseScreenPosition = sf::Mouse::getPosition(*m_Window);

		// --------------------Update--------------------

		Update(dt);

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

void GameBehavior::StartGame()
{
	m_GameState->StartGame();
}

void GameBehavior::MainMenu()
{
	m_GameState->MainMenu();
}

void GameBehavior::OptionsMenu()
{
	m_GameState->OptionsMenu();
}

void GameBehavior::ExitGame()
{
	m_Running = false;
}

void GameBehavior::TogglePause()
{
	m_GameState->TogglePause();
}

void GameBehavior::Update(float dt)
{
	InputManager::GetInstance().Update();
	m_GameState->Update(dt);
	InputManager::GetInstance().Clear();
}

void GameBehavior::Render()
{
	m_GameState->Render();
}

void GameBehavior::Initialize(int windowWidth, int windowHeight)
{
	sf::String programName = "TopDownAIProject";

	m_Window = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), programName, sf::Style::Default);
	m_Window->setFramerateLimit(60);
	m_Window->setVerticalSyncEnabled(true);

	if (!m_BasicFont.loadFromFile("Resources/Fonts/calibri.ttf"))
		cout << "Could not find font calibri.ttf" << endl;
}


sf::RenderWindow* GameBehavior::State::GetWindow() const
{
	return m_GameBehavior->m_Window;
}

sf::Vector2i GameBehavior::State::GetMouseScreenPosition() const
{
	return m_GameBehavior->m_MouseScreenPosition;
}

sf::Font* GameBehavior::State::GetBasicFont() const
{
	return &m_GameBehavior->m_BasicFont;
}

GameBehavior::State::State(State *parentState, GameBehavior *gameBehavior)
	: m_ParentState(parentState),
	m_GameBehavior(gameBehavior)
{

}


void GameBehavior::CompositeState::Transit(State *from, State *to)
{
	bool contains = false;
	for each (State *s in m_ChildStates)
	{
		if (from == s)
		{
			s->Exit(false);
			delete s;
			s = to;
			s->Enter(false);
			contains = true;
			break;
		}
	}
	if (!contains)
	{
		m_ParentState->Transit(from, to);
	}
}

void GameBehavior::CompositeState::Enter(bool initialization)
{
	for each (State *s in m_ChildStates)
	{
		s->Enter(initialization);
	}
}

void GameBehavior::CompositeState::Exit(bool finalization)
{
	for each (State *s in m_ChildStates)
	{
		s->Exit(finalization);
		delete s;
	}
	m_ChildStates.clear();
}

bool GameBehavior::CompositeState::StartGame()
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->StartGame())
			handled = true;
	}
	return handled;
}

bool GameBehavior::CompositeState::MainMenu()
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->MainMenu())
			handled = true;
	}
	return handled;
}

bool GameBehavior::CompositeState::OptionsMenu()
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->OptionsMenu())
			handled = true;
	}
	return handled;
}

bool GameBehavior::CompositeState::TogglePause()
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->TogglePause())
			handled = true;
	}
	return handled;
}

bool GameBehavior::CompositeState::Update(float dt)
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->Update(dt))
			handled = true;
	}
	return handled;
}

bool GameBehavior::CompositeState::Render()
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->Render())
			handled = true;
	}
	return handled;
}

GameBehavior::CompositeState::CompositeState(State *parentState, GameBehavior *gameBehavior)
	: State(parentState, gameBehavior)
{

}


void GameBehavior::DecoratedState::Transit(State *from, State *to)
{
	if (m_ChildState == from)
	{
		m_ChildState->Exit(false);
		delete m_ChildState;
		m_ChildState = to;
		m_ChildState->Enter(false);
	}
	else
	{
		m_ParentState->Transit(from, to);
	}
}

void GameBehavior::DecoratedState::Enter(bool initialization)
{
	m_ChildState->Enter(initialization);
}

void GameBehavior::DecoratedState::Exit(bool finalization)
{
	m_ChildState->Exit(finalization);
	delete m_ChildState;
}

bool GameBehavior::DecoratedState::StartGame()
{
	return m_ChildState->StartGame();
}

bool GameBehavior::DecoratedState::MainMenu()
{
	return m_ChildState->MainMenu();
}

bool GameBehavior::DecoratedState::OptionsMenu()
{
	return m_ChildState->OptionsMenu();
}

bool GameBehavior::DecoratedState::TogglePause()
{
	return m_ChildState->TogglePause();
}

bool GameBehavior::DecoratedState::Update(float dt)
{
	return m_ChildState->Update(dt);
}

bool GameBehavior::DecoratedState::Render()
{
	return m_ChildState->Render();
}

GameBehavior::DecoratedState::DecoratedState(State *parentState, GameBehavior *gameBehavior)
	: State(parentState, gameBehavior)
{

}


void GameBehavior::LeafState::Transit(State *from, State *to)
{
	m_ParentState->Transit(from, to);
}

void GameBehavior::LeafState::Enter(bool initialization){}

void GameBehavior::LeafState::Exit(bool finalization){}

bool GameBehavior::LeafState::StartGame()
{
	return false;
}

bool GameBehavior::LeafState::MainMenu()
{
	return false;
}

bool GameBehavior::LeafState::OptionsMenu()
{
	return false;
}

bool GameBehavior::LeafState::TogglePause()
{
	return false;
}

bool GameBehavior::LeafState::Update(float dt)
{
	return false;
}

bool GameBehavior::LeafState::Render()
{
	return false;
}

GameBehavior::LeafState::LeafState(State *parentState, GameBehavior *gameBehavior)
	: State(parentState, gameBehavior)
{

}


GameBehavior::GameState::GameState(State *parentState, GameBehavior *gameBehavior)
	: DecoratedState(parentState, gameBehavior)
{
	m_ChildState = new MenuGameState(this, gameBehavior);
}

bool GameBehavior::GameState::StartGame()
{
	Transit(m_ChildState, new PlayGameState(this, m_GameBehavior));
	return true;
}


GameBehavior::MenuGameState::MenuGameState(State *parentState, GameBehavior *gameBehavior)
	: DecoratedState(parentState, gameBehavior)
{
	m_ChildState = new TitleMenuState(parentState, gameBehavior);
}

bool GameBehavior::MenuGameState::MainMenu()
{
	if (!m_ChildState->MainMenu())
	{
		Transit(m_ChildState, new MainMenuState(this, m_GameBehavior));
	}
	return m_ChildState->MainMenu();
}

bool GameBehavior::MenuGameState::OptionsMenu()
{
	if (!m_ChildState->OptionsMenu())
	{
		Transit(m_ChildState, new OptionsMenuState(this, m_GameBehavior));
	}
	return m_ChildState->OptionsMenu();
}

bool GameBehavior::MenuGameState::Render()
{
	return m_ChildState->Render();
}


GameBehavior::TitleMenuState::TitleMenuState(State *parentState, GameBehavior *gameBehavior)
	: LeafState(parentState, gameBehavior)
{
	m_TitleText.setString("TopDownAIProject");
	m_ContinueText.setString("Press Space to Continue");
	m_TitleText.setCharacterSize(60);
	m_ContinueText.setCharacterSize(20);
	m_TitleText.setFillColor(sf::Color::Red);
	m_ContinueText.setFillColor(sf::Color::White);
	m_TitleText.setPosition(sf::Vector2f(100.f, 100.f));
	m_ContinueText.setPosition(sf::Vector2f(100.f, 200.f));
	m_TitleText.setFont(*GetBasicFont());
	m_ContinueText.setFont(*GetBasicFont());
}

bool GameBehavior::TitleMenuState::Update(float dt)
{
	if (InputManager::GetInstance().GetInput(SPACE))
		m_GameBehavior->MainMenu();
	return true;
}

bool GameBehavior::TitleMenuState::Render()
{
	GetWindow()->draw(m_TitleText);
	GetWindow()->draw(m_ContinueText);
	return true;
}


GameBehavior::MainMenuState::MainMenuState(State *parentState, GameBehavior *gameBehavior)
	: LeafState(parentState, gameBehavior)
{
	for (size_t i = 0; i < 3; i++)
	{
		sf::RectangleShape *shape = new sf::RectangleShape(sf::Vector2f(300.f, 80.f));
		shape->setFillColor(sf::Color(0, 64, 192, 255));
		shape->setOutlineColor(sf::Color(255, 255, 255, 255));
		shape->setOutlineThickness(-5.f);
		m_Buttons[i] = new UIButton(sf::IntRect(0, 0, 300, 80), "", GetBasicFont(), shape);
		m_Buttons[i]->SetPosition(sf::Vector2f(100.f, (i + 1) * 100.f));
	}
	m_Buttons[0]->SetString("Play");
	m_Buttons[1]->SetString("Options");
	m_Buttons[2]->SetString("Exit");
}

void GameBehavior::MainMenuState::Exit(bool finalization)
{
	for each (UIButton *b in m_Buttons)
	{
		delete b;
	}
}

bool GameBehavior::MainMenuState::MainMenu()
{
	return true;
}

bool GameBehavior::MainMenuState::Update(float dt)
{
	if (InputManager::GetInstance().GetInput(MOUSELEFTCLICK))
	{
		for each (UIButton *b in m_Buttons)
		{
			if (b->GetMouseover(GetMouseScreenPosition()))
			{
				sf::String buttonString = b->GetString();
				if (buttonString == "Play")
					m_GameBehavior->StartGame();
				else if (buttonString == "Options")
					m_GameBehavior->OptionsMenu();
				else
					m_GameBehavior->ExitGame();
				break;
			}
		}
	}
	return true;
}

bool GameBehavior::MainMenuState::Render()
{
	for each (UIButton *b in m_Buttons)
	{
		b->Render(m_GameBehavior->m_Window);
	}
	return true;
}


GameBehavior::OptionsMenuState::OptionsMenuState(State *parentState, GameBehavior *gameBehavior)
	: LeafState(parentState, gameBehavior)
{
	for (size_t i = 0; i < 3; i++)
	{
		sf::RectangleShape *shape = new sf::RectangleShape(sf::Vector2f(300.f, 80.f));
		shape->setFillColor(sf::Color(0, 64, 192, 255));
		shape->setOutlineColor(sf::Color(255, 255, 255, 255));
		shape->setOutlineThickness(-5.f);
		m_Buttons[i] = new UIButton(sf::IntRect(0, 0, 300, 80), "", GetBasicFont(), shape);
		m_Buttons[i]->SetPosition(sf::Vector2f(100.f, (i + 1) * 100.f));
	}
	m_Buttons[0]->SetString("Back");
	m_Buttons[1]->SetString("Option1");
	m_Buttons[2]->SetString("Option2");
}

void GameBehavior::OptionsMenuState::Exit(bool finalization)
{
	for each (UIButton *b in m_Buttons)
	{
		delete b;
	}
}

bool GameBehavior::OptionsMenuState::OptionsMenu()
{
	return true;
}

bool GameBehavior::OptionsMenuState::Update(float dt)
{
	if (InputManager::GetInstance().GetInput(MOUSELEFTCLICK))
	{
		for each (UIButton *b in m_Buttons)
		{
			if (b->GetMouseover(GetMouseScreenPosition()))
			{
				sf::String buttonString = b->GetString();
				if (buttonString == "Back")
					m_GameBehavior->MainMenu();
				else if (buttonString == "Option1")
					;
				else
					;
				break;
			}
		}
	}
	return true;
}

bool GameBehavior::OptionsMenuState::Render()
{
	for each (UIButton *b in m_Buttons)
	{
		b->Render(m_GameBehavior->m_Window);
	}
	return true;
}


GameBehavior::PlayGameState::PlayGameState(State *parentState, GameBehavior *gameBehavior)
	: DecoratedState(parentState, gameBehavior)
{
	m_Paused = false;
	m_ChildState = new PausePlayState(parentState, gameBehavior);
	m_Board = new Board();
}

void GameBehavior::PlayGameState::Enter(bool initialization)
{
	m_ChildState->Enter(initialization);
}

void GameBehavior::PlayGameState::Exit(bool finalization)
{
	m_ChildState->Exit(finalization);
	delete m_ChildState;
	delete m_Board;
}

bool GameBehavior::PlayGameState::TogglePause()
{
	m_Paused = !m_Paused;
	return true;
}

bool GameBehavior::PlayGameState::Update(float dt)
{
	if (InputManager::GetInstance().GetInput(ESCCLICK))
		TogglePause();
	if (m_Paused) return m_ChildState->Update(dt);
	else
	{

		m_Board->Update(dt);
		return true;
	}
}

bool GameBehavior::PlayGameState::Render()
{
	m_Board->Render(GetWindow());
	if (m_Paused) return m_ChildState->Render();
	else return true;
}


GameBehavior::PausePlayState::PausePlayState(State *parentState, GameBehavior *gameBehavior)
	: LeafState(parentState, gameBehavior)
{
	m_Background = new sf::RectangleShape(sf::Vector2f(1200.f, 900.f));
	m_Background->setFillColor(sf::Color(0, 0, 0, 128));
	for (size_t i = 0; i < 2; i++)
	{
		sf::RectangleShape *shape = new sf::RectangleShape(sf::Vector2f(300.f, 80.f));
		shape->setFillColor(sf::Color(0, 64, 192, 255));
		shape->setOutlineColor(sf::Color(255, 255, 255, 255));
		shape->setOutlineThickness(-5.f);
		m_Buttons[i] = new UIButton(sf::IntRect(0, 0, 300, 80), "", GetBasicFont(), shape);
		m_Buttons[i]->SetPosition(sf::Vector2f(450.f, (i + 4) * 100.f));
	}
	m_Buttons[0]->SetString("Back to Game");
	m_Buttons[1]->SetString("Exit Game");
}

void GameBehavior::PausePlayState::Exit(bool finalization)
{
	delete m_Background;
	delete m_Buttons[0];
	delete m_Buttons[1];
}

bool GameBehavior::PausePlayState::Update(float dt)
{
	if (InputManager::GetInstance().GetInput(MOUSELEFTCLICK))
	{
		for each (UIButton *b in m_Buttons)
		{
			if (b->GetMouseover(GetMouseScreenPosition()))
			{
				sf::String buttonString = b->GetString();
				if (buttonString == "Back to Game")
					m_GameBehavior->TogglePause();
				else
					m_GameBehavior->ExitGame();
				break;
			}
		}
	}
	return true;
}

bool GameBehavior::PausePlayState::Render()
{
	GetWindow()->draw(*m_Background);
	for each (UIButton *b in m_Buttons)
	{
		b->Render(m_GameBehavior->m_Window);
	}
	return true;
}