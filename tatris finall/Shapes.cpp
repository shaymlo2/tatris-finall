#include "Shapes.h"
#include "Point.h"


//The function build the new shape by one point
Shapes::Shapes(Type type, const Point& newS, char c1)
{

	t = type;

	switch (t)
	{
	case SQUARE:
		body = new Point[size4];
		body[0] = newS;
		body[1] =newS.right();
		body[2] = newS.down();
		body[3] = body[2].right();
		c = '#';
		size = size4;
		break;

	case LINE:
		body = new Point[size4];
		body[0] = newS;
		body[1] = newS.right();
		body[2] = body[1].right();
		body[3] = body[2].right();
		size = size4;
		c = '#';
		break;

	case JOKER:
		body = new Point[size1];
		body[0] = newS;
		size = size1;
		c = '$';
		break;

	case BOOMB:
		body = new Point[size1];
		body[0] = newS;
		size = size1;
		c = '@';
		break;
	default:
		break;
	}
}

//Draw the shape by it's point.
void Shapes::draw(char ch)
{
	if (t == BOOMB || t == JOKER)
		body[0].draw(ch);

	else if (t == SQUARE || t == LINE)
	{
		for (int i = 0; i < size4; ++i)
			body[i].draw(ch);
	}
}
//"Converts" the key to direction.
Point::DIR Shapes::getDir(char keyPressed)
{
	switch (keyPressed)
	{
	case 'd':
		return Point::DIR::LEFT;
		break;
	case 'f':
		return Point::DIR::DOWN;
		break;
	case 'g':
		return Point::DIR::RIGHT;
		break;
	case 'r':
		return Point::DIR::UP;
		break;
	default:
		return Point::DIR::NONE;
	}
}

//move the shapes by the dir
void Shapes::move(char keyPressed, bool flage)
{
	Point::DIR dir = getDir(keyPressed);
	if (dir != Point::DIR::UP)
	{
		if (flage) //The flage is intended  to help the Joker to penetrate
			draw(' ');

		if (t == BOOMB || t == JOKER)
		{
			body[0].move(dir, 1);
		}

		else if (t == SQUARE || t == LINE)
		{
			for (int i = 0; i < size4; ++i) {
				body[i].move(dir, 1);
			}
		}
		draw();
	}
	else
		rotation(body);
}

// rotate the line depend it's state: Vertical or horizontal
void Shapes::rotation(Point * shape)
{
	
	switch (t)
	{
	case Shapes::LINE:
		draw(' ');
		//Vertical:
		if (body[0].getX() == body[1].getX())
		{
			shape[0].move(Point::DIR::LEFT,1);
			shape[0].move(Point::DIR::DOWN,1);
			shape[2].move(Point::DIR::RIGHT, 1);
			shape[2].move(Point::DIR::UP, 1);
			shape[3].move(Point::DIR::RIGHT, 2);
			shape[3].move(Point::DIR::UP, 2);
		}
		else //Horizontal
		{
			shape[0].move(Point::DIR::UP, 1);
			shape[0].move(Point::DIR::RIGHT, 1);
			shape[2].move(Point::DIR::DOWN, 1);
			shape[2].move(Point::DIR::LEFT, 1);
			shape[3].move(Point::DIR::DOWN, 2);
			shape[3].move(Point::DIR::LEFT, 2);
		}
		draw();
		break;
	
	default:
		break;
	}
}



