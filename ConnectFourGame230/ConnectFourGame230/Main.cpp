#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

//Functions
int		CheckInput(int);
bool	CheckWin(char**, bool&, bool&);
void	GridCreation(char**&, bool*&, bool*&);
void	GridDeletion(char**&, bool*&, bool*&);
void	GridCopy();
void	DisplayGrid();
void	GameOptions();
void	AITurn();
void	PlayerTurn();
void	DropPiece(char, char**, bool*, bool*);
void	UndoDrop(char**, bool*, bool*);
void	RemovePiece(char**, bool*, bool*);
void	UndoRemove(char, char**, bool*, bool*);

//Variables
char	p1Piece;
char	p2Piece;
char**	grid;
char**	aiGrid;
int		gridLength;
int		gridWidth;
int		targetCombo;
int		inputI;
int		curRow;
int		curCol;
bool	gameRunning;
bool	gamePlaying;
bool	newSettings;
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
bool*	aiColFull;
bool*	aiColEmpty;

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
				newSettings = true;
				badInput = false;
			}
			else if (inputI == 0)
			{
				gameRunning = false;
				gamePlaying = false;
				newSettings = false;
				badInput = false;
			}
			else
			{
				cout << "Input error! Please try again.\n" << endl;
				badInput = true;
			}
		} while (badInput);
		cout << endl;

		//Game Start
		while (gamePlaying)
		{
			if(newSettings)
			{
				//Options
				GameOptions();
			}

			//Initializations
			GridCreation(grid, colFull, colEmpty);
			if (aiGame)
			{
				GridCreation(aiGrid, aiColFull, aiColEmpty);
				GridCopy();
			}
			player1Turn = false;
			stillPlaying = true;
			player1Wins = false;
			player2Wins = false;

			p1Piece = 'X';
			p2Piece = 'O';

			while (stillPlaying)
			{
				DisplayGrid();
				player1Turn = !player1Turn;

				if (aiGame && !player1Turn)
				{	AITurn();	}
				else
				{	PlayerTurn();	}
			}

			DisplayGrid();
			if (player1Wins && player2Wins)
			{	cout << "Tie game! Nobody wins!\n" << endl;	}
			else if (player1Wins)
			{	cout << "Player 1 wins!\n" << endl;	}
			else if (player2Wins)
			{	cout << "Player 2 wins!\n" << endl;	}
			else
			{	cout << "Nobody won? There might have been an issue here..." << endl;	}

			GridDeletion(grid, colFull, colEmpty);
			if (aiGame)
			{	GridDeletion(aiGrid, aiColFull, aiColEmpty);	}


			do
			{
				cout << "Would you like to play again?\nEnter \"1\" to restart this game\nEnter \"2\" to create a new game\nEnter \"0\" to go to main menu" << endl;

				cin >> inputI;
				inputI = CheckInput(inputI);

				if (inputI == 1)
				{
					newSettings = false;
					badInput = false;
				}
				else if (inputI == 2)
				{
					newSettings = true;
					badInput = false;
				}
				else if (inputI == 0)
				{
					gamePlaying = false;
					badInput = false;
				}
				else
				{
					cout << "Input error! Please try again.\n" << endl;
					badInput = true;
				}
			} while (badInput);
		}
	} while (gameRunning);

	return 0;
}

int CheckInput(int input)
{
	if (!cin)
	{
		cin.clear();
		cin.ignore();
		return -11;
	}
	else
	{	return input;	}
}

bool CheckWin(char** theGrid, bool& p1Win, bool& p2Win)
{
	char targetChar = theGrid[curRow][curCol];
	int count;

	for (int i = 0; i < 4; i += 1)
	{
		count = 1;

		switch (i)
		{
		case 0:
			//Horizontal Check
			if (wrapBoard)
			{
				for (int j = 1; true; j += 1)
				{
					if (theGrid[curRow][(gridWidth + curCol + j) % gridWidth] == targetChar)
					{	count += 1;	}
					else
					{	break;	}
				}
				for (int j = 1; true; j += 1)
				{
					if (theGrid[curRow][(gridWidth + curCol - j) % gridWidth] == targetChar)
					{	count += 1;	}
					else
					{	break;	}
				}
			}
			else
			{
				for (int j = 1; j < gridWidth - curCol; j += 1)
				{
					if (theGrid[curRow][curCol + j] == targetChar)
					{	count += 1;	}
					else
					{	break;	}
				}

				for (int j = 1; j < curCol + 1; j += 1)
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
			for (int j = 1; j < gridLength - curRow + 1; j += 1)
			{
				if (theGrid[curRow + j][curCol] == targetChar)
				{	count += 1;	}
				else
				{	break;	}
			}
			break;
		case 2:
			//Diagonal Check: top left to bottom right
			if (wrapBoard)
			{
				for (int j = 1; j < gridLength - curRow + 1; j += 1)
				{
					if (theGrid[curRow + j][(gridWidth + curCol + j) % gridWidth] == targetChar)
					{	count += 1;	}
					else
					{	break;	}
				}

				for (int j = 1; j < curRow - 1; j += 1)
				{
					if (theGrid[curRow - j][(gridWidth + curCol - j) % gridWidth] == targetChar)
					{	count += 1;	}
					else
					{	break;	}
				}
			}
			else
			{
				for (int j = 1; (j < gridWidth - curCol && j < gridLength - curRow + 1); j += 1)
				{
					if (theGrid[curRow + j][curCol + j] == targetChar)
					{	count += 1;	}
					else
					{	break;	}
				}

				for (int j = 1; (j < curCol + 1 && j < curRow - 1); j += 1)
				{
					if (theGrid[curRow - j][curCol - j] == targetChar)
					{	count += 1;	}
					else
					{	break;	}
				}
			}
			break;
		case 3:
			//Diagonal Check: bottom left to top right
			if (wrapBoard)
			{
				for (int j = 1; (j < curRow - 1); j += 1)
				{
					if (theGrid[curRow - j][(gridWidth + curCol + j) % gridWidth] == targetChar)
					{	count += 1;	}
					else
					{	break;	}
				}
				
				for (int j = 1; (j < gridLength - curRow + 1); j += 1)
				{
					if (theGrid[curRow + j][(gridWidth + curCol - j) % gridWidth] == targetChar)
					{	count += 1;	}
					else
					{	break;	}
				}
			}
			else
			{
				for (int j = 1; (j < gridWidth - curCol && j < curRow - 1); j += 1)
				{
					if (theGrid[curRow - j][curCol + j] == targetChar)
					{	count += 1;	}
					else
					{	break;	}
				}

				for (int j = 1; (j < curCol + 1 && j < gridLength - curRow + 1); j += 1)
				{
					if (theGrid[curRow + j][curCol - j] == targetChar)
					{	count += 1;	}
					else
					{	break;	}
				}
			}
			break;
		}

		if (count >= targetCombo)
		{
			if (targetChar == p1Piece) { p1Win = true; }
			else if (targetChar == p2Piece) { p2Win = true; }
			return true;
		}
	}

	return false;
}

void GridCreation(char** &theGrid, bool* &colCheckF, bool* &colCheckE)
{
	theGrid = new char*[gridLength + 1];

	for (int i = 0; i <= gridLength; i += 1)
	{	theGrid[i] = new char[gridWidth];	}

	colCheckF = new bool[gridWidth];
	colCheckE = new bool[gridWidth];

	for (int i = 0; i <= gridLength; i += 1)
	{
		for (int j = 0; j < gridWidth; j += 1)
		{
			if (i == 0)
			{
				theGrid[i][j] = '0'+(j + 1);
				colCheckF[j] = false;
				colCheckE[j] = true;
			}
			else
			{	theGrid[i][j] = '.';	}
		}
	}
}

void GridDeletion(char** &theGrid, bool* &colCheckF, bool* &colCheckE)
{
	for (int i = 0; i <= gridLength; i += 1)
	{
		delete[] theGrid[i];
		theGrid[i] = NULL;
	}

	delete[] theGrid;
	theGrid = NULL;
	delete[] colCheckF;
	colCheckF = NULL;
	delete[] colCheckE;
	colCheckE = NULL;
}

void GridCopy()
{
	for (int i = 0; i <= gridLength; i += 1)
	{
		for (int j = 0; j < gridWidth; j += 1)
		{
			aiGrid[i][j] = grid[i][j];
			
			if (i == 0)
			{
				aiColFull[j] = colFull[j];
				aiColEmpty[j] = colEmpty[j];
			}
		}
	}
}

void DisplayGrid()
{
	for (int i = 0; i <= gridLength; i += 1)
	{
		for (int j = 0; j < gridWidth; j += 1)
		{	cout << grid[i][j] << " ";	}

		cout << endl;
	}
	cout << endl;
}

void GameOptions()
{
	do
	{
		cout << "Who do you want to be your opponent?\nEnter \"1\" for an AI\nEnter \"2\" for another human" << endl;

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
	cout << endl;

	do
	{
		cout << "Do you want the board to wrap around horizontally?\nEnter \"1\" for an Yes\nEnter \"2\" for No" << endl;

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
	cout << endl;

	do
	{
		cout << "Do you want the option each turn to remove a piece from the bottom of a column\ninstead of dropping a piece?\nEnter \"1\" for an Yes\nEnter \"2\" for No" << endl;

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
	cout << endl;

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
	cout << endl;

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
	cout << endl;

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
	cout << endl;
}

void AITurn()
{
	cout << "It's the AI's turn! The AI will decide where to drop an \"O\"" << (removePieces ? " or remove a piece." : ".") << endl;

	GridCopy();
	bool instantWin = false;
	bool lossInevitable = false;

	/* Priority Index
	 *-1 = Danger! Needs Checking
	 * 0 = Unsafe
	 * 1 = Last Resort
	 * 2 = Safe
	 * 3 = Block P1 Removal Win
	 * 4 = Block P1 Drop Win
	 * 5 = Block P1 Both Win
	 * 6 = Instant Win
	 */
	int* priorityRemCol = new int[gridWidth];
	for (int i = 0; i < gridWidth; i += 1)
	{	priorityRemCol[i] = 0;	}

	int* priorityDropCol = new int[gridWidth];
	for (int i = 0; i < gridWidth; i += 1)
	{	priorityDropCol[i] = 0;	}

	if (removePieces)
	{
		for (int i = 0; (i < gridWidth && !instantWin); i += 1)
		{
			if (!aiColEmpty[i])
			{
				curCol = i;
				char savePiece = aiGrid[gridLength][curCol];
				RemovePiece(aiGrid, aiColFull, aiColEmpty);
				UndoRemove(savePiece, aiGrid, aiColFull, aiColEmpty);

				if (!stillPlaying)
				{
					if(player2Wins && !player1Wins)
					{
						instantWin = true;
						priorityRemCol[curCol] = 6;
					}
					else if (player2Wins && player1Wins)
					{	priorityRemCol[curCol] = 1;	}
					else if (!player2Wins && player1Wins)
					{	priorityRemCol[curCol] = -1;	}

					stillPlaying = true;
					player1Wins = false;
					player2Wins = false;
				}
				else
				{	priorityRemCol[curCol] = 2;	}
			}
			else
			{	priorityRemCol[i] = 0;	}
		}

		for (int i = 0; i < gridWidth; i += 1)
		{
			if (!instantWin && !lossInevitable && priorityRemCol[i] == -1)
			{
				bool blockFound = false;

				for (int j = 0; j < gridWidth; j += 1)
				{
					if (priorityRemCol[j] == 2 || priorityRemCol[j] == 3)
					{
						curCol = j;
						char savePiece1 = aiGrid[gridLength][curCol];
						RemovePiece(aiGrid, aiColFull, aiColEmpty);

						if (!aiColEmpty[i])
						{
							curCol = i;
							char savePiece2 = aiGrid[gridLength][curCol];
							RemovePiece(aiGrid, aiColFull, aiColEmpty);
							UndoRemove(savePiece2, aiGrid, aiColFull, aiColEmpty);
						}

						curCol = j;
						UndoRemove(savePiece1, aiGrid, aiColFull, aiColEmpty);

						if (!stillPlaying)
						{
							if (!player2Wins && player1Wins)
							{	priorityRemCol[curCol] = 0;	}

							stillPlaying = true;
							player1Wins = false;
							player2Wins = false;
						}
						else
						{
							priorityRemCol[curCol] = 3;
							blockFound = true;
						}
					}
				}

				if (!blockFound)
				{
					for (int j = 0; j < gridWidth; j += 1)
					{
						if (!aiColFull[j])
						{
							curCol = j;
							DropPiece(p2Piece, aiGrid, aiColFull, aiColEmpty);

							curCol = i;
							char savePiece1 = aiGrid[gridLength][curCol];
							RemovePiece(aiGrid, aiColFull, aiColEmpty);
							UndoRemove(savePiece1, aiGrid, aiColFull, aiColEmpty);

							curCol = j;
							UndoDrop(aiGrid, aiColFull, aiColEmpty);

							if (!stillPlaying)
							{
								if (player2Wins && player1Wins)
								{	priorityDropCol[j] = 1;	}

								stillPlaying = true;
								player1Wins = false;
								player2Wins = false;
							}
						}
					}

					lossInevitable = true;	
				}

				priorityRemCol[i] = 0;
			}
			else if (instantWin && priorityRemCol[i] == 6)
			{
				curCol = i;
				cout << "The AI is removing a piece from column " << curCol << "." << endl << endl;
				RemovePiece(grid, colFull, colEmpty);

				delete[] priorityRemCol;
				priorityRemCol = NULL;

				delete[] priorityDropCol;
				priorityDropCol = NULL;
				return;
			}
		}
	}

	for (int i = 0; (i < gridWidth && !instantWin); i += 1)
	{
		if (!aiColFull[i])
		{
			curCol = i;
			DropPiece(p2Piece, aiGrid, aiColFull, aiColEmpty);
			UndoDrop(aiGrid, aiColFull, aiColEmpty);

			if (!stillPlaying)
			{
				instantWin = true;
				priorityDropCol[curCol] = 6;

				stillPlaying = true;
				player1Wins = false;
				player2Wins = false;
			}
			else
			{
				DropPiece(p1Piece, aiGrid, aiColFull, aiColEmpty);
				UndoDrop(aiGrid, aiColFull, aiColEmpty);

				if (!stillPlaying)
				{
					priorityDropCol[curCol] = -1;

					stillPlaying = true;
					player1Wins = false;
					player2Wins = false;
				}
				else if(priorityDropCol[curCol] != 1)
				{	priorityDropCol[curCol] = 2;	}
			}
		}
		else
		{	priorityDropCol[i] = 0;	}
	}

	for (int i = 0; (i < gridWidth && !instantWin); i += 1)
	{
		if (priorityDropCol[i] == -1)
		{
			curCol = i;
			DropPiece(p2Piece, aiGrid, aiColFull, aiColEmpty);

			bool lossCheck = false;

			for (int j = 0; j < gridWidth; j += 1)
			{
				if (!aiColFull[j])
				{
					curCol = j;
					DropPiece(p1Piece, aiGrid, aiColFull, aiColEmpty);
					UndoDrop(aiGrid, aiColFull, aiColEmpty);

					if (!stillPlaying)
					{
						lossCheck = true;

						stillPlaying = true;
						player1Wins = false;
						player2Wins = false;
					}
				}
			}

			curCol = i;
			UndoDrop(aiGrid, aiColFull, aiColEmpty);

			if (!lossCheck)
			{	priorityDropCol[curCol] = 4;	}
			else
			{
				for (int j = 0; j < gridWidth; j += 1)
				{
					if (priorityRemCol[j] > 1)
					{
						curCol = j;
						char savePiece1 = aiGrid[gridLength][curCol];
						RemovePiece(aiGrid, aiColFull, aiColEmpty);

						curCol = i;
						DropPiece(p1Piece, aiGrid, aiColFull, aiColEmpty);
						UndoDrop(aiGrid, aiColFull, aiColEmpty);

						curCol = j;
						UndoRemove(savePiece1, aiGrid, aiColFull, aiColEmpty);

						if (!stillPlaying)
						{
							stillPlaying = true;
							player1Wins = false;
							player2Wins = false;
						}
						else
						{
							lossCheck = false;

							// priority 2 goes to 4
							// priority 3 goes to 5
							if (priorityRemCol[j] != 6)
							{	priorityRemCol[j] += 2;	}
						}
					}
				}

				if (lossCheck)
				{	lossInevitable = true;	}

				priorityDropCol[i] = 2;
			}
		}
	}

	int highestPriority = 0;
	int highestCount = 0;

	for (int i = 0; i < gridWidth; i += 1)
	{
		if ((instantWin && priorityDropCol[i] == 6) || (lossInevitable && priorityDropCol[i] == 1))
		{
			curCol = i;
			cout << "The AI is dropping a piece in column " << curCol << "." << endl << endl;
			DropPiece(p2Piece, grid, colFull, colEmpty);

			delete[] priorityRemCol;
			priorityRemCol = NULL;

			delete[] priorityDropCol;
			priorityDropCol = NULL;
			return;
		}
		else if (lossInevitable && priorityRemCol[i] == 1)
		{
			curCol = i;
			cout << "The AI is removing a piece from column " << curCol << "." << endl << endl;
			RemovePiece(grid, colFull, colEmpty);

			delete[] priorityRemCol;
			priorityRemCol = NULL;

			delete[] priorityDropCol;
			priorityDropCol = NULL;
			return;
		}

		if (priorityDropCol[i] > highestPriority)
		{
			highestPriority = priorityDropCol[i];
			highestCount = 1;
		}
		else if (priorityDropCol[i] == highestPriority)
		{	highestCount += 1;	}

		if (priorityRemCol[i] > highestPriority)
		{
			highestPriority = priorityRemCol[i];
			highestCount = 1;
		}
		else if (priorityRemCol[i] == highestPriority)
		{	highestCount += 1;	}
	}

	int chosenMove = rand() % highestCount;
	highestCount = 0;

	for (int i = 0; i < gridWidth; i += 1)
	{
		if (priorityDropCol[i] == highestPriority)
		{
			if (highestCount == chosenMove)
			{
				curCol = i;
				cout << "The AI is dropping a piece in column " << curCol << "." << endl << endl;
				DropPiece(p2Piece, grid, colFull, colEmpty);

				delete[] priorityRemCol;
				priorityRemCol = NULL;

				delete[] priorityDropCol;
				priorityDropCol = NULL;
				return;
			}
			else
			{	highestCount += 1;	}
		}
		if (priorityRemCol[i] == highestPriority)
		{
			if (highestCount == chosenMove)
			{
				curCol = i;
				cout << "The AI is removing a piece from column " << curCol << "." << endl << endl;
				RemovePiece(grid, colFull, colEmpty);

				delete[] priorityRemCol;
				priorityRemCol = NULL;

				delete[] priorityDropCol;
				priorityDropCol = NULL;
				return;
			}
			else
			{	highestCount += 1;	}
		}
	}

	delete[] priorityRemCol;
	priorityRemCol = NULL;

	delete[] priorityDropCol;
	priorityDropCol = NULL;
}

void PlayerTurn()
{
	char curPiece = (player1Turn ? p1Piece : p2Piece);
	do
	{
		string output = (string)"Player " + curPiece + "\'s turn! Please choose a column in which to drop an \"" + curPiece + "\" [1 to " + to_string(gridWidth) + "]";
		cout << output << endl;
		if (removePieces)
		{	output = (string)"Or, choose a column from which to remove the bottom piece. [-1 to -" + to_string(gridWidth) + "]";	}
		cout << output << endl << "Enter \"0\" to display the board again" << endl;

		cin >> inputI;
		inputI = CheckInput(inputI);

		if (inputI == 0)
		{
			DisplayGrid();
			badInput = true;
		}
		else if (inputI >= 1 && inputI <= gridWidth)
		{
			if (!colFull[inputI-1])
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
		else if (removePieces && inputI >= -gridWidth && inputI <= -1)
		{
			inputI = -inputI;
			if (!colEmpty[inputI-1])
			{
				curCol = inputI - 1;
				inputI = -inputI;
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
	{	DropPiece(curPiece, grid, colFull, colEmpty);	}
	else if (inputI < 0)
	{	RemovePiece(grid, colFull, colEmpty);	}
}

void DropPiece(char thePiece, char** theGrid, bool* fullCheck, bool* emptyCheck)
{
	for (int i = 1; i <= gridLength; i += 1)
	{
		if (theGrid[i][curCol] != '.')
		{
			theGrid[i - 1][curCol] = thePiece;
			curRow = i - 1;
			if (curRow == 1)
			{	fullCheck[curCol] = true;	}
			stillPlaying = !CheckWin(theGrid, player1Wins, player2Wins);
			return;
		}
	}

	theGrid[gridLength][curCol] = thePiece;
	curRow = gridLength;
	emptyCheck[curCol] = false;
	stillPlaying = !CheckWin(theGrid, player1Wins, player2Wins);
}

void UndoDrop(char** theGrid, bool* fullCheck, bool* emptyCheck) {
	theGrid[curRow][curCol] = '.';
	if (curRow == 1) {	fullCheck[curCol] = false;	}
	else if (curRow == gridLength) { emptyCheck[curCol] = true;	}
}

void RemovePiece(char** theGrid, bool* fullCheck, bool* emptyCheck)
{
	for (int i = gridLength; i > 1; i -= 1)
	{	theGrid[i][curCol] = theGrid[i - 1][curCol];	}

	if (theGrid[1][curCol] != '.')
	{
		fullCheck[curCol] = false;
		theGrid[1][curCol] = '.';
	}

	for (int i = gridLength; i > 0; i -= 1)
	{
		if (theGrid[i][curCol] != '.')
		{
			curRow = i;
			stillPlaying = (stillPlaying && !CheckWin(grid, player1Wins, player2Wins));
		}
		else if (i == gridLength)
		{
			emptyCheck[curCol] = true;
			return;
		}
	}
}

void UndoRemove(char thePiece, char** theGrid, bool* fullCheck, bool* emptyCheck)
{
	if (emptyCheck[curCol] == true)
	{	emptyCheck[curCol] = false;	}
	else
	{
		for (int i = 1; i < gridLength; i += 1)
		{	theGrid[i][curCol] = theGrid[i + 1][curCol];	}
	}

	if(theGrid[1][curCol] != '.')
	{	fullCheck[curCol] = true;	}

	theGrid[gridLength][curCol] = thePiece;
}