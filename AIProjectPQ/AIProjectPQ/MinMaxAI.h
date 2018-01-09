#ifndef RAYZAPPER_MINMAXAI_H
#define RAYZAPPER_MINMAXAI_H

#include <vector>

using namespace std;

struct Move
{
	size_t m_X, m_Y, m_Direction;
};

struct Node
{
	Move m_Move;
	int m_Cost;
};

struct VirtualSlot
{
	int m_Value;
	VirtualSlot *m_NeighborSlots[4] = { nullptr, nullptr, nullptr, nullptr };
};

class VirtualBoard
{
public:
	VirtualBoard();
	~VirtualBoard();
	VirtualSlot *m_BoardSlots[8][8];
	void RunMove(Move move);
	void ClearMatches(vector<vector<VirtualSlot*>> matches);
	void Settle();
	void Fill();
	vector<Move> GetPossibleMoves();
	bool MoveGivesMatches(Move move);
	vector<vector<VirtualSlot*>> GetMatches();
	VirtualBoard& operator=(const VirtualBoard &board);
private:
	vector<vector<VirtualSlot*>> GetRowMatches(size_t y);
	vector<vector<VirtualSlot*>> GetCollumnMatches(size_t x);
};

class MinMaxAI
{
public:
	MinMaxAI();
	~MinMaxAI();
	void ScanBoard(int board[8][8]);
	bool MoveReady();
	bool GetThinking();
	Move GetMove();
	void SetMove();
	void SetThinking(bool thinking);
private:
	void FirstStage();
	Node* MiniMax(VirtualBoard board, bool maximizing, int depth, int parentCost);

	bool m_MoveReady = false, m_Thinking = false;
	int m_Stage = 0;
	vector<Move> m_CurrentPossibleMoves;
	VirtualBoard m_CurrentBoard;
	size_t m_Y1 = 0, m_X1 = 0;
	Move m_Move;
};

#endif