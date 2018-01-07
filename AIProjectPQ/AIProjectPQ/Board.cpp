#include "Board.h"

Board::Board()
{
	sf::RectangleShape slotShape;
	slotShape.setFillColor(sf::Color(64, 32, 0, 255));
	slotShape.setOutlineColor(sf::Color(128, 64, 0, 255));
	slotShape.setOutlineThickness(-2.f);
	slotShape.setSize(sf::Vector2f(100.f, 100.f));
	slotShape.setOrigin(sf::Vector2f(50.f, 50.f));

	m_PieceShapes[0] = new sf::CircleShape(40.f, 4);
	m_PieceShapes[0]->setFillColor(sf::Color::Yellow);
	m_PieceShapes[1] = new sf::CircleShape(40.f, 8);
	m_PieceShapes[1]->setFillColor(sf::Color::Blue);
	m_PieceShapes[2] = new sf::CircleShape(40.f, 3);
	m_PieceShapes[2]->setFillColor(sf::Color::Green);
	m_PieceShapes[3] = new sf::CircleShape(40.f, 32);
	m_PieceShapes[3]->setFillColor(sf::Color::White);
	m_PieceShapes[4] = new sf::CircleShape(40.f, 6);
	m_PieceShapes[4]->setFillColor(sf::Color::Red);
	m_PieceShapes[5] = new sf::CircleShape(40.f, 5);
	m_PieceShapes[5]->setFillColor(sf::Color(255, 128, 0, 255));
	for each (sf::Shape *s in m_PieceShapes)
	{
		s->setOrigin(40.f, 40.f);
	}

	for (size_t y = 0; y < 8; y++)
	{
		for (size_t x = 0; x < 8; x++)
		{
			m_BoardSlots[y][x] = new Slot(new sf::RectangleShape(slotShape));
			m_BoardSlots[y][x]->SetPosition(sf::Vector2f(250.f + x * 100.f, 50.f + y * 100.f));
			if (x < 7)
				m_BoardSlots[y][x]->SetNeighbor(m_BoardSlots[y][x + 1], 0);
			if (y < 7)
				m_BoardSlots[y][x]->SetNeighbor(m_BoardSlots[y + 1][x], 1);
			if (x > 0)
				m_BoardSlots[y][x]->SetNeighbor(m_BoardSlots[y][x - 1], 2);
			if (y > 0)
				m_BoardSlots[y][x]->SetNeighbor(m_BoardSlots[y - 1][x], 3);
		}
	}

	GenerateBoard();
}

Board::~Board()
{
	for each (Slot *s in m_BoardSlots)
	{
		delete s;
	}
	for each (sf::Shape *s in m_PieceShapes)
	{
		delete s;
	}
}

void Board::Update(float dt)
{
	for (size_t y = 0; y < 8; y++)
	{
		for (size_t x = 0; x < 8; x++)
		{
			m_BoardSlots[y][x]->GetPiece()->Update(dt);
		}
	}
}

void Board::Render(sf::RenderWindow *window)
{
	for each (Slot *s in m_BoardSlots)
	{
		s->Render(window);
	}
	for each (Slot *s in m_BoardSlots)
	{
		s->GetPiece()->Render(window);
	}
}

void Board::GenerateBoard()
{
	int types[8][8];

	for (size_t y = 0; y < 8; y++)
	{
		for (size_t x = 0; x < 8; x++)
		{
			bool set = false;
			while (!set)
			{
				set = true;
				types[y][x] = rand() % 6;
				if (x > 1)
					if (types[y][x] == types[y][x - 1] && types[y][x] == types[y][x - 2])
						set = false;
				if (y > 1)
					if (types[y][x] == types[y - 1][x] && types[y][x] == types[y - 2][x])
						set = false;
			}
			Piece *piece = new Piece(new sf::CircleShape(*dynamic_cast<sf::CircleShape*>(m_PieceShapes[(size_t)types[y][x]])));
			piece->SetPosition(sf::Vector2f(250.f + x * 100.f, y * 100.f - 750.f - x * 100.f));
			m_BoardSlots[y][x]->GetPiece()->SetPiece(piece);
			m_BoardSlots[y][x]->GetPiece()->FallingMotion(50.f + y * 100.f);
		}
	}
}

void Board::SwapPiece(size_t indexY, size_t indexX, int direction)
{
	PieceBehavior *temp = m_BoardSlots[indexY][indexX]->GetPiece();
	if (direction = 0)
	{
		m_BoardSlots[indexY][indexX]->SetPiece(m_BoardSlots[indexY][indexX + 1]->GetPiece());
		m_BoardSlots[indexY][indexX + 1]->SetPiece(temp);
	}
	else if (direction = 1)
	{
		m_BoardSlots[indexY][indexX]->SetPiece(m_BoardSlots[indexY + 1][indexX]->GetPiece());
		m_BoardSlots[indexY + 1][indexX]->SetPiece(temp);
	}
	else if (direction = 2)
	{
		m_BoardSlots[indexY][indexX]->SetPiece(m_BoardSlots[indexY][indexX - 1]->GetPiece());
		m_BoardSlots[indexY][indexX - 1]->SetPiece(temp);
	}
	else
	{
		m_BoardSlots[indexY][indexX]->SetPiece(m_BoardSlots[indexY - 1][indexX]->GetPiece());
		m_BoardSlots[indexY - 1][indexX]->SetPiece(temp);
	}
}

sf::IntRect Board::GetSlotHitbox(size_t indexY, size_t indexX)
{
	return m_BoardSlots[indexY][indexX]->GetHitbox();
}

PieceBehavior* Board::GetPiece(size_t indexY, size_t indexX)
{
	return m_BoardSlots[indexY][indexX]->GetPiece();
}


Board::Slot::Slot(sf::Shape *drawShape)
	: GameObject(drawShape)
{
	m_Piece = new PieceBehavior();

	m_Hitbox.left = 0;
	m_Hitbox.top = 0;
	m_Hitbox.width = 100;
	m_Hitbox.height = 100;
}

Board::Slot::~Slot()
{
	delete m_Piece;
}

void Board::Slot::SetNeighbor(Slot *slot, size_t index)
{
	m_NeighborSlots[index] = slot;
}

sf::IntRect Board::Slot::GetHitbox()
{
	return m_Hitbox;
}

PieceBehavior* Board::Slot::GetPiece()
{
	return m_Piece;
}

void Board::Slot::SetPosition(sf::Vector2f position)
{
	m_Position = position;
	m_Sprite.setPosition(position);
	m_Hitbox.left = (int)position.x - 50;
	m_Hitbox.top = (int)position.y - 50;
	if (m_Shape != nullptr) m_Shape->setPosition(position);
}

void Board::Slot::SetHitbox(sf::IntRect hitbox)
{
	m_Hitbox = hitbox;
}

void Board::Slot::SetPiece(PieceBehavior *piece)
{
	m_Piece = piece;
}