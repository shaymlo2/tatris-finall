#pragma once
#include <iostream>
#include "Point.h"

using namespace std;

constexpr int INIT_POINT_X = 15;
constexpr int INIT_POINT_Y = 5;

class Shapes {
public:
	enum Type { SQUARE = 0, LINE, JOKER, BOOMB };
	enum Size { size4 = 4, size1 = 1 };

private:
	Point *body;
	char c;
	Type t;
	Size size;

public:

	Shapes(Type type, const Point& newS, char c1='#');
	~Shapes() { delete[]body; }

	//move and draw functions
	Point::DIR getDir(char keyPressed);
	void draw() { draw(c); }
	void draw(char ch);
	void move(char keyPressed = 'f', bool flage=true);
	void rotation(Point * shape);

	//Get data from the shape
	Point* getPoints() const { return body; }
	const int getSize() const {return size;}
	Type getType() const { return t; }
	char getChar() const {return c;}
};