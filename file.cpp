#include <windows.h>
#include <iostream>
#include <clocale>
#include <cstdio>
#include <conio.h>
#include <cstring>

class Level {
public:
	static const int Size_Strings = 10;
	static const int Size_Columns = 20;
	char *name;
	char map [Size_Strings][Size_Columns];
	Level() {
		this->name = "Level";
		this->setLevel(1);
	}
	~Level() {
		//free(this->name);
	}
	bool setLevel(int level) {
		switch(level) {
		case 1:
			for (int i = 0; i < this->Size_Strings; i++) {
				for (int j = 0; j < this->Size_Columns; j++) {
					if (i == 0) {
						this->map[i][j] = 110;
					} else
					if (i == 1) {
						this->map[i][j] = 101;
					} else
					if (i == 2) {
						this->map[i][j] = 102;
					} else {
						this->map[i][j] = 100;
					}
				}
			}
			//free(this->name);
			this->name = "Level 1";
			printf("Level was created");
			return true;
			break;
		}
	return false;
	}

};
/*
class First_Level : Level {
public:
	First_Level() {
		this->name = "First_Level";
		
	}
};
*/
struct Platform {
	char symbol;
	char color;
	char length;
	COORD position;
	Platform() {
		this->symbol = 110;
		this->color = 10;
		this->length = 3;
		this->position.X = (Level::Size_Columns/2 - 1);
		this->position.Y = (Level::Size_Strings - 2);
	}
};

struct Ball {
	COORD position;
	COORD course;
	char symbol;
	Ball() {
		this->course.X = 0;
		this->course.Y = 0;
		this->position.X = (Level::Size_Columns/2);
		this->position.Y = (Level::Size_Strings - 3);
		this->symbol = 4;
	}
};
struct Game {
	int lifes;
	long points;
	int speed;
	char stopSymbol;
	char *saveStatus;
	Game() {
		stopSymbol = 113; //q
		lifes = 3;
		points = 0;
		speed = 10;
		saveStatus = "NEW";
	}
};

Level CurrentLevel;
Platform CurrentPlatform;	
Ball CurrentBall;
Game CurrentGame;

char config_file_name_ca[] = "config.cnf";
DWORD FILE_PATH_BUF_DW = 255;
char FILE_PATH_ca [255];


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

bool saveConfig()
{
	FILE *file_Fp;
	if ((file_Fp = fopen(config_file_name_ca, "w")) != NULL)
	{
		fprintf(file_Fp, "SPEED %i \n", CurrentGame.speed);
		fprintf(file_Fp, "LIFES %i \n", CurrentGame.lifes);
		fprintf(file_Fp, "POINTS %i \n", CurrentGame.points);
		fprintf(file_Fp, "BALL_position %i %i \n", CurrentBall.position.X, CurrentBall.position.Y);
		fprintf(file_Fp, "BALL_course %i %i \n", CurrentBall.course.X, CurrentBall.course.Y);
		fprintf(file_Fp, "PLATFORM_position %i %i \n", CurrentPlatform.position.X, CurrentPlatform.position.Y);
		fprintf(file_Fp, "LEVEL %s \n", CurrentLevel.name);
		fprintf(file_Fp, "STATUS %s \n", CurrentGame.saveStatus);
		for (int i = 0; i < CurrentLevel.Size_Strings; i++) 
		{
			for (int j = 0; j < CurrentLevel.Size_Columns; j++) 
			{
				fprintf(file_Fp, "%c ", CurrentLevel.map[i][j]);
			}
			fprintf(file_Fp, "\n");
		}
		fclose(file_Fp);
		return true;
	} else {

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
		fscanf(file_Fp, "", &CurrentGame.speed);
		fscanf(file_Fp, "", CurrentGame.lifes);
		fscanf(file_Fp, "", CurrentGame.points);
		fscanf(file_Fp, "", CurrentBall.position.X, CurrentBall.position.Y);
		fscanf(file_Fp, "", CurrentBall.course.X, CurrentBall.course.Y);
		fscanf(file_Fp, "", CurrentPlatform.position.X, CurrentPlatform.position.Y);
		fscanf(file_Fp, "", CurrentLevel.name);
		fscanf(file_Fp, "", CurrentGame.saveStatus);
		if (CurrentGame.saveStatus == "LOAD") {
			for (int i = 0; i < Level::Size_Strings; i++) {
				for (int j = 0; j < Level::Size_Columns; j++) {
					fscanf(file_Fp, "%i", CurrentLevel.map[i][j]);
				}
			}	
		}

		fclose(file_Fp);
		return true;
	} else {
		if (createConfig()) {
			if ((file_Fp = fopen(config_file_name_ca, "r")) != NULL) 
			{
				// считывание!!!


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
	setlocale(LC_CTYPE, "RUS");
	//GetCurrentDirectoryA(FILE_PATH_BUF_DW, FILE_PATH_ca);
	//printf(FILE_PATH_ca);
	saveConfig();

	system("pause");
	return 0;
}
