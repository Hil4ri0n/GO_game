//#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include"conio2.h"
#include"vector.h"
#define CORNER_X 50
#define CORNER_Y 7
#define KEYS_X 1
#define KEYS_Y 3

typedef struct{
	int x;
	int y;
	int player; //PointKo isn't valid if it equals 0
}PointKo;

void printBoard(vectorV_t board, const int boardsize, int tabX, int tabY);
void placeStone(int tabX, int tabY, int& playerTurn, vectorV_t board, const int boardsize, int& whiteScore, int& blackScore, PointKo& ko);
int isSurrounded(int tabX, int tabY, int playerTurn, vectorV_t board, const int boardsize); //checking if a given field is surrounded
int isOccupied(int tabX, int tabY, vectorV_t board, const int boardsize);//checking if a given field is occupied
void simpleCapture(int tabX, int tabY, int playerTurn, vectorV_t board, const int boardsize, int& whiteScore, int& blackScore);//capturing single stone
void simpleScore(int tabX, int tabY, int playerTurn, vectorV_t board, const int boardsize, int& whiteScore, int& blackScore);//granting a point after simpleCapture
void reset(vectorV_t board, int& boardsize, int& whiteScore, int& blackScore, int& isHandicap, int& playerTurn);//reseting game
PointKo findKoPoint(int tabX, int tabY, int playerTurn, vectorV_t board, const int boardsize);//finding field that isn't available beacuse of KO rule
int koRule(int tabX, int tabY, PointKo point, int playerTurn);//bool function deciding if KO rule forbids a move
void start(int x, int y, int& isHandicap, int& boardsize);// starting game
void handicap(vectorV_t board, const int boardsize, int& playerTurn);
//functions responsible for capturing multiple stones
void floodFill(int tabX, int tabY, int playerTurn, vectorV_t board, const int boardsize, vectorPt_t& visited, int& pomocnicza);//eliminating stones from the board
//function that determins if capturing multiple stones is possible
//if not, it will undo the floodFill function
//if yes, it will grant points to a certain player
void multipleCapture(int tabX, int tabY, int playerTurn, vectorV_t board, const int boardsize, int& whiteScore, int& blackScore);
//function calling multipleCapture in four directions
void multipleScore(int tabX, int tabY, int playerTurn, vectorV_t board, const int boardsize, int& whiteScore, int& blackScore);
void save(int playerTurn, vectorV_t board, const int boardsize, int whiteScore, int blackScore, PointKo ko);//saving game
void load(int& playerTurn, vectorV_t board, int& boardsize, int& whiteScore, int& blackScore, PointKo& ko);//loading saved game


int main() {
	_setcursortype(_NOCURSOR);

	int zn = 0, x = CORNER_X, y = CORNER_Y, zero = 0;
	char txt[40];
	int playerTurn{2}, whiteScore{}, blackScore{}, tabX{}, tabY{};
	int isHandicap = 0;//game starts with handicap if it's greater than 0
	int boardsize{};

	
	//Displaying starting menu
	start(4, 6, isHandicap, boardsize);

	PointKo ko;
	ko.player = 0;
	ko.x = -1;
	ko.y = -1;
	vectorV_t board;
	init(&board);
	for (int i = 0; i < boardsize; i++) {
		vectorCH_t line;
		init(&line);
		for (int j = 0; j < boardsize; j++) {
			push_back(&line,'.');
		}
		push_back(&board, line);
	}
	gotoxy(x, y);
	if (isHandicap) handicap(board, boardsize, playerTurn);


	// if the program is compiled in pure C, then we need to initialize
	// the library ourselves; __cplusplus is defined only if a C++ compiler
	// is used
#ifndef __cplusplus
	Conio2_Init();
#endif
	// settitle sets the window title
	settitle("Piotr, Kolasiñski, 193275");
	
	
	
	do {
		_setcursortype(_NOCURSOR);

		tabX = (x - CORNER_X) / 2;
		tabY = y - CORNER_Y;
		
		textbackground(BLACK);
		
		clrscr();

		textcolor(7);
		
		gotoxy(KEYS_X, KEYS_Y);
		cputs("i - place a stone");
		gotoxy(KEYS_X, KEYS_Y + 1);
		cputs("n - start new game");
		gotoxy(KEYS_X, KEYS_Y + 2);
		cputs("s - save game");
		gotoxy(KEYS_X, KEYS_Y + 3);
		cputs("l - load game");
		gotoxy(KEYS_X, KEYS_Y + 4);
		cputs("q - quit");
		gotoxy(KEYS_X, KEYS_Y + 5);
		cputs("space - confirm action");
		gotoxy(KEYS_X, KEYS_Y + 6);
		cputs("backspace - delete a letter");
		// print out the code of the last key pressed
		if (zero) sprintf_s(txt, "key code: 0x00 0x%02x", zn);
		else sprintf_s(txt, "key code: 0x%02x", zn);
		gotoxy(KEYS_X, KEYS_Y + 7);
		cputs(txt);

		printBoard(board,boardsize,tabX,tabY);

		gotoxy(KEYS_X, KEYS_Y + 8);
		sprintf_s(txt, "cursors position: %d, %d ", x, y);
		cputs(txt);
		gotoxy(KEYS_X, KEYS_Y + 9);
		if(playerTurn == 1) cputs("Now Playing : White(O)");
		else cputs("Now Playing : Black(X)");
		gotoxy(KEYS_X, KEYS_Y + 10);
		sprintf_s(txt, "tabX: %d, tabY: %d", tabX, tabY);
		cputs(txt);
		gotoxy(KEYS_X, KEYS_Y + 11);
		sprintf_s(txt, "White Score: %2d, Black Score: %2d", whiteScore, blackScore);
		cputs(txt);
		gotoxy(x, y);

		zero = 0;
		zn = getch();
		if (zn == 0) {
			zero = 1;		
			zn = getch();		
			if (zn == 0x48) y--;	
			else if (zn == 0x50) y++;
			else if (zn == 0x4b) x -= 2;
			else if (zn == 0x4d) x += 2;

			if (x < CORNER_X) x = CORNER_X;
			if (x > CORNER_X + boardsize * 2 - 2) x = CORNER_X + boardsize * 2 - 2;
			if (y < CORNER_Y) y = CORNER_Y;
			if (y > CORNER_Y + boardsize - 1)  y = CORNER_Y + boardsize - 1;

		}
		else if (zn == 'i') placeStone(tabX, tabY, playerTurn, board, boardsize, whiteScore, blackScore, ko);
		else if (zn == 'n') {
			reset(board, boardsize, whiteScore, blackScore, isHandicap, playerTurn);
			ko.player = 0;
		}
		else if (zn == 's') save(playerTurn, board, boardsize, whiteScore, blackScore, ko);
		else if (zn == 'l') load(playerTurn, board, boardsize, whiteScore, blackScore, ko);

	} while (zn != 'q');

	_setcursortype(_NORMALCURSOR);	
	// show the cursor so it will be
	// visible after the program ends
	return 0;
}

//SAVE
void save(int playerTurn, vectorV_t board, const int boardsize, int whiteScore, int blackScore, PointKo ko) {
	FILE* file = NULL;
	char fileName[101];
	int x = CORNER_X;
	int zn = 0;
	int i = 0;
	do {
		gotoxy(CORNER_X, CORNER_Y - 2);
		cputs("Enter name of the file:");
		gotoxy(x, CORNER_Y - 1);
		clreol();
		zn = getch();
		do {
			if (zn == 0x08) {
				if (i > 0) i--;
				if (x > CORNER_X) x--;
				gotoxy(x, CORNER_Y - 1);
				putch(' ');
			}
			else {
				gotoxy(CORNER_X, CORNER_Y - 2);
				cputs("Enter name of the file:");
				gotoxy(x, CORNER_Y - 1);
				putch(zn);
				fileName[i++] = zn;
				gotoxy(++x, CORNER_Y - 1);
			}
			zn = getch();
		} while (zn != ' ');
		fileName[i] = '\0';

		fopen_s(&file, fileName, "w");
	} while (file == NULL);
	
	fprintf(file, "%d\n", playerTurn);
	fprintf(file, "%d\n", boardsize);
	fprintf(file, "%d\n", whiteScore);
	fprintf(file, "%d\n", blackScore);
	fprintf(file, "%d\n", ko.x);
	fprintf(file, "%d\n", ko.y);
	fprintf(file, "%d\n", ko.player);

	for (int i = 0; i < boardsize; i++) {
		for (int j = 0; j < boardsize; j++) {
			fprintf(file, "%c\n", board.ptr[i].ptr[j]);
		}
	}
	fclose(file);
}

void load(int& playerTurn, vectorV_t board, int& boardsize, int& whiteScore, int& blackScore, PointKo& ko) {
	FILE* file = NULL;
	char fileName[101];
	int x = CORNER_X;
	int zn = 0;
	int i = 0;
	do {
		x = CORNER_X;
		i = 0;
		gotoxy(CORNER_X, CORNER_Y - 2);
		cputs("Enter name of the file:");
		gotoxy(x, CORNER_Y - 1);
		clreol();
		zn = getch();
		do {
			if (zn == 0x08) {
				if (i > 0) i--;
				if (x > CORNER_X) x--;
				gotoxy(x, CORNER_Y - 1);
				putch(' ');
			}
			else {
				gotoxy(CORNER_X, CORNER_Y - 2);
				cputs("Enter name of the file:");
				gotoxy(x, CORNER_Y - 1);
				putch(zn);
				fileName[i++] = zn;
				gotoxy(++x, CORNER_Y - 1);
			}
			zn = getch();
		} while (zn != ' ');
		fileName[i] = '\0';

		fopen_s(&file, fileName, "r");
	} while (file == 0);

	if(fscanf_s(file, "%d", &playerTurn) == EOF) return;
	if(fscanf_s(file, "%d", &boardsize) == EOF) return;
	if(fscanf_s(file, "%d", &whiteScore) == EOF) return;
	if(fscanf_s(file, "%d", &blackScore) == EOF)return;
	if(fscanf_s(file, "%d", &ko.x) == EOF)return;
	if(fscanf_s(file, "%d", &ko.y) == EOF)return;
	if(fscanf_s(file, "%d", &ko.player) == EOF)return;

	for (int i = 0; i < boardsize; i++) {
		for (int j = 0; j < boardsize; j++) {
			if(fscanf_s(file, "\n%c", &board.ptr[i].ptr[j],1) == EOF)return;
		}
	}
	fclose(file);
}

//STARTING GAME
void start(int x, int y, int& isHandicap, int& boardsize) {
	int option = 1, zn = 0;
	boardsize = 0;
	do {
		clrscr();
		textbackground(BLACK);
		textcolor(7);
		gotoxy(x, y+1);		
		cputs("CHOOSE SIZE OF THE BOARD:");		
		gotoxy(x, y+2);
		if (option == 1) textcolor(GREEN);
		cputs("19 x 19");
		textcolor(7);
		gotoxy(x, y+3);
		if (option == 2) textcolor(GREEN);
		cputs("13 x 13");
		textcolor(7);
		gotoxy(x, y+4);
		if (option == 3) textcolor(GREEN);
		cputs("9 x 9");
		textcolor(7);
		gotoxy(x, y+5);
		if (option == 4) textcolor(GREEN);
		cputs("CUSTOM SIZE");
		gotoxy(x, y + 10);
		textcolor(MAGENTA);
		cputs("PRESS SPACE TO CONFIRM");
		textcolor(7);
		
		zn = getch();
		if (zn == 0) {		
			zn = getch();		
			if (zn == 0x48) {				
				option--;
			}
			else if (zn == 0x50) {
				
				option++;
			}
			if (option < 1) option = 4;
			if (option > 4) option = 1;
		};
	} while (zn != ' ');
	clrscr();
	if (option == 1) boardsize = 19;
	if (option == 2) boardsize = 13;
	if (option == 3) boardsize = 9;
	if (option == 4) { //getting boardsize from player and displaying typed keys on the screen
		
		zn = 0;
		boardsize = 0;
		textcolor(MAGENTA);
		gotoxy(2, y + 11);
		cputs("ENTER SIZE OF THE BOARD");
		gotoxy(2, y + 12);
		cputs("PRESS SPACE TO CONFIRM");
		textcolor(7);
		gotoxy(x, y + 6);
		zn = getch();
		do {
			putch(zn);
			boardsize = boardsize * 10 + zn - 48;
			textcolor(MAGENTA);
			gotoxy(2, y + 11);
			cputs("ENTER SIZE OF THE BOARD");
			gotoxy(2, y + 12);
			cputs("PRESS SPACE TO CONFIRM");
			textcolor(7);
			gotoxy(++x, y + 6);
			zn = getch();
		} while (zn != ' ');
	}
	clrscr();
	option = 1;
	do {
		clrscr();
		textbackground(BLACK);
		textcolor(7);
		gotoxy(x, y + 1);
		cputs("DO YOU WANT TO PLAY WITH HANDICAP");
		gotoxy(x, y + 2);
		if (option == 1) textcolor(GREEN);
		cputs("YES");
		textcolor(7);
		gotoxy(x, y + 3);
		if (option == 2) textcolor(GREEN);
		cputs("NO");
		gotoxy(x, y + 10);
		textcolor(MAGENTA);
		cputs("PRESS SPACE TO CONFIRM");
		textcolor(7);

		zn = getch();
		if (zn == 0) {		
			zn = getch();		
			if (zn == 0x48) {
				option--;
			}
			else if (zn == 0x50) {

				option++;
			}
			if (option < 1) option = 2;
			if (option > 2) option = 1;
		};
	} while (zn != ' ');
	if (option == 1) isHandicap = 1;
	else isHandicap = 0;
}

void handicap(vectorV_t board, const int boardsize, int& playerTurn) {

	int x = CORNER_X, y = CORNER_Y, zn = 0, tabX{}, tabY{};
	clrscr();
	printBoard(board, boardsize,tabX,tabY);
	do {
		_setcursortype(_NOCURSOR);

		gotoxy(KEYS_X, KEYS_Y);
		cputs("i - place a stone");
		gotoxy(KEYS_X, KEYS_Y + 1);
		cputs("space - start");
		gotoxy(KEYS_X, KEYS_Y + 2);
		cputs("Now Playing : Black(X)");
		tabX = (x - CORNER_X) / 2;
		tabY = y - CORNER_Y;
		printBoard(board, boardsize,tabX,tabY);
		gotoxy(x, y);
		zn = getch();
		if (zn == 0) {
			zn = getch();
			if (zn == 0x48) y--;
			else if (zn == 0x50) y++;
			else if (zn == 0x4b) x -= 2;
			else if (zn == 0x4d) x += 2;

			if (x < CORNER_X) x = CORNER_X;
			if (x > CORNER_X + boardsize * 2 - 2) x = CORNER_X + boardsize * 2 - 2;
			if (y < CORNER_Y) y = CORNER_Y;
			if (y > CORNER_Y + boardsize - 1)  y = CORNER_Y + boardsize - 1;
		}
		else if (zn == 'i') board.ptr[tabY].ptr[tabX] = 'X';
		clrscr();
	} while (zn != ' ');
	playerTurn = 1;
}



//RESETING GAME
void reset(vectorV_t board, int& boardsize, int& whiteScore, int& blackScore, int& isHandicap, int&playerTurn) {
	for (int i = 0; i < boardsize; i++) {
		for (int j = 0; j < boardsize; j++) {
			board.ptr[i].ptr[j] = '.';
		}
	}
	whiteScore = 0;
	blackScore = 0;
	start(4, 6, isHandicap, boardsize);
	if (isHandicap) handicap(board, boardsize, playerTurn);
}

//Printing a Board
void printBoard(vectorV_t board, const int boardsize, int tabX, int tabY) {
	int board_x = CORNER_X, board_y = CORNER_Y;
	gotoxy(CORNER_X, CORNER_Y);
	textbackground(DARKGRAY);
	textcolor(BLACK);
	for (int i = 0; i < boardsize ; i++) {
		board_x = CORNER_X;

		for (int j = 0; j < boardsize ; j++) {
			
			gotoxy(board_x, board_y);
			if (i == tabY && j == tabX) textbackground(RED);
			putch(board.ptr[i].ptr[j]);
			textbackground(DARKGRAY);
			board_x++;
			gotoxy(board_x, board_y);
			putch(' ');
			board_x++;
		}
		board_y++;
	}
	textbackground(BLACK);
	textcolor(WHITE);
}

void placeStone(int tabX, int tabY, int& playerTurn, vectorV_t board, const int boardsize, int& whiteScore, int& blackScore, PointKo& ko) {

	if (!isOccupied(tabX, tabY, board, boardsize)) { //checking if field is occupied
		PointKo newKo = findKoPoint(tabX, tabY, playerTurn, board, boardsize);//creating new PointKo that will be valid in the next turn
		if (koRule(tabX, tabY, ko, playerTurn)) {//checking if KO rule allows to make a move
			char playerStone = (playerTurn == 1) ? 'O' : 'X';
			board.ptr[tabY].ptr[tabX] = playerStone;//placing stone
			multipleScore(tabX, tabY, playerTurn, board, boardsize, whiteScore, blackScore);//checking possibility of a capture
			multipleCapture(tabX, tabY, 3 - playerTurn, board, boardsize, whiteScore, blackScore);
			simpleCapture(tabX, tabY, playerTurn, board, boardsize, whiteScore, blackScore);
			// if a stone is still surrouned then move wasn't allowed to be made and has to be undone
			if (isSurrounded(tabX, tabY, playerTurn, board, boardsize)) board.ptr[tabY].ptr[tabX] = '.'; 
			else {//if a move was succesfully made
				playerTurn = 3 - playerTurn;
				ko = newKo;
			}
		}
	}
}


//checking if a given stone on a given field is Surrounded
int isSurrounded(int tabX, int tabY, int playerTurn, vectorV_t board, const int boardsize){
	char enemyStone;
	if (playerTurn == 1) enemyStone = 'X';
	else enemyStone = 'O';

	if ( tabX < boardsize - 1  && board.ptr[tabY].ptr[tabX + 1] != enemyStone) return 0;
	if ( tabX > 0 &&  board.ptr[tabY].ptr[tabX - 1] != enemyStone) return 0;
	if ( tabY < boardsize - 1 && board.ptr[tabY + 1].ptr[tabX]  != enemyStone) return 0;
	if ( tabY > 0  && board.ptr[tabY - 1].ptr[tabX] != enemyStone) return 0;
	return 1;
}

//checking if a field is occupied
int isOccupied(int tabX, int tabY, vectorV_t board, const int boardsize) {
	if (board.ptr[tabY].ptr[tabX] == '.') return 0;
	else return 1;
}

//function checking if a capture has occured
void simpleCapture(int tabX, int tabY, int playerTurn, vectorV_t board, const int boardsize, int& whiteScore, int& blackScore) {
	int enemyPlayer;
	char enemyStone;
	if (playerTurn == 1) {
		enemyPlayer = 2;
		enemyStone = 'X';
	}
	else {
		enemyPlayer = 1;
		enemyStone = 'O';
	}
	//checking surrounding fields for simple capture opportunity
	if (tabX > 0) {
		if (isSurrounded(tabX - 1, tabY, enemyPlayer, board, boardsize) && board.ptr[tabY].ptr[tabX - 1] == enemyStone) {
			simpleScore(tabX - 1, tabY, playerTurn, board, boardsize, whiteScore, blackScore);
		}
	}
	if (tabX < boardsize - 1) {
		if (isSurrounded(tabX + 1, tabY, enemyPlayer, board, boardsize) && board.ptr[tabY].ptr[tabX + 1] == enemyStone) {
			simpleScore(tabX + 1, tabY, playerTurn, board, boardsize, whiteScore, blackScore);
		}
	}
	if (tabY > 0) {
		if (isSurrounded(tabX, tabY - 1, enemyPlayer, board, boardsize) && board.ptr[tabY - 1].ptr[tabX] == enemyStone) {
			simpleScore(tabX, tabY - 1, playerTurn, board, boardsize, whiteScore, blackScore);
		}
	}
	if (tabY < boardsize - 1) {
		if (isSurrounded(tabX, tabY + 1, enemyPlayer, board, boardsize) && board.ptr[tabY + 1].ptr[tabX] == enemyStone) {
			simpleScore(tabX, tabY + 1, playerTurn, board, boardsize, whiteScore, blackScore);
		}
	}
}

//function eliminating surrounded stone and granting point to the right player
void simpleScore(int tabX, int tabY, int playerTurn, vectorV_t board, const int boardsize, int& whiteScore, int& blackScore) {
	board.ptr[tabY].ptr[tabX] = '.';
	if (playerTurn == 1) whiteScore++;
	else blackScore++;
}

PointKo findKoPoint(int tabX, int tabY, int playerTurn, vectorV_t board,const int boardsize) {
	int enemyPlayer;
	if (playerTurn == 1) {
		enemyPlayer = 2;
		board.ptr[tabY].ptr[tabX] = 'O';
	}
	else {
		enemyPlayer = 1;
		board.ptr[tabY].ptr[tabX] = 'X';
	}
	//checking if there will be only one captured stone because there can be only one PointKo
	int countControl{};
	//creating a point that will be forbidden for a player whose stone  has been captured in a previous turn
	PointKo result;
	result.player = enemyPlayer;
	result.x = 0;
	result.y = 0;

	if (tabX > 0) {
		if (isSurrounded(tabX - 1, tabY, enemyPlayer, board, boardsize) && board.ptr[tabY].ptr[tabX - 1] != '.') {
			result.x = tabX - 1;
			result.y = tabY;
			countControl++;
		}
	}

	if (tabX < boardsize - 1) {
		if (isSurrounded(tabX + 1, tabY, enemyPlayer, board, boardsize) && board.ptr[tabY].ptr[tabX + 1] != '.') {
			result.x = tabX + 1;
			result.y = tabY;
			countControl++;
		}
	}

	if (tabY > 0) {
		if (isSurrounded(tabX, tabY - 1, enemyPlayer, board, boardsize) && board.ptr[tabY - 1].ptr[tabX] != '.') {
			result.x = tabX;
			result.y = tabY - 1;
			countControl++;
		}
	}

	if (tabY < boardsize - 1) {
		if (isSurrounded(tabX, tabY + 1, enemyPlayer, board, boardsize) && board.ptr[tabY + 1].ptr[tabX] != '.') {
			result.x = tabX;
			result.y = tabY + 1;
			countControl++;
		}
	}
	board.ptr[tabY].ptr[tabX] = '.';

	if (countControl > 1 || countControl == 0) result.player = 0;

	return result;
}

int koRule(int tabX, int tabY, PointKo point, int playerTurn) {
	if (point.player == 0) return true;
	if (tabX == point.x && tabY == point.y && playerTurn == point.player) return 0;
	return 1;
}

//MULTIPLE CAPTURE

//implementation of a flood fill algorithm that removes enemy stones
void floodFill(int tabX, int tabY, int playerTurn, vectorV_t board, const int boardsize, vectorPt_t& visited, int& pomocnicza) {

	char  playerStone;
	if (playerTurn == 1) {
		playerStone = 'O';
	}
	else {
		playerStone = 'X';
	}

	if (tabX < 0 || tabY < 0 || tabX >= boardsize || tabY >= boardsize) return;
	if (board.ptr[tabY].ptr[tabX] == playerStone) return;
	if (board.ptr[tabY].ptr[tabX] == '?') return;
	if (board.ptr[tabY].ptr[tabX] == '.') {
		pomocnicza = 0;
		return;
	}

	Point point;
	point.x = tabX;
	point.y = tabY;
	board.ptr[tabY].ptr[tabX] = '?';
	push_back(&visited, point);

	floodFill(tabX - 1, tabY, playerTurn, board, boardsize, visited, pomocnicza);
	floodFill(tabX + 1, tabY, playerTurn, board, boardsize, visited, pomocnicza);
	floodFill(tabX, tabY - 1, playerTurn, board, boardsize, visited, pomocnicza);
	floodFill(tabX, tabY + 1, playerTurn, board, boardsize, visited, pomocnicza);
}

//function that determins if capturing multiple stones is possible
//if not, it will undo the floodFill function
//if yes, it will grant points to a certain player
void multipleCapture(int tabX, int tabY, int playerTurn, vectorV_t board, const int boardsize, int& whiteScore, int& blackScore) {
	vectorPt_t visited;
	init(&visited);

	int pomocnicza = 1;
	floodFill(tabX, tabY, playerTurn, board, boardsize, visited, pomocnicza);

	char enemyStone = (playerTurn == 1) ? 'X' : 'O';

	if (!pomocnicza) {
		for (int i = 0; i < visited.count; i++) {
			int x = visited.ptr[i].x, y = visited.ptr[i].y;
			board.ptr[y].ptr[x] = enemyStone;
		}
	}
	else {
		for (int i = 0; i < visited.count; i++) {
			int x = visited.ptr[i].x, y = visited.ptr[i].y;
			board.ptr[y].ptr[x] = '.';
		}
		if (playerTurn == 1) whiteScore += visited.count;
		else blackScore += visited.count;
	}
	delete_vector(&visited);

}
//calling multipleCapture at fields surrounding placed stone
void multipleScore(int tabX, int tabY, int playerTurn, vectorV_t board, const int boardsize, int& whiteScore, int& blackScore) {

	char enemyStone = (playerTurn == 1) ? 'X' : 'O';

	if (tabX - 1 >= 0 && board.ptr[tabY].ptr[tabX - 1] == enemyStone) multipleCapture(tabX - 1, tabY, playerTurn, board, boardsize, whiteScore, blackScore);
	if (tabX + 1 < boardsize && board.ptr[tabY].ptr[tabX + 1] == enemyStone) multipleCapture(tabX + 1, tabY, playerTurn, board, boardsize, whiteScore, blackScore);
	if (tabY - 1 >= 0 && board.ptr[tabY - 1].ptr[tabX] == enemyStone) multipleCapture(tabX, tabY - 1, playerTurn, board, boardsize, whiteScore, blackScore);
	if (tabY + 1 < boardsize && board.ptr[tabY + 1].ptr[tabX] == enemyStone) multipleCapture(tabX, tabY + 1, playerTurn, board, boardsize, whiteScore, blackScore);
}



