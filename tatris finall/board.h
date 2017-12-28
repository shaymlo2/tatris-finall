#pragma once
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Point.h"
#include "Shapes.h"

using namespace std;

constexpr int START_POINT_X = 11;
constexpr int START_POINT_Y = 5;

constexpr int LEFT_LIMIT = 11;
constexpr int RIGHT_LIMIT = 21;
constexpr int BOTTOM_LIMIT = 19;

constexpr char SPACE = ' ';

class Board {

	int col;
	int row;
	char **boardGame;
	Shapes* shape;
	Shapes::Type typeShape;
	int speed = 500;
	int speedLevel = 0;
	int countShapes = 0;
	int score = 0;
	bool endGame = false;
	bool isInLayer = false;

public:

	Board();
	~Board();

	void menu(char keyPressed);// the menu foe the game
	void specification();
	void gameOver(bool &flage);
	void run();// the main function of the game.

	//momve nd update functions
	void drawBoard(char** boardGame); // prints all the instrodactions, the menu and the board
	void printBoard(char ** boardGame);//prints only the new data in the board
	void updateBoardGame(char** boardGame, Shapes * shape, char ch);
	void moveAndUpdate(char** boardGame, Shapes * shape, char keyPressed);
	void moveAndUpdateAUX(char** boardGame, Shapes * shape, char keyPressed);

	//Handles deletes rows.
	void checkDeletRow(char ** boardGame);
	void deletRow(char ** boardGame, int d_row);
	void updateTheScore(char ** boardGame, int numOfRowsDelete);

	//functions that dynamically update the behavior of the boomb.
	void deleteByBoom(char ** boardGame, Shapes * shape);
	void updateAfterBoomb(char ** boardGame, int y, int x);

	//check functions for all the shapes and the limits.
	bool check(char** boardGame, Point * body, Shapes::Type t, char keyPressed);
	bool checkSquare(char** boardGame, Point * p, char keyPressed, int steps = 0);
	bool checkJoker(char** boardGame, Point * p, char keyPressed);
	bool checkBoomb(char** boardGame, Point * p, char keyPressed, int steps = 0);
	bool checkLine(char** boardGame, Point * p, char keyPressed, int steps = 0);
	bool checkIndexLR(char** boardGame, int set_Y, int set_X);
	bool checkIndexD(char** boardGame, int set_Y, int set_X);

	//Auxiliary functions for fast drop
	void quick(char **boardGame, Shapes *shape);
	int quickjoker(char **boardGame, Point * p);
	int howManyToMove(char **boardGame, Shapes *shape);
	bool isEmpty(char ** boardGame);
	bool isBounded(int y,int x);

};