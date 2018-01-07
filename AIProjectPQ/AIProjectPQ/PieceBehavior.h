#ifndef RAYZAPPER_PIECEBEHAVIOR_H
#define RAYZAPPER_PIECEBEHAVIOR_H

#include "Piece.h"

class PieceBehavior
{
public:
	PieceBehavior();
	~PieceBehavior();
	void CreationPhase();
	void AlivePhase();
	void DestructionPhase();
	void DeadPhase();
	void IdleMotion();
	void MovingMotion(sf::Vector2f targetPosition);
	void FallingMotion(float targetHeight);
	void Update(float dt);
	void Render(sf::RenderWindow *window);

	Piece* GetPiece();
	void SetPiece(Piece *piece);
private:
	class State
	{
	public:
		virtual void Transit(State *to, State *from) = 0;
		virtual void Enter(bool initialization) = 0;
		virtual void Exit(bool finalization) = 0;
		virtual bool CreationPhase() = 0;
		virtual bool AlivePhase() = 0;
		virtual bool DestructionPhase() = 0;
		virtual bool DeadPhase() = 0;
		virtual bool IdleMotion() = 0;
		virtual bool MovingMotion(sf::Vector2f targetPosition) = 0;
		virtual bool FallingMotion(float targetHeight) = 0;
		virtual bool Update(float dt) = 0;
		virtual bool Render(sf::RenderWindow *window) = 0;

		sf::Vector2f GetPiecePosition();
		void SetPiecePosition(sf::Vector2f position);
		void OffsetPiecePosition(sf::Vector2f offset);
	protected:
		State *m_ParentState;
		PieceBehavior *m_PieceBehavior;
		State(State *parentState, PieceBehavior *pieceBehavior);
	};

	class CompositeState : public State
	{
	public:
		virtual void Transit(State *to, State *from);
		virtual void Enter(bool initialization);
		virtual void Exit(bool finalization);
		virtual bool CreationPhase();
		virtual bool AlivePhase();
		virtual bool DestructionPhase();
		virtual bool DeadPhase();
		virtual bool IdleMotion();
		virtual bool MovingMotion(sf::Vector2f targetPosition);
		virtual bool FallingMotion(float targetHeight);
		virtual bool Update(float dt);
		virtual bool Render(sf::RenderWindow *window);
	protected:
		CompositeState(State *parentState, PieceBehavior *pieceBehavior);
		vector<State*> m_ChildStates;
	};

	class DecoratedState : public State
	{
	public:
		virtual void Transit(State *to, State *from);
		virtual void Enter(bool initialization);
		virtual void Exit(bool finalization);
		virtual bool CreationPhase();
		virtual bool AlivePhase();
		virtual bool DestructionPhase();
		virtual bool DeadPhase();
		virtual bool IdleMotion();
		virtual bool MovingMotion(sf::Vector2f targetPosition);
		virtual bool FallingMotion(float targetHeight);
		virtual bool Update(float dt);
		virtual bool Render(sf::RenderWindow *window);
	protected:
		DecoratedState(State *parentState, PieceBehavior *pieceBehavior);
		State *m_ChildState;
	};

	class LeafState : public State
	{
	public:
		virtual void Transit(State *to, State *from);
		virtual void Enter(bool initialization);
		virtual void Exit(bool finalization);
		virtual bool CreationPhase();
		virtual bool AlivePhase();
		virtual bool DestructionPhase();
		virtual bool DeadPhase();
		virtual bool IdleMotion();
		virtual bool MovingMotion(sf::Vector2f targetPosition);
		virtual bool FallingMotion(float targetHeight);
		virtual bool Update(float dt);
		virtual bool Render(sf::RenderWindow *window);
	protected:
		LeafState(State *parentState, PieceBehavior *pieceBehavior);
	};

	class PieceState : public CompositeState
	{
	public:
		PieceState(State *parentState, PieceBehavior *pieceBehavior);
		virtual bool Render(sf::RenderWindow *window);
	};

	class PhaseState : public DecoratedState
	{
	public:
		PhaseState(State *parentState, PieceBehavior *pieceBehavior);
		virtual bool CreationPhase();
		virtual bool AlivePhase();
		virtual bool DestructionPhase();
		virtual bool DeadPhase();
	};

	class CreationState : public LeafState
	{
	public:
		CreationState(State *parentState, PieceBehavior *pieceBehavior);
		virtual bool CreationPhase();
	};

	class AliveState : public LeafState
	{
	public:
		AliveState(State *parentState, PieceBehavior *pieceBehavior);
		virtual bool AlivePhase();
	};

	class DestructionState : public LeafState
	{
	public:
		DestructionState(State *parentState, PieceBehavior *pieceBehavior);
		virtual bool DestructionPhase();
	};

	class DeadState : public LeafState
	{
	public:
		DeadState(State *parentState, PieceBehavior *pieceBehavior);
		virtual bool DeadPhase();
	};

	class MotionState : public DecoratedState
	{
	public:
		MotionState(State *parentState, PieceBehavior *pieceBehavior);
		virtual bool IdleMotion();
		virtual bool MovingMotion(sf::Vector2f targetPosition);
		virtual bool FallingMotion(float targetHeight);
	};

	class IdleState : public LeafState
	{
	public:
		IdleState(State *parentState, PieceBehavior *pieceBehavior);
		virtual bool IdleMotion();
	};

	class MovingState : public LeafState
	{
	public:
		MovingState(State *parentState, PieceBehavior *pieceBehavior);
		virtual bool MovingMotion(sf::Vector2f targetPosition);
		virtual bool Update(float dt);
	private:
		sf::Vector2f m_TargetPosition, m_DeltaVector;
		float m_Speed, m_Length;
	};

	class FallingState : public LeafState
	{
	public:
		FallingState(State *parentState, PieceBehavior *pieceBehavior);
		virtual bool FallingMotion(float targetHeight);
		virtual bool Update(float dt);
	private:
		float m_TargetHeight, m_Speed;
	};

	PieceState *m_PieceState;
	Piece *m_Piece;
};

#endif