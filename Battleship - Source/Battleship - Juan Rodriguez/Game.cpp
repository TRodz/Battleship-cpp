//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2005 - 2015 Media Design School
// 
// File name   : Game.cpp
// Description : This class contains the code for the actual game
// Author      : Juan Rodriguez
// Mail        : juan.rod6618@mediadesignschool.com
// 

// This condition will make certain code only run if _DEBUG is defined (when in Debug build)
#ifdef _DEBUG
#define DEBUG_IF(cond) if(cond)
#else
#define DEBUG_IF(cond) if(false)
#endif

// Library includes
#include <iostream>
#include <string>
#include <ctime>
#include <windows.h>

// This include
#include "Game.h"

// Wrapping std::functions
using std::string;
using std::cout;
using std::cin;

// Initializing static variables
const int    Game::MAX_SHIPS = 5;
const string Game::SHIP_NAME[5] = {
	"Aircraft Carrier ",
	"Battleship       ",
	"Destroyer        ",
	"Submarine        ",
	"Patrol Boat      "
};

/// Constructor
Game::Game() {
	srand((unsigned int)time(NULL));
	/// For determining which space has been taken in order to place ships
	spaceTaken = false;
	/// Player ships
	currentShipsPlaced[0] = 0;
	/// Computer ships
	currentShipsPlaced[1] = 0;
	/// Hit/miss counters
	// Player
	hitCounter[0] = 0;
	// Computer
	hitCounter[1] = 0;
}

/// Destructor
Game::~Game() {
	delete[] currentPlayerShip;
	delete[] currentComputerShip;
	currentPlayerShip = 0;
	currentComputerShip = 0;
}

/// Game start ///
void Game::setCollisionGrid() {
	for (int i = 0; i < 12; ++i) {
		for (int j = 0; j < 12; ++j) {
			playerGrid[i][j] = 0;
			playerGrid[11][j] = 1;
			playerGrid[i][11] = 1;
		}
	}
	for (int i = 0; i < 12; ++i) {
		for (int j = 0; j < 12; ++j) {
			computerGrid[i][j] = 0;
			computerGrid[11][j] = 1;
			computerGrid[i][11] = 1;
		}
	}
}

void Game::setAvailableShips() {
	currentPlayerShip = new int[5];
	currentComputerShip = new int[5];
	for (int i = 0; i < 5; ++i) {
		currentPlayerShip[i] = 0;
		currentComputerShip[i] = 0;
	}
}

void Game::setState(string sortValue) {
	cancelSort = sortValue;
}

string Game::getState() const {
	return cancelSort;
}

int Game::getPlacedShips(int i) const {
	return MAX_SHIPS - currentShipsPlaced[i];
}

bool Game::canPlaceShips(int i) const {
	return (getPlacedShips(i) > 0);
}

int Game::getShipSize(int shipCode) const {
	if (shipCode == 0)
		return 5;
	else if (shipCode == 1)
		return 4;
	else if (shipCode == 2)
		return 3;
	else if (shipCode == 3)
		return 3;
	else if (shipCode == 4)
		return 2;
	else
		return 0;
}

char Game::getShipCode(int shipCode) const {
	if (shipCode == 0)
		return '@';
	else if (shipCode == 1)
		return '#';
	else if (shipCode == 2)
		return '&';
	else if (shipCode == 3)
		return '*';
	else if (shipCode == 4)
		return '$';
	else
		return '\0';
}

/**************************************/
/*                                    */
/*                MENU                */
/*                                    */
/**************************************/

void Game::drawMenuScreen() {
	clearInput(0);
	int x = 17;
	int y = 0;
	print(x, y, GREEN, "___  ____ ___ ___ _    ____ ____ _  _ _ ___ ");
	print(x, ++y, GREEN, "|__] |__|  |   |  |    |___ [__  |__| | |__]");
	print(x, ++y, GREEN, "|__] |  |  |   |  |___ |___ ___] |  | | |   ");
	y = 10;
	print(x, y, "1) Play          2) Credits          3) Exit");
	print(x, ++y, "Type your option and press ");
	print(GREEN, "RETURN");
	print(x, ++y, "Option");
	print(GREEN, "> ");
	y = 16;
	DEBUG_IF(1) {
		print(0, 24, "Debug mode (Can see computer's movements)");
	}
}

int Game::setMenu() {
	string inputOption;
	menuOption = stringToInt(inputOption, "Please provide a correct choice", btw, 1, 3, 25, 12, 8, 1);;
	return menuOption;
}

/**************************************/
/*                                    */
/*                SETUP               */
/*                                    */
/**************************************/

/// Draw setup screen - LEFT GRID
void Game::drawSetupScreen() {
	clearInput(0);
	char letter = 'A';
	// Player
	print(7, 0, YELLOW, "Player");
	setColor(TEAL);
	for (int i = 0; i <= 10; ++i) {
		for (int j = 0; j <= 10; ++j) {
			// Drawing the horizontal row numbers
			if ((i > 0 && i <= 10) && j == 0) {
				print(i * 2 - 1, j + 1, letter);
				letter++;
			}
			// Drawing the vertical row numbers
			if ((j > 0 && j <= 10) && i == 0) {
				print(i, j + 1, j - 1);
			}
		}
	}

	// Computer
	DEBUG_IF(1) {
		letter = 'A';
		print(6, 13, YELLOW, "Opponent");
		setColor(TEAL);
		for (int i = 0; i <= 10; ++i) {
			for (int j = 0; j <= 10; ++j) {
				if ((i > 0 && i <= 10) && j == 0) {
					print(i * 2 - 1, j + 14, letter);
					letter++;
				}
				if ((j > 0 && j <= 10) && i == 0) {
					print(i, j + 14, j - 1);
				}
			}
		}
	}

	// Water
	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 10; ++j) {
			print(i + 1, j + 2, DARKTEAL, "~");
			DEBUG_IF(1) {
				print(i + 1, j + 15, DARKTEAL, "~");
			}
		}
	}
}

/// Deciding the setup mode
int Game::setSetupMode() {

	// Printing instructions
	int x = 44;
	int y = 0;

	print(x, y, GREEN, "INSTRUCTIONS:");
	print(x, ++y, "You must set up 5 ships. They have");
	print(x, ++y, "varying lenghts, so plan properly.");
	print(x, ++y, "You must choose your setup method.");
	print(x, y += 2, GREEN, "FOR MANUAL SETUP:");
	print(x, ++y, "You'll be asked for a position to");
	print(x, ++y, "place your ship as follows:");
	print(x, ++y, "Rows      (A - J)");
	print(x, ++y, "Columns   (0 - 9)");
	print(x, ++y, "Direction (Vertical - Horizontal)");
	print(x, y += 2, GREEN, "IN GAME:");
	print(x, ++y, "Destroy your opponent's ships.");
	print(x, ++y, "Guess a position e.g. A0, B7, D4");
	print(x, ++y, "If there's a ship, it will hit it");
	print(x, ++y, "marking the ");
	print(RED, "spot ");
	print(WHITE, "where it was hit.");
	print(x, y += 2, "The opponent can hit you as well.");
	print(x, y += 2, GREEN, "SELECT SETUP METHOD (ESC to exit)");
	print(x, ++y, GREEN, "1)");
	print(WHITE, " Automatic - ");
	print(GREEN, "2)");
	print(WHITE, " Manual");
	print(x, ++y, "Type your option and press ");
	print(GREEN, "RETURN");
	print(x, ++y, "Option");
	print(GREEN, "> ");
	x = 52;

	// Reading our sort value (1: auto | 2: manual)
	sortValue = stringToInt(inputSort, "Please provide a correct option", eq, 1, 2, x, y, 8, 1);
	if (sortValue != -1) {
		return sortValue;
	}
	else {
		setState("NOTCANCELED");
		return 0;
	}
}

/// Get sort mode
int Game::getSetupMode() const {
	return sortValue;
}

/// Print available ships
void Game::printAvailableShips() {
	print(50, 0, "Available ships (ESC to exit)");
	for (int i = 0; i < 5; ++i) {
		if (currentPlayerShip[i] == 0) {
			print(56, i + 1, std::to_string(i + 1) + ") " + SHIP_NAME[i] + "(");
			print(GREEN, getShipCode(i));
			cout << ")";
		}
		else {
			print(56, i + 1, std::to_string(i + 1) + ") ");
			print(RED, "ALEADY PLACED");
		}
	}
}

/// Setting rows and columns
void Game::getCoordinates(string& inputRow, string& inputCol, int& row, int& col, int x, int y) {
	/// Rows (HORIZONTAL)
	while (true) {
		print(x, y, "> Select row (A - J)");
		print(GREEN, "> ");
		inputRow = readLimitedInput(x + 22, y);
		if (inputRow == "CANCEL") {
			col = -1;
			setState("CANCEL");
			return;
		}
		setColor(WHITE);
		if (inputRow >= "A" && inputRow <= "J") {
			char charToInt = inputRow[0];
			row = charToInt - 65;

			for (int i = 44; i < 80; ++i)
				print(i, y + 1, " ");

			break;
		}
		else {
			print(x + 22, y, " ");
			print(x, y + 1, "Provide a correct option");
		}
	}
	/// Columns (VERTICAL)
	while (true) {
		print(x, y += 2, "> Select column (0 - 9)");
		print(GREEN, "> ");
		col = stringToInt(inputCol, "Provide a correct option", btw, 0, 9, x + 25, y, 25, 1);
		if (col == -1) {
			setState("CANCEL");
			return;
		}
		for (int i = 44; i < 80; ++i)
			print(i, y + 1, " ");

		break;
	}
}

/// Generating a random coordinate for the computer
void Game::randomCoordinate(int currentTurn, int & row, int & col, int x, int y) {

	while (true) {
		char charToString;
		row = rand() % 10;
		col = rand() % 10;
		// Player (AI VS AI)
		if (currentTurn == 0) {
			// If it's already been hit or missed, we 're-roll' our PSNG
			if (computerHitGrid[row][col] == 1 || computerMissGrid[row][col]) {
				row = rand() % 10;
				col = rand() % 10;
			}
			else {
				charToString = row + 65;
				inputRow[0] = charToString;
				break;
			}
		}
		else {
			// If it's already been hit or missed, we 're-roll' our PSNG
			if (playerHitGrid[row][col] == 1 || playerMissGrid[row][col]) {
				row = rand() % 10;
				col = rand() % 10;
			}
			else {
				char charToString = row + 65;
				inputRow[0] = charToString;
				break;
			}
		}

	}
}

/// Automatic sort
void Game::sortAuto() {
	//clearInput(44);
	//print(44, 0, GREEN, "> Setting your ships................");
	while (getPlacedShips(0) > 0) {
		createShipAuto(0);
	}
	/*print(76, 0, YELLOW, "DONE");

	print(44, 1, GREEN, "> Setting opponents's ships.........");*/
	while (getPlacedShips(1) > 0) {
		createShipAuto(1);
	}
	/*print(76, 1, YELLOW, "DONE");
	print(44, 3, GREEN, "> Setup finished!");
	print(44, 5, GREEN, "> Setting up the match..............");*/
	setState("NOTCANCELED");
}

/// Manual sort
void Game::sortManual() {
	// We want to check if either the player and computer have available ships to place
	while (getPlacedShips(0) > 0) {

		clearInput(44);
		printAvailableShips();

		while (true) {
			print(50, 7, "Select a ship to place");
			print(GREEN, ">");
			shipModel = stringToInt(inputShip, "Provide a correct option", outNIR, 0, 5, 74, 7, 24, 1);
			if (shipModel == -1) {
				setState("CANCEL");
				return;
			}
			if (currentPlayerShip[shipModel - 1] == 1) {
				print(74, 7, " ");
				print(50, 8, "Ship already used");
			}
			else {
				break;
			}
		}
		createShipManual();
	}
	/*clearInput(44);
	print(44, 0, GREEN, "> Please wait.......................");
	print(44, 1, GREEN, "> Setting opponents's ships.........");*/
	while (getPlacedShips(1) > 0) {
		createShipAuto(1);
	}
	/*print(76, 1, YELLOW, "DONE");
	print(44, 3, GREEN, "> Setup finished!");
	print(44, 5, GREEN, "> Setting up the match..............");*/
	setState("NOTCANCELED");
	//Sleep(3000);
	
}

/// Creating a ship (AUTOMATIC)
void Game::createShipAuto(int player) {
	
	int tempShip = rand() % 5;
	int tempRow = rand() % 10;
	int tempCol = rand() % 10;
	int tempDir = rand() % 2;

	// Checking whether or not the ship is available for use
	while (true) {
		if (player == 0 || player == 1) {
			if (player == 0) {
				if (currentPlayerShip[tempShip] == 1) {
					tempShip = rand() % 5;
				}
				else
					break;
			}
			else if (player == 1) {
				if (currentComputerShip[tempShip] == 1) {
					tempShip = rand() % 5;
				}
				else
					break;
			}
		}
	}
	placeShip(tempDir, tempRow, tempCol, tempShip, player);
}

/// Creating a ship (MANUAL)
void Game::createShipManual() {
	// Vertical screen navigation
	int x = 50;
	int y = 7;
	// Since arrays are counted starting from 0
	int SHIP_ID = shipModel - 1;
	
	string inputCol, inputDir, confirmPlacement;
	int row, col;
	char dir;

	print(x, ++y, WHITE, "Selected ");
	print(GREEN, SHIP_NAME[SHIP_ID]); 
	cout << "(";
	print(GREEN, getShipCode(SHIP_ID));
	cout << ")";
	print(x, ++y, "It has a length of.........");
	setColor(GREEN);
	cout << getShipSize(SHIP_ID);
	setColor(WHITE);

	// Gettings rows and columns
	getCoordinates(inputRow, inputCol, row, col, x, y += 2);
	y += 4;

	/// Direction
	while (true) {
		print(x, y, "Direction");
		print(50, y + 1, "V(ertical) - H(orizontal)");
		print(GREEN, "> ");
		inputDir = readLimitedInput(x + 27, y + 1);
		if (inputDir == "CANCEL") {
			setState("CANCEL");
			return;
		}
		setColor(WHITE);
		if (inputDir == "V" || inputDir == "H") {

			if (inputDir == "V")
				dir = 1;
			else if (inputDir == "H")
				dir = -1;
			
			for (int i = 44; i < 80; ++i) 
				print(i, y + 2, " ");
			
			break;
		}
		else {
			print(77, y + 1, " ");
			print(50, y + 2, "Provide a correct option");
		}
	}

	y += 3;

	/// Confirming placement before doing anything else
	while (true) {
		print(50, y, "Confirm placing (Y/N)");
		print(GREEN, "> ");
		confirmPlacement = readLimitedInput(x + 23, y);

		if (confirmPlacement == "N") {
			return;
		}
		else if (!(confirmPlacement == "N" || confirmPlacement == "Y")) {
			print(73, y, " ");
			print(50, y + 1, "Provide a correct option");
		}
		else {
			break;
		}
	}
	placeShip(dir, row, col, SHIP_ID, 0);
}

/// Placing the ships for the player and the AI
/// It relies on the value of 'spaceTaken' to operate
/// Directions -> 1: Vertical - 2: Horizontal
void Game::placeShip(int dir, int row, int col, int SHIP_ID, int player) {

	// For our loops
	int position;
	int currentGridPos;
	int outOfBoundsPos;

	if (dir == 1) {
		position = col;
	}
	else {
		position = row;
	}
	
	// Collision checking (includes out of bounds)
	for (int tempPos = position; tempPos < getShipSize(SHIP_ID) + position; ++tempPos) {

		if (player == 0) {
			if (dir == 1) {
				currentGridPos = playerGrid[row][tempPos];
				outOfBoundsPos = playerGrid[row][tempPos + 1];
			}
			else {
				currentGridPos = playerGrid[tempPos][col];
				outOfBoundsPos = playerGrid[tempPos + 1][col];
			}

			if (tempPos >= 9 && outOfBoundsPos == 1) {
				if (getSetupMode() == 2) {
					tempPos -= 2;
					print(50, 20, RED, "####  ");
					print(YELLOW, "OUT OF BOUNDS");
					print(RED, "  ####");
					print(50, 21, RED, "####    ");
					print(YELLOW, "TRY AGAIN");
					print(RED, "    ####");
					Sleep(1500);
				}
				return;
			}
		}
		else if (player == 1) {
			if (dir == 1) {
				currentGridPos = computerGrid[row][tempPos];
				outOfBoundsPos = computerGrid[row][tempPos + 1];
			}
			else {
				currentGridPos = computerGrid[tempPos][col];
				outOfBoundsPos = computerGrid[tempPos + 1][col];
			}

			if (tempPos >= 9 && outOfBoundsPos == 1) {
				return;
			}
		}
		

		if (currentGridPos >= 1 && currentGridPos <= 5) {
			spaceTaken = true;
			if (getSetupMode() == 2 && player == 0) {
				print(50, 20, RED, "##  "); 
				print(YELLOW, "Position " + inputRow + "-" + std::to_string(tempPos) + " occupied");
				print(RED, "  ##");
				print(50, 21, RED, "##       ");
				print(YELLOW, "TRY AGAIN");
				print(RED, "       ##");
				Sleep(1500);
			}
			break;
		}
	}

	// Proceeding to the actual placing
	if (spaceTaken == false) {
		for (int tempPosition = position; tempPosition < getShipSize(SHIP_ID) + position; ++tempPosition) {
			if (dir == 1) {
				if (player == 0) {
					playerGrid[row][tempPosition] = SHIP_ID + 1;
					print(row * 2 + 1, tempPosition + 2, GREEN, getShipCode(SHIP_ID));
				}
				else {
					computerGrid[row][tempPosition] = SHIP_ID + 1;
					DEBUG_IF(1) {
						print(row * 2 + 1, tempPosition + 15, GREEN, getShipCode(SHIP_ID));
					}
				}

			}
			else {
				if (player == 0) {
					playerGrid[tempPosition][col] = SHIP_ID + 1;
					print(tempPosition * 2 + 1, col + 2, GREEN, getShipCode(SHIP_ID));
				}
				else {
					computerGrid[tempPosition][col] = SHIP_ID + 1;
					DEBUG_IF(1) {
						print(tempPosition * 2 + 1, col + 15, GREEN, getShipCode(SHIP_ID));
					}
				}

			}
		}
		// Making the ship unavailable now that it has been placed
		// 0 = player, 1 = AI
		if (player == 0) {
			currentPlayerShip[SHIP_ID] = 1;
		}
		else if (player == 1) {
			currentComputerShip[SHIP_ID] = 1;
		}
		currentShipsPlaced[player]++;
	}
	else {
		spaceTaken = false;
	}
}

/**************************************/
/*                                    */
/*                GAME                */
/*                                    */
/**************************************/

/// Drawing the game screen - MIDDLE GRID
void Game::drawGameScreen() {
	clearInput(44);
	char letter = 'A';
	// Player
	print(29, 0, YELLOW, "Moves");
	setColor(TEAL);
	for (int i = 0; i <= 10; ++i) {
		for (int j = 0; j <= 10; ++j) {
			// Drawing the horizontal row numbers
			if ((i > 0 && i <= 10) && j == 0) {
				print(i * 2 + 21, j + 1, letter);
				letter++;
			}
			// Drawing the vertical row letters
			if ((j > 0 && j <= 10) && i == 0) {
				print(i + 22, j + 1, j - 1);
			}
		}
	}

	// Computer
	DEBUG_IF(1) {
		letter = 'A';
		print(29, 13, YELLOW, "Moves");
		setColor(TEAL);
		for (int i = 0; i <= 10; ++i) {
			for (int j = 0; j <= 10; ++j) {
				// Drawing the horizontal row numbers
				if ((i > 0 && i <= 10) && j == 0) {
					print(i * 2 + 21, j + 14, letter);
					letter++;
				}
				// Drawing the vertical row letters
				if ((j > 0 && j <= 10) && i == 0) {
					print(i + 22, j + 14, j - 1);
				}
			}
		}
	}

	// Water
	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 10; ++j) {
			print(i + 23, j + 2, DARKTEAL, "~");
			DEBUG_IF(1) {
				print(i + 23, j + 15, DARKTEAL, "~");
			}
		}
	}
}

void Game::update() {
	string inputCol;
	int row, col;
	int x = 44;
	int y = 0;
	while (true) {
		// Reminder for ship sizes and codes
		for (int i = x; i < 80; ++i) {
			for (int j = 0; j <= 7; ++j) {
				if (i == x || i == 78)
					print(i, y + 17 + j, "||");
				if (j == 0 || j == 7)
					print(i, y + 17 + j, "=");
			}
		}
		print(x + 10, y + 18, GREEN, "SHIP INFORMATION");
		for (int i = 0; i < 5; ++i) {
			print(x + 2, y + 19 + i, SHIP_NAME[i] + "- (");
			print(RED, getShipCode(i));
			print(WHITE, ") - Size: " + std::to_string(getShipSize(i)));
		}
		
		// Player
		if (turn % 2 == 0) {
			
			print(x,     y, YELLOW, "> YOUR TURN (ESC to exit)");
			print(x, y + 1, GREEN, "Pick a position e.g. A-4, F-8, C-6");
			print(x, y + 2, GREEN, "Try to hit your opponent's ships!");

			while (true) {
				getCoordinates(inputRow, inputCol, row, col, x, y + 3);

				// Going back to previous menu
				if (col == -1) {
					setState("CANCEL");
					return;
				}
				else {
					setState("NOTCANCELED");
				}

				// Making sure we don't hit the same position twice
				if (computerHitGrid[row][col] == 1 || computerMissGrid[row][col] == 1) {
					print(x, y + 7, RED, "Position already hit. Try again");
					print(x + 22, y + 3, " ");
					print(x + 25, y + 5, " ");
				}
				else {
					// Clearing previous text
					for (int i = 44; i < 80; i++) {
						print(i, y + 1, " ");
						print(i, y + 2, " ");
						print(i, y + 3, " ");
						print(i, y + 5, " ");
						print(i, y + 7, " ");
					}
					print(x, y + 2, WHITE, "> You call " + inputRow + "-" + std::to_string(col));
					break;
				}
			}
		}
		// Computer
		else {
			print(x, y, YELLOW, "> OPPONENT'S TURN");
			randomCoordinate(1, row, col, x, y + 1);
			print(x, y + 2, WHITE, "> Opponent calls " + inputRow + "-" + std::to_string(col));
		}

		// Checking collisions
		checkHit(row, col, turn);
		// Asking for confirmation to continue
		confirmRETURN(x, y + 6);

		// Clearing input
		clearInput(44);

		// Determining if a winner has been found
		if (hitCounter[turn % 2] == 17) {
			getWinner(turn % 2);
		}

		turn++;

	} // while true loop
}

// currentTurn: -> Turn % 2 (0 for player, 1 for computer)
void Game::checkHit(int row, int col, int currentTurn) {

	/// Player
	if (turn % 2 == 0) {

		// Hit
		if (computerGrid[row][col] >= 1 && computerGrid[row][col] <= 5) {
			print(44, 4, RED, "> You hit " + SHIP_NAME[ computerGrid[row][col] - 1 ]);
			computerHitGrid[row][col] = 1;
			print(row * 2 + 23, col + 2, RED, getShipCode( computerGrid[row][col] - 1 ));
			hitCounter[turn % 2]++;
		}
		// Miss
		else {
			print(44, 4, PINK, "> You missed your shot!");
			computerMissGrid[row][col] = 1;
			print(row * 2 + 23, col + 2, PINK, 'o');
		}
	} // Player
	/// Computer
	else if (turn % 2 == 1) {

		// Hit
		if (playerGrid[row][col] >= 1 && playerGrid[row][col] <= 5) {
			print(44, 4, RED, "> Opponent hits " + SHIP_NAME[ playerGrid[row][col] - 1 ]);
			playerHitGrid[row][col] = 1;
			DEBUG_IF(1) {
				print(row * 2 + 23, col + 15, RED, getShipCode( playerGrid[row][col] - 1 ));
			}
			hitCounter[turn % 2]++;
		}
		// Miss
		else {
			print(44, 4, PINK, "> Your opponent missed!");
			playerMissGrid[row][col] = 1;
			DEBUG_IF(1) {
				print(row * 2 + 23, col + 15, PINK, 'o');
			}
		}
	} // Computer
}

void Game::getWinner(int currentPlayer) {
	clearInput(44);
	if (currentPlayer == 0) {
		print(44, 10, YELLOW, "YOU WIN!");
		gotoxy(44, 11, true);
		confirmRETURN(34, 16);
	}
	else {
		print(44, 10, YELLOW, "COMPUTER WINS!");
		gotoxy(44, 11, true);
		confirmRETURN(34, 16);
	}
}

/**************************************/
/*                                    */
/*             GAME OVER              */
/*                                    */
/**************************************/

void Game::drawGameOverScreen() {
	clearInput(0);
	int x = 17;
	int y = 0;
	print(x, y, GREEN, "___  ____ ___ ___ _    ____ ____ _  _ _ ___ ");
	print(x, ++y, GREEN, "|__] |__|  |   |  |    |___ [__  |__| | |__]");
	print(x, ++y, GREEN, "|__] |  |  |   |  |___ |___ ___] |  | | |   ");
	y = 10;
	print(x, y, "PLAY AGAIN?");
	print(x, ++y, "Y(es)/N(no)");
	print(x, ++y, "Type your option and press ");
	print(GREEN, "RETURN");
	print(x, ++y, "Option");
	print(GREEN, "> ");
}

string Game::setGameOver() {
	string inputOption;
	while (true) {
		inputOption = readLimitedInput(25, 13);
		if (inputOption != "Y" && inputOption != "N") {
			print(25, 13, " ");
			print(17, 14, "Please provide a correct option");
		}
		else {
			for (int i = 44; i < 80; ++i) {
				print(i, 13, " ");
			}
			break;
		}
	}
	return inputOption;
}

/**************************************/
/*                                    */
/*           QUIT / CREDITS           */
/*                                    */
/**************************************/

void Game::drawQuitScreen() {
	clearInput(0);
	int x = 5;
	int y = 2;
	print(x, ++y, GREEN, ".___________. __    __       ___      .__   __.  __  ___      _______.");
	print(x, ++y, GREEN, "|           ||  |  |  |     /   \\     |  \\ |  | |  |/  /     /       |");
	print(x, ++y, GREEN, "`---|  |----`|  |__|  |    /  ^  \\    |   \\|  | |  '  /     |   (----`");
	print(x, ++y, GREEN, "    |  |     |   __   |   /  /_\\  \\   |  . `  | |    <       \\   \\    ");
	print(x, ++y, GREEN, "    |  |     |  |  |  |  /  _____  \\  |  |\\   | |  .  \\  .----)   |   ");
	print(x, ++y, GREEN, "    |__|     |__|  |__| /__/     \\__\\ |__| \\__| |__|\\__\\ |_______/    ");
	x = 24;
	++y;
	print(x, ++y, GREEN, " _______   ______   .______      ");
	print(x, ++y, GREEN, "|   ____| /  __  \\  |   _  \\     ");
	print(x, ++y, GREEN, "|  |__   |  |  |  | |  |_)  |    ");
	print(x, ++y, GREEN, "|   __|  |  |  |  | |      /     ");
	print(x, ++y, GREEN, "|  |     |  `--'  | |  |\\  \\----.");
	print(x, ++y, GREEN, "|__|      \\______/  | _| `._____|");
	x = 5;
	++y;
	print(x, ++y, GREEN, ".______    __          ___   ____    ____  __  .__   __.   _______  __  ");
	print(x, ++y, GREEN, "|   _  \\  |  |        /   \\  \\   \\  /   / |  | |  \\ |  |  /  _____||  | ");
	print(x, ++y, GREEN, "|  |_)  | |  |       /  ^  \\  \\   \\/   /  |  | |   \\|  | |  |  __  |  | ");
	print(x, ++y, GREEN, "|   ___/  |  |      /  /_\\  \\  \\_    _/   |  | |  . `  | |  | |_ | |  | ");
	print(x, ++y, GREEN, "|  |      |  `----./  _____  \\   |  |     |  | |  |\\   | |  |__| | |__| ");
	print(x, ++y, GREEN, "| _|      |_______/__/     \\__\\  |__|     |__| |__| \\__|  \\______| (__) ");
	x = 25;
	y+=2;
	confirmRETURN(x, y);
}

void Game::drawCreditsScreen() {
	clearInput(0);
	int x = 25;
	int y = 3;

	for (int i = 0; i < 80; ++i) {
		for (int j = 0; j < 24; ++j) {
			if (j == 0 || j == 23) {
				print(i, j, GREEN, "#");
			} if (i == 0 || i == 79) {
				print(i, j, GREEN, "#");
			}
		}
	}

	print(x, y += 2, "Media Design School");
	print(x, y += 2, "Auckland");
	print(x, y += 2, "New Zealand");
	y += 2;
	print(x, y += 2, "Lecturer: Bindu Ojha");
	print(x, y += 2, "Student: Juan Rodriguez");
	y += 4;
	confirmRETURN(x, y);
}