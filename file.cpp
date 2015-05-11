#include <windows.h>
#include <iostream>
#include <clocale>
#include <cstdio>
#include <conio.h>
#include <cstring>

struct pos {
	int X;
	int Y;
};

struct Level {
public:
	static const int Size_Strings = 10; // размер по Y
	static const int Size_Columns = 20; //размер по X
	int number; // номер уровня
	char back; // background
	char map [Size_Strings][Size_Columns]; // карта уровня
	int minSpeedTime; //начальная скорость шара на уровне 
	int maxSpeedTime; //максимальная скорость шара на уровне
	int stepNorm; // количество шагов шара для изменения скорости
	Level() {
		this->setStandard();
	}
	~Level() {
		//free(this->name);
	}
	void setStandard() {
		this->number = 1; // устанавливаем первый уровень
		this->loadLevel(this->number); // вначале грузим первый уровень
		this->back = 32;
		this->minSpeedTime = 30;
		this->maxSpeedTime = 7;
		this->stepNorm = 4;
	}
	bool loadLevel(int level); //загружает уровень в зависимости от номера
	void End(bool status); // отслеживает окончание уровня
	bool allBlocksDestroyed(); // отслеживает уничтожение блоков 
};

struct Platform {
	char symbol; // символы используемые в качестве платформы
	char color; //цвет платформы
	char length; //длина платформы
	pos position; // координаты левого конца
	Platform() {
		this->setStandard(); //
	}

	void setStandard() { // установка начальных параметров платформы
		this->symbol = 110; //
		this->color = 10; //
		this->length = 3; //
		this->position.X = (Level::Size_Columns/2 - 1); // позиция о X
		this->position.Y = (Level::Size_Strings - 2); // позиция по Y
	}

	void setPosition(pos pos) {
		this->position.X = pos.X;
		this->position.Y = pos.Y;
	}
	void setColor(int col) {
		this->color = col;
	}
	bool moveControl(int control);//Глобальная функция проверки перемещения
	bool outOfSize(int control);//Проверка выхода за границы экрана
	bool blockCollision(int control);//Проверка столкновения с блоком
	bool ballCollision(int control);//Проверка столкновения с мячом
	void step(int course); //перемещение
};

struct Ball {
	pos position; // позиция мяча
	pos course; // направление мяча
	char symbol; //цвет символа
	int color; //цвет мяча
	int speed; // текущая скорость
	int timer; //количество обновлений
	int stepNum; //счетчик шагов

	Ball() {
		this->setStandard();
	}

	void setStandard();

	void setPosition(pos pos) {
		this->position.X = pos.X;
		this->position.Y = pos.Y;
	}
	void setColor(int col) {
		this->color = col;
	}
	void step(); // шаг мяча
	void setCourse(int side); //1 - вправо, 0 - влево
	void collision(); //столкновения и выход за границы окна
	void speedControl(); //контроль скорости
	void speedUp(int spd);
};

struct Game {
	int lifes; //жизни
	long points; // очки игрока
	int speed; // текущая скорость игры
	int maxSpeed; // максимальная скорость игры
	int minSpeed; // начальная скорость игры
	char stopSymbol; // символ, по нажатию на который игра прерывается!
	char pauseSymbol; // символ паузы!
	int saveStatus; // статус сохранения: 0 - новая игра, 1 - загрузка
	int FPS; //количество кадров
	Game() {
		this->setStandard();
	}

	void setStandard() { // устанавливает начальные значения
		this->FPS = 30;
		minSpeed = 1000;
		maxSpeed = 50;
		stopSymbol = 113; //символ оканчивающий игру
		pauseSymbol = 32;
		lifes = 3;
		points = 0;
		speed = minSpeed;
		saveStatus = 0; //0 - new, 1 - load
	}

	void increasePoints(char c); // увеличивает очки в зависимости от элемента
	void speedUp(int spd); // изменяет скорость
	void setLifes(); //изменяет количество попыток
	void render(); //рисует все
	void destroyBlock(int y, int x); // обработка уничтожения блоков
	void printInfo();
	void Start(); // Начало игры
	void End(); //Конец уровня!
};

Level CurrentLevel; // объект уровня
Platform CurrentPlatform;// объект платформы
Game CurrentGame; // объект текущей игры
Ball CurrentBall; //объект мяча
HANDLE hConsole; // обработчик

char config_file_name_ca[] = "config.cnf"; //файл конфигурации
//DWORD FILE_PATH_BUF_DW = 255;
//char FILE_PATH_ca [255];


void goToXY(int col, int row) //перемещение на нужную позицию
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






bool createConfig() //создание файла концигурации
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

void printGame() { // выводит на экран текущие показатели
	printf("%i \n", CurrentGame.lifes);
	printf("%i \n", CurrentGame.points);
	printf("%i \n", CurrentBall.speed);
	printf("%i \n", CurrentBall.timer);
	printf("%i \n", CurrentBall.stepNum);
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

bool saveConfig() // сохранение концигурации при выходе
{
	FILE *file_Fp;
	if ((file_Fp = fopen(config_file_name_ca, "w")) != NULL)
	{
		fprintf(file_Fp, "%i \n", CurrentGame.lifes);
		fprintf(file_Fp, "%i \n", CurrentGame.points);
		fprintf(file_Fp, "%i \n", CurrentBall.speed);
		fprintf(file_Fp, "%i \n", CurrentBall.timer);
		fprintf(file_Fp, "%i \n", CurrentBall.stepNum);
		fprintf(file_Fp, "%i %i \n", CurrentBall.position.X, CurrentBall.position.Y);
		fprintf(file_Fp, "%i %i \n", CurrentBall.course.X, CurrentBall.course.Y);
		fprintf(file_Fp, "%i %i \n", CurrentPlatform.position.X, CurrentPlatform.position.Y);
		fprintf(file_Fp, "%i \n", CurrentLevel.number);
		fprintf(file_Fp, "%i \n", CurrentGame.saveStatus);
		for (int i = 0; i < CurrentLevel.Size_Strings; i++) 
		{
			for (int j = 0; j < CurrentLevel.Size_Columns; j++) 
			{
				fprintf(file_Fp, "%i ", CurrentLevel.map[i][j]);
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

bool readConfig() // чтение и загрузка конфигурации
{
	FILE *file_Fp;
	if ((file_Fp = fopen(config_file_name_ca, "r")) != NULL)
	{
		//Считывание!!!
		fscanf(file_Fp, "%i", &CurrentGame.lifes);
		fscanf(file_Fp, "%i", &CurrentGame.points);
		fscanf(file_Fp, "%i", &CurrentBall.speed);
		fscanf(file_Fp, "%i", &CurrentBall.timer);
		fscanf(file_Fp, "%i", &CurrentBall.stepNum);
		fscanf(file_Fp, "%i %i", &CurrentBall.position.X, &CurrentBall.position.Y);
		fscanf(file_Fp, "%i %i", &CurrentBall.course.X, &CurrentBall.course.Y);
		fscanf(file_Fp, "%i %i", &CurrentPlatform.position.X, &CurrentPlatform.position.Y);
		fscanf(file_Fp, "%i", &CurrentLevel.number);
		fscanf(file_Fp, "%i", &CurrentGame.saveStatus);
		if (CurrentGame.saveStatus == 1) {
			for (int i = 0; i < Level::Size_Strings; i++) {
				for (int j = 0; j < Level::Size_Columns; j++) {
					fscanf(file_Fp, "%i", &CurrentLevel.map[i][j]);
				}
			}	
			printf("Загружена прошлая игра!\n");
			system("pause");
			system("cls");
		} else if (CurrentGame.saveStatus == 0) {
			CurrentLevel.loadLevel(CurrentLevel.number);
			//printf("Новая игра!");
		}
		fclose(file_Fp);
		return true;
	} else {
		if (createConfig()) {
			if ((file_Fp = fopen(config_file_name_ca, "r")) != NULL) 
			{
				// считывание!!!
				fscanf(file_Fp, "%i", &CurrentGame.lifes);
				fscanf(file_Fp, "%i", &CurrentGame.points);	
				fscanf(file_Fp, "%i", &CurrentBall.speed);
				fscanf(file_Fp, "%i", &CurrentBall.timer);
				fscanf(file_Fp, "%i", &CurrentBall.stepNum);
				fscanf(file_Fp, "%i %i", &CurrentBall.position.X, &CurrentBall.position.Y);
				fscanf(file_Fp, "%i %i", &CurrentBall.course.X, &CurrentBall.course.Y);
				fscanf(file_Fp, "%i %i", &CurrentPlatform.position.X, &CurrentPlatform.position.Y);
				fscanf(file_Fp, "%i", &CurrentLevel.number);
				fscanf(file_Fp, "%i", &CurrentGame.saveStatus);
				if (CurrentGame.saveStatus == 1) {
					for (int i = 0; i < Level::Size_Strings; i++) {
						for (int j = 0; j < Level::Size_Columns; j++) {
							fscanf(file_Fp, "%i", &CurrentLevel.map[i][j]);
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
///////
///////
/////////			MAIN FUNCTION
//////////

int main (int argc, char **argv[]) 
{
	setlocale(LC_ALL, "Russian");

	//saveConfig();
	readConfig();
	CurrentGame.Start();
	return 0;
}



///////
//////			LEVEL FUNCTIONS
//////////
///////////
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
			CurrentBall.setStandard();
			CurrentGame.setStandard();
			CurrentPlatform.setStandard();

			//printf("Level was created");
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
			this->number = 2;
			CurrentBall.position.X = 0;
			CurrentGame.setStandard();
			CurrentPlatform.setStandard();
			printf("Level was created");
			return true;
		break;
		default:
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
			CurrentBall.setStandard();
			CurrentGame.setStandard();
			CurrentPlatform.setStandard();
			//printf("Level was created");
			return true;
		break;
		}
	return false;
}

void Level::End(bool status) {
	system("cls");
	char c;
	if (status == true) {
		printf("Уровень пройден: %i очков.\n", CurrentGame.points);
		printf("Начать следующий уровень?\n");
		c = _getch();
		if (c == 'y' || c == 'Y') {
			CurrentLevel.number++;
			CurrentLevel.loadLevel(CurrentLevel.number);
			CurrentBall.setStandard();
			CurrentPlatform.setStandard();
			CurrentGame.setStandard();
			CurrentGame.saveStatus = 1;
			saveConfig();
		} else {
			saveConfig();
			CurrentGame.End();
		}
	} else {
		printf("Начать заново? (y/n)\n");
		c = _getch();
		if (c == 'y' || c == 'Y') {
			CurrentLevel.loadLevel(CurrentLevel.number);
			CurrentPlatform.setStandard();
			CurrentBall.setStandard();
			CurrentGame.setStandard();
		} else {
			CurrentGame.End();
		}
	}
}


///////////
//////////
//////////        BALL FUNCTIONS
//////////
void Ball::collision() {
	// обработка выхода за экран!
	if ((this->position.X <= 0) && (this->course.X < 0)) {
		this->course.X = -(this->course.X);
	}
	if ((this->position.Y <= 0) && (this->course.Y < 0)) {
		this->course.Y = -(this->course.Y);
	}
	if ((this->position.X >= (CurrentLevel.Size_Columns - 1)) && (this->course.X > 0)) {
		this->course.X = -(this->course.X);
	}
	if (this->position.Y == (CurrentLevel.Size_Strings - 1)) {
		// Проигрыш!!!!
		CurrentGame.lifes--;
		CurrentBall.setStandard(); //установка начального положения шара
		CurrentPlatform.setStandard(); //установка начального положения платформы
		if (CurrentGame.lifes == 0) {
		//	 обработка конца игры!
			CurrentLevel.End(false);
		}
	}
	//Конец обработки выхода за экран
	//Столкновение с платформой!
	if ((this->position.X >= CurrentPlatform.position.X && //Если шарик находится над платформой и идет в низ
		 this->position.X < (CurrentPlatform.position.X + CurrentPlatform.length)) &&
		 (this->position.Y == (CurrentPlatform.position.Y - 1)) &&
		  (this->course.Y > 0)
		 )
	{
		this->course.Y = -(this->course.Y);
	}
	if ((this->position.X >= CurrentPlatform.position.X && 
		 this->position.X < (CurrentPlatform.position.X + CurrentPlatform.length)) &&
		 (this->position.Y == (CurrentPlatform.position.Y + 1)) &&
		  (this->course.Y < 0)
		 )
	{
		this->course.Y = -(this->course.Y);
	}
	if ((this->position.X == (CurrentPlatform.position.X - 1) 
		) && (this->position.Y == CurrentPlatform.position.Y)
		&& (this->course.X > 0)
	) {
		this->course.X = -(this->course.X);
	}
	if ((this->position.X == (CurrentPlatform.position.X + CurrentPlatform.length) 
		) && this->position.Y == CurrentPlatform.position.Y
		&& (this->course.X < 0)
	) {
		this->course.X = -(this->course.X);
	}
	//столкновение с платформой по диагоналям
	if(
		(this->position.X == (CurrentPlatform.position.X - 1)) &&  
		(this->position.Y == (CurrentPlatform.position.Y - 1)) &&
		(this->course.X > 0 && this->course.Y > 0)
	) {
		this->course.X = -(this->course.X);
		this->course.Y = -(this->course.Y);
	}
	if(
		(((this->position.X) == (CurrentPlatform.position.X + CurrentPlatform.length))) &&  
		((this->position.Y) == (CurrentPlatform.position.Y - 1)) &&
		(this->course.X < 0 && this->course.Y > 0)
	) {
		this->course.X = -(this->course.X);
		this->course.Y = -(this->course.Y);
	}
	if(
		(this->position.X == (CurrentPlatform.position.X + CurrentPlatform.length)) &&  
		(this->position.Y == (CurrentPlatform.position.Y + 1)) &&
		(this->course.X < 0 && this->course.Y < 0)
	) {
		this->course.X = -(this->course.X);
		this->course.Y = -(this->course.Y);
	}
	if(
		(this->position.X == (CurrentPlatform.position.X - 1)) &&  
		(this->position.Y == (CurrentPlatform.position.Y + 1)) &&
		(this->course.X > 0 && this->course.Y < 0)
	) {
		this->course.X = -(this->course.X);
		this->course.Y = -(this->course.Y);
	}
	// КОНЕЦ обработки столкновений с платформой
	//Столкновение с блоками
	int i = 0; //счетчик столкновений
	if ((CurrentLevel.map[this->position.Y - 1][this->position.X] != CurrentLevel.back) && (this->course.Y < 0)) {
		CurrentGame.points += 100; // очки за столкновение
		CurrentLevel.map[this->position.Y - 1][this->position.X] = CurrentLevel.back;
		this->course.Y = -(this->course.Y);
		i++;
	}
	if ((CurrentLevel.map[this->position.Y][this->position.X + 1] != CurrentLevel.back) && (this->course.X < 0)) {
		CurrentGame.points += 100; // очки за столкновение
		CurrentLevel.map[this->position.Y][this->position.X + 1] = CurrentLevel.back;
		this->course.X = -(this->course.X);
		i++;
	}
	if ((CurrentLevel.map[this->position.Y + 1][this->position.X] != CurrentLevel.back) && (this->course.Y > 0)) {
		CurrentGame.points += 100; // очки за столкновение
		CurrentLevel.map[this->position.Y + 1][this->position.X] = CurrentLevel.back;
		this->course.Y = -(this->course.Y);
		i++;
	}
	if ((CurrentLevel.map[this->position.Y][this->position.X - 1]) != CurrentLevel.back && (this->course.X > 0)) {
		CurrentGame.points += 100; // очки за столкновение
		CurrentLevel.map[this->position.Y][this->position.X - 1] = CurrentLevel.back;
		this->course.X = -(this->course.X);
		i++;
	}	
	//Столкновения по диагонали
	if (i == 0) { //Если не случилось столкновений по горизонтали/вертикали, то обрабатываем диагональные
		if (CurrentLevel.map[this->position.Y + 1][this->position.X + 1] != CurrentLevel.back && 
			(this->course.X > 0) && (this->course.Y > 0) // если блок правее по горизонтали и ниже вертикали и шар идет вправо + вниз
		) {
			CurrentGame.points += 100;// очки за столкновение
			CurrentLevel.map[this->position.Y + 1][this->position.X + 1] = CurrentLevel.back;			
			i++;
		}
		if (CurrentLevel.map[this->position.Y - 1][this->position.X + 1] != CurrentLevel.back && 
			(this->course.X > 0) && (this->course.Y < 0) // если блок правее по горизонтали и выше вертикали и шар идет вправо + вверх
		) {
			CurrentGame.points += 100;// очки за столкновение
			CurrentLevel.map[this->position.Y - 1][this->position.X + 1] = CurrentLevel.back;
			i++;
		}
		if (CurrentLevel.map[this->position.Y - 1][this->position.X - 1] != CurrentLevel.back && 
			(this->course.X < 0) && (this->course.Y < 0) // если блок левее по горизонтали и ниже вертикали и шар идет влево + вниз
		) {
			CurrentGame.points += 100;// очки за столкновение
			CurrentLevel.map[this->position.Y - 1][this->position.X - 1] = CurrentLevel.back;
			i++;
		}
		if (CurrentLevel.map[this->position.Y + 1][this->position.X - 1] != CurrentLevel.back && 
			(this->course.X < 0) && (this->course.Y > 0) // если блок левее по горизонтали и выше вертикали и шар идет влево + вверх
		) {
			CurrentGame.points += 100; // очки за столкновение
			CurrentLevel.map[this->position.Y + 1][this->position.X - 1] = CurrentLevel.back;
			i++;
		}
		if (i != 0) {
			this->course.Y = -(this->course.Y);
			this->course.X = -(this->course.X);	
		}
	}
	//Конец обработки столкновений с блоками

}

////////////
////////////
////////////			GAME FUNCTIONS
////////////
bool Platform::outOfSize(int course){
	// обработка выхода за экран
	if((this->position.Y == 0) && (course == 1)) {
		return false;
	}
	if(((this->position.X + this->length) >= (Level::Size_Columns)  - 1) && (course == 2)) {
		return false;
	}
	if((this->position.Y >= (Level::Size_Strings - 1)) && (course == 3)) {
		return false;
	}
	if((this->position.X == 0) && (course == 4)) {
		return false;
	}
	return true;

}

bool Platform::blockCollision(int course){
	//Столкновение с блоками
	if (course == 1) {//^
		for(int i = 0; i < this->length; i++) {
			if (CurrentLevel.map[this->position.Y - 1][this->position.X + i] != CurrentLevel.back) {
				return false;
			}
		}
	}
	if (course == 2) {//->
		if (CurrentLevel.map[this->position.Y][this->position.X + this->length] != CurrentLevel.back) {
			return false;
		}
	}
	if (course == 3) {//\/
		for(int i = 0; i < this->length; i++) {
			if (CurrentLevel.map[this->position.Y + 1][this->position.X + i] != CurrentLevel.back) {
				return false;
			}
		}
	}
	if (course == 4) {//<-
		if (CurrentLevel.map[this->position.Y][this->position.X  - 1] != CurrentLevel.back) {
			return false;
		}
	}

	return true; // если нет препятствий - то возвращаем тру
}


bool Platform::ballCollision(int course) {
		//Столкновение с мячом
	if (course == 3) {//\/
		for(int i = 0; i < this->length; i++) {
			if (((this->position.Y + 1) == CurrentBall.position.Y) && ((this->position.X + i) == CurrentBall.position.X)){
				return false;
			}
		}
	}
	if (course == 2) {//->
		if ((this->position.Y == CurrentBall.position.Y) && ((this->position.X + this->length) == CurrentBall.position.X)) {
			return false;
		}
	}
	if (course == 1) {//^
		for(int i = 0; i < this->length; i++) {
			if (((this->position.Y - 1) == CurrentBall.position.Y) && ((this->position.X + i) == CurrentBall.position.X)){
				return false;
			}
		}
	}
	if (course == 4) {//<-
		if ((this->position.Y == CurrentBall.position.Y) && ((this->position.X - 1) == CurrentBall.position.X)) {
			return false;
		}
	}

	return true; // если нет препятствий - то возвращаем тру
}

void Platform::step(int course){
	switch(course) {
	case 1:
		this->position.Y--;
		break;
	case 2:
		this->position.X++;
		break;
	case 3:
		this->position.Y++;
		break;
	case 4:
		this->position.X--;
		break;
	}
}


bool Platform::moveControl(int course){
	if ((this->outOfSize(course) && this->ballCollision(course)) && this->blockCollision(course)){		
		return true;
	} else {
		return false;
	}

}

void Ball::step(){
	if (this->course.X > 0) {
		this->position.X++;
	}else {
		this->position.X--;
	}
	if (this->course.Y > 0) {
		this->position.Y++;
	}else {
		this->position.Y--;
	}

}

void Ball::setStandard() {//установка начального положения мяча
		this->course.X = 1; // 1 - вправо, -1 - влево
		this->course.Y = -1;// 1 - вниз, -1 - вверх
		this->position.X = (Level::Size_Columns/2);
		this->position.Y = (Level::Size_Strings/2);
		this->symbol = 4;
		this->speed = CurrentLevel.minSpeedTime;
		this->timer = 0;
		this->stepNum = 0;
	}
////////////
/////////// GAME FUNCTIONS
////////////
///////////

void Game::Start() {
	int IR1 = 0;
	int IR2 = 0;
	while (true) {		
		if (_kbhit()) {
			IR1 = _getch();
			if (IR1 == 224) {
				IR2 = _getch();
				if (IR2 == 75) {
					if(CurrentPlatform.moveControl(4))
					{
						CurrentPlatform.step(4);
					}	
					//continue;
				}
				if (IR2 == 77) {
					if(CurrentPlatform.moveControl(2))
					{
						CurrentPlatform.step(2);
					}	
					//continue;
				}
				if (IR2 == 72) {
					if(CurrentPlatform.moveControl(1))
					{
						CurrentPlatform.step(1);
					}	
					//continue;
				}
				if (IR2 == 80) {
					if(CurrentPlatform.moveControl(3))
					{
						CurrentPlatform.step(3);
					}	
					//continue;
				}
			}
			if (IR1 == CurrentGame.stopSymbol) {
				CurrentLevel.End(false);
			}
			if (IR1 == CurrentGame.pauseSymbol) {
				system("pause");
				system("cls");
			}
		}
	CurrentBall.timer++; // счетчик проходов для замедления скорости шара
	if (CurrentBall.timer >= CurrentBall.speed) {
		CurrentBall.collision();
		CurrentBall.step(); // если достаточно раз прошел - шаг шара
		CurrentBall.timer = 0;
		CurrentBall.stepNum++;
	}
	if (CurrentBall.stepNum >= CurrentLevel.stepNorm) { //если прошли опред кол-во шагов, то ускоряемся
		CurrentBall.speedUp(2);
		CurrentBall.stepNum = 0;
	}
	this->render();
	if (CurrentLevel.allBlocksDestroyed()){
		CurrentLevel.End(true);
	}
	Sleep(CurrentGame.FPS);

//	CurrentBall.collision();
	}//end while
}


void Game::increasePoints(char c) {
	this->points += 100;
	//Прибавление очков в зависимости от разрушенного блока!
}
void Game::destroyBlock(int y, int x) {
	CurrentLevel.map[y][x] = CurrentLevel.back;
}

void Game::render() { //рисователь
	//system("cls");
	goToXY(0,0);// Сначало идем в начало экрана
	printf("LEVEL %i \n", CurrentLevel.number);
	for (int i=0; i<Level::Size_Strings; i++)
	{
		for (int j=0; j<Level::Size_Columns; j++) 
		{
			if (CurrentPlatform.position.X == j && CurrentPlatform.position.Y == i)
			{
				for(int k = 0; k < CurrentPlatform.length; k++) 
					printf("%c", CurrentPlatform.symbol);
				j += (CurrentPlatform.length - 1); // j
				continue;
			}
			else if (CurrentBall.position.X == j && CurrentBall.position.Y == i) {
				printf("%c", CurrentBall.symbol);
				continue;
			}else {
				printf("%c", CurrentLevel.map[i][j]);
			}

		}
		printf("\n");
	}
	printInfo();
}

void Game::End() { // перенести функцию в Level.End
	printf("Сохранить сессию?");
	char c;
	c = _getch();
	if (c == 'y' || c == 'Y') {
		CurrentGame.saveStatus = 1;
	} else {
		CurrentGame.saveStatus = 0;
	}
	saveConfig();		
	exit(0);
}

void Game::printInfo() {
	printf("Очки: %i Жизни: %i\n", this->points, this->lifes);
}

void Ball::speedUp(int spd) {
	if (this->speed >= CurrentLevel.maxSpeedTime)
		this->speed -= spd;
}
bool Level::allBlocksDestroyed() {
	for(int i = 0; i < Level::Size_Strings; i++) {
		for(int j = 0; j < Level::Size_Columns; j++){
			if (this->map[i][j] != this->back) {
				return false;
			}
		}
	}
	return true;
}
