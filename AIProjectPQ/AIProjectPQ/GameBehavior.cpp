#include "GameBehavior.h"

GameBehavior::GameBehavior()
{
	m_GameState = new GameState(nullptr, this);
}

GameBehavior::~GameBehavior()
{
	m_GameState->Exit(true);
	delete m_GameState;
}

void GameBehavior::Update(float dt)
{
	m_GameState->Update(dt);
}


GameBehavior::State::State(State *parentState, GameBehavior *gameBehavior)
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

bool GameBehavior::DecoratedState::Update(float dt)
{
	return m_ChildState->Update(dt);
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

bool GameBehavior::LeafState::Update(float dt)
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
	SplashGameState *startGameState = new SplashGameState(this, gameBehavior);
	m_ChildState = startGameState;
}