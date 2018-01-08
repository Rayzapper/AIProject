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

void GameBehavior::StartGame(bool AI)
{
	m_GameState->StartGame(AI);
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

void GameBehavior::ChangeTurn()
{
	m_GameState->ChangeTurn();
}

bool GameBehavior::GetAI()
{
	return m_GameState->GetAI();
}

size_t GameBehavior::GetCurrentTurn()
{
	return m_GameState->GetCurrentTurn();
}

int GameBehavior::GetPlayerHealth(size_t index)
{
	return m_GameState->GetPlayerHealth(index);
}

bool GameBehavior::GetPlayerWaiting(size_t index)
{
	return m_GameState->GetPlayerWaiting(index);
}

void GameBehavior::SetPlayerWaiting(size_t index, bool waiting)
{
	m_GameState->SetPlayerWaiting(index, waiting);
}

void GameBehavior::OffsetPlayerHealth(size_t index, int offset)
{
	m_GameState->OffsetPlayerHealth(index, offset);
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
	for (size_t i = 0; i < m_ChildStates.size(); i++)
	{
		if (from == m_ChildStates[i])
		{
			m_ChildStates[i]->Exit(false);
			delete m_ChildStates[i];
			m_ChildStates[i] = to;
			m_ChildStates[i]->Enter(false);
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
	for (size_t i = 0; i < m_ChildStates.size(); i++)
	{
		m_ChildStates[i]->Enter(initialization);
	}
}

void GameBehavior::CompositeState::Exit(bool finalization)
{
	for (size_t i = 0; i < m_ChildStates.size(); i++)
	{
		m_ChildStates[i]->Exit(finalization);
		delete m_ChildStates[i];
	}
	m_ChildStates.clear();
}

bool GameBehavior::CompositeState::StartGame(bool AI)
{
	bool handled = false;
	for (size_t i = 0; i < m_ChildStates.size(); i++)
	{
		if (m_ChildStates[i]->StartGame(AI))
			handled = true;
	}
	return handled;
}

bool GameBehavior::CompositeState::MainMenu()
{
	bool handled = false;
	for (size_t i = 0; i < m_ChildStates.size(); i++)
	{
		if (m_ChildStates[i]->MainMenu())
			handled = true;
	}
	return handled;
}

bool GameBehavior::CompositeState::OptionsMenu()
{
	bool handled = false;
	for (size_t i = 0; i < m_ChildStates.size(); i++)
	{
		if (m_ChildStates[i]->OptionsMenu())
			handled = true;
	}
	return handled;
}

bool GameBehavior::CompositeState::TogglePause()
{
	bool handled = false;
	for (size_t i = 0; i < m_ChildStates.size(); i++)
	{
		if (m_ChildStates[i]->TogglePause())
			handled = true;
	}
	return handled;
}

void GameBehavior::CompositeState::ChangeTurn() {}

bool GameBehavior::CompositeState::GetAI()
{
	return false;
}

size_t GameBehavior::CompositeState::GetCurrentTurn()
{
	return 2;
}

int GameBehavior::CompositeState::GetPlayerHealth(size_t index)
{
	return -1;
}

bool GameBehavior::CompositeState::GetPlayerWaiting(size_t index)
{
	return false;
}

void GameBehavior::CompositeState::SetPlayerWaiting(size_t index, bool waiting) {}

void GameBehavior::CompositeState::OffsetPlayerHealth(size_t index, int offset) {}

bool GameBehavior::CompositeState::Update(float dt)
{
	bool handled = false;
	for (size_t i = 0; i < m_ChildStates.size(); i++)
	{
		if (m_ChildStates[i]->Update(dt))
			handled = true;
	}
	return handled;
}

bool GameBehavior::CompositeState::Render()
{
	bool handled = false;
	for (size_t i = 0; i < m_ChildStates.size(); i++)
	{
		if (m_ChildStates[i]->Render())
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

bool GameBehavior::DecoratedState::StartGame(bool AI)
{
	return m_ChildState->StartGame(AI);
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

void GameBehavior::DecoratedState::ChangeTurn()
{
	m_ChildState->ChangeTurn();
}

bool GameBehavior::DecoratedState::GetAI()
{
	return m_ChildState->GetAI();
}

size_t GameBehavior::DecoratedState::GetCurrentTurn()
{
	return m_ChildState->GetCurrentTurn();
}

int GameBehavior::DecoratedState::GetPlayerHealth(size_t index)
{
	return m_ChildState->GetPlayerHealth(index);
}

bool GameBehavior::DecoratedState::GetPlayerWaiting(size_t index)
{
	return m_ChildState->GetPlayerWaiting(index);
}

void GameBehavior::DecoratedState::SetPlayerWaiting(size_t index, bool waiting)
{
	m_ChildState->SetPlayerWaiting(index, waiting);
}

void GameBehavior::DecoratedState::OffsetPlayerHealth(size_t index, int offset)
{
	m_ChildState->OffsetPlayerHealth(index, offset);
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

bool GameBehavior::LeafState::StartGame(bool AI)
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

void GameBehavior::LeafState::ChangeTurn() {}

bool GameBehavior::LeafState::GetAI()
{
	return false;
}

size_t GameBehavior::LeafState::GetCurrentTurn()
{
	return 2;
}

int GameBehavior::LeafState::GetPlayerHealth(size_t index)
{
	return -1;
}

bool GameBehavior::LeafState::GetPlayerWaiting(size_t index)
{
	return false;
}

void GameBehavior::LeafState::SetPlayerWaiting(size_t index, bool waiting) {}

void GameBehavior::LeafState::OffsetPlayerHealth(size_t index, int offset) {}

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

bool GameBehavior::GameState::StartGame(bool AI)
{
	Transit(m_ChildState, new PlayGameState(this, m_GameBehavior, AI));
	return true;
}

bool GameBehavior::GameState::MainMenu()
{
	if (!m_ChildState->MainMenu())
	{
		Transit(m_ChildState, new MenuGameState(this, m_GameBehavior));
		m_ChildState->MainMenu();
	}
	return m_ChildState->MainMenu();
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
		m_ChildState->MainMenu();
	}
	return m_ChildState->MainMenu();
}

bool GameBehavior::MenuGameState::OptionsMenu()
{
	if (!m_ChildState->OptionsMenu())
	{
		Transit(m_ChildState, new OptionsMenuState(this, m_GameBehavior));
		m_ChildState->OptionsMenu();
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
	for (size_t i = 0; i < 4; i++)
	{
		sf::RectangleShape *shape = new sf::RectangleShape(sf::Vector2f(300.f, 80.f));
		shape->setFillColor(sf::Color(0, 64, 192, 255));
		shape->setOutlineColor(sf::Color(255, 255, 255, 255));
		shape->setOutlineThickness(-5.f);
		m_Buttons[i] = new UIButton(sf::IntRect(0, 0, 300, 80), "", GetBasicFont(), shape);
		m_Buttons[i]->SetPosition(sf::Vector2f(100.f, (i + 1) * 100.f));
	}
	m_Buttons[0]->SetString("Play against Player");
	m_Buttons[1]->SetString("Play against AI");
	m_Buttons[2]->SetString("Options");
	m_Buttons[3]->SetString("Exit");
}

void GameBehavior::MainMenuState::Exit(bool finalization)
{
	for (size_t i = 0; i < 4; i++)
	{
		delete m_Buttons[i];
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
		for (size_t i = 0; i < 4; i++)
		{
			if (m_Buttons[i]->GetMouseover(GetMouseScreenPosition()))
			{
				sf::String buttonString = m_Buttons[i]->GetString();
				if (buttonString == "Play against Player")
					m_GameBehavior->StartGame(false);
				else if (buttonString == "Play against AI")
					m_GameBehavior->StartGame(true);
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
	for (size_t i = 0; i < 4; i++)
	{
		m_Buttons[i]->Render(m_GameBehavior->m_Window);
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
	for (size_t i = 0; i < 3; i++)
	{
		delete m_Buttons[i];
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
		for (size_t i = 0; i < 3; i++)
		{
			if (m_Buttons[i]->GetMouseover(GetMouseScreenPosition()))
			{
				sf::String buttonString = m_Buttons[i]->GetString();
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
	for (size_t i = 0; i < 3; i++)
	{
		m_Buttons[i]->Render(m_GameBehavior->m_Window);
	}
	return true;
}


GameBehavior::PlayGameState::PlayGameState(State *parentState, GameBehavior *gameBehavior, bool AI)
	: CompositeState(parentState, gameBehavior)
{
	m_Paused = false;
	m_ChildStates.push_back(new PausePlayState(parentState, gameBehavior));
	m_Board = new Board();
	m_SelectionShape.setFillColor(sf::Color::Transparent);
	m_SelectionShape.setOutlineColor(sf::Color::Yellow);
	m_SelectionShape.setOutlineThickness(-5.f);
	m_SelectionShape.setSize(sf::Vector2f(95.f, 95.f));
	m_SelectionShape.setOrigin(sf::Vector2f(47.5f, 47.5f));
	m_SelectedSlot = nullptr;

	PlayerState *playerState = new PlayerState(parentState, gameBehavior, AI);
	m_ChildStates.push_back(playerState);
}

void GameBehavior::PlayGameState::Enter(bool initialization)
{
	for (size_t i = 0; i < m_ChildStates.size(); i++)
	{
		m_ChildStates[i]->Enter(initialization);
	}
}

void GameBehavior::PlayGameState::Exit(bool finalization)
{
	for (size_t i = 0; i < m_ChildStates.size(); i++)
	{
		m_ChildStates[i]->Exit(finalization);
		delete m_ChildStates[i];
	}
	delete m_Board;
}

bool GameBehavior::PlayGameState::TogglePause()
{
	m_Paused = !m_Paused;
	return true;
}

void GameBehavior::PlayGameState::ChangeTurn()
{
	m_ChildStates[1]->ChangeTurn();
}

bool GameBehavior::PlayGameState::GetAI()
{
	return m_ChildStates[1]->GetAI();
}

size_t GameBehavior::PlayGameState::GetCurrentTurn()
{
	return m_ChildStates[1]->GetCurrentTurn();
}

int GameBehavior::PlayGameState::GetPlayerHealth(size_t index)
{
	return m_ChildStates[1]->GetPlayerHealth(index);
}

bool GameBehavior::PlayGameState::GetPlayerWaiting(size_t index)
{
	return m_ChildStates[1]->GetPlayerWaiting(index);
}

void GameBehavior::PlayGameState::SetPlayerWaiting(size_t index, bool waiting)
{
	m_ChildStates[1]->SetPlayerWaiting(index, waiting);
}

void GameBehavior::PlayGameState::OffsetPlayerHealth(size_t index, int offset)
{
	m_ChildStates[1]->OffsetPlayerHealth(index, offset);
}

bool GameBehavior::PlayGameState::Update(float dt)
{
	if (InputManager::GetInstance().GetInput(ESCCLICK))
		TogglePause();
	if (m_Paused)
	{
		bool handled = false;
		for (size_t i = 0; i < m_ChildStates.size(); i++)
		{
			if (m_ChildStates[i]->Update(dt))
				handled = true;
		}
		return handled;
	}
	else
	{
		m_ChildStates[1]->Update(dt);
		if (InputManager::GetInstance().GetInput(MOUSELEFTCLICK) && (!m_GameBehavior->GetPlayerWaiting(0) || (!m_GameBehavior->GetPlayerWaiting(1) && !m_GameBehavior->GetAI())))
		{
			for (size_t y = 0; y < 8; y++)
			{
				for (size_t x = 0; x < 8; x++)
				{
					if (m_Board->GetSlot(y, x)->GetMouseover(GetMouseScreenPosition()))
					{
						if (m_SelectedSlot == nullptr)
							m_SelectedSlot = m_Board->GetSlot(y, x);
						else
						{
							for (size_t i = 0; i < 4; i++)
							{
								if (m_Board->GetSlot(y, x) == m_SelectedSlot->GetNeighbor(i))
								{
									m_Board->SwapPieces(m_SelectedSlot, m_SelectedSlot->GetNeighbor(i));
								}
							}
							m_SelectedSlot = nullptr;
						}
					}
				}
			}
		}
		else if (InputManager::GetInstance().GetInput(MOUSERIGHTCLICK))
		{
			m_SelectedSlot = nullptr;
		}
		if (m_SelectedSlot != nullptr)
		{
			m_SelectionShape.setPosition(m_SelectedSlot->GetPosition());
		}
		m_Board->Update(dt);
		return true;
	}
}

bool GameBehavior::PlayGameState::Render()
{
	m_Board->Render(GetWindow());
	if (m_SelectedSlot != nullptr)
	{
		GetWindow()->draw(m_SelectionShape);
	}
	if (m_Paused)
	{
		bool handled = false;
		for (size_t i = 0; i < m_ChildStates.size(); i++)
		{
			if (m_ChildStates[i]->Render())
				handled = true;
		}
		return handled;
	}
	else return true;
}


GameBehavior::PausePlayState::PausePlayState(State *parentState, GameBehavior *gameBehavior)
	: LeafState(parentState, gameBehavior)
{
	m_Background = new sf::RectangleShape(sf::Vector2f(1200.f, 900.f));
	m_Background->setFillColor(sf::Color(0, 0, 0, 128));
	for (size_t i = 0; i < 3; i++)
	{
		sf::RectangleShape *shape = new sf::RectangleShape(sf::Vector2f(300.f, 80.f));
		shape->setFillColor(sf::Color(0, 64, 192, 255));
		shape->setOutlineColor(sf::Color(255, 255, 255, 255));
		shape->setOutlineThickness(-5.f);
		m_Buttons[i] = new UIButton(sf::IntRect(0, 0, 300, 80), "", GetBasicFont(), shape);
		m_Buttons[i]->SetPosition(sf::Vector2f(450.f, (i + 3) * 100.f));
	}
	m_Buttons[0]->SetString("Back to Game");
	m_Buttons[1]->SetString("Back to Main Menu");
	m_Buttons[2]->SetString("Exit Game");
}

void GameBehavior::PausePlayState::Exit(bool finalization)
{
	delete m_Background;
	delete m_Buttons[0];
	delete m_Buttons[1];
	delete m_Buttons[2];
}

bool GameBehavior::PausePlayState::Update(float dt)
{
	if (InputManager::GetInstance().GetInput(MOUSELEFTCLICK))
	{
		for (size_t i = 0; i < 3; i++)
		{
			if (m_Buttons[i]->GetMouseover(GetMouseScreenPosition()))
			{
				sf::String buttonString = m_Buttons[i]->GetString();
				if (buttonString == "Back to Game")
					m_GameBehavior->TogglePause();
				else if (buttonString == "Back to Main Menu")
					m_GameBehavior->MainMenu();
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
	for (size_t i = 0; i < 3; i++)
	{
		m_Buttons[i]->Render(m_GameBehavior->m_Window);
	}
	return true;
}


GameBehavior::PlayerState::PlayerState(State *parentState, GameBehavior *gameBehavior, bool AI)
	: LeafState(parentState, gameBehavior)
{
	m_AI = AI;
	m_PlayerHealth[0] = 30;
	m_PlayerHealth[1] = 30;
	m_PlayerWaiting[0] = true;
	m_PlayerWaiting[1] = true;
	m_CurrentPlayerTurn = 0;
}

void GameBehavior::PlayerState::ChangeTurn()
{
	if (m_CurrentPlayerTurn == 0)
		m_CurrentPlayerTurn = 1;
	else
		m_CurrentPlayerTurn = 0;
}

bool GameBehavior::PlayerState::GetAI()
{
	return m_AI;
}

size_t GameBehavior::PlayerState::GetCurrentTurn()
{
	return m_CurrentPlayerTurn;
}

int GameBehavior::PlayerState::GetPlayerHealth(size_t index)
{
	return m_PlayerHealth[index];
}

bool GameBehavior::PlayerState::GetPlayerWaiting(size_t index)
{
	return m_PlayerWaiting[index];
}

void GameBehavior::PlayerState::SetPlayerWaiting(size_t index, bool waiting)
{
	m_PlayerWaiting[index] = waiting;
}

void GameBehavior::PlayerState::OffsetPlayerHealth(size_t index, int offset)
{
	m_PlayerHealth[index] += offset;
}