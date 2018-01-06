#ifndef RAYZAPPER_UNITBEHAVIOR_H
#define RAYZAPPER_UNITBEHAVIOR_H

#include <SFML\Graphics.hpp>
#include <vector>
#include "GameActor.h"

using namespace std;

class ActorBehavior
{
public:
	ActorBehavior();
	~ActorBehavior();
	void CreationPhase();
	void AlivePhase();
	void DestructionPhase();
	void DeadPhase();
	void Update(float dt, sf::Vector2i mousePosition, vector<ActorBehavior*> units);
	void Render(sf::RenderWindow *window);
	GameActor* GetActor();
	void SetActor(GameActor *actor);
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
		virtual bool Update(float dt, sf::Vector2i mousePosition, vector<ActorBehavior*> units) = 0;

		sf::Vector2f GetUnitPosition();
		float GetUnitFacing();
		float GetUnitSpeed();
		void SetUnitPosition(sf::Vector2f position);
		void SetUnitFacing(float facing);
	protected:
		State *m_ParentState;
		ActorBehavior *m_ActorBehavior;
		State(State *parentState, ActorBehavior *unitBehavior);
	};

	class CompositeState : public State
	{
	public:
		virtual void Transit(State *from, State *to);
		virtual void Enter(bool initialization);
		virtual void Exit(bool finalization);
		virtual bool CreationPhase();
		virtual bool AlivePhase();
		virtual bool DestructionPhase();
		virtual bool DeadPhase();
		virtual bool Update(float dt, sf::Vector2i mousePosition, vector<ActorBehavior*> units);
	protected:
		CompositeState(State *parentState, ActorBehavior *unitBehavior);
		vector<State*> m_ChildStates;
	};

	class DecoratedState : public State
	{
	public:
		virtual void Transit(State *from, State *to);
		virtual void Enter(bool initialization);
		virtual void Exit(bool finalization);
		virtual bool CreationPhase();
		virtual bool AlivePhase();
		virtual bool DestructionPhase();
		virtual bool DeadPhase();
		virtual bool Update(float dt, sf::Vector2i mousePosition, vector<ActorBehavior*> units);
	protected:
		DecoratedState(State *parentState, ActorBehavior *unitBehavior);
		State *m_ChildState;
	};

	class LeafState : public State
	{
	public:
		virtual void Transit(State *from, State *to);
		virtual void Enter(bool initialization);
		virtual void Exit(bool finalization);
		virtual bool CreationPhase();
		virtual bool AlivePhase();
		virtual bool DestructionPhase();
		virtual bool DeadPhase();
		virtual bool Update(float dt, sf::Vector2i mousePosition, vector<ActorBehavior*> units);
	protected:
		LeafState(State *parentState, ActorBehavior *unitBehavior);
	};

	class PhaseState : public DecoratedState
	{
	public:
		PhaseState(State *parentState, ActorBehavior *unitBehavior);
		virtual bool CreationPhase();
		virtual bool AlivePhase();
		virtual bool DestructionPhase();
		virtual bool DeadPhase();
	};

	class MotionState : public DecoratedState
	{
	public:
		MotionState(State *parentState, ActorBehavior *unitBehavior);
	};

	class UnitState : public CompositeState
	{
	public:
		UnitState(State *parentState, ActorBehavior *unitBehavior);
	};

	class CreationState : public LeafState
	{
	public:
		CreationState(State *parentState, ActorBehavior *unitBehavior);
		virtual bool CreationPhase();
	};

	class AliveState : public LeafState
	{
	public:
		AliveState(State *parentState, ActorBehavior *unitBehavior);
		virtual bool AlivePhase();
	};

	class DestructionState : public LeafState
	{
	public:
		DestructionState(State *parentState, ActorBehavior *unitBehavior);
		virtual bool DestructionPhase();
	};

	class DeadState : public LeafState
	{
	public:
		DeadState(State *parentState, ActorBehavior *unitBehavior);
		virtual bool DeadPhase();
	};

	UnitState *m_UnitState;
	GameActor *m_Actor;
};

#endif