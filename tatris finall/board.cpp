#include "Board.h"
#include "Shapes.h"

Board::Board() : col(10), row(15)
{
	boardGame = new char *[row]; // allocating the columns
	for (int i = 0; i < row; i++)
		boardGame[i] = new char[col]; // allocating the  rows


	for (int i = 0; i < row; i++) 
		for (int m = 0; m < col; m++)
			boardGame[i][m] = SPACE;
}

Board::~Board()
{
	for (int i = 0; i < row; i++)
		delete[] boardGame[i];

	delete[]boardGame;
}

void Board::run()
{
	char keyPressed;
	bool flage = true;;

	drawBoard(boardGame);

	time_t dummy;// gets the current shape type randomly.
	srand((unsigned int)time(NULL));

	//the game will start by pressing '1'
	while (!endGame)
	{
		keyPressed = _getch();
		menu(keyPressed);
	}

	//while the game is on
	while (endGame)
	{
		//After each iteration we need to check if there is a row that we need to delete
		checkDeletRow(boardGame);
		printBoard(boardGame);
		
		//every iteration a shape will be "chosen" randomly.
       	dummy = rand() % 4;
		typeShape = (Shapes::Type)dummy;

		shape = new Shapes(typeShape, { INIT_POINT_X,INIT_POINT_Y }); // allocation
		countShapes++;
		shape->draw();

		keyPressed = 0;

		//we always go down, so we need to check if the shape is not blocked.
		if (!(Board::check(boardGame, shape->getPoints(), shape->getType(), 'f')))
		{
			menu('9');
			if (!endGame)
			{
				gameOver(flage);
				break;
			}
		}

		// the decent is down, so we need to check if the shape can do that.
		while (Board::check(boardGame, shape->getPoints(), shape->getType(), 'f'))
		{
			if (_kbhit())
			{		
				menu(keyPressed = _getch());
				if (!endGame) 
				{
					gameOver(flage);
					break;
				}
			}
			else
				keyPressed = 0;

			//special key for the Joker that gives the abillity to stop the shape.
			if (Shapes::JOKER == shape->getType())
				if (keyPressed == 's')
				{
				    updateBoardGame(boardGame, shape, shape->getChar());
					break;
				}

			//in case we can go down- we need update the board game.
			moveAndUpdate(boardGame, shape, 'f');

			//the rapid decline button
			if (keyPressed == 'e')
				quick(boardGame, shape);


			//check for the keypressed and update the board game accordingly
			else if (Board::check(boardGame, shape->getPoints(), shape->getType(), keyPressed))
				moveAndUpdate(boardGame, shape, keyPressed);

			Sleep(speed);
		}

		//if case that the shape is boomb we need to blow anyone around.
		if (shape->getType() == Shapes::BOOMB && flage)
		{
			deleteByBoom(boardGame, shape);
			printBoard(boardGame);
		}

		//the shape is dynamically assigned
		delete shape;
	}
}

void Board::menu(char keyPressed)
{ 
	switch (keyPressed)
	{
		//whait until the game will start
	case '1':
		endGame = true;
		break;

		///pause the game until the next '2'
	case '2':
		while (_getch() != '2')
		{
			//loop until press again '2'
		}
		break;
	
	//speed up
	case '3':
		if (speed > 200 && speed <= 500)
		{
			speed -= 50;
			speedLevel++;
		}
		break;

		//speed down
	case '4':
		if (speed >= 200 && speed < 500)
		{
			speed += 50;
			speedLevel--;
		}
		break;

	case '9': //is ESC
		endGame = false;
		break;
	
	}
}

void Board::moveAndUpdate(char ** boardGame, Shapes * shape, char keyPressed)
{
	int set_X = (shape->getPoints())[0].getX();
	int set_Y = (shape->getPoints())[0].getY();

	//The Joker has the ability to access the form
	if (shape->getType() == Shapes::JOKER && set_Y < BOTTOM_LIMIT) 
	{
		if (boardGame[set_Y - START_POINT_Y + 1][set_X - START_POINT_X] != SPACE)
		{
		//In case the Joker is going to enter a layer
			if (!isInLayer)
			{
				isInLayer = true;
				updateBoardGame(boardGame, shape, SPACE);
				shape->move(keyPressed);// moves the shape down every iteration.
			}
			else
				shape->move(keyPressed, false);// moves the shape down every iteration.
		}
		else
		{//The Joker is already inside a layer
			if (isInLayer)
			{
				isInLayer = false;
				shape->move(keyPressed, false);// moves the shape down every iteration.
				updateBoardGame(boardGame, shape, shape->getChar());
			}
			else 
				moveAndUpdateAUX(boardGame, shape, keyPressed);
		}
	}
	else
		moveAndUpdateAUX(boardGame, shape, keyPressed);

	printBoard(boardGame);
}

void Board::moveAndUpdateAUX(char ** boardGame, Shapes * shape, char keyPressed)
{
	updateBoardGame(boardGame, shape, SPACE);
	shape->move(keyPressed);// moves the shape down every iteration.
	updateBoardGame(boardGame, shape, shape->getChar());
}

void Board::checkDeletRow(char ** boardGame)
{
	bool check;
	int numOfRowsDelete = 0;
	for (int i = row - 1; i >= 0; i--)
	{
		check = true;
		for (int j = col - 1; j >= 0; j--)
		{
			if (boardGame[i][j] == SPACE)
				check = false;
		}
		//If all the line is full so we need to  "clean" the line
		if (check)
		{
			numOfRowsDelete++;
			deletRow(boardGame, i);
			i++;
		}
	}
		updateTheScore(boardGame, numOfRowsDelete);
}

void Board::updateTheScore(char ** boardGame, int numOfRowsDelete)
{// update the score required by the number of rows that need to be deleted.
	if (numOfRowsDelete > 0)
	{
		if (isEmpty(boardGame))
			score += 800;
		else {
			while (numOfRowsDelete >= 3)
			{
				score += 500;
				numOfRowsDelete -= 3;
			}
			while (numOfRowsDelete >= 2)
			{
				score += 300;
				numOfRowsDelete -= 2;
			}

			if (numOfRowsDelete == 1)
				score += 100;
		}
	}
}

bool Board::isEmpty(char ** boardGame)
{
	//checks if the board is empty in purpose to update the board.
	for (int i = 0; i < row; i++)
		for (int m = 0; m < col; m++)
			if (boardGame[i][m] != SPACE)
				return false;
	return true;
}

void Board::deletRow(char **boardGame, int d_row)
{
	for (int i = d_row; i > 0; i--)
	{
		for (int k = 0; k < col; k++)
			boardGame[i][k] = boardGame[i - 1][k];
	}
	for (int i = 0; i < col; i++)
		boardGame[0][i] = SPACE;
}

void Board::deleteByBoom(char ** boardGame, Shapes * shape)
{
	//we will clean all the cell in 3x3 that in the limits of the board.
	int set_X = (shape->getPoints())[0].getX();
	int set_Y = (shape->getPoints())[0].getY();

	//check 3x3
	for (int i = -1; i <= 1;i++)
		for (int k = -1;k <= 1;k++)
		{
			//checks if the cell is in the limits
			if (isBounded((set_Y + i), (set_X + k)))
				updateAfterBoomb(boardGame, (set_Y - START_POINT_Y + i), (set_X - START_POINT_X + k));
		}
	// the boomb dos'nt get -50 for deleting it self
	score += 50;
}

bool Board::isBounded(int y, int x)
{
	if (x < LEFT_LIMIT)
		return false;
	else if (x > RIGHT_LIMIT)
		return false;
	else if (y > BOTTOM_LIMIT)
		return false;
	return true;
}

void Board::updateAfterBoomb(char ** boardGame, int y, int x)
{
  //we will reduce 50 points for each character that the shapes has deleted.
	if (boardGame[y][x] != SPACE)
	{
		score -= 50;
		boardGame[y][x] = SPACE;
	}
}

bool Board::check(char ** boardGame, Point * body, Shapes::Type t, char keyPressed)
{
	//This function checks whether the move is possible depending 
	// on the character that received.
	Point* p = body;

	switch (t)
	{
	case Shapes::SQUARE:
		return checkSquare(boardGame, p, keyPressed);

	case Shapes::LINE:
		return checkLine(boardGame, p, keyPressed);

	case Shapes::JOKER:
		return checkJoker(boardGame, p, keyPressed);

	case Shapes::BOOMB:
		return checkBoomb(boardGame, p, keyPressed);
	}
	return true;
}

bool Board::checkLine(char ** boardGame, Point* p, char keyPressed, int steps)
{	
	//The rectangle has two options to display.Vertical or horizontal.
	//for any key received we will check wether move or rotation is possible
	//by examining indexes in the matrix.

	//vertical:
	if (p[0].getY() == p[1].getY())
	{
		if (keyPressed == 'g')//in case the dir is right
			return checkIndexLR(boardGame, p[3].getY(), (p[3].getX() + 1));


		else if (keyPressed == 'd')// in case the dir is left

			return checkIndexLR(boardGame, p[0].getY(), (p[0].getX() - 1));

		else if (keyPressed == 'f')//in case the dir is down
		{
			for (int i = 0; i < 4; i++)
			{
				if (!(checkIndexD(boardGame, (p[i].getY() + 1) + steps, p[i].getX())))
					return false;
			}
		}

		else if (keyPressed == 'r')//for rotation
		{
			if (p[0].getY() > BOTTOM_LIMIT - 2)
				return false;
		}

		return true;
	}
	else //horizontal:
	{
		if (keyPressed == 'g')//in case the dir is right
		{
			for (int i = 0; i < 4; i++)
				if (!(checkIndexLR(boardGame, p[i].getY(), (p[i].getX() + 1))))
					return false;
		}

		else if (keyPressed == 'd')// in case the dir is left
		{
			for (int i = 0; i < 4; i++)
			{
				if (!(checkIndexLR(boardGame, p[i].getY(), (p[i].getX() - 1))))
					return false;
			}
		}

		else if (keyPressed == 'f')//in case the dir is down
			return checkIndexD(boardGame, (p[3].getY() + 1) + steps, p[3].getX());
		
		else if (keyPressed == 'r')//for rotation
		{
			if (p[0].getX() < LEFT_LIMIT + 2)
				return false;
			else if (p[3].getX() >= RIGHT_LIMIT - 2)
				return false;
		}
		return true;
	}
}

bool Board::checkBoomb(char ** boardGame, Point * p, char keyPressed, int steps)
{
	//for any key received we will check wether move or rotation is possible
	//by examining indexes in the matrix.
	if (keyPressed == 'g')//in case the dir is right
		return checkIndexLR(boardGame, p[0].getY(), (p[0].getX() + 1));

	if (keyPressed == 'd')// in case the dir is left
		return checkIndexLR(boardGame, p[0].getY(), (p[0].getX() - 1));

	if (keyPressed == 'f')//in case the dir is down
		return checkIndexD(boardGame, (p[0].getY() + 1) + steps, p[0].getX());

	return false;
}

bool Board::checkJoker(char ** boardGame, Point * p, char keyPressed)
{
	//for any key received we will check wether move or rotation is possible
	//by examining indexes in the matrix.

	int set_X, set_Y;
	bool flag = false;

	if (keyPressed == 'g')//in case the dir is right
		return checkIndexLR(boardGame, p[0].getY(), (p[0].getX() + 1));

	if (keyPressed == 'd')// in case the dir is left
		return checkIndexLR(boardGame, p[0].getY(), (p[0].getX() - 1));

	if (keyPressed == 'f')//in case the dir is down
	{
		set_X = p[0].getX();
		set_Y = p[0].getY() + 1;

		//checks if there is space under it- in the same x, but differnt y.
		while (set_Y <= 19 && !flag)
		{
			if (boardGame[set_Y - START_POINT_Y][set_X - START_POINT_X] == SPACE)
				flag = true;
			set_Y++;
		}
		return flag;
	}
	return false;
}

bool Board::checkSquare(char ** boardGame, Point * p, char keyPressed, int steps)
{
	//for any key received we will check wether move or rotation is possible
	//by examining indexes in the matrix.

	if (keyPressed == 'g')//in case the dir is right
	{
		if (!checkIndexLR(boardGame, p[1].getY(), (p[1].getX() + 1)))
			return false;

		return checkIndexLR(boardGame, p[3].getY(), (p[3].getX() + 1));
	}

	if (keyPressed == 'd')// in case the dir is left
	{
		if (!checkIndexLR(boardGame, p[0].getY(), (p[0].getX() - 1)))
			return false;

		return checkIndexLR(boardGame, p[2].getY(), (p[2].getX() - 1));
	}

	if (keyPressed == 'f')//in case the dir is down
	{
		if (!checkIndexD(boardGame, (p[2].getY() + 1) + steps, p[2].getX()))
			return false;

		return checkIndexD(boardGame, (p[3].getY() + 1) + steps, p[3].getX());
	}
	return true;
}

bool Board::checkIndexLR(char ** boardGame, int set_Y, int set_X)
{
	//Encloses the right and left 
	bool flage = true;
	if (set_X >= LEFT_LIMIT && set_X <= RIGHT_LIMIT) //limit
	{
		if (boardGame[set_Y - START_POINT_Y][set_X - START_POINT_X] != SPACE)
			flage = false;
	}
	else
		flage = false;

	return flage;
}

bool Board::checkIndexD(char ** boardGame, int set_Y, int set_X)
{
	//Encloses the lower limit.
	bool flage = true;
	if (set_Y <= BOTTOM_LIMIT)
	{
		if (boardGame[set_Y - START_POINT_Y][set_X - START_POINT_X] != SPACE)
			flage = false;
	}
	else
		flage = false;

	return flage;
}

void Board::updateBoardGame(char ** boardGame, Shapes * shape, char ch)
{
	//we will update the new location of the shape in the board
	int size = shape->getSize();
	int set_X, set_Y;

	for (int i = 0; i < size; i++)
	{
		set_X = (shape->getPoints() + i)->getX();
		set_Y = (shape->getPoints() + i)->getY();

		boardGame[set_Y - START_POINT_Y][set_X - START_POINT_X] = ch;
	}
}

void Board::quick(char ** boardGame, Shapes *shape)
{
	//To achieve the rapid decline button, we need to see how many spteps we
	// need to go down and then we will update the board game.
	int count = 0;
	count = howManyToMove(boardGame, shape);

	for (int i = 0; i < count; i++)
		moveAndUpdate(boardGame, shape, 'f');

	drawBoard(boardGame);
}

int Board::howManyToMove(char ** boardGame, Shapes *shape)
{
	int count = 0;
	Shapes::Type t = shape->getType();
	Point *p = shape->getPoints();

	//For each shape we will check how nany steps it need to do.
	//Foe each step there is a different check
	switch (t)
	{
	case Shapes::SQUARE:
		while (checkSquare(boardGame, p, 'f', count))
			count++;
		break;
	case Shapes::LINE:
		while (checkLine(boardGame, p, 'f', count))
			count++;
		break;
	case Shapes::JOKER:
		count = quickjoker(boardGame, p);
		break;
	case Shapes::BOOMB:
		while (checkBoomb(boardGame, p, 'f', count))
			count++;
		break;
	}
	return count;
}

int Board::quickjoker(char ** boardGame, Point * p)
{
	//We pass rows backwards and looking for the first space from the bottom.
	for (int i = row - 1; i >= (p[0].getY() - START_POINT_Y); i--)
	{
		if (boardGame[i][p[0].getX() - START_POINT_X] == SPACE)
			return (i - (p[0].getY() - START_POINT_Y));
	}
	return 0;
}

void Board::drawBoard(char** board)
{
	//Enclosing the limits
	for (int i = 0; i < row; i++)
	{
		gotoxy(10, 5 + i);
		cout << '|';
		gotoxy(21, 5 + i);
		cout << '|';

	}

	//Enclosing the limits
	for (int j = 0; j < col; j++)
	{
		gotoxy(11 + j, 20);
		cout << '-';
	}

	//print theinformation and the specification.
	printBoard(board);
	specification();
}

void Board::printBoard(char ** boardGame)
{
	//The updated information is displayed on every iteration.
	hideCursor();
	gotoxy(0, 0);

	cout << "1 - Start Game        | 2 - Pause        | 3 - Speed Up        | 4 - Speed Down        | 9-Exit  " << endl;
	cout << "Speed: " << speedLevel << "              |Score: " << score << endl;
	cout <<"Shapes Down: " << countShapes << endl;
	 
	cout << endl << endl;

	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
		{
			gotoxy(11 + j, 5 + i);
			cout << boardGame[i][j];
		}
}

void Board::specification()
{
	gotoxy(30, 7);
	cout << "specification:";
	gotoxy(30, 8);
	cout << "Press 'D' to move left.";
	gotoxy(30, 9);
	cout << "Press 'G' to move right.";
	gotoxy(30, 10);
	cout << "Press 'F to move right.";
	gotoxy(30, 11);
	cout << "Press 'R' to rotate.";
	gotoxy(30, 12);
	cout << "Press 'E' to move to the bottom.";
	gotoxy(30, 13);
	cout << "Press 'S' to stop the Joker.";
	
}

void Board::gameOver(bool &flage)
{
	gotoxy(11, 10);
	cout << "GAME OVER!";
	gotoxy(0, 25);
	flage = false;
}
