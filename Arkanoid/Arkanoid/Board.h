#pragma once
#include <iostream>
#include <Windows.h>

struct plataforma {
	int posX;
	int posY;
}plat;

struct ball {
	int posX;
	int posY;
}pelota;

struct tablero {
     int NUM_ROWS = 20;
	 int NUM_COLUMNS = 20;
	 int FILAS_BLOQUES = 5;
}tabl;

char board[99][99];

bool tieneLadrillo[99][99];

void setColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

}

void initializeBoard(){
	for (int i = 0; i < tabl.FILAS_BLOQUES; i++) {
		for (int j = 0; j < tabl.NUM_COLUMNS; j++) {
			tieneLadrillo[i][j] = true;
		}
	}
	for (int i = 0; i < tabl.NUM_ROWS; i++) {
		for (int j = 0; j < tabl.NUM_COLUMNS; j++) {
			board[i][j] = ' ';
		}
	}
	for (int i = 0; i < tabl.NUM_ROWS; i++) {
		for (int j = 0; j < tabl.NUM_COLUMNS; j++){
			if(i < tabl.FILAS_BLOQUES){
				board[i][j] = '@';
			}
			else {
				if (i == plat.posY && j == plat.posX) {
					board[i][j - 1] = '-';
					board[i][j] = '-';
					board[i][j + 1] = '-';
				}
				else if (i == pelota.posY && j == pelota.posX) {
					board[i][j] = '*';
				}
				else if (board[i][j] != '-') {
					board[i][j] = ' ';
				}
			}
		}

	}
}

void updateBoard() {
	for (int i = 0; i < tabl.NUM_ROWS; i++) {
		for (int j = 0; j < tabl.NUM_COLUMNS; j++) {
			board[i][j] = ' ';
		}
	}
	for (int i = 0; i < tabl.NUM_ROWS; i++) {
		for (int j = 0; j < tabl.NUM_COLUMNS; j++) {
			if (i < tabl.FILAS_BLOQUES && tieneLadrillo[i][j] == true) {
				board[i][j] = '@';
			}
			else {
				if (i == plat.posY && j == plat.posX) {
					board[i][j - 1] = '-';
					board[i][j] = '-';
					board[i][j + 1] = '-';
				}
				else if (i == pelota.posY && j == pelota.posX) {
					board[i][j] = '*';
				}
				else if (board[i][j] != '-') {
					board[i][j] = ' ';
				}
			}
		}

	}
}

void visualizeBoard() {
	int color = 2;
	setColor(2);
	for (int z = 0; z <= tabl.NUM_COLUMNS; z++) {
		std::cout << "_";
	}
	std::cout << std::endl;
	for (int i = 0; i < tabl.NUM_ROWS; i++) {
		std::cout << "|";
		for (int j = 0; j < tabl.NUM_COLUMNS; j++) {
			if (board[i][j] == '@') {
				color = 5;
			}
			else if (board[i][j] == '*') {
				color = 4;
			}
			else if (board[i][j] == '-') {
				color = 6;
			}
			setColor(color);
			std::cout << board[i][j];
		}
		setColor(2);
		std::cout << "|" << std::endl;
	}
	
	for (int z = 0; z <= tabl.NUM_COLUMNS; z++) {
		std::cout << "_";
	}
	std::cout << std::endl;
}

void movePlatform(int mov) {
	if (mov == -1 && plat.posX > 1) {
		plat.posX = plat.posX - 1;
	}
	if (mov == 1 && plat.posX < tabl.NUM_COLUMNS - 2) {
		plat.posX = plat.posX + 1;
	}
}

bool hayLadrillo(int x,int y) {
	return board[y][x] == '@';
}

void eliminarLadrillo(int x, int y) {

	tieneLadrillo[x][y] = false;
}

bool hayPlataforma(int x, int y) {
	return board[y][x] == '-';
}

bool existenBloques() {
	bool existen = false;
	for (int i = 0; i < tabl.FILAS_BLOQUES; i++) {
		for (int j = 0; j < tabl.NUM_COLUMNS; j++) {
			if (tieneLadrillo[i][j] == true) {
				existen = true;
				break;
			}
		}
	}
	return existen;
}

