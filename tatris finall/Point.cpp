#include "Point.h"

//Get a direction and number of movements and returns the new cordinates.
void Point::changeDir(DIR dir, int nMove)
{
	switch (dir) {
	case DIR::LEFT:
		dir_x = -nMove;
		dir_y = 0;
		break;

	case DIR::RIGHT:
		dir_x = nMove;
		dir_y = 0;
		break;

	case DIR::DOWN:
		dir_x = 0;
		dir_y = nMove;
		break;

	case DIR::UP:
		dir_x = 0;
		dir_y = -nMove;
		break;

	case DIR::NONE:
		dir_x = 0;
		dir_y = 0;
		break;
	}

	x = (x + dir_x + 21) % 21;
	y = (y + dir_y + 21) % 21;

}
