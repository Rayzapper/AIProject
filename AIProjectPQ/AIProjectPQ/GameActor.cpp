#include "GameActor.h"

GameActor::~GameActor()
{

}

float GameActor::GetFacing() const
{
	return m_Facing;
}

float GameActor::GetSpeed() const
{
	return m_Speed;
}

void GameActor::SetFacing(float angle)
{
	m_Facing = angle;
	while (m_Facing < 0)
		m_Facing += PI * 2;
	while (m_Facing >= PI * 2)
		m_Facing -= PI * 2;
}

void GameActor::SetSpeed(float speed)
{
	m_Speed = speed;
}

void GameActor::OffsetFacing(float offset)
{
	m_Facing += offset;
	while (m_Facing < 0)
		m_Facing += PI * 2;
	while (m_Facing >= PI * 2)
		m_Facing -= PI * 2;
}

void GameActor::OffsetSpeed(float offset)
{
	m_Speed += offset;
}

GameActor::GameActor()
	: GameObject()
{

}

GameActor::GameActor(sf::Shape *drawShape)
	: GameObject(drawShape)
{

}


Player::Player()
{

}

Player::Player(sf::Shape *drawShape)
{

}

void Player::SetupStats()
{

}


Turret::Turret()
	: GameActor()
{

}

Turret::Turret(sf::Shape *drawShape)
	: GameActor(drawShape)
{

}

void Turret::SetupStats()
{

}