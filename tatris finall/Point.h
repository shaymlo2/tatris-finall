#pragma once

#include <iostream>
#include "utils.h"
using namespace std;

class Point {
public:
	enum DIR { LEFT, RIGHT, DOWN, UP, NONE };

private:
	int x, y;
	int dir_x = 1, dir_y = 0;
	

public:

	Point(int x1 = 1, int y1 = 1){
		x = x1;
		y = y1;
	}

	//Get the coordinates
	int getX() const {return x;}
	int getY()const {return y;}

	//move
	Point right() const {return Point(x + 1, y);}
	Point down() const {return Point(x, y + 1);}

	void changeDir(DIR dir, int nMove);
	void move(DIR dir, int nMove) { changeDir(dir, nMove);}

	void draw(char c = '*')
	{
		gotoxy(x, y);
		cout << c << endl;
		cout.flush();
	}
};