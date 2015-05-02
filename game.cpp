#include <windows.h>
#include <iostream>
#include <conio.h>
#include <ctime>



#define h_MatrixSize_i 8 // horizontal size
#define v_MatrixSize_i 8 // vertical size
char current_PATH_ca[255];
DWORD buf_length = 255;


HANDLE hConsole; //console handler

void getCurrentPath(){
	GetCurrentDirectoryA(buf_length, current_PATH_ca);

}

void goToXY(int col, int row) { // come to position
	
	COORD coord; //coordinate
	coord.X = col;
	coord.Y = row;

	HANDLE hConsoleComeTo = GetStdHandle(STD_OUTPUT_HANDLE); // catch a handler
	SetConsoleCursorPosition(hConsoleComeTo, coord); //set cursor's position

}

struct hero { //our manipulator

	char face; //it will be shown
	int X, Y;

} man;


char boardMatrix[h_MatrixSize_i][v_MatrixSize_i] = // this is our placement
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

void render() { //our painter
	goToXY(0,0);// at first - come to 0,0
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
	setlocale(LC_CTYPE, "RUS");

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 14);

	getCurrentPath();
	printf("%s", current_PATH_ca);
	system("pause");

	system("cls");

	man.face = 110;
	man.X = 0;
	man.Y = 0;
	//char readSymbol = 'd';
	int IR1 = 0;
	int IR2 = 0;
		render();
	while (true) {		
//		render();
		if (_kbhit()) {
			IR1 = _getch();
			if (IR1 == 224) {
				IR2 = _getch();
				if (IR2 == 75) {
					if (man.X > 0)
					{
						man.X--;
						render();	
					}
					
					continue;
				}
				if (IR2 == 77) {
					if (man.X < (h_MatrixSize_i - 1))
					{
						man.X++;
						render();
					}
					continue;
				}
				if (IR2 == 72) {
					if (man.Y > 0) 
					{
						man.Y--;
						render();
					}
					continue;
				}
				if (IR2 == 80) {
					if (man.Y < (v_MatrixSize_i - 1)) 
					{
						man.Y++;	
						render();
					}
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
