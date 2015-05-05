#include <windows.h>
#include <iostream>
#include <clocale>
#include <cstdio>
#include <conio.h>
#include <cstring>

struct Level {
public:
	static const int Size_Strings = 10;
	static const int Size_Columns = 20;
	int number;
	char map [Size_Strings][Size_Columns];
	Level() {
		this->number = 1;
		this->loadLevel(1);
	}
	~Level() {
		//free(this->name);
	}
	bool loadLevel(int level);

};

struct Platform {
	char symbol;
	char color;
	char length;
	COORD position;
	Platform() {
		this->setStandard();
	}

	void setStandard() {
		this->symbol = 110;
		this->color = 10;
		this->length = 3;
		this->position.X = (Level::Size_Columns/2 - 1);
		this->position.Y = (Level::Size_Strings - 2);
		
	}

	void setPosition(COORD pos) {
		this->position.X = pos.X;
		this->position.Y = pos.Y;
	}
	void setColor(int col) {
		this->color = col;
	}
};

struct Ball {
	COORD position;
	COORD course;
	char symbol;
	int color;
	Ball() {
		this->setStandard();
	}

	void setStandard() {
		this->course.X = 1; // 1 - вправо, -1 - влево
		this->course.Y = 1;// 1 - вверх, -1 - вниз
		this->position.X = (Level::Size_Columns/2);
		this->position.Y = (Level::Size_Strings - 3);
		this->symbol = 4;		
	}

	void setPosition(COORD pos) {
		this->position.X = pos.X;
		this->position.Y = pos.Y;
	}
	void setColor(int col) {
		this->color = col;
	}
	void setCourse(int side); //1 - вправо, 0 - влево
	void collision();


	}

};
struct Game {
	int lifes;
	long points;
	int speed;
	char stopSymbol;
	int saveStatus;
	Game() {
		this->setStandard();
	}

	void setStandard() {
		stopSymbol = 113; //q
		lifes = 3;
		points = 0;
		speed = 10;
		saveStatus = 0; //0 - new, 1 - load
	
	}

	void increasePoints(int add);
	void upSpeed();
	void reduceLifes();

};

Level CurrentLevel;
Platform CurrentPlatform;	
Ball CurrentBall;
Game CurrentGame;
HANDLE hConsole;

char config_file_name_ca[] = "config.cnf";
DWORD FILE_PATH_BUF_DW = 255;
char FILE_PATH_ca [255];


void goToXY(int col, int row) 
{ // come to position
	
	COORD coord; //coordinate
	coord.X = col;
	coord.Y = row;

	HANDLE hConsoleComeTo = GetStdHandle(STD_OUTPUT_HANDLE); // catch a handler
	SetConsoleCursorPosition(hConsoleComeTo, coord); //set cursor's position

}

void setElementColor(){

	SetConsoleTextAttribute(hConsole, 14);

}



void render() { //our painter
	goToXY(0,0);// at first - come to 0,0
	for (int i=0; i<Level::Size_Strings; i++)
	{
		for (int j=0; j<Level::Size_Columns; j++) 
		{
			if (CurrentPlatform.position.X == j && CurrentPlatform.position.Y == i)
			{
				for(int k = 0; k < CurrentPlatform.length; k++) 
					printf("%c", CurrentPlatform.symbol);
				continue;
			}
			else {
				printf("%c", boardMatrix[i][j]);
			}
		}
		printf("\n");
	}

	
}



bool createConfig() 
{
	FILE *config_Fp;
	if ((config_Fp = fopen(config_file_name_ca, "w")) != NULL)
	{
		printf("Конфигурационный файл создан!");
		return true;
	} else {
		printf("Не удалось создать конфигурационный файл!!!!");
		return false;
	}


}

void printGame() {
	printf("%i \n", CurrentGame.speed);
	printf("%i \n", CurrentGame.lifes);
	printf("%i \n", CurrentGame.points);
	printf("%i %i \n", CurrentBall.position.X, CurrentBall.position.Y);
	printf("%i %i \n", CurrentBall.course.X, CurrentBall.course.Y);
	printf("%i %i \n", CurrentPlatform.position.X, CurrentPlatform.position.Y);
	printf("%i \n", CurrentLevel.number);
	printf("%i \n", CurrentGame.saveStatus);
	for (int i = 0; i < CurrentLevel.Size_Strings; i++) 
	{
		for (int j = 0; j < CurrentLevel.Size_Columns; j++) 
		{
			printf("%c ", CurrentLevel.map[i][j]);
		}
		printf("\n");
	}
		
}

bool saveConfig()
{
	FILE *file_Fp;
	if ((file_Fp = fopen(config_file_name_ca, "w")) != NULL)
	{
		fprintf(file_Fp, "%i \n", CurrentGame.speed);
		fprintf(file_Fp, "%i \n", CurrentGame.lifes);
		fprintf(file_Fp, "%i \n", CurrentGame.points);
		fprintf(file_Fp, "%i %i \n", CurrentBall.position.X, CurrentBall.position.Y);
		fprintf(file_Fp, "%i %i \n", CurrentBall.course.X, CurrentBall.course.Y);
		fprintf(file_Fp, "%i %i \n", CurrentPlatform.position.X, CurrentPlatform.position.Y);
		fprintf(file_Fp, "%i \n", CurrentLevel.number);
		fprintf(file_Fp, "%i \n", CurrentGame.saveStatus);
		for (int i = 0; i < CurrentLevel.Size_Strings; i++) 
		{
			for (int j = 0; j < CurrentLevel.Size_Columns; j++) 
			{
				fprintf(file_Fp, "%c ", CurrentLevel.map[i][j]);
			}
			fprintf(file_Fp, "\n");
		}
		fclose(file_Fp);
		system("cls");
		printf("Сессия сохранена!");
		return true;
	} else {

		system("cls");
		printf("Не удается записать конфигурационные данные!!!");
		fclose(file_Fp);
		return false;
	}
}

bool readConfig()
{
	FILE *file_Fp;
	if ((file_Fp = fopen(config_file_name_ca, "r")) != NULL)
	{
		//Считывание!!!
		fscanf(file_Fp, "%i", &CurrentGame.speed);
		fscanf(file_Fp, "%i", &CurrentGame.lifes);
		fscanf(file_Fp, "%i", &CurrentGame.points);
		fscanf(file_Fp, "%i %i", &CurrentBall.position.X, &CurrentBall.position.Y);
		fscanf(file_Fp, "%i %i", &CurrentBall.course.X, &CurrentBall.course.Y);
		fscanf(file_Fp, "%i %i", &CurrentPlatform.position.X, &CurrentPlatform.position.Y);
		fscanf(file_Fp, "%i", &CurrentLevel.number);
		fscanf(file_Fp, "%i", &CurrentGame.saveStatus);
		if (CurrentGame.saveStatus == 1) {
			for (int i = 0; i < Level::Size_Strings; i++) {
				for (int j = 0; j < Level::Size_Columns; j++) {
					fscanf(file_Fp, "%c", CurrentLevel.map[i][j]);
				}
			}	
			printf("Загружена прошлая игра!");
		} else if (CurrentGame.saveStatus == 0) {
			CurrentLevel.loadLevel(CurrentLevel.number);
			printf("Новая игра!");
		}
		fclose(file_Fp);
		return true;
	} else {
		if (createConfig()) {
			if ((file_Fp = fopen(config_file_name_ca, "r")) != NULL) 
			{
				// считывание!!!
				fscanf(file_Fp, "%i", &CurrentGame.speed);
				fscanf(file_Fp, "%i", &CurrentGame.lifes);
				fscanf(file_Fp, "%i", &CurrentGame.points);
				fscanf(file_Fp, "%i %i", &CurrentBall.position.X, &CurrentBall.position.Y);
				fscanf(file_Fp, "%i %i", &CurrentBall.course.X, &CurrentBall.course.Y);
				fscanf(file_Fp, "%i %i", &CurrentPlatform.position.X, &CurrentPlatform.position.Y);
				fscanf(file_Fp, "%i", &CurrentLevel.number);
				fscanf(file_Fp, "%i", &CurrentGame.saveStatus);
				if (CurrentGame.saveStatus == 1) {
					for (int i = 0; i < Level::Size_Strings; i++) {
						for (int j = 0; j < Level::Size_Columns; j++) {
							fscanf(file_Fp, "%c", CurrentLevel.map[i][j]);
						}
					}	
				} else if (CurrentGame.saveStatus == 0) {
					CurrentLevel.loadLevel(CurrentLevel.number);
					printf("Новая игра!");
				}
				fclose(file_Fp);
				return true;
			} else {
				printf("Конфигурационный файл существует, но из него невозможно считать данные!");
				fclose(file_Fp);
				return false;
			}
		} else {
			printf("Не удается записать и прочитать данные!");
			return false;
		}

	}
}



int main (int argc, char **argv[]) 
{
	setlocale(LC_ALL, "Russian");
	//GetCurrentDirectoryA(FILE_PATH_BUF_DW, FILE_PATH_ca);
	//printf(FILE_PATH_ca);
	saveConfig();
	if (readConfig()) {
		printGame();
		system("pause");
		system("cls");
		saveConfig();
	}

	system("pause");
	return 0;
}





bool Level::loadLevel(int level) {
		switch(level) {
		case 1:
			for (int i = 0; i < this->Size_Strings; i++) {
				for (int j = 0; j < this->Size_Columns; j++) {
					if (i == 0) {
						this->map[i][j] = 110;
					} else
					if (i == 1) {
						this->map[i][j] = 103;
					} else
					if (i == 2) {
						this->map[i][j] = 102;
					} else {
						this->map[i][j] = 32;
					}
				}
			}
			//free(this->name);
			this->number = 1;
			CurrentBall.position.X = 0;
			CurrentGame.setStandard();
			CurrentPlatform.setStandard();

			printf("Level was created");
			return true;
		break;
		case 2:
			for (int i = 0; i < this->Size_Strings; i++) {
				for (int j = 0; j < this->Size_Columns; j++) {
					if (i == 0) {
						this->map[i][j] = 111;
					} else
					if (i == 1) {
						this->map[i][j] = 101;
					} else
					if (i == 2) {
						this->map[i][j] = 107;
					} else {
						this->map[i][j] = 32;
					}
				}
			}
			//free(this->name);
			this->number = 1;
			CurrentBall.position.X = 0;
			CurrentGame.setStandard();
			CurrentPlatform.setStandard();

			printf("Level was created");
			return true;
		break;
		}
	return false;
}

	
void Ball::collision() {

	if ((this->position.X >= CurrentPlatform.position.X && 
		 this->position.X <= (CurrentPlatform.position.X + CurrentPlatform.length)) &&
		 (this->position.Y == (CurrentPlatform.position.X - 1) || 
		  this->position.Y == (CurrentPlatform.position.X - 1)
		 )
	   ) 
	{
		this->course.Y = -(this->course.Y);
	}
	if ((this->position.X == (CurrentPlatform.position.X - 1) || 
		 this->position.X == (CurrentPlatform.position.X - 1)
		) && this->position.Y == CurrentPlatform.position.Y
	) {
		this->course.X = -(this->course.X);
	}
	if(
		(((this->position.X - 1) == (CurrentPlatform.position.X - 1)) ||
		 ((this->position.X + 1) == (CurrentPlatform.position.X + CurrentPlatform.length))
		) &&  (((this->position.Y - 1) == (CurrentPlatform.position.Y - 1)) ||
		 ((this->position.Y + 1) == (CurrentPlatform.position.X + CurrentPlatform.length))
		)
	) {
		this->course.X = -(this->course.X);
		this->course.Y = -(this->course.Y);
	}
	//Столкновение с блоками
	if (CurrentLevel.map[this->position.Y + 1][this->position.X] != 32) {
		CurrentLevel.map[this->position.Y + 1][this->position.X] = 32;
		this->course.Y = -(this->course.Y);
	}
	if (CurrentLevel.map[this->position.Y][this->position.X + 1] != 32) {
		CurrentLevel.map[this->position.Y][this->position.X + 1] = 32;
		this->course.X = -(this->course.X);
	}
	if (CurrentLevel.map[this->position.Y - 1][this->position.X] != 32) {
		CurrentLevel.map[this->position.Y - 1][this->position.X] = 32;
		this->course.Y = -(this->course.Y);
	}
	if (CurrentLevel.map[this->position.Y][this->position.X - 1] != 32) {
		CurrentLevel.map[this->position.Y][this->position.X - 1] = 32;
		this->course.X = -(this->course.X);
	}
	//Конец обработки столкновений с блоками

}
