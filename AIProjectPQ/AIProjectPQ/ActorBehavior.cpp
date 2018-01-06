#include "ActorBehavior.h"
#include <iostream>

ActorBehavior::ActorBehavior()
{
	m_UnitState = new UnitState(nullptr, this);
}

ActorBehavior::~ActorBehavior()
{
	m_UnitState->Exit(true);
	delete m_UnitState;
	delete m_Actor;
}

void ActorBehavior::CreationPhase()
{
	m_UnitState->CreationPhase();
}

void ActorBehavior::AlivePhase()
{
	m_UnitState->AlivePhase();
}

void ActorBehavior::DestructionPhase()
{
	m_UnitState->DestructionPhase();
}

void ActorBehavior::DeadPhase()
{
	m_UnitState->DeadPhase();
}

void ActorBehavior::Update(float dt, sf::Vector2i mousePosition, vector<ActorBehavior*> units)
{
	m_UnitState->Update(dt, mousePosition, units);
}

void ActorBehavior::Render(sf::RenderWindow *window)
{
	m_Actor->Render(window);
}

GameActor* ActorBehavior::GetActor()
{
	return m_Actor;
}

void ActorBehavior::SetActor(GameActor *actor)
{
	m_Actor = actor;
}


sf::Vector2f ActorBehavior::State::GetUnitPosition()
{
	return m_ActorBehavior->GetActor()->GetPosition();
}

float ActorBehavior::State::GetUnitFacing()
{
	return m_ActorBehavior->GetActor()->GetFacing();
}

float ActorBehavior::State::GetUnitSpeed()
{
	return m_ActorBehavior->GetActor()->GetSpeed();
}

void ActorBehavior::State::SetUnitPosition(sf::Vector2f position)
{
	m_ActorBehavior->GetActor()->SetPosition(position);
}

void ActorBehavior::State::SetUnitFacing(float facing)
{
	m_ActorBehavior->GetActor()->SetFacing(facing);
}

ActorBehavior::State::State(State *parentState, ActorBehavior *unitBehavior)
	: m_ParentState(parentState),
	m_ActorBehavior(unitBehavior)
{

}


void ActorBehavior::CompositeState::Transit(State *from, State *to)
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

void ActorBehavior::CompositeState::Enter(bool initialization)
{
	for each (State *s in m_ChildStates)
	{
		s->Enter(initialization);
	}
}

void ActorBehavior::CompositeState::Exit(bool finalization)
{
	for each (State *s in m_ChildStates)
	{
		s->Exit(finalization);
		delete s;
	}
	m_ChildStates.clear();
}

bool ActorBehavior::CompositeState::CreationPhase()
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->CreationPhase())
			handled = true;
	}
	return handled;
}

bool ActorBehavior::CompositeState::AlivePhase()
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->AlivePhase())
			handled = true;
	}
	return handled;
}

bool ActorBehavior::CompositeState::DestructionPhase()
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->DestructionPhase())
			handled = true;
	}
	return handled;
}

bool ActorBehavior::CompositeState::DeadPhase()
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->DeadPhase())
			handled = true;
	}
	return handled;
}

bool ActorBehavior::CompositeState::Update(float dt, sf::Vector2i mousePosition, vector<ActorBehavior*> units)
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->Update(dt, mousePosition, units))
			handled = true;
	}
	return handled;
}

ActorBehavior::CompositeState::CompositeState(State *parentState, ActorBehavior *unitBehavior)
	: State(parentState, unitBehavior)
{

}


void ActorBehavior::DecoratedState::Transit(State *from, State *to)
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

void ActorBehavior::DecoratedState::Enter(bool initialization)
{
	m_ChildState->Enter(initialization);
}

void ActorBehavior::DecoratedState::Exit(bool finalization)
{
	m_ChildState->Exit(finalization);
	delete m_ChildState;
}

bool ActorBehavior::DecoratedState::CreationPhase()
{
	return m_ChildState->CreationPhase();
}

bool ActorBehavior::DecoratedState::AlivePhase()
{
	return m_ChildState->AlivePhase();
}

bool ActorBehavior::DecoratedState::DestructionPhase()
{
	return m_ChildState->DestructionPhase();
}

bool ActorBehavior::DecoratedState::DeadPhase()
{
	return m_ChildState->DeadPhase();
}

bool ActorBehavior::DecoratedState::Update(float dt, sf::Vector2i mousePosition, vector<ActorBehavior*> units)
{
	return m_ChildState->Update(dt, mousePosition, units);
}

ActorBehavior::DecoratedState::DecoratedState(State *parentState, ActorBehavior *unitBehavior)
	: State(parentState, unitBehavior)
{

}


void ActorBehavior::LeafState::Transit(State *from, State *to)
{
	m_ParentState->Transit(from, to);
}

void ActorBehavior::LeafState::Enter(bool initialization) {}

void ActorBehavior::LeafState::Exit(bool finalization) {}

bool ActorBehavior::LeafState::CreationPhase()
{
	return false;
}

bool ActorBehavior::LeafState::AlivePhase()
{
	return false;
}

bool ActorBehavior::LeafState::DestructionPhase()
{
	return false;
}

bool ActorBehavior::LeafState::DeadPhase()
{
	return false;
}

bool ActorBehavior::LeafState::Update(float dt, sf::Vector2i mousePosition, vector<ActorBehavior*> units)
{
	return false;
}

ActorBehavior::LeafState::LeafState(State *parentState, ActorBehavior *unitBehavior)
	: State(parentState, unitBehavior)
{

}


ActorBehavior::PhaseState::PhaseState(State *parentState, ActorBehavior *unitBehavior)
	: DecoratedState(parentState, unitBehavior)
{
	CreationState *startPhaseState = new CreationState(this, unitBehavior);
	m_ChildState = startPhaseState;
}

bool ActorBehavior::PhaseState::CreationPhase()
{
	if (!m_ChildState->CreationPhase())
	{
		Transit(m_ChildState, new CreationState(this, m_ActorBehavior));
	}
	return true;
}

bool ActorBehavior::PhaseState::AlivePhase()
{
	if (!m_ChildState->AlivePhase())
	{
		Transit(m_ChildState, new AliveState(this, m_ActorBehavior));
	}
	return true;
}

bool ActorBehavior::PhaseState::DestructionPhase()
{
	if (!m_ChildState->DestructionPhase())
	{
		Transit(m_ChildState, new DestructionState(this, m_ActorBehavior));
	}
	return true;
}

bool ActorBehavior::PhaseState::DeadPhase()
{
	if (!m_ChildState->DeadPhase())
	{
		Transit(m_ChildState, new DeadState(this, m_ActorBehavior));
	}
	return true;
}


ActorBehavior::MotionState::MotionState(State *parentState, ActorBehavior *unitBehavior)
	: DecoratedState(parentState, unitBehavior)
{
	
}


ActorBehavior::UnitState::UnitState(State *parentState, ActorBehavior *unitBehavior)
	: CompositeState(parentState, unitBehavior)
{
	PhaseState *startPhaseState = new PhaseState(this, unitBehavior);
	MotionState *startMotionState = new MotionState(this, unitBehavior);
	m_ChildStates.push_back(startPhaseState);
	m_ChildStates.push_back(startMotionState);
}


ActorBehavior::CreationState::CreationState(State *parentState, ActorBehavior *unitBehavior)
	: LeafState(parentState, unitBehavior)
{

}

bool ActorBehavior::CreationState::CreationPhase()
{
	return true;
}


ActorBehavior::AliveState::AliveState(State *parentState, ActorBehavior *unitBehavior)
	: LeafState(parentState, unitBehavior)
{

}

bool ActorBehavior::AliveState::AlivePhase()
{
	return true;
}


ActorBehavior::DestructionState::DestructionState(State *parentState, ActorBehavior *unitBehavior)
	: LeafState(parentState, unitBehavior)
{

}

bool ActorBehavior::DestructionState::DestructionPhase()
{
	return true;
}


ActorBehavior::DeadState::DeadState(State *parentState, ActorBehavior *unitBehavior)
	: LeafState(parentState, unitBehavior)
{

}

bool ActorBehavior::DeadState::DeadPhase()
{
	return true;
}