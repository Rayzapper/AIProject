#ifndef RAYZAPPER_BOARD_H
#define RAYZAPPER_BOARD_H

#include "PieceBehavior.h"

class Slot : public GameObject
{
public:
	Slot(sf::Shape *drawShape);
	~Slot();
	void SetNeighbor(Slot *slot, size_t index);
	sf::IntRect GetHitbox();
	PieceBehavior* GetPiece();
	Slot* GetNeighbor(size_t index);
	virtual void SetPosition(sf::Vector2f position);
	void SetHitbox(sf::IntRect hitbox);
	void SetPiece(PieceBehavior *piece);
	bool GetMouseover(sf::Vector2i mouseScreenPosition);
private:
	sf::IntRect m_Hitbox;
	PieceBehavior *m_Piece;
	Slot *m_NeighborSlots[4] = { nullptr, nullptr, nullptr, nullptr };
};

class Board
{
public:
	Board();
	~Board();
	void Update(float dt);
	void Render(sf::RenderWindow *window);
	void GenerateBoard();
	bool PiecesMoving();
	bool MatchPossible();
	vector<vector<Slot*>> GetMatches();
	vector<vector<Slot*>> GetSwapMatches(Slot *slotFrom, Slot *slotTo);
	void SwapPieces(Slot *slotFrom, Slot *slotTo);
	void ClearMatches(vector<vector<Slot*>> matches);
	Slot* GetSlot(size_t indexY, size_t indexX);
private:
	vector<vector<Slot*>> GetRowMatches(size_t y);
	vector<vector<Slot*>> GetCollumnMatches(size_t x);
	bool RowMatchPossible(size_t y);
	bool CollumnMatchPossible(size_t x);
	void SettleBoard();
	void FillBoard();
	Slot* m_BoardSlots[8][8];
	sf::Shape *m_PieceShapes[6];
};

#endif