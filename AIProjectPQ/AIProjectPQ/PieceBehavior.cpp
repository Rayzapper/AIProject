#include "PieceBehavior.h"

PieceBehavior::PieceBehavior()
{
	m_PieceState = new PieceState(nullptr, this);
}

PieceBehavior::~PieceBehavior()
{
	m_PieceState->Exit(true);
	delete m_PieceState;
	delete m_Piece;
}

void PieceBehavior::CreationPhase()
{
	m_PieceState->CreationPhase();
}

void PieceBehavior::AlivePhase()
{
	m_PieceState->AlivePhase();
}

void PieceBehavior::DestructionPhase()
{
	m_PieceState->DestructionPhase();
}

void PieceBehavior::DeadPhase()
{
	m_PieceState->DeadPhase();
}

void PieceBehavior::IdleMotion()
{
	m_PieceState->IdleMotion();
}

void PieceBehavior::MovingMotion(sf::Vector2f targetPosition)
{
	m_PieceState->MovingMotion(targetPosition);
}

void PieceBehavior::FallingMotion(float targetHeight)
{
	m_PieceState->FallingMotion(targetHeight);
}

void PieceBehavior::Update(float dt)
{
	m_PieceState->Update(dt);
}

void PieceBehavior::Render(sf::RenderWindow *window)
{
	m_PieceState->Render(window);
}

Piece* PieceBehavior::GetPiece()
{
	return m_Piece;
}

void PieceBehavior::SetPiece(Piece *piece)
{
	m_Piece = piece;
}


sf::Vector2f PieceBehavior::State::GetPiecePosition()
{
	return m_PieceBehavior->m_Piece->GetPosition();
}

void PieceBehavior::State::SetPiecePosition(sf::Vector2f position)
{
	m_PieceBehavior->m_Piece->SetPosition(position);
}

void PieceBehavior::State::OffsetPiecePosition(sf::Vector2f offset)
{
	m_PieceBehavior->m_Piece->OffsetPosition(offset);
}

PieceBehavior::State::State(State *parentState, PieceBehavior *pieceBehavior)
	: m_ParentState(parentState),
	m_PieceBehavior(pieceBehavior)
{

}


void PieceBehavior::CompositeState::Transit(State *from, State *to)
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

void PieceBehavior::CompositeState::Enter(bool initialization)
{
	for each (State *s in m_ChildStates)
	{
		s->Enter(initialization);
	}
}

void PieceBehavior::CompositeState::Exit(bool finalization)
{
	for each (State *s in m_ChildStates)
	{
		s->Exit(finalization);
		delete s;
	}
	m_ChildStates.clear();
}

bool PieceBehavior::CompositeState::CreationPhase()
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->CreationPhase())
			handled = true;
	}
	return handled;
}

bool PieceBehavior::CompositeState::AlivePhase()
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->AlivePhase())
			handled = true;
	}
	return handled;
}

bool PieceBehavior::CompositeState::DestructionPhase()
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->DestructionPhase())
			handled = true;
	}
	return handled;
}

bool PieceBehavior::CompositeState::DeadPhase()
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->DeadPhase())
			handled = true;
	}
	return handled;
}

bool PieceBehavior::CompositeState::IdleMotion()
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->IdleMotion())
			handled = true;
	}
	return handled;
}

bool PieceBehavior::CompositeState::MovingMotion(sf::Vector2f targetPosition)
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->MovingMotion(targetPosition))
			handled = true;
	}
	return handled;
}

bool PieceBehavior::CompositeState::FallingMotion(float targetHeight)
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->FallingMotion(targetHeight))
			handled = true;
	}
	return handled;
}

bool PieceBehavior::CompositeState::Update(float dt)
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->Update(dt))
			handled = true;
	}
	return handled;
}

bool PieceBehavior::CompositeState::Render(sf::RenderWindow *window)
{
	bool handled = false;
	for each (State *s in m_ChildStates)
	{
		if (s->Render(window))
			handled = true;
	}
	return handled;
}

PieceBehavior::CompositeState::CompositeState(State *parentState, PieceBehavior *pieceBehavior)
	: State(parentState, pieceBehavior)
{

}


void PieceBehavior::DecoratedState::Transit(State *from, State *to)
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

void PieceBehavior::DecoratedState::Enter(bool initialization)
{
	m_ChildState->Enter(initialization);
}

void PieceBehavior::DecoratedState::Exit(bool finalization)
{
	m_ChildState->Exit(finalization);
	delete m_ChildState;
}

bool PieceBehavior::DecoratedState::CreationPhase()
{
	return m_ChildState->CreationPhase();
}

bool PieceBehavior::DecoratedState::AlivePhase()
{
	return m_ChildState->AlivePhase();
}

bool PieceBehavior::DecoratedState::DestructionPhase()
{
	return m_ChildState->DestructionPhase();
}

bool PieceBehavior::DecoratedState::DeadPhase()
{
	return m_ChildState->DeadPhase();
}

bool PieceBehavior::DecoratedState::IdleMotion()
{
	return m_ChildState->IdleMotion();
}

bool PieceBehavior::DecoratedState::MovingMotion(sf::Vector2f targetPosition)
{
	return m_ChildState->MovingMotion(targetPosition);
}

bool PieceBehavior::DecoratedState::FallingMotion(float targetHeight)
{
	return m_ChildState->FallingMotion(targetHeight);
}

bool PieceBehavior::DecoratedState::Update(float dt)
{
	return m_ChildState->Update(dt);
}

bool PieceBehavior::DecoratedState::Render(sf::RenderWindow *window)
{
	return m_ChildState->Render(window);
}

PieceBehavior::DecoratedState::DecoratedState(State *parentState, PieceBehavior *pieceBehavior)
	: State(parentState, pieceBehavior)
{

}


void PieceBehavior::LeafState::Transit(State *from, State *to)
{
	m_ParentState->Transit(from, to);
}

void PieceBehavior::LeafState::Enter(bool initialization) {}

void PieceBehavior::LeafState::Exit(bool finalization) {}

bool PieceBehavior::LeafState::CreationPhase()
{
	return false;
}

bool PieceBehavior::LeafState::AlivePhase()
{
	return false;
}

bool PieceBehavior::LeafState::DestructionPhase()
{
	return false;
}

bool PieceBehavior::LeafState::DeadPhase()
{
	return false;
}

bool PieceBehavior::LeafState::IdleMotion()
{
	return false;
}

bool PieceBehavior::LeafState::MovingMotion(sf::Vector2f targetPosition)
{
	return false;
}

bool PieceBehavior::LeafState::FallingMotion(float targetHeight)
{
	return false;
}

bool PieceBehavior::LeafState::Update(float dt)
{
	return false;
}

bool PieceBehavior::LeafState::Render(sf::RenderWindow *window)
{
	return false;
}

PieceBehavior::LeafState::LeafState(State *parentState, PieceBehavior *pieceBehavior)
	: State(parentState, pieceBehavior)
{

}


PieceBehavior::PieceState::PieceState(State *parentState, PieceBehavior *pieceBehavior)
	: CompositeState(parentState, pieceBehavior)
{
	m_ChildStates.push_back(new PhaseState(this, pieceBehavior));
	m_ChildStates.push_back(new MotionState(this, pieceBehavior));
}

bool PieceBehavior::PieceState::Render(sf::RenderWindow *window)
{
	m_PieceBehavior->m_Piece->Render(window);
	for each (State *s in m_ChildStates)
	{
		s->Render(window);
	}
	return true;
}


PieceBehavior::PhaseState::PhaseState(State *parentState, PieceBehavior *pieceBehavior)
	: DecoratedState(parentState, pieceBehavior)
{
	m_ChildState = new CreationState(this, pieceBehavior);
}

bool PieceBehavior::PhaseState::CreationPhase()
{
	if (!m_ChildState->CreationPhase())
	{
		Transit(m_ChildState, new CreationState(this, m_PieceBehavior));
		m_ChildState->CreationPhase();
	}
	return true;
}

bool PieceBehavior::PhaseState::AlivePhase()
{
	if (!m_ChildState->AlivePhase())
	{
		Transit(m_ChildState, new AliveState(this, m_PieceBehavior));
		m_ChildState->AlivePhase();
	}
	return true;
}

bool PieceBehavior::PhaseState::DestructionPhase()
{
	if (!m_ChildState->DestructionPhase())
	{
		Transit(m_ChildState, new DestructionState(this, m_PieceBehavior));
		m_ChildState->DestructionPhase();
	}
	return true;
}

bool PieceBehavior::PhaseState::DeadPhase()
{
	if (!m_ChildState->DeadPhase())
	{
		Transit(m_ChildState, new DeadState(this, m_PieceBehavior));
		m_ChildState->DeadPhase();
	}
	return true;
}


PieceBehavior::CreationState::CreationState(State *parentState, PieceBehavior *pieceBehavior)
	: LeafState(parentState, pieceBehavior)
{

}

bool PieceBehavior::CreationState::CreationPhase()
{
	return true;
}


PieceBehavior::AliveState::AliveState(State *parentState, PieceBehavior *pieceBehavior)
	: LeafState(parentState, pieceBehavior)
{

}

bool PieceBehavior::AliveState::AlivePhase()
{
	return true;
}


PieceBehavior::DestructionState::DestructionState(State *parentState, PieceBehavior *pieceBehavior)
	: LeafState(parentState, pieceBehavior)
{

}

bool PieceBehavior::DestructionState::DestructionPhase()
{
	return true;
}


PieceBehavior::DeadState::DeadState(State *parentState, PieceBehavior *pieceBehavior)
	: LeafState(parentState, pieceBehavior)
{

}

bool PieceBehavior::DeadState::DeadPhase()
{
	return true;
}


PieceBehavior::MotionState::MotionState(State *parentState, PieceBehavior *pieceBehavior)
	: DecoratedState(parentState, pieceBehavior)
{
	m_ChildState = new IdleState(this, pieceBehavior);
}

bool PieceBehavior::MotionState::IdleMotion()
{
	if (!m_ChildState->IdleMotion())
	{
		Transit(m_ChildState, new IdleState(this, m_PieceBehavior));
		m_ChildState->IdleMotion();
	}
	return true;
}

bool PieceBehavior::MotionState::MovingMotion(sf::Vector2f targetPosition)
{
	if (!m_ChildState->MovingMotion(targetPosition))
	{
		Transit(m_ChildState, new MovingState(this, m_PieceBehavior));
		m_ChildState->MovingMotion(targetPosition);
	}
	return true;
}

bool PieceBehavior::MotionState::FallingMotion(float targetHeight)
{
	if (!m_ChildState->FallingMotion(targetHeight))
	{
		Transit(m_ChildState, new FallingState(this, m_PieceBehavior));
		m_ChildState->FallingMotion(targetHeight);
	}
	return true;
}


PieceBehavior::IdleState::IdleState(State *parentState, PieceBehavior *pieceBehavior)
	: LeafState(parentState, pieceBehavior)
{

}

bool PieceBehavior::IdleState::IdleMotion()
{
	return true;
}


PieceBehavior::MovingState::MovingState(State *parentState, PieceBehavior *pieceBehavior)
	: LeafState(parentState, pieceBehavior)
{

}

bool PieceBehavior::MovingState::MovingMotion(sf::Vector2f targetPosition)
{
	m_TargetPosition = targetPosition;
	m_Speed = 0.f;
	m_DeltaVector = m_TargetPosition - GetPiecePosition();
	m_Length = sqrtf(m_DeltaVector.x * m_DeltaVector.x + m_DeltaVector.y * m_DeltaVector.y);
	m_DeltaVector.x /= m_Length;
	m_DeltaVector.y /= m_Length;
	return true;
}

bool PieceBehavior::MovingState::Update(float dt)
{
	m_Speed += dt * 9.82f * 2;
	OffsetPiecePosition(m_DeltaVector * m_Speed);
	m_Length -= m_Speed;
	if (m_Length <= 0)
	{
		SetPiecePosition(m_TargetPosition);
		m_PieceBehavior->IdleMotion();
	}
	return true;
}


PieceBehavior::FallingState::FallingState(State *parentState, PieceBehavior *pieceBehavior)
	: LeafState(parentState, pieceBehavior)
{

}

bool PieceBehavior::FallingState::FallingMotion(float targetHeight)
{
	m_Speed = 0.f;
	m_TargetHeight = targetHeight;
	return true;
}

bool PieceBehavior::FallingState::Update(float dt)
{
	m_Speed += dt * 9.82f * 2;
	OffsetPiecePosition(sf::Vector2f(0.f, m_Speed));
	sf::Vector2f currentPosition = GetPiecePosition();
	if (currentPosition.y >= m_TargetHeight)
	{
		currentPosition.y = m_TargetHeight;
		SetPiecePosition(currentPosition);
		m_PieceBehavior->IdleMotion();
	}
	return true;
}