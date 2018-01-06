#ifndef RAYZAPPER_GAMEBEHAVIOR_H
#define RAYZAPPER_GAMEBEHAVIOR_H

#include <SFML\Graphics.hpp>

using namespace std;

class Camera;
class UIButton;

class GameBehavior
{
public:
	GameBehavior();
	~GameBehavior();
	void Run();
private:
	void StartGame();
	void MainMenu();
	void OptionsMenu();
	void ExitGame();

	class State
	{
	public:
		virtual void Transit(State *to, State *from) = 0;
		virtual void Enter(bool initialization) = 0;
		virtual void Exit(bool finalization) = 0;
		virtual bool StartGame() = 0;
		virtual bool MainMenu() = 0;
		virtual bool OptionsMenu() = 0;
		virtual bool Update(float dt) = 0;
		virtual bool Render() = 0;

		sf::RenderWindow* GetWindow() const;
		Camera* GetCamera() const;
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
		virtual bool StartGame();
		virtual bool MainMenu();
		virtual bool OptionsMenu();
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
		virtual bool StartGame();
		virtual bool MainMenu();
		virtual bool OptionsMenu();
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
		virtual bool StartGame();
		virtual bool MainMenu();
		virtual bool OptionsMenu();
		virtual bool Update(float dt);
		virtual bool Render();
	protected:
		LeafState(State *parentState, GameBehavior *gameBehavior);
	};

	class GameState : public DecoratedState
	{
	public:
		GameState(State *parentState, GameBehavior *gameBehavior);
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
		UIButton *m_Buttons[3];
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
		PlayGameState(State *parentState, GameBehavior *gameBehavior);
		virtual bool StartGame();
	};

	class PlayState : public DecoratedState
	{
	public:
		PlayState(State *parentState, GameBehavior *gameBehavior);
	};

	void Update(float dt);
	void Render();
	void Initialize(int windowWidth, int windowHeight);
	GameState *m_GameState;
	sf::RenderWindow *m_Window;
	Camera *m_Camera;
	sf::Vector2i m_MouseScreenPosition;
	sf::Font m_BasicFont;
	bool m_Running = true;
};

#endif