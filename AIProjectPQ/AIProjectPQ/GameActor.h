#ifndef RAYZAPPER_UNIT_H
#define RAYZAPPER_UNIT_H

#include "GameObject.h"

class GameActor : public GameObject
{
public:
	~GameActor();
	virtual void SetupStats() = 0;
	float GetFacing() const;
	float GetSpeed() const;
	void SetFacing(float angle);
	void SetSpeed(float speed);
	void OffsetFacing(float offset);
	void OffsetSpeed(float offset);
protected:
	GameActor();
	GameActor(sf::Shape *drawShape);
	float m_Facing, m_Speed;
	sf::Vector2f m_Velocity = sf::Vector2f(0.f, 0.f);
	int m_Health;
};

class Player : public GameActor
{
public:
	Player();
	Player(sf::Shape *drawShape);
	virtual void SetupStats();
private:

};

class Turret : public GameActor
{
public:
	Turret();
	Turret(sf::Shape *drawShape);
	virtual void SetupStats();
private:

};

#endif