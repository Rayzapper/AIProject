#ifndef RAYZAPPER_GAMEBEHAVIOR_H
#define RAYZAPPER_GAMEBEHAVIOR_H

#include <SFML\Graphics.hpp>

using namespace std;

class GameBehavior
{
public:
	GameBehavior();
	~GameBehavior();
	void Update(float dt);
private:
	class State
	{
	public:
		virtual void Transit(State *to, State *from) = 0;
		virtual void Enter(bool initialization) = 0;
		virtual void Exit(bool finalization) = 0;
		virtual bool Update(float dt) = 0;
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
		virtual bool Update(float dt);
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
		virtual bool Update(float dt);
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
		virtual bool Update(float dt);
	protected:
		LeafState(State *parentState, GameBehavior *gameBehavior);
	};

	class GameState : public DecoratedState
	{
	public:
		GameState(State *parentState, GameBehavior *gameBehavior);
	};

		class SplashGameState : public LeafState
		{
		public:
			SplashGameState(State *parentState, GameBehavior *gameBehavior);
		};

		class MenuGameState : public DecoratedState
		{
		public:
			MenuGameState(State *parentState, GameBehavior *gameBehavior);
		};

			class TitleMenuState : public LeafState
			{
			public:
				TitleMenuState(State *parentState, GameBehavior *gameBehavior);
			};

			class MainMenuState : public LeafState
			{
			public:
				MainMenuState(State *parentState, GameBehavior *gameBehavior);
			};

			class OptionsMenuState : public LeafState
			{
			public:
				OptionsMenuState(State *parentState, GameBehavior *gameBehavior);
			};

		class PlayGameState : public CompositeState
		{
		public:
			PlayGameState(State *parentState, GameBehavior *gameBehavior);
		private:

		};

			class PlayState : public DecoratedState
			{
			public:
				PlayState(State *parentState, GameBehavior *gameBehavior);
			};

			class PauseState : public LeafState
			{
			public:
				PauseState(State *parentState, GameBehavior *gameBehavior);
			};

	GameState *m_GameState;
};

#endif