#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

//Functions
int		CheckInput(int);
bool	CheckWin(char**, bool&, bool&);
void	GridCreation(char**);
void	GridDeletion(char**);
void	DisplayGrid();
void	GameOptions();
void	AITurn();
void	PlayerTurn();
void	DropPiece(char, char**, bool*, bool*);
void	RemovePiece(char**, bool*, bool*);

//Variables
char	newPiece;
char**	grid;
int		gridLength;
int		gridWidth;
int		targetCombo;
int		inputI;
int		curRow;
int		curCol;
bool	gameRunning;
bool	gamePlaying;
bool	badInput;
bool	aiGame;
bool	wrapBoard;
bool	removePieces;
bool	player1Turn;
bool	stillPlaying;
bool	player1Wins;
bool	player2Wins;
bool*	colFull;
bool*	colEmpty;

int main()
{
	//Game loop
	do
	{
		//Main Menu
		do
		{
			cout << "Welcome to Connect Four\nEnter \"1\" to start the game\nEnter \"0\" to quit" << endl;

			cin >> inputI;
			inputI = CheckInput(inputI);

			if (inputI == 1)
			{
				gameRunning = true;
				gamePlaying = true;
				badInput = false;
			}
			else if (inputI == 0)
			{
				gameRunning = false;
				gamePlaying = false;
				badInput = false;
			}
			else
			{
				cout << "Input error! Please try again.\n" << endl;
				badInput = true;
			}
		} while (badInput);

		//Game Start
		while (gamePlaying)
		{
			//Options
			GameOptions();

			//Initializations
			GridCreation(grid);
			player1Turn = false;
			stillPlaying = true;

			while (stillPlaying)
			{
				DisplayGrid();
				player1Turn = !player1Turn;

				//Set new piece
				if (player1Turn)
				{	newPiece = 'X';	}
				else
				{	newPiece = 'O';	}

				if (aiGame)
				{	AITurn();	}
				else
				{	PlayerTurn();	}
			}

			DisplayGrid();
			if (player1Wins && player2Wins)
			{	cout << "Tie game! Nobody wins!";	}
			else if (player1Wins)
			{	cout << "Player 1 wins!";	}
			else if (player2Wins)
			{	cout << "Player 2 wins!";	}
		}
	} while (gameRunning);
}

int CheckInput(int input)
{
	if (!cin)
	{
		cin.clear();
		cin.ignore();
		return -1;
	}
	else
	{	return input;	}
}

bool CheckWin(char** theGrid, bool& p1Win, bool& p2Win)
{
	char targetChar = theGrid[curRow][curCol];
	int count = 0;

	for (int i = 0; i < 4; i += 1)
	{
		switch (i)
		{
		case 0:
			//Horizontal Check
			if (wrapBoard)
			{
				for (int j = 0; true; j += 1)
				{
					if (theGrid[curRow][(curCol + j) % gridWidth] == targetChar)
					{	count += 1;	}
					else
					{	break;	}
				}
				for (int j = 0; true; j += 1)
				{
					if (theGrid[curRow][(curCol - j) % gridWidth] == targetChar)
					{	count += 1;	}
					else
					{	break;	}
				}
			}
			else
			{
				for (int j = 0; j < gridWidth - curCol; j += 1)
				{
					if (theGrid[curRow][curCol + j] == targetChar)
					{	count += 1;	}
					else
					{	break;	}
				}

				for (int j = 0; j < curCol; j += 1)
				{
					if (theGrid[curRow][curCol - j] == targetChar)
					{	count += 1;	}
					else
					{	break;	}
				}
			}
			break;
		case 1:
			//Vertical Check
			for (int j = 0; j < gridLength - curRow; j += 1)
			{
				if (theGrid[curRow + j][curCol] == targetChar)
				{	count += 1;	}
				else
				{	break;	}
			}
			break;
		case 2:
			//Horizontal Check: top left to bottom right
			if (wrapBoard)
			{
				for (int j = 0; true; j += 1)
				{

				}
			}
			else
			{
				for (int j = 0; (j < gridWidth - curCol && j < gridLength - curRow); j += 1)
				{
					if (theGrid[curRow + j][curCol + j] == targetChar)
					{	count += 1;	}
					else
					{	break;	}
				}

				for (int j = 0; (j < curCol && j < curRow - 1) == targetChar; j += 1)
				{
					if (theGrid[curRow - j][curCol - j])
					{	count += 1;	}
					else
					{	break;	}
				}
			}
			break;
		case 3:
			//Horizontal Check: bottom left to top right
			for (int j = 0; (j < gridWidth - curCol && j < curRow - 1); j += 1)
			{
				if (theGrid[curRow - j][curCol + j] == targetChar)
				{	count += 1;	}
				else
				{	break;	}
			}

			for (int j = 0; (j < curCol && j < gridLength - curRow); j += 1)
			{
				if (theGrid[curRow + j][curCol - j] == targetChar)
				{	count += 1;	}
				else
				{	break;	}
			}
			break;
		}
				if (count >= targetCombo - 1)
				{	return true;	}
	}

	return false;
}

void GridCreation(char** theGrid)
{
	theGrid = new char*[gridLength + 1];

	for (int i = 0; i < gridLength; i += 1)
	{	theGrid[i] = new char[gridWidth];	}

	colFull = new bool[gridWidth];
	colEmpty = new bool[gridWidth];

	for (int i = 0; i < gridLength - 1; i += 1)
	{
		for (int j = 0; j < gridWidth - 1; j += 1)
		{
			if (i = 0)
			{
				theGrid[i][j] = (char)(j + 1);
				colFull[j] = false;
				colEmpty[j] = true;
			}
			else
			{	theGrid[i][j] = '.';	}
		}
	}
}

void GridDeletion(char** theGrid)
{
	for (int i = 0; i < gridLength; i += 1)
	{	delete[] theGrid[i];	}

	delete[] theGrid;
}

void DisplayGrid()
{
	for (int i = 0; i < gridLength - 1; i += 1)
	{
		for (int j = 0; j < gridWidth - 1; j += 1)
		{	cout << grid[i][j] << " ";	}

		cout << endl;
	}
}

void GameOptions()
{
	do
	{
		cout << "Who do you want to be your opponent?\nEnter \"1\" for an AI\nEnter\"2\" for another human";

		cin >> inputI;
		inputI = CheckInput(inputI);

		if (inputI == 1)
		{
			aiGame = true;
			badInput = false;
		}
		else if (inputI == 2)
		{
			aiGame = false;
			badInput = false;
		}
		else
		{
			cout << "Input error! Please try again.\n" << endl;
			badInput = true;
		}
	} while (badInput);

	do
	{
		cout << "Do you want the board to wrap around horizontally?\nEnter \"1\" for an Yes\nEnter\"2\" for No";

		cin >> inputI;
		inputI = CheckInput(inputI);

		if (inputI == 1)
		{
			wrapBoard = true;
			badInput = false;
		}
		else if (inputI == 2)
		{
			wrapBoard = false;
			badInput = false;
		}
		else
		{
			cout << "Input error! Please try again.\n" << endl;
			badInput = true;
		}
	} while (badInput);

	do
	{
		cout << "Do you want the option each turn to remove a piece from the bottom of a column instead of dropping?\nEnter \"1\" for an Yes\nEnter\"2\" for No";

		cin >> inputI;
		inputI = CheckInput(inputI);

		if (inputI == 1)
		{
			removePieces = true;
			badInput = false;
		}
		else if (inputI == 2)
		{
			removePieces = false;
			badInput = false;
		}
		else
		{
			cout << "Input error! Please try again.\n" << endl;
			badInput = true;
		}
	} while (badInput);

	do
	{
		cout << "How many rows would you like the game to have? [3 to 10]" << endl;

		cin >> inputI;
		inputI = CheckInput(inputI);

		if (inputI >= 3 && inputI <= 10)
		{
			gridLength = inputI;
			badInput = false;
		}
		else
		{
			cout << "Input error! Please try again.\n" << endl;
			badInput = true;
		}
	} while (badInput);

	do
	{
		cout << "How many columns would you like the game to have? [3 to 10]" << endl;

		cin >> inputI;
		inputI = CheckInput(inputI);

		if (inputI >= 3 && inputI <= 10)
		{
			gridWidth = inputI;
			badInput = false;
		}
		else
		{
			cout << "Input error! Please try again.\n" << endl;
			badInput = true;
		}
	} while (badInput);

	do
	{
		cout << "How many pieces in a row do you need to win? [3";
		if (gridLength != 3 && gridWidth != 3)
		{
			cout << " to ";

			if (gridLength < gridWidth)
			{	cout << gridLength;	}
			else
			{	cout << gridWidth;	}
		}
		else
		{	cout << " only";	}
		cout << "]" << endl;

		cin >> inputI;
		inputI = CheckInput(inputI);

		if (inputI >= 3 && inputI <= gridLength && inputI <= gridWidth)
		{
			targetCombo = inputI;
			badInput = false;
		}
		else
		{
			cout << "Input error! Please try again.\n" << endl;
			badInput = true;
		}
	} while (badInput);
}

void AITurn()
{

}

void PlayerTurn()
{
	do
	{
		string output = (string)"Player " + newPiece + "\'s turn! Please choose a column in which to drop an \"" + newPiece + "\" [1 to" + to_string(gridWidth) + "]";
		cout << output << endl;
		if (removePieces)
		{	output = (string)"Or, choose a column from witch to remove the bottom piece. [-1 to " + to_string(gridWidth) + "]";	}
		cout << output << "Enter \"0\" to display the board again" << endl;

		cin >> inputI;
		inputI = CheckInput(inputI);

		if (inputI >= 3 && inputI <= gridWidth)
		{
			if (!colFull[inputI])
			{
				curCol = inputI - 1;
				badInput = false;
			}
			else
			{
				cout << "Column is full! Please try again.\n" << endl;
				badInput = true;
			}
		}
		else if (removePieces && inputI >= -gridWidth && inputI <= -3)
		{
			if (!colEmpty[inputI])
			{
				curCol = inputI - 1;
				badInput = false;
			}
			else
			{
				cout << "Column is empty! Please try again. \n" << endl;
				badInput = true;
			}
		}
		else
		{
			cout << "Input error! Please try again.\n" << endl;
			badInput = true;
		}
	} while (badInput);

	if (inputI > 0)
	{	DropPiece(newPiece, grid, colFull, colEmpty);	}
	else if (inputI < 0)
	{	RemovePiece(grid, colFull, colEmpty);	}
}

void DropPiece(char thePiece, char** theGrid, bool* fullCheck, bool* emptyCheck)
{
	for (int i = 1; i < gridLength; i += 1)
	{
		if (theGrid[i][curCol] != '.')
		{
			theGrid[i - 1][curCol] = thePiece;
			curRow = i - 1;
			if (i - 1 == 1)
			{	fullCheck[curCol] = true;	}
			stillPlaying = !CheckWin(grid, player1Wins, player2Wins);
			return;
		}
	}

	theGrid[gridLength - 1][curCol] = thePiece;
	curRow = gridLength - 1;
	emptyCheck[curCol] = false;
}

void RemovePiece(char** theGrid, bool* fullCheck, bool* emptyCheck)
{
	for (int i = gridLength - 1; i > 0; i -= 1)
	{
		if (i != 1)
		{	theGrid[i][curCol] = theGrid[i - 1][curCol];	}
		else
		{	
			if (theGrid[i][curCol] != '.')
			{	fullCheck[curCol] = false;	}
			theGrid[i][curCol = '.'];
		}
	}
	for (int i = gridLength - 1; i > 0; i -= 1)
	{
		if (theGrid[i][curCol] != '.')
		{
			curRow = i;
			stillPlaying = (stillPlaying || CheckWin(grid, player1Wins, player2Wins));
		}
		else if (i == gridLength - 1)
		{	emptyCheck[curCol] = true;	}
	}
}