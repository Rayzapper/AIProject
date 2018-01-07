#include "Piece.h"

Piece::Piece(sf::Shape *drawShape)
	: GameObject(drawShape)
{

}

Piece::~Piece()
{

}

int Piece::GetType()
{
	return m_PieceType;
}

void Piece::SetType(int type)
{
	m_PieceType = type;
}