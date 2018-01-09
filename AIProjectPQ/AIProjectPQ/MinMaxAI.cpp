#include "MinMaxAI.h"
#include <limits>
#include <iostream>

MinMaxAI::MinMaxAI()
{

}

MinMaxAI::~MinMaxAI()
{

}

void MinMaxAI::ScanBoard(int board[8][8])
{
	for (size_t y = 0; y < 8; y++)
	{
		for (size_t x = 0; x < 8; x++)
		{
			m_CurrentBoard.m_BoardSlots[y][x]->m_Value = board[y][x];
		}
	}
}

bool MinMaxAI::MoveReady()
{
	return m_MoveReady;
}

bool MinMaxAI::GetThinking()
{
	return m_Thinking;
}

Move MinMaxAI::GetMove()
{
	return m_Move;
}

void MinMaxAI::SetMove()
{
	Node *node = MiniMax(m_CurrentBoard, true, 3, 0);
	m_Move = node->m_Move;
}

void MinMaxAI::SetThinking(bool thinking)
{
	m_Thinking = thinking;
}

void MinMaxAI::FirstStage()
{
	Move move1, move2;
	if (m_X1 < 7)
	{
		move1.m_X = m_X1;
		move1.m_Y = m_Y1;
		move1.m_Direction = 0;
		if (m_CurrentBoard.MoveGivesMatches(move1))
			m_CurrentPossibleMoves.push_back(move1);
	}
	if (m_Y1 < 7)
	{
		move2.m_X = m_X1;
		move2.m_Y = m_Y1;
		move2.m_Direction = 1;
		if (m_CurrentBoard.MoveGivesMatches(move2))
			m_CurrentPossibleMoves.push_back(move2);
	}

	m_X1++;
	if (m_X1 == 8)
	{
		m_X1 = 0;
		m_Y1++;
	}
	if (m_Y1 == 8)
	{
		m_Y1 = 0;
		m_Stage = 1;
	}
}

Node* MinMaxAI::MiniMax(VirtualBoard board, bool maximizing, int depth, int parentCost)
{
	Node *bestNode = new Node();
	bestNode->m_Cost = parentCost;

	if (depth == 0)
		return bestNode;
	vector<Move> possibleMoves = board.GetPossibleMoves();
	if (maximizing)
	{
		bestNode->m_Cost = numeric_limits<int>::min();
		for (size_t i = 0; i < possibleMoves.size(); i++)
		{
			Node *child = new Node();
			child->m_Cost = parentCost;
			VirtualBoard newBoard;
			newBoard = board;
			newBoard.RunMove(possibleMoves[i]);
			vector<vector<VirtualSlot*>> matches = newBoard.GetMatches();
			while (matches.size() > 0)
			{
				for (size_t j = 0; j < matches.size(); j++)
				{
					int id = matches[j][0]->m_Value;
					if (id == 3)
						child->m_Cost += (int)matches[j].size() - 2;
				}
				newBoard.ClearMatches(matches);
				matches.clear();
				newBoard.Fill();
				matches = newBoard.GetMatches();
			}
			child = MiniMax(newBoard, false, depth - 1, child->m_Cost);
			if (child->m_Cost > bestNode->m_Cost)
			{
				bestNode->m_Cost = child->m_Cost;
				bestNode->m_Move = possibleMoves[i];
			}
		}
	}
	else
	{
		bestNode->m_Cost = numeric_limits<int>::max();
		for (size_t i = 0; i < possibleMoves.size(); i++)
		{
			Node *child = new Node();
			child->m_Cost = parentCost;
			VirtualBoard newBoard;
			newBoard = board;
			newBoard.RunMove(possibleMoves[i]);
			vector<vector<VirtualSlot*>> matches = newBoard.GetMatches();
			while (matches.size() > 0)
			{
				for (size_t j = 0; j < matches.size(); j++)
				{
					int id = matches[j][0]->m_Value;
					if (id == 3)
						child->m_Cost -= (int)(matches[j].size() - 2);
				}
				newBoard.ClearMatches(matches);
				matches.clear();
				newBoard.Fill();
				matches = newBoard.GetMatches();
			}
			child = MiniMax(newBoard, true, depth - 1, child->m_Cost);
			if (child->m_Cost < bestNode->m_Cost)
			{
				bestNode->m_Cost = child->m_Cost;
				bestNode->m_Move = possibleMoves[i];
			}
		}
	}

	return bestNode;
}


VirtualBoard::VirtualBoard()
{
	for (size_t y = 0; y < 8; y++)
	{
		for (size_t x = 0; x < 8; x++)
		{
			m_BoardSlots[y][x] = new VirtualSlot();
		}
	}
	for (size_t y = 0; y < 8; y++)
	{
		for (size_t x = 0; x < 8; x++)
		{
			if (x < 7)
				m_BoardSlots[y][x]->m_NeighborSlots[0] = m_BoardSlots[y][x + 1];
			if (y < 7)
				m_BoardSlots[y][x]->m_NeighborSlots[1] = m_BoardSlots[y + 1][x];
			if (x > 0)
				m_BoardSlots[y][x]->m_NeighborSlots[2] = m_BoardSlots[y][x - 1];
			if (y > 0)
				m_BoardSlots[y][x]->m_NeighborSlots[3] = m_BoardSlots[y - 1][x];
		}
	}
}

VirtualBoard::~VirtualBoard()
{
	for (size_t y = 0; y < 8; y++)
	{
		for (size_t x = 0; x < 8; x++)
		{
			//delete m_BoardSlots[y][x];
		}
	}
}

void VirtualBoard::RunMove(Move move)
{
	int temp = m_BoardSlots[move.m_Y][move.m_X]->m_Value;
	if (move.m_Direction == 0)
	{
		m_BoardSlots[move.m_Y][move.m_X]->m_Value = m_BoardSlots[move.m_Y][move.m_X + 1]->m_Value;
		m_BoardSlots[move.m_Y][move.m_X + 1]->m_Value = temp;
	}
	else
	{
		m_BoardSlots[move.m_Y][move.m_X]->m_Value = m_BoardSlots[move.m_Y + 1][move.m_X]->m_Value;
		m_BoardSlots[move.m_Y + 1][move.m_X]->m_Value = temp;
	}
}

void VirtualBoard::ClearMatches(vector<vector<VirtualSlot*>> matches)
{
	for (size_t i = 0; i < matches.size(); i++)
	{
		for (size_t j = 0; j < matches[i].size(); j++)
		{
			matches[i][j]->m_Value = -1;
		}
	}
	Settle();
}

void VirtualBoard::Settle()
{
	for (size_t i = 1; i < 8; i++)
	{
		size_t y = 7 - i;
		for (size_t x = 0; x < 8; x++)
		{
			VirtualSlot *targetSlot = m_BoardSlots[y][x];
			int id = targetSlot->m_Value, temp;
			if (id != -1)
			{
				while (targetSlot->m_NeighborSlots[1] != nullptr)
				{
					if (targetSlot->m_NeighborSlots[1]->m_Value == -1)
					{
						targetSlot = targetSlot->m_NeighborSlots[1];
					}
					else break;
				}
				temp = targetSlot->m_Value;
				targetSlot->m_Value = id;
				m_BoardSlots[y][x]->m_Value = temp;
			}
		}
	}
}

void VirtualBoard::Fill()
{
	for (size_t y = 0; y < 8; y++)
	{
		for (size_t x = 0; x < 8; x++)
		{
			if (m_BoardSlots[y][x]->m_Value == -1)
				m_BoardSlots[y][x]->m_Value = rand() % 6;
		}
	}
}

vector<Move> VirtualBoard::GetPossibleMoves()
{
	vector<Move> moves;
	for (size_t y = 0; y < 8; y++)
	{
		for (size_t x = 0; x < 8; x++)
		{
			if (x < 7)
			{
				Move move;
				move.m_X = x;
				move.m_Y = y;
				move.m_Direction = 0;
				if (MoveGivesMatches(move))
					moves.push_back(move);
			}
			if (y < 7)
			{
				Move move;
				move.m_X = x;
				move.m_Y = y;
				move.m_Direction = 1;
				if (MoveGivesMatches(move))
					moves.push_back(move);
			}
		}
	}
	return moves;
}

bool VirtualBoard::MoveGivesMatches(Move move)
{
	int id1 = m_BoardSlots[move.m_Y][move.m_X]->m_Value, id2;
	if (move.m_Direction == 0)
	{
		id2 = m_BoardSlots[move.m_Y][move.m_X + 1]->m_Value;
		if (id1 == id2)
			return false;
		if (move.m_Y > 1)
		{
			if (id2 != -1 && m_BoardSlots[move.m_Y - 1][move.m_X]->m_Value == id2 && m_BoardSlots[move.m_Y - 2][move.m_X]->m_Value == id2)
				return true;
			if (id1 != -1 && m_BoardSlots[move.m_Y - 1][move.m_X + 1]->m_Value == id1 && m_BoardSlots[move.m_Y - 2][move.m_X + 1]->m_Value == id1)
				return true;
		}
		if (move.m_Y < 6)
		{
			if (id2 != -1 && m_BoardSlots[move.m_Y + 1][move.m_X]->m_Value == id2 && m_BoardSlots[move.m_Y + 2][move.m_X]->m_Value == id2)
				return true;
			if (id1 != -1 && m_BoardSlots[move.m_Y + 1][move.m_X + 1]->m_Value == id1 && m_BoardSlots[move.m_Y + 2][move.m_X + 1]->m_Value == id1)
				return true;
		}
		if (move.m_X > 1)
			if (id2 != -1 && m_BoardSlots[move.m_Y][move.m_X - 1]->m_Value == id2 && m_BoardSlots[move.m_Y][move.m_X - 2]->m_Value == id2)
				return true;
		if (move.m_X < 5)
			if (id1 != -1 && m_BoardSlots[move.m_Y][move.m_X + 2]->m_Value == id1 && m_BoardSlots[move.m_Y][move.m_X + 3]->m_Value == id1)
				return true;
		if (move.m_Y > 0 && move.m_Y < 7)
		{
			if (id2 != -1 && m_BoardSlots[move.m_Y - 1][move.m_X]->m_Value == id2 && m_BoardSlots[move.m_Y + 1][move.m_X]->m_Value == id2)
				return true;
			if (id1 != -1 && m_BoardSlots[move.m_Y - 1][move.m_X + 1]->m_Value == id1 && m_BoardSlots[move.m_Y + 1][move.m_X + 1]->m_Value == id1)
				return true;
		}
	}
	else
	{
		id2 = m_BoardSlots[move.m_Y + 1][move.m_X]->m_Value;
		if (id1 == id2)
			return false;
		if (move.m_X > 1)
		{
			if (id2 != -1 && m_BoardSlots[move.m_Y][move.m_X - 1]->m_Value == id2 && m_BoardSlots[move.m_Y][move.m_X - 2]->m_Value == id2)
				return true;
			if (id1 != -1 && m_BoardSlots[move.m_Y + 1][move.m_X - 1]->m_Value == id1 && m_BoardSlots[move.m_Y + 1][move.m_X - 2]->m_Value == id1)
				return true;
		}
		if (move.m_X < 6)
		{
			if (id2 != -1 && m_BoardSlots[move.m_Y][move.m_X + 1]->m_Value == id2 && m_BoardSlots[move.m_Y][move.m_X + 2]->m_Value == id2)
				return true;
			if (id1 != -1 && m_BoardSlots[move.m_Y + 1][move.m_X + 1]->m_Value == id1 && m_BoardSlots[move.m_Y + 1][move.m_X + 2]->m_Value == id1)
				return true;
		}
		if (move.m_Y > 1)
			if (id2 != -1 && m_BoardSlots[move.m_Y - 1][move.m_X]->m_Value == id2 && m_BoardSlots[move.m_Y - 2][move.m_X]->m_Value == id2)
				return true;
		if (move.m_Y < 5)
			if (id1 != -1 && m_BoardSlots[move.m_Y + 2][move.m_X]->m_Value == id1 && m_BoardSlots[move.m_Y + 3][move.m_X]->m_Value == id1)
				return true;
		if (move.m_X > 0 && move.m_X < 7)
		{
			if (id2 != -1 && m_BoardSlots[move.m_Y][move.m_X - 1]->m_Value == id2 && m_BoardSlots[move.m_Y][move.m_X + 1]->m_Value == id2)
				return true;
			if (id1 != -1 && m_BoardSlots[move.m_Y + 1][move.m_X - 1]->m_Value == id1 && m_BoardSlots[move.m_Y + 1][move.m_X + 1]->m_Value == id1)
				return true;
		}
	}
	return false;
}

vector<vector<VirtualSlot*>> VirtualBoard::GetMatches()
{
	vector<vector<VirtualSlot*>> list, temp;

	for (size_t y = 0; y < 8; y++)
	{
		temp = GetRowMatches(y);
		for (size_t i = 0; i < temp.size(); i++)
		{
			list.push_back(temp[i]);
		}
	}
	for (size_t x = 0; x < 8; x++)
	{
		temp = GetCollumnMatches(x);
		for (size_t i = 0; i < temp.size(); i++)
		{
			list.push_back(temp[i]);
		}
	}
	return list;
}

vector<vector<VirtualSlot*>> VirtualBoard::GetRowMatches(size_t y)
{
	vector<vector<VirtualSlot*>> matches;
	for (size_t x = 0; x < 6; x++)
	{
		vector<VirtualSlot*> match;
		int id = m_BoardSlots[y][x]->m_Value;
		match.push_back(m_BoardSlots[y][x]);
		while (match.back()->m_NeighborSlots[0]->m_Value == id)
		{
			x++;
			match.push_back(match.back()->m_NeighborSlots[0]);
			if (match.back()->m_NeighborSlots[0] == nullptr)
				break;
		}
		if (match.size() > 2) matches.push_back(match);
	}
	return matches;
}

vector<vector<VirtualSlot*>> VirtualBoard::GetCollumnMatches(size_t x)
{
	vector<vector<VirtualSlot*>> matches;
	for (size_t y = 0; y < 6; y++)
	{
		vector<VirtualSlot*> match;
		int id = m_BoardSlots[y][x]->m_Value;
		match.push_back(m_BoardSlots[y][x]);
		while (match.back()->m_NeighborSlots[1]->m_Value == id)
		{
			y++;
			match.push_back(match.back()->m_NeighborSlots[1]);
			if (match.back()->m_NeighborSlots[1] == nullptr)
				break;
		}
		if (match.size() > 2) matches.push_back(match);
	}
	return matches;
}

VirtualBoard& VirtualBoard::operator=(const VirtualBoard &board)
{
	for (size_t y = 0; y < 8; y++)
	{
		for (size_t x = 0; x < 8; x++)
		{
			m_BoardSlots[y][x]->m_Value = board.m_BoardSlots[y][x]->m_Value;
		}
	}
	return *this;
}