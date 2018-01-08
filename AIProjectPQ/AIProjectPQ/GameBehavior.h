#ifndef RAYZAPPER_GAMEBEHAVIOR_H
#define RAYZAPPER_GAMEBEHAVIOR_H

#include <SFML\Graphics.hpp>
#include "UIButton.h"
#include "Board.h"

using namespace std;

class GameBehavior
{
public:
	GameBehavior();
	~GameBehavior();
	void Run();
private:
	void StartGame(bool AI);
	void MainMenu();
	void OptionsMenu();
	void ExitGame();
	void TogglePause();
	virtual void ChangeTurn();
	virtual bool GetAI();
	virtual size_t GetCurrentTurn();
	virtual int GetPlayerHealth(size_t index);
	virtual bool GetPlayerWaiting(size_t index);
	virtual void SetPlayerWaiting(size_t index, bool waiting);
	virtual void OffsetPlayerHealth(size_t index, int offset);

	class State
	{
	public:
		virtual void Transit(State *to, State *from) = 0;
		virtual void Enter(bool initialization) = 0;
		virtual void Exit(bool finalization) = 0;
		virtual bool StartGame(bool AI) = 0;
		virtual bool MainMenu() = 0;
		virtual bool OptionsMenu() = 0;
		virtual bool TogglePause() = 0;
		virtual void ChangeTurn() = 0;
		virtual bool GetAI() = 0;
		virtual size_t GetCurrentTurn() = 0;
		virtual int GetPlayerHealth(size_t index) = 0;
		virtual bool GetPlayerWaiting(size_t index) = 0;
		virtual void SetPlayerWaiting(size_t index, bool waiting) = 0;
		virtual void OffsetPlayerHealth(size_t index, int offset) = 0;
		virtual bool Update(float dt) = 0;
		virtual bool Render() = 0;

		sf::RenderWindow* GetWindow() const;
		sf::Vector2i GetMouseScreenPosition() const;
		sf::Font* GetBasicFont() const;
	protected:
		State *m_ParentState;
		GameBehavior *m_GameBehavior;
		State(State *parentState, GameBehavior *gameBehavior);
	};

	class CompositeState : public State
	{
	public:
		virtual void Transit(State *from, State *to);
		virtual void Enter(bool initialization);
		virtual void Exit(bool finalization);
		virtual bool StartGame(bool AI);
		virtual bool MainMenu();
		virtual bool OptionsMenu();
		virtual bool TogglePause();
		virtual void ChangeTurn();
		virtual bool GetAI();
		virtual size_t GetCurrentTurn();
		virtual int GetPlayerHealth(size_t index);
		virtual bool GetPlayerWaiting(size_t index);
		virtual void SetPlayerWaiting(size_t index, bool waiting);
		virtual void OffsetPlayerHealth(size_t index, int offset);
		virtual bool Update(float dt);
		virtual bool Render();
	protected:
		CompositeState(State *parentState, GameBehavior *gameBehavior);
		vector<State*> m_ChildStates;
	};

	class DecoratedState : public State
	{
	public:
		virtual void Transit(State *from, State *to);
		virtual void Enter(bool initialization);
		virtual void Exit(bool finalization);
		virtual bool StartGame(bool AI);
		virtual bool MainMenu();
		virtual bool OptionsMenu();
		virtual bool TogglePause();
		virtual void ChangeTurn();
		virtual bool GetAI();
		virtual size_t GetCurrentTurn();
		virtual int GetPlayerHealth(size_t index);
		virtual bool GetPlayerWaiting(size_t index);
		virtual void SetPlayerWaiting(size_t index, bool waiting);
		virtual void OffsetPlayerHealth(size_t index, int offset);
		virtual bool Update(float dt);
		virtual bool Render();
	protected:
		DecoratedState(State *parentState, GameBehavior *gameBehavior);
		State *m_ChildState;
	};

	class LeafState : public State
	{
	public:
		virtual void Transit(State *from, State *to);
		virtual void Enter(bool initialization);
		virtual void Exit(bool finalization);
		virtual bool StartGame(bool AI);
		virtual bool MainMenu();
		virtual bool OptionsMenu();
		virtual bool TogglePause();
		virtual void ChangeTurn();
		virtual bool GetAI();
		virtual size_t GetCurrentTurn();
		virtual int GetPlayerHealth(size_t index);
		virtual bool GetPlayerWaiting(size_t index);
		virtual void SetPlayerWaiting(size_t index, bool waiting);
		virtual void OffsetPlayerHealth(size_t index, int offset);
		virtual bool Update(float dt);
		virtual bool Render();
	protected:
		LeafState(State *parentState, GameBehavior *gameBehavior);
	};

	class GameState : public DecoratedState
	{
	public:
		GameState(State *parentState, GameBehavior *gameBehavior);
		virtual bool StartGame(bool AI);
		virtual bool MainMenu();
	};

	class MenuGameState : public DecoratedState
	{
	public:
		MenuGameState(State *parentState, GameBehavior *gameBehavior);
		virtual bool MainMenu();
		virtual bool OptionsMenu();
		virtual bool Render();
	};

	class TitleMenuState : public LeafState
	{
	public:
		TitleMenuState(State *parentState, GameBehavior *gameBehavior);
		virtual bool Update(float dt);
		virtual bool Render();
	private:
		sf::Text m_TitleText, m_ContinueText;
	};

	class MainMenuState : public LeafState
	{
	public:
		MainMenuState(State *parentState, GameBehavior *gameBehavior);
		virtual void Exit(bool finalization);
		virtual bool MainMenu();
		virtual bool Update(float dt);
		virtual bool Render();
	private:
		UIButton *m_Buttons[4];
	};

	class OptionsMenuState : public LeafState
	{
	public:
		OptionsMenuState(State *parentState, GameBehavior *gameBehavior);
		virtual void Exit(bool finalization);
		virtual bool OptionsMenu();
		virtual bool Update(float dt);
		virtual bool Render();
	private:
		UIButton *m_Buttons[3];
	};

	class PlayGameState : public CompositeState
	{
	public:
		PlayGameState(State *parentState, GameBehavior *gameBehavior, bool AI);
		virtual void Enter(bool initialization);
		virtual void Exit(bool finalization);
		virtual bool TogglePause();
		virtual void ChangeTurn();
		virtual bool GetAI();
		virtual size_t GetCurrentTurn();
		virtual int GetPlayerHealth(size_t index);
		virtual bool GetPlayerWaiting(size_t index);
		virtual void SetPlayerWaiting(size_t index, bool waiting);
		virtual void OffsetPlayerHealth(size_t index, int offset);
		virtual bool Update(float dt);
		virtual bool Render();
	private:
		bool m_Paused;
		Board *m_Board;
		sf::RectangleShape m_SelectionShape;
		Slot *m_SelectedSlot;
	};

	class PausePlayState : public LeafState
	{
	public:
		PausePlayState(State *parentState, GameBehavior *gameBehavior);
		virtual void Exit(bool finalization);
		virtual bool Update(float dt);
		virtual bool Render();
	private:
		UIButton *m_Buttons[3];
		sf::RectangleShape *m_Background;
	};

	class PlayerState : public LeafState
	{
	public:
		PlayerState(State *parentState, GameBehavior *gameBehavior, bool AI);
		virtual void ChangeTurn();
		virtual bool GetAI();
		virtual size_t GetCurrentTurn();
		virtual int GetPlayerHealth(size_t index);
		virtual bool GetPlayerWaiting(size_t index);
		virtual void SetPlayerWaiting(size_t index, bool waiting);
		virtual void OffsetPlayerHealth(size_t index, int offset);
	private:
		size_t m_CurrentPlayerTurn;
		bool m_AI;
		int m_PlayerHealth[2];
		bool m_PlayerWaiting[2];
	};

	void Update(float dt);
	void Render();
	void Initialize(int windowWidth, int windowHeight);
	GameState *m_GameState;
	sf::RenderWindow *m_Window;
	sf::Vector2i m_MouseScreenPosition;
	sf::Font m_BasicFont;
	bool m_Running = true;
};

#endif