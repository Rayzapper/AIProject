#ifndef RAYZAPPER_PIECEBEHAVIOR_H
#define RAYZAPPER_PIECEBEHAVIOR_H

#include "Piece.h"

class PieceBehavior
{
public:
	PieceBehavior();
	~PieceBehavior();
	void AlivePhase();
	void DeadPhase();
	void IdleMotion();
	void MovingMotion(sf::Vector2f targetPosition);
	void IllegalMotion(sf::Vector2f targetPosition);
	void FallingMotion(float targetHeight);
	void Update(float dt);
	void Render(sf::RenderWindow *window);

	Piece* GetPiece();
	int GetPieceType();
	bool GetDead();
	bool Moving();
	void SetPiece(Piece *piece);
	void SetPieceType(int type);
private:
	class State
	{
	public:
		virtual void Transit(State *to, State *from) = 0;
		virtual void Enter(bool initialization) = 0;
		virtual void Exit(bool finalization) = 0;
		virtual bool AlivePhase() = 0;
		virtual bool DeadPhase() = 0;
		virtual bool IdleMotion() = 0;
		virtual bool MovingMotion(sf::Vector2f targetPosition) = 0;
		virtual bool IllegalMotion(sf::Vector2f targetPosition) = 0;
		virtual bool FallingMotion(float targetHeight) = 0;
		virtual bool Update(float dt) = 0;
		virtual bool Render(sf::RenderWindow *window) = 0;
		virtual bool Moving() = 0;
		virtual bool GetDead() = 0;

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
		virtual bool AlivePhase();
		virtual bool DeadPhase();
		virtual bool IdleMotion();
		virtual bool MovingMotion(sf::Vector2f targetPosition);
		virtual bool IllegalMotion(sf::Vector2f targetPosition);
		virtual bool FallingMotion(float targetHeight);
		virtual bool Update(float dt);
		virtual bool Render(sf::RenderWindow *window);
		virtual bool Moving();
		virtual bool GetDead();
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
		virtual bool AlivePhase();
		virtual bool DeadPhase();
		virtual bool IdleMotion();
		virtual bool MovingMotion(sf::Vector2f targetPosition);
		virtual bool IllegalMotion(sf::Vector2f targetPosition);
		virtual bool FallingMotion(float targetHeight);
		virtual bool Update(float dt);
		virtual bool Render(sf::RenderWindow *window);
		virtual bool Moving();
		virtual bool GetDead();
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
		virtual bool AlivePhase();
		virtual bool DeadPhase();
		virtual bool IdleMotion();
		virtual bool MovingMotion(sf::Vector2f targetPosition);
		virtual bool IllegalMotion(sf::Vector2f targetPosition);
		virtual bool FallingMotion(float targetHeight);
		virtual bool Update(float dt);
		virtual bool Render(sf::RenderWindow *window);
		virtual bool Moving();
		virtual bool GetDead();
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
		virtual bool AlivePhase();
		virtual bool DeadPhase();
	};

	class AliveState : public LeafState
	{
	public:
		AliveState(State *parentState, PieceBehavior *pieceBehavior);
		virtual bool AlivePhase();
	};

	class DeadState : public LeafState
	{
	public:
		DeadState(State *parentState, PieceBehavior *pieceBehavior);
		virtual bool DeadPhase();
		virtual bool GetDead();
	};

	class MotionState : public DecoratedState
	{
	public:
		MotionState(State *parentState, PieceBehavior *pieceBehavior);
		virtual bool IdleMotion();
		virtual bool MovingMotion(sf::Vector2f targetPosition);
		virtual bool IllegalMotion(sf::Vector2f targetPosition);
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
		virtual bool Moving();
	private:
		sf::Vector2f m_TargetPosition, m_DeltaVector;
		float m_Length;
	};

	class IllegalMovementState : public LeafState
	{
	public:
		IllegalMovementState(State *parentState, PieceBehavior *pieceBehavior);
		virtual bool IllegalMotion(sf::Vector2f targetPosition);
		virtual bool Update(float dt);
		virtual bool Moving();
	private:
		sf::Vector2f m_TargetPosition, m_DeltaVector, m_StartPosition;
		float m_Length, m_Distance;
		bool m_To;
	};

	class FallingState : public LeafState
	{
	public:
		FallingState(State *parentState, PieceBehavior *pieceBehavior);
		virtual bool FallingMotion(float targetHeight);
		virtual bool Update(float dt);
		virtual bool Moving();
	private:
		float m_TargetHeight, m_Speed;
	};

	PieceState *m_PieceState;
	Piece *m_Piece;
};

#endif