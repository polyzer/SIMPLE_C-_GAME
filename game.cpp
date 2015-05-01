#include <windows.h>
#include <iostream>
#include <conio.h>
#include <ctime>

#define h_MatrixSize_i 8
#define v_MatrixSize_i 8

HANDLE hConsole;

void goToXY(int col, int row) {
	
	COORD coord;
	coord.X = col;
	coord.Y = row;

	HANDLE hConsoleComeTo = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleComeTo, coord);

}

struct hero {

	char face;
	int X, Y;

} man;

char boardMatrix[h_MatrixSize_i][v_MatrixSize_i] = 
{
	{100, 100, 100, 100, 100, 100, 100, 100},
	{100, 100, 100, 100, 100, 100, 100, 100},
	{100, 100, 100, 100, 100, 100, 100, 100},
	{100, 100, 100, 100, 100, 100, 100, 100},
	{100, 100, 100, 100, 100, 100, 100, 100},
	{100, 100, 100, 100, 100, 100, 100, 100},
	{100, 100, 100, 100, 100, 100, 100, 100},
	{100, 100, 100, 100, 100, 100, 100, 100}

};

//char keyPressed;

void render() { //функция рисования
	goToXY(0,0);
	for (int i=0; i<h_MatrixSize_i; i++)
	{
		for (int j=0; j<v_MatrixSize_i; j++) 
		{
			if (man.X == j && man.Y == i)
			{
				printf("%c", man.face);
				continue;
			}
			else
				printf("%c", boardMatrix[i][j]);
		}
		printf("\n");
	}

	
}

int main ( int argc, char **argv[]) 
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 14);

	man.face = 110;
	man.X = 0;
	man.Y = 0;
	//char readSymbol = 'd';
	int IR1 = 0;
	int IR2 = 0;
	while (true) {		
		render();
		if (_kbhit()) {
			IR1 = _getch();
			if (IR1 == 224) {
				IR2 = _getch();
				if (IR2 == 75) {
					man.X--;
					continue;
				}
				if (IR2 == 77) {
					man.X++;
					continue;
				}
				if (IR2 == 72) {
					man.Y--;
					continue;
				}
				if (IR2 == 80) {
					man.Y++;	
					continue;
				}
			}
			if (IR1 == 27) {
				break;
			}
		}
	}

	system("pause");
	
	return 0;
}
