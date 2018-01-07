#ifndef RAYZAPPER_PIECE_H
#define RAYZAPPER_PIECE_H

#include "GameObject.h"

class Piece : public GameObject
{
public:
	Piece(sf::Shape *drawShape);
	~Piece();
	int GetType();
	void SetType(int type);
private:
	int m_PieceType;
};

#endif