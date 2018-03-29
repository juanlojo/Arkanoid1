#include "Board.h"
#include <fstream>
#include <cstdio>
#include <Windows.h>
#include <iostream>

HANDLE hStdin;
DWORD cNumRead, FdwMode, FdwOldMode;
DWORD c = 0;
INPUT_RECORD Buffer[128];
KEY_EVENT_RECORD ker;

void setupKeyboard();
int movePlayer();
int moveBall();
int sx = 1;
int sy = -1;

int main() {
	tabl.FILAS_BLOQUES = 5;
	tabl.NUM_COLUMNS = 20;
	tabl.NUM_ROWS = 20;

	std::ifstream fichero("config.txt");
	char linea[128];
	char parametro[4];
	int j = 0;
	int contParametro = 0;
	while (!fichero.eof()) {
		fichero >> linea;
		for (int i = 0; i < strlen(linea); i++) {
			if (linea[i] != ';') {
				parametro[j] = linea[i];
				j++;
			}
			else {
				parametro[j] = '\0';
				j = 0;
				if (contParametro == 0) {
					tabl.NUM_ROWS = atoi(parametro);
				}
				else if (contParametro == 1) {
					tabl.NUM_COLUMNS = atoi(parametro);
				}
				else if (contParametro == 2) {
					tabl.FILAS_BLOQUES = atoi(parametro);
				}
				contParametro++;
			}
		}
	}
	std::cout << tabl.NUM_ROWS << std::endl;
	std::cout << tabl.NUM_COLUMNS << std::endl;
	std::cout << tabl.FILAS_BLOQUES << std::endl;

	plat.posX = tabl.NUM_COLUMNS / 2;
	plat.posY = tabl.NUM_ROWS - 4;

	pelota.posX = plat.posX;
	pelota.posY = plat.posY - 1;

	setupKeyboard();
	initializeBoard();
	visualizeBoard();
	int tecla = 3;
	while (tecla != 0) {
		tecla = movePlayer();
		if (tecla == -1 || tecla == 1) {
			moveBall();
			system("cls");
			updateBoard();
			visualizeBoard();
			movePlatform(tecla);
			system("cls");
			updateBoard();
			visualizeBoard();
			if (!existenBloques()) {
				std::cout << "HAS GANADO" << std::endl;
				break;
			}
		}
	}
	system("pause");
	return 0;
}

void setupKeyboard() {
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE) {
		std::cout << "Error" << std::endl;
	}
	if (!GetConsoleMode(hStdin, &FdwOldMode)) {
		std::cout << "Error" << std::endl;
	}
	FdwMode = ENABLE_WINDOW_INPUT;
	if (!SetConsoleMode(hStdin, FdwMode)) {
		std::cout << "Error" << std::endl;
	}
}

//Devuelve 0 si pulsa ESC, -1 si pulsa izquierda, 1 si pulsa derecha

int movePlayer() {
	int tecla = 3;
	GetNumberOfConsoleInputEvents(hStdin, &c);
	if (c > 0) {
		ReadConsoleInput(hStdin, Buffer, 128, &cNumRead);
		for (int i = 0; i < (int)cNumRead; i++) {
			switch (Buffer->EventType)
			{
			case KEY_EVENT:
				ker = Buffer->Event.KeyEvent;
				if (ker.bKeyDown) {
					int code = (int)ker.wVirtualKeyCode;
					if (ker.wVirtualKeyCode == VK_LEFT) {
						tecla = -1;
					}
					if (ker.wVirtualKeyCode == VK_RIGHT) {
						tecla = 1;
					}	
					if (ker.wVirtualKeyCode == VK_ESCAPE) {
						tecla = 0;
					}
				}
				break;
			default:
				break;
			}
		}
	}
	return tecla;
}

int moveBall() {
	if (pelota.posX + sx > tabl.NUM_COLUMNS - 1 || pelota.posX + sx < 0) {
		sx *= -1;
	}
	if (pelota.posY + sy < 0) {
		sy *= -1;
	}
	if (pelota.posY + sy > tabl.NUM_ROWS - 1) {
		std::cout << "GAME OVER";
		system("pause");
		return false;
	}
	if (hayLadrillo(pelota.posX + sx, pelota.posY + sy)) {
		eliminarLadrillo(pelota.posY + sy, pelota.posX + sx);
		sy *= -1;
	}
	if (hayPlataforma(pelota.posX + sx, pelota.posY + sy)) {
		sy *= -1;
		if (pelota.posX + sx == plat.posX) {
			sx = 0;
		}
		else if (pelota.posX + sx == plat.posX + 1) {
			sx += 1;
		}
		else if (pelota.posX + sx == plat.posX + 1) {
			sx += -1;
		}
	}
	pelota.posX += sx;
	pelota.posY += sy;
	Sleep(60);
}

