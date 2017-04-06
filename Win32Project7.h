#include "stdafx.h"
struct pos
{
	UINT x, y,value;
	pos(){}
	pos(UINT _x, UINT _y, UINT _value) : x(_x), y(_y), value(_value){}
};
class UNIT
{
protected:
	UINT value;
	bool color;
	pos position;
	static UINT counter;
	UINT ID;
	UINT BMP;
	friend void Reset_counter();
public:
	UNIT(){}
	UNIT(UINT val, bool col, UINT _x, UINT _y, UINT _bmp) : value(val), color(col), BMP(_bmp) { position.x = _x; position.y = _y; ID=counter++; }
	UINT getID()
	{
		return ID;
	}
	bool getColor()
	{
		return color;
	}
	UINT getValue()
	{
		return value;
	}
	pos getPosition()
	{
		return position;
	}
	
	UINT getBMP(){ return BMP; }
	virtual pos* generate_steps() = 0;
	void go2(UINT _x, UINT _y);
};
void Reset_counter()
{
	UNIT::counter = 0;
}
UINT UNIT::counter = 0;
static UNIT* chessboard[8][8] = { NULL };
UNIT* figures[32] = { NULL };
bool B_ROOK1cstlng = 1, B_ROOK2cstlng = 1, W_ROOK1cstlng = 1, W_ROOK2cstlng = 1;
UINT ID_WHITE_KING, ID_BLACK_KING, ID_W_ROOK1, ID_B_ROOK1, ID_W_ROOK2, ID_B_ROOK2;
bool course = WHITE;
bool B_SHAH,W_SHAH;
bool enclosure = 1;
bool User_color = WHITE;
UNIT* last2stepped = NULL;
UNIT* lastking = NULL;
BOOL CALLBACK Figure_Select_Proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
HMODULE hLib;

void UNIT::go2(UINT _x, UINT _y)
{
	course = course ^ 1;
	B_SHAH = W_SHAH = 0;
	UINT ID = this->getID();
	if ((ID > 0 && ID < 8) || (ID>15 && ID<24))
	{
		if (User_color == WHITE)
		{
			if (color == WHITE)
			{
				if (_y == 8)
				{
					lastking = this;
				}
				if (position.y == 2 && _y == 4)
					last2stepped = this;
				if (_y == last2stepped->getPosition().y+1)
				if (_x == last2stepped->getPosition().x)
				{
					figures[last2stepped->getID()] = NULL;
					chessboard[last2stepped->getPosition().x-1][last2stepped->getPosition().y-1] = NULL;
				}
			}
			else
			{
				if (_y == 1)
				{
					lastking = this;
				}
				if (position.y == 7 && _y == 5)
					last2stepped = this;
				if (_y == last2stepped->getPosition().y - 1)
				if (_x == last2stepped->getPosition().x)
				{
					figures[last2stepped->getID()] = NULL;
					chessboard[last2stepped->getPosition().x - 1][last2stepped->getPosition().y - 1] = NULL;
				}
			}
		}
		else
		{
			if (color == WHITE)
			{
				if (_y == 1)
				{
					lastking = this;					
				}
				if (position.y == 7 && _y == 5)
					last2stepped = this;
				if (_y == last2stepped->getPosition().y - 1)
				if (_x == last2stepped->getPosition().x)
				{
					figures[last2stepped->getID()] = NULL;
					chessboard[last2stepped->getPosition().x - 1][last2stepped->getPosition().y - 1] = NULL;
				}
			}
			else
			{
				if (_y == 8)
				{
					lastking = this;
				}
				if (position.y == 2 && _y == 4)
					last2stepped = this;
				if (_y == last2stepped->getPosition().y + 1)
				if (_x == last2stepped->getPosition().x)
				{
					figures[last2stepped->getID()] = NULL;
					chessboard[last2stepped->getPosition().x - 1][last2stepped->getPosition().y - 1] = NULL;
				}
			}
		}
	}
	if (ID == ID_WHITE_KING)
	{
		if (_x == position.x + 2)
		{
			if (W_ROOK2cstlng == 1)
			{
				chessboard[7][0]->go2(chessboard[7][0]->getPosition().x - 2, position.y);
				course = course ^ 1;
			}
		}
		else if (_x == position.x - 2)
		{
			if (W_ROOK1cstlng == 1)
			{
				chessboard[0][0]->go2(chessboard[7][0]->getPosition().x + 2, position.y);
				course = course ^ 1;
			}
		}
		W_ROOK1cstlng = 0;
		W_ROOK2cstlng = 0;
	}
	if (ID == ID_BLACK_KING)
	{
		if (_x == position.x + 2)
		{
			if (B_ROOK2cstlng == 1)
			{
				chessboard[7][7]->go2(chessboard[7][7]->getPosition().x - 2, position.y);
				course = course ^ 1;
			}
		}
		else if (_x == position.x - 2)
		{
			if (B_ROOK1cstlng == 1)
			{
				chessboard[0][7]->go2(chessboard[7][7]->getPosition().x + 2, position.y);
				course = course ^ 1;
			}
		}
		B_ROOK1cstlng = 0;
		B_ROOK2cstlng = 0;
	}
	if (ID == ID_W_ROOK1)
		W_ROOK1cstlng = 0;
	if (ID == ID_W_ROOK2)
		W_ROOK2cstlng = 0;
	if (ID == ID_B_ROOK1)
		B_ROOK1cstlng = 0;
	if (ID == ID_B_ROOK2)
		B_ROOK2cstlng = 0;

	if (chessboard[_x - 1][_y - 1] != NULL)
	if (chessboard[_x - 1][_y - 1]->getColor() != color)
	{
		figures[chessboard[_x - 1][_y - 1]->getID()] = NULL;
		delete chessboard[_x - 1][_y - 1];
	}
	chessboard[position.x - 1][position.y - 1] = NULL;
	chessboard[_x - 1][_y - 1] = this;
	chessboard[_x - 1][_y - 1]->position.x = _x;
	chessboard[_x - 1][_y - 1]->position.y = _y;
	if (lastking == this)
		DialogBox(hLib, MAKEINTRESOURCE(IDD_FIGURE_SELECT), NULL, Figure_Select_Proc);
	pos* p = chessboard[_x - 1][_y - 1]->generate_steps();
	if (p != NULL)
	if (color == WHITE)
	{
		UINT s = _msize(p) / sizeof(pos);
		for (int i = 0; i < s; ++i)
		if (p[i].x == figures[ID_BLACK_KING]->getPosition().x && p[i].y == figures[ID_BLACK_KING]->getPosition().y)
		{
			B_SHAH = 1;
			MessageBox(NULL, "Shah to black King!", "MSG", MB_OK);
		}
	}
	else
	{
		UINT s = _msize(p) / sizeof(pos);
		for (int i = 0; i < s; ++i)
		if (p[i].x == figures[ID_WHITE_KING]->getPosition().x && p[i].y == figures[ID_WHITE_KING]->getPosition().y)
		{
			W_SHAH = 1;
			MessageBox(NULL, "Shah to white King!", "MSG", MB_OK);
		}
	}
}

struct FOCUS
{
	UNIT* pFocusedU;
	UINT lx, ly;
};

bool check_field(bool _col, UINT _x, UINT _y)
{
	pos* p=NULL;
	for (int i = 0; i < 32; ++i)
	{
		if (figures[i] != NULL)
			if (figures[i]->getID() != ID_WHITE_KING && figures[i]->getID() != ID_BLACK_KING)
				if (figures[i]->getColor() != _col)
				{
					p = figures[i]->generate_steps();
					if (p != NULL)
					{
						UINT s = _msize(p) / sizeof(pos);
						for (int j = 0; j < s; ++j)
						if (p[j].x == _x && p[j].y == _y)
							return 1;
					}
				}
	}
	return 0;
}

bool check_shah(UNIT* u, UINT _x, UINT _y)
{
	if (enclosure == 1)
	{
		enclosure = 0;
		UNIT* u1=NULL;
		UINT ID = 0;
		bool b = 0;
		if (chessboard[_x - 1][_y - 1]==NULL)
		chessboard[_x - 1][_y - 1] = u;
		else
		{
			u1 = chessboard[_x - 1][_y - 1];
			chessboard[_x - 1][_y - 1] = u;
			ID=u1->getID();
			figures[ID] = NULL;
			b = 1;
		}
		if (u->getColor() == WHITE)
		{
			if (W_SHAH) if (check_field(WHITE, figures[ID_WHITE_KING]->getPosition().x, figures[ID_WHITE_KING]->getPosition().y))
			{
				chessboard[_x - 1][_y - 1] = u1;
				if (b)
				{
					figures[ID] = u1;
					b = 0;
				}
				enclosure = 1;
				return 1;
			}
		}
		else
		{
			if (B_SHAH) if (check_field(BLACK, figures[ID_BLACK_KING]->getPosition().x, figures[ID_BLACK_KING]->getPosition().y))
			{
				chessboard[_x - 1][_y - 1] = u1;
				if (b)
				{
					figures[ID] = u1;
					b = 0;
				}
				enclosure = 1;
				return 1;
			}
		}
		chessboard[_x - 1][_y - 1] = u1;
		if (b)
		{
			figures[ID] = u1;
			b = 0;
		}
		enclosure = 1;
	}	
	return 0;
}

pos* add(pos* p, pos a)
{
	if (p==NULL)
	{
		pos* l = new pos();
		*l = a;
		return l;
	}
	UINT s = _msize(p) / sizeof(pos);
	pos* l = new pos[s + 1];
	for (int i = 0; i < s; ++i)
		l[i] = p[i];
	l[s] = a;
	return l;
}

void AI(UINT n)
{

}

struct PAWN : public UNIT
{
	PAWN(bool _col, UINT _x, UINT _y) : UNIT(2, _col, _x, _y,(_col==WHITE)? ID_W_PAWN : ID_B_PAWN){}
	pos* generate_steps()
	{
		pos* a = NULL;
		if (color == WHITE)
		{
			if (User_color == WHITE)
			{
				if (chessboard[position.x - 1][position.y] == NULL)
				if (check_shah(this, position.x, position.y + 1) == 0)
					a = new pos(position.x, position.y + 1, 1);
				if (position.y == 5)
				{
					UINT x=last2stepped->getPosition().x;
					if (x == position.x + 1)
						a = add(a, pos(position.x + 1, 6, 2));
					if (x == position.x - 1)
						a = add(a, pos(position.x - 1, 6, 2));
				}
				if (position.y == 2)
				{
					if (chessboard[position.x - 1][position.y] == NULL && chessboard[position.x - 1][position.y + 1] == NULL)
					if (check_shah(this, position.x, position.y + 2) == 0)
						a = add(a, pos(position.x, position.y + 2, 1));
				}
				if (position.x + 1<9 && position.x + 1 > 0 && position.y + 1<9 && position.y + 1 > 0)if (chessboard[position.x][position.y] != NULL)
				if (chessboard[position.x][position.y]->getColor() != color)
				{
					if (check_shah(this, position.x + 1, position.y + 1) == 0)
						a = add(a, pos(position.x + 1, position.y + 1, chessboard[position.x][position.y]->getValue()));
				}
				if (position.x - 1<9 && position.x - 1 > 0 && position.y + 1<9 && position.y + 1 > 0)if (chessboard[position.x - 2][position.y] != NULL)
				if (chessboard[position.x - 2][position.y]->getColor() != color)
				{
					if (check_shah(this, position.x - 1, position.y + 1) == 0)
						a = add(a, pos(position.x - 1, position.y + 1, chessboard[position.x - 2][position.y]->getValue()));
				}
			}
			else
			{
				if (chessboard[position.x - 1][position.y-2] == NULL)
				if (check_shah(this, position.x, position.y - 1) == 0)
					a = new pos(position.x, position.y - 1, 1);
				if (position.y == 5)
				{
					UINT x = last2stepped->getPosition().x;
					if (x == position.x + 1)
						a = add(a, pos(position.x + 1, 6, 2));
					if (x == position.x - 1)
						a = add(a, pos(position.x - 1, 6, 2));
				}
				if (position.y == 7)
				{
					if (chessboard[position.x - 1][position.y - 3] == NULL && chessboard[position.x - 1][position.y - 3] == NULL)
					if (check_shah(this, position.x, position.y - 2) == 0)
						a = add(a, pos(position.x, position.y - 2, 1));
				}
				if (position.x + 1<9 && position.x + 1 > 0 && position.y - 1<9 && position.y - 1 > 0)if (chessboard[position.x][position.y-2] != NULL)
				if (chessboard[position.x][position.y-2]->getColor() != color)
				{
					if (check_shah(this, position.x + 1, position.y - 1) == 0)
						a = add(a, pos(position.x + 1, position.y - 1, chessboard[position.x][position.y-2]->getValue()));
				}
				if (position.x - 1<9 && position.x - 1 > 0 && position.y - 1<9 && position.y - 1 > 0)if (chessboard[position.x - 2][position.y-2] != NULL)
				if (chessboard[position.x - 2][position.y-2]->getColor() != color)
				{
					if (check_shah(this, position.x - 1, position.y - 1) == 0)
						a = add(a, pos(position.x - 1, position.y - 1, chessboard[position.x - 2][position.y-2]->getValue()));
				}
			}
		}
		else
		{
			if (User_color == WHITE)
			{
				if (chessboard[position.x - 1][position.y - 2] == NULL)
				if (check_shah(this, position.x, position.y - 1) == 0)
					a = new pos(position.x, position.y - 1, 1);

				if (position.y == 4)
				{
					UINT x = last2stepped->getPosition().x;
					if (x == position.x + 1)
						a = add(a, pos(position.x + 1, 3, 2));
					if (x == position.x - 1)
						a = add(a, pos(position.x - 1, 3, 2));
				}

				if (position.y == 7)
				{
					if (chessboard[position.x - 1][position.y - 2] == NULL && chessboard[position.x - 1][position.y - 3] == NULL)
					if (check_shah(this, position.x, position.y - 2) == 0)
						a = add(a, pos(position.x, position.y - 2, 1));
				}
				if (position.x + 1<9 && position.x + 1 > 0 && position.y - 1<9 && position.y - 1 > 0)if (chessboard[position.x][position.y - 2] != NULL)
				if (chessboard[position.x][position.y - 2]->getColor() != color)
				{
					if (check_shah(this, position.x + 1, position.y - 1) == 0)
						a = add(a, pos(position.x + 1, position.y - 1, chessboard[position.x][position.y - 2]->getValue()));
				}
				if (position.x - 1<9 && position.x - 1 > 0 && position.y - 1<9 && position.y - 1 > 0)if (chessboard[position.x - 2][position.y - 2] != NULL)
				if (chessboard[position.x - 2][position.y - 2]->getColor() != color)
				{
					if (check_shah(this, position.x - 1, position.y - 1) == 0)
						a = add(a, pos(position.x - 1, position.y - 1, chessboard[position.x - 2][position.y - 2]->getValue()));
				}
			}
			else
			{
				if (chessboard[position.x - 1][position.y] == NULL)
				if (check_shah(this, position.x, position.y + 1) == 0)
					a = new pos(position.x, position.y + 1, 1);

				if (position.y == 4)
				{
					UINT x = last2stepped->getPosition().x;
					if (x == position.x + 1)
						a = add(a, pos(position.x + 1, 3, 2));
					if (x == position.x - 1)
						a = add(a, pos(position.x - 1, 3, 2));
				}

				if (position.y == 2)
				{
					if (chessboard[position.x - 1][position.y] == NULL && chessboard[position.x - 1][position.y+1] == NULL)
					if (check_shah(this, position.x, position.y+2) == 0)
						a = add(a, pos(position.x, position.y + 2, 1));
				}
				if (position.x + 1<9 && position.x + 1 > 0 && position.y + 1<9 && position.y + 1 > 0)if (chessboard[position.x][position.y] != NULL)
				if (chessboard[position.x][position.y]->getColor() != color)
				{
					if (check_shah(this, position.x + 1, position.y + 1) == 0)
						a = add(a, pos(position.x + 1, position.y + 1, chessboard[position.x][position.y]->getValue()));
				}
				if (position.x - 1<9 && position.x - 1 > 0 && position.y + 1<9 && position.y + 1 > 0)if (chessboard[position.x - 2][position.y] != NULL)
				if (chessboard[position.x - 2][position.y]->getColor() != color)
				{
					if (check_shah(this, position.x - 1, position.y + 1) == 0)
						a = add(a, pos(position.x - 1, position.y + 1, chessboard[position.x - 2][position.y]->getValue()));
				}
			}
		}
		return a;
	}
};

struct ROOK : public UNIT
{
	ROOK(bool _col, UINT _x, UINT _y) : UNIT(10, _col, _x, _y, (_col == WHITE) ? ID_W_ROOK : ID_B_ROOK){}
	pos* generate_steps()
	{
		pos* a = NULL;
		int x = position.x;
		while (x < 8)
		{
			++x;
			if (x>0 && x <= 8) if (chessboard[x - 1][position.y - 1] == NULL)
			{
				if (check_shah(this, x, position.y) == 0)
					a = add(a, pos(x, position.y, 0));
			}
			else
			{
				if (chessboard[x-1][position.y-1]->getColor() != color)
				if (check_shah(this, x, position.y) == 0)
					a = add(a, pos(x,position.y,chessboard[x-1][position.y-1]->getValue()));
				break;
			}
		}
		x = position.x;
		while (x > 0)
		{
			--x;
			if (x > 0 && x <= 8) if (chessboard[x - 1][position.y - 1] == NULL)
			{
				if (check_shah(this, x, position.y) == 0)
					a = add(a, pos(x, position.y, 0));
			}
			else
			{
				if (chessboard[x - 1][position.y - 1]->getColor() != color)
				if (check_shah(this, x, position.y) == 0)
					a = add(a, pos(x, position.y, chessboard[x - 1][position.y - 1]->getValue()));
				break;
			}
		}
		int y = position.y;
		while (y < 8)
		{
			++y;
			if (y>0 && y <= 8) if (chessboard[position.x - 1][y - 1] == NULL)
			{
				if (check_shah(this, position.x, y) == 0)
					a = add(a, pos(position.x, y, 0));
			}
			else
			{
				if (chessboard[position.x - 1][y - 1]->getColor() != color)
				if (check_shah(this, position.x, y) == 0)
					a = add(a, pos(position.x, y, chessboard[position.x - 1][y - 1]->getValue()));
				break;
			}
		}
		y = position.y;
		while (y > 0)
		{
			--y;
			if (y > 0 && y <= 8) if (chessboard[position.x - 1][y - 1] == NULL)
			{
				if (check_shah(this, position.x, y) == 0)
					a = add(a, pos(position.x, y, 0));
			}
			else
			{
				if (chessboard[position.x - 1][y - 1]->getColor() != color)
				if (check_shah(this, position.x, y) == 0)
					a = add(a, pos(position.x, y, chessboard[position.x - 1][y - 1]->getValue()));
				break;
			}
		}
		return a;
	}
};

struct HORSE : public UNIT
{
	HORSE(bool _col, UINT _x, UINT _y) : UNIT(6, _col, _x, _y, (_col == WHITE) ? ID_W_HORSE : ID_B_HORSE){}
	pos* h(pos* a, UINT x, UINT y)
	{
		if (x<9 && x>0 && y<9 && y>0)
		if (chessboard[x - 1][y - 1] != NULL)
		{
			if (chessboard[x - 1][y - 1]->getColor() != color)
			if (check_shah(this, x, y) == 0)
				a = add(a, pos(x, y, chessboard[x - 1][y - 1]->getValue()));
		}
		else if (check_shah(this, x, y) == 0)
			a = add(a, pos(x, y, 0));
		return a;
	}
	pos* generate_steps()
	{
		pos* a = NULL;
		UINT y = position.y + 1;
		UINT x = position.x + 2;
		a = h(a, x, y);
		y = position.y + 1;
		x = position.x - 2;
		a = h(a, x, y);
		y = position.y - 1;
		x = position.x + 2;
		a = h(a, x, y);
		y = position.y - 1;
		x = position.x - 2;
		a = h(a, x, y);
		y = position.y + 2;
		x = position.x + 1;
		a = h(a, x, y);
		y = position.y + 2;
		x = position.x - 1;
		a = h(a, x, y);
		y = position.y - 2;
		x = position.x + 1;
		a = h(a, x, y);
		y = position.y - 2;
		x = position.x - 1;
		a = h(a, x, y);
		return a;
	}
};

struct OFFICER : public UNIT
{
	OFFICER(bool _col, UINT _x, UINT _y) : UNIT(6, _col, _x, _y, (_col == WHITE) ? ID_W_OFFICER : ID_B_OFFICER){}
	pos* generate_steps()
	{
		pos* a = NULL;
		UINT i = 1;
		while (position.x + i > 0 && position.x + i < 9 && position.y + i>0 && position.y + i < 9)
		{
			if (chessboard[position.x + i - 1][position.y + i - 1] != NULL)
			{
				if (chessboard[position.x + i - 1][position.y + i - 1]->getColor() != color) 
				if (check_shah(this, position.x + i, position.y + i) == 0)
					a = add(a, pos(position.x + i, position.y + i, chessboard[position.x + i - 1][position.y + i - 1]->getValue()));
				break;
			}
			else if (check_shah(this, position.x + i, position.y + i) == 0)
				a = add(a, pos(position.x + i, position.y + i, 0));
			++i;
		}

		i = 1;
		while (position.x + i > 0 && position.x + i < 9 && position.y - i>0 && position.y - i < 9)
		{
			if (chessboard[position.x + i - 1][position.y - i - 1] != NULL)
			{
				if (chessboard[position.x + i - 1][position.y - i - 1]->getColor() != color)
				if (check_shah(this, position.x + i, position.y - i) == 0)
					a = add(a, pos(position.x + i, position.y - i, chessboard[position.x + i - 1][position.y - i - 1]->getValue()));
				break;
			}
			else if (check_shah(this, position.x + i, position.y - i) == 0)
				a = add(a, pos(position.x + i, position.y - i, 0));
			++i;
		}

		i = 1;
		while (position.x - i > 0 && position.x - i < 9 && position.y + i>0 && position.y + i < 9)
		{
			if (chessboard[position.x - i - 1][position.y + i - 1] != NULL)
			{
				if (chessboard[position.x - i - 1][position.y + i - 1]->getColor() != color)
				if (check_shah(this, position.x - i, position.y + i) == 0)
					a = add(a, pos(position.x - i, position.y + i, chessboard[position.x - i - 1][position.y + i - 1]->getValue()));
				break;
			}
			else if (check_shah(this, position.x - i, position.y + i) == 0)
				a = add(a, pos(position.x - i, position.y + i, 0));
			++i;
		}

		i = 1;
		while (position.x - i > 0 && position.x - i < 9 && position.y - i>0 && position.y - i < 9)
		{
			if (chessboard[position.x - i - 1][position.y - i - 1] != NULL)
			{
				if (chessboard[position.x - i - 1][position.y - i - 1]->getColor() != color)
				if (check_shah(this, position.x - i, position.y - i) == 0)
					a = add(a, pos(position.x - i, position.y - i, chessboard[position.x - i - 1][position.y - i - 1]->getValue()));
				break;
			}
			else if (check_shah(this, position.x - i, position.y - i) == 0)
				a = add(a, pos(position.x - i, position.y - i, 0));
			++i;
		}
		return a;
	}
};

struct QUEEN : public UNIT
{
	QUEEN(bool _col, UINT _x, UINT _y) : UNIT(18, _col, _x, _y, (_col == WHITE) ? ID_W_QUEEN : ID_B_QUEEN){}
	pos* generate_steps()
	{
		pos* a = NULL;
		UINT i = 1;
		while (position.x + i > 0 && position.x + i < 9 && position.y + i>0 && position.y + i < 9)
		{
			if (chessboard[position.x + i - 1][position.y + i - 1] != NULL)
			{
				if (chessboard[position.x + i - 1][position.y + i - 1]->getColor() != color)
				if (check_shah(this, position.x + i, position.y + i) == 0)
					a = add(a, pos(position.x + i, position.y + i, chessboard[position.x + i - 1][position.y + i - 1]->getValue()));
				break;
			}
			else if (check_shah(this, position.x + i, position.y + i) == 0)
				a = add(a, pos(position.x + i, position.y + i, 0));
			++i;
		}

		i = 1;
		while (position.x + i > 0 && position.x + i < 9 && position.y - i>0 && position.y - i < 9)
		{
			if (chessboard[position.x + i - 1][position.y - i - 1] != NULL)
			{
				if (chessboard[position.x + i - 1][position.y - i - 1]->getColor() != color)
				if (check_shah(this, position.x + i, position.y - i) == 0)
					a = add(a, pos(position.x + i, position.y - i, chessboard[position.x + i - 1][position.y - i - 1]->getValue()));
				break;
			}
			else if (check_shah(this, position.x + i, position.y - i) == 0)
				a = add(a, pos(position.x + i, position.y - i, 0));
			++i;
		}

		i = 1;
		while (position.x - i > 0 && position.x - i < 9 && position.y + i>0 && position.y + i < 9)
		{
			if (chessboard[position.x - i - 1][position.y + i - 1] != NULL)
			{
				if (chessboard[position.x - i - 1][position.y + i - 1]->getColor() != color)
				if (check_shah(this, position.x - i, position.y + i) == 0)
					a = add(a, pos(position.x - i, position.y + i, chessboard[position.x - i - 1][position.y + i - 1]->getValue()));
				break;
			}
			else if (check_shah(this, position.x - i, position.y + i) == 0)
				a = add(a, pos(position.x - i, position.y + i, 0));
			++i;
		}

		i = 1;
		while (position.x - i > 0 && position.x - i < 9 && position.y - i>0 && position.y - i < 9)
		{
			if (chessboard[position.x - i - 1][position.y - i - 1] != NULL)
			{
				if (chessboard[position.x - i - 1][position.y - i - 1]->getColor() != color)
				if (check_shah(this, position.x - i, position.y - i) == 0)
					a = add(a, pos(position.x - i, position.y - i, chessboard[position.x - i - 1][position.y - i - 1]->getValue()));
				break;
			}
			else if (check_shah(this, position.x - i, position.y - i) == 0)
				a = add(a, pos(position.x - i, position.y - i, 0));
			++i;
		}
		int x = position.x;
		while (x < 8)
		{
			++x;
			if (x>0 && x <= 8) if (chessboard[x - 1][position.y - 1] == NULL)
			{
				if (check_shah(this, x, position.y) == 0)
					a = add(a, pos(x, position.y, 0));
			}
			else
			{
				if (chessboard[x - 1][position.y - 1]->getColor() != color) if (check_shah(this, x, position.y) == 0)
					a = add(a, pos(x, position.y, chessboard[x - 1][position.y - 1]->getValue()));
				break;
			}
		}
		x = position.x;
		while (x > 0)
		{
			--x;
			if (x > 0 && x <= 8) if (chessboard[x - 1][position.y - 1] == NULL)
			{
				if (check_shah(this, x, position.y) == 0)
					a = add(a, pos(x, position.y, 0));
			}
			else
			{
				if (chessboard[x - 1][position.y - 1]->getColor() != color) if (check_shah(this, x, position.y) == 0)
					a = add(a, pos(x, position.y, chessboard[x - 1][position.y - 1]->getValue()));
				break;
			}
		}
		int y = position.y;
		while (y < 8)
		{
			++y;
			if (y>0 && y <= 8) if (chessboard[position.x - 1][y - 1] == NULL)
			{
				if (check_shah(this, position.x, y) == 0)
					a = add(a, pos(position.x, y, 0));
			}
			else
			{
				if (chessboard[position.x - 1][y - 1]->getColor() != color) if (check_shah(this, position.x, y) == 0)
					a = add(a, pos(position.x, y, chessboard[position.x - 1][y - 1]->getValue()));
				break;
			}
		}
		y = position.y;
		while (y > 0)
		{
			--y;
			if (y > 0 && y <= 8) if (chessboard[position.x - 1][y - 1] == NULL)
			{
				if (check_shah(this, position.x, y) == 0)
					a = add(a, pos(position.x, y, 0));
			}
			else
			{
				if (chessboard[position.x - 1][y - 1]->getColor() != color) if (check_shah(this, position.x, y) == 0)
					a = add(a, pos(position.x, y, chessboard[position.x - 1][y - 1]->getValue()));
				break;
			}
		}
		return a;
	}
};

struct KING : public UNIT
{
	KING(bool _col, UINT _x, UINT _y) : UNIT(0, _col, _x, _y, (_col == WHITE) ? ID_W_KING : ID_B_KING){}
	pos* generate_steps()
	{
		pos* a = NULL;
		if (position.x+1>0 && position.x+1< 9 && position.y>0 && position.y < 9)
		if (chessboard[position.x][position.y - 1] != NULL)
		{
			if (chessboard[position.x][position.y - 1]->getColor() != color && check_field(color, position.x + 1, position.y)==0)
				a = add(a, pos(position.x+1, position.y, chessboard[position.x][position.y - 1]->getValue()));
		}
		else if (check_field(color, position.x + 1, position.y) == 0)
			a = add(a, pos(position.x + 1, position.y, 0));

		if (position.x + 1>0 && position.x + 1< 9 && position.y+1>0 && position.y+1< 9)
		if (chessboard[position.x][position.y] != NULL)
		{
			if (chessboard[position.x][position.y]->getColor() != color && check_field(color, position.x + 1, position.y + 1) == 0)
				a = add(a, pos(position.x+1, position.y + 1, chessboard[position.x][position.y]->getValue()));
		}
		else if (check_field(color, position.x + 1, position.y + 1) == 0)
			a = add(a, pos(position.x + 1, position.y + 1, 0));

		if (position.x + 1>0 && position.x + 1< 9 && position.y-1>0 && position.y-1< 9)
		if (chessboard[position.x][position.y - 2] != NULL)
		{
			if (chessboard[position.x][position.y - 2]->getColor() != color && check_field(color, position.x + 1, position.y - 1) == 0)
				a = add(a, pos(position.x +1, position.y - 1, chessboard[position.x][position.y - 2]->getValue()));
		}
		else if (check_field(color, position.x + 1, position.y - 1) == 0)
			a = add(a, pos(position.x+1, position.y - 1, 0));

		if (position.x - 1>0 && position.x - 1< 9 && position.y>0 && position.y < 9)
		if (chessboard[position.x-2][position.y - 1] != NULL)
		{
			if (chessboard[position.x - 2][position.y - 1]->getColor() != color && check_field(color, position.x - 1, position.y) == 0)
				a = add(a, pos(position.x-1, position.y, chessboard[position.x-2][position.y - 1]->getValue()));
		}
		else if (check_field(color, position.x - 1, position.y) == 0)
			a = add(a, pos(position.x-1, position.y, 0));

		if (position.x - 1>0 && position.x - 1< 9 && position.y+1>0 && position.y+1 < 9)
		if (chessboard[position.x-2][position.y] != NULL)
		{
			if (chessboard[position.x - 2][position.y]->getColor() != color && check_field(color, position.x - 1, position.y + 1) == 0)
				a = add(a, pos(position.x-1, position.y + 1, chessboard[position.x-2][position.y]->getValue()));
		}
		else if (check_field(color, position.x - 1, position.y + 1) == 0)
			a = add(a, pos(position.x - 1, position.y + 1, 0));

		if (position.x - 1>0 && position.x - 1< 9 && position.y - 1>0 && position.y - 1 < 9)
		if (chessboard[position.x - 2][position.y-2] != NULL)
		{
			if (chessboard[position.x - 2][position.y - 2]->getColor() != color && check_field(color, position.x - 1, position.y - 1) == 0)
				a = add(a, pos(position.x - 1, position.y - 1, chessboard[position.x = 2][position.y - 2]->getValue()));
		}
		else if (check_field(color, position.x - 1, position.y - 1) == 0)
			a = add(a, pos(position.x - 1, position.y - 1, 0));

		if (position.x>0 && position.x< 9 && position.y+1>0 && position.y+1< 9)
		if (chessboard[position.x-1][position.y] != NULL)
		{
			if (chessboard[position.x - 1][position.y]->getColor() != color && check_field(color, position.x, position.y + 1) == 0)
				a = add(a, pos(position.x, position.y + 1, chessboard[position.x-1][position.y]->getValue()));
		}
		else if (check_field(color, position.x, position.y + 1) == 0)
			a = add(a, pos(position.x, position.y + 1, 0));

		if (position.x>0 && position.x< 9 && position.y-1>0 && position.y-1 < 9)
		if (chessboard[position.x-1][position.y - 2] != NULL)
		{
			if (chessboard[position.x - 1][position.y - 2]->getColor() != color && check_field(color, position.x, position.y - 1) == 0)
				a = add(a, pos(position.x, position.y - 1, chessboard[position.x - 1][position.y - 2]->getValue()));
		}
		else if (check_field(color, position.x, position.y - 1) == 0)
			a = add(a, pos(position.x, position.y - 1, 0));

		if (color == WHITE)
		{
			if (W_ROOK2cstlng==1)
			if (chessboard[position.x][position.y - 1] == NULL && chessboard[position.x + 1][position.y - 1] == NULL)
			if (check_field(color, position.x + 1, position.y) == 0 && check_field(color, position.x + 2, position.y) == 0)
				a = add(a, pos(position.x + 2, position.y, 1));
			if (W_ROOK1cstlng == 1)
			if (chessboard[position.x-2][position.y - 1] == NULL && chessboard[position.x -3][position.y - 1] == NULL)
			if (check_field(color, position.x - 1, position.y) == 0 && check_field(color, position.x - 2, position.y) == 0)
				a = add(a, pos(position.x - 2, position.y, 1));
		}
		else
		{
			if (B_ROOK2cstlng == 1)
			if (chessboard[position.x][position.y - 1] == NULL && chessboard[position.x + 1][position.y - 1] == NULL)
			if (check_field(color, position.x + 1, position.y) == 0 && check_field(color, position.x + 2, position.y) == 0)
				a = add(a, pos(position.x + 2, position.y, 1));
			if (B_ROOK1cstlng == 1)
			if (chessboard[position.x - 2][position.y - 1] == NULL && chessboard[position.x - 3][position.y - 1] == NULL)
			if (check_field(color, position.x - 1, position.y) == 0 && check_field(color, position.x - 2, position.y) == 0)
				a = add(a, pos(position.x - 2, position.y, 1));
		}
		return a;
	}
};
