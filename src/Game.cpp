#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>

#include "Snake.h"

using namespace std;

extern int map[5][40][60];
extern void appearPosion(int stage_num, WINDOW *win1);
extern void appearGrowth(int stage_num, WINDOW *win1);
extern void appearSpeed(int stage_num, WINDOW *win1);
extern void disappearPoison(int stage_num, WINDOW *win1);
extern void disappearGrowth(int stage_num, WINDOW *win1);
extern void disappearSpeed(int stage_num, WINDOW *win1);
extern void setSpeed(float f);
extern void crushItem(WINDOW *win1);
extern vector<Vector> minusSnake(WINDOW *win1);
extern position getHead();
extern vector<position> vPoison_item;
extern vector<position> vGrow_item;
extern vector<position> vSpeed_item;

char missionB = 'X'; 
char missionGrowth = 'X';
char missionPoison = 'X';
char missionGate = 'X';
string whyDead = "";  

int num_missionB = 6; 
int num_missionGrowth = 2;
int num_missionPoison = 2;
int num_missionGate = 1;


void newWindow(float y, float x){ 
    clear();
    initscr();
    noecho();
    getmaxyx(stdscr, y, x);
}


int UserInput(){ 
    int UserInput = getch();
    refresh();
    endwin();
    clear();
    return UserInput;
}


int startGame(float y, float x) {
    newWindow(y,x);
    printw("start game (y/n)");
    return UserInput();
}


int finishWindow(float y, float x){ 
    newWindow(y,x);
    printw(whyDead.data());
    printw("\n \nYou Dead!");
    return UserInput();
}
 

void drawGameMap(WINDOW* win, Snake& snake, char* table, int row, int col)
{
	werase(win);

	for(int i=0; i<(row*col); i++)
	{
		if(table[i]!=' ')
		{
			int y = i / col;
			int x = i - (y*col);
			int ch;
			switch(table[i])
			{
				case '0': 
                    ch = ' ';
                    break;
                case '1':
                    ch = '*';
                    break;
                case '2':
                    ch = '+';
                    break;
                case '4':
                    ch = '@';
                    break;
				case 'h':
					ch = '#';
					break;
				case 'b':
					ch = '#';
					break;
				case '8': 
					ch = '1';
					break;
				case '9': 
					ch = '2';
					break;
				case '3':
                    ch = '0';
                    break;
                case '6':
                    ch = 'x';
                    break;
                case '5':
					ch = 'S';
					break;
			}
			mvwaddch(win, y, x, ch);
		}
	}
	wrefresh(win);
}


void updateMap(Snake& snake, int map[40][60]) { 
	snake.setGate(map); 
}


void printScoreBoard(WINDOW* w, int snakeLen, int level, int growthItem, int poisonItem, int Gate){
	werase(w);
	wborder(w, '|','|','~','~','.','.','.','.');
	mvwprintw(w, 1, 1, " Score");
	mvwprintw(w, 2, 1, "   Length: %d/%d", snakeLen, snakeMaxLen);
	mvwprintw(w, 3, 1, "   Growth: %d ", growthItem);
	mvwprintw(w, 4, 1, "   Poison: %d ", poisonItem);
	mvwprintw(w, 5, 1, "   Gate  : %d ", Gate);
	mvwprintw(w, 6, 1, "   Level : %d ", level);
	wrefresh(w);
}


void printMission(WINDOW* w, int level){
	werase(w);
	wborder(w, '|','|','~','~','.','.','.','.');
	mvwprintw(w, 1, 1, " Missions");
	mvwprintw(w, 2, 1, "   Length(%d):   %c", num_missionB, missionB);
	mvwprintw(w, 3, 1, "   Growth(%d):   %c", num_missionGrowth, missionGrowth);
	mvwprintw(w, 4, 1, "   Poison(%d):   %c", num_missionGrowth, missionPoison);
	mvwprintw(w, 5, 1, "   Gate  (%d):   %c", num_missionGate, missionGate);
	wrefresh(w);
}


int noticeChangeLevel(float y, float x, int level){ 
	clear();
	initscr();
	noecho();
	
	getmaxyx(stdscr, y, x);
	newWindow(y,x);
	if (level==1){
		printw("Start Game! \n Level 1");
	}
	else{
		string lev = to_string(level);
		printw("Level Completed! \n");
		printw(lev.data());
		printw("\nPress Enter");
	}
	return UserInput();
}


void setMission(Snake& snake, WINDOW *win1){
	if(vGrow_item.empty() ==0){
		position head = snake.getHead();
		if(head == vGrow_item.back()){
		snake.crushItem(win1);
		snake.changeSnakeLen();
		snake.growthItem++;
		}
	}
	if(vPoison_item.empty() ==0){
		position head = snake.getHead();
		if(head == vPoison_item.back()){
		snake.minusSnake(win1);
		snake.changeSnakeLen();
		snake.poisonItem++;
		}
	}
	if(vSpeed_item.empty() ==0){
		position head = snake.getHead();
		if(head == vSpeed_item.back()){
				snake.setSpeed(0.9);
				disappearSpeed(snake.getLevel()-1,win1);
			appearSpeed(snake.getLevel()-1,win1);
		}
	}
	if(snake.getSize() == num_missionB) {missionB ='O';}
	if(snake.growthItem == num_missionGrowth) {missionGrowth = 'O';}
	if(snake.poisonItem == num_missionPoison) {missionPoison = 'O';}
	if(snake.getGateCnt() == num_missionGate) {missionGate = 'O';}
}


void nextLevel(Snake& snake,WINDOW *win1){
	if((missionB == 'O')&&(missionGate=='O')&&(missionGrowth=='O')&&(missionPoison=='O')){
		snake.resize(4);
		snake.setSpeed(1.0);
		snake.growthItem =0;
		snake.poisonItem =0;
		snake.setGateCnt(0);
		missionB = 'X'; 
		missionGrowth = 'X';
		missionPoison = 'X';
		missionGate = 'X';
		num_missionB++; 
		num_missionGrowth++;
		num_missionPoison++;
		num_missionGate++;
		disappearGrowth(snake.getLevel()-1,win1);
		disappearPoison(snake.getLevel()-1,win1);
		disappearSpeed(snake.getLevel()-1,win1);
		snake.setLevel(snake.getLevel()+1);
		if (noticeChangeLevel(0,0, snake.getLevel()) == 13) {}; 
	}
}


void game() { 

	float x, y;
	initscr();
	noecho();
	cbreak();

	getmaxyx(stdscr, y, x);
	WINDOW *win1 = newwin(40, 60, 0, 0); 
	WINDOW *scoreBoard = newwin(8, 20, 8,60);
	wrefresh(scoreBoard);
	WINDOW *mission = newwin(8, 20, 0,60);
	wrefresh(mission);

	Snake snake(40, 60);
	
	int growCnt = 0;
	int poisonCnt = 0;
	int speedCnt = 0;
	int mapCnt = 0;

	while(!snake.getEnd())
	{
		WINDOW *win1 = newwin(40, 60, 0, 0);
		printScoreBoard(scoreBoard, snake.getSnakeLen(), snake.getLevel(), snake.growthItem, snake.poisonItem, snake.getGateCnt());
		printMission(mission, snake.getLevel());

		srand(time(NULL));
		char *map_table = snake.setMaptoList(map[snake.getLevel()-1]);
		nodelay(win1, TRUE);
		keypad(win1, TRUE);
		refresh();

		wrefresh(win1);
		drawGameMap(win1, snake, map_table, snake.getRow(), snake.getCol()); 
		setMission(snake,win1);
		if (mapCnt == 0) {
			(snake, map[snake.getLevel()-1]);
		}
		mapCnt+= 1;
		if (mapCnt == 100) {
			snake.removeGate(map[snake.getLevel()-1]);
			updateMap(snake, map[snake.getLevel()-1]);
			mapCnt = 1;
		}
		if (growCnt == 0) {
			appearGrowth(snake.getLevel()-1,win1);
		}
		growCnt+= 1;
		if (growCnt == 77) { 
			disappearGrowth(snake.getLevel()-1,win1);
			appearGrowth(snake.getLevel()-1,win1);
			growCnt = 1;
		}
		if (poisonCnt == 0) {
			appearPosion(snake.getLevel()-1,win1);

		}
		poisonCnt+= 1;
		if (poisonCnt == 67) { 
			disappearPoison(snake.getLevel()-1,win1);
			appearPosion(snake.getLevel()-1,win1);
			poisonCnt = 1;
		}
		if (speedCnt == 0) {
			appearSpeed(snake.getLevel()-1, win1);
		}
		speedCnt+= 1;
		if (speedCnt == 87) {
			disappearSpeed(snake.getLevel()-1,win1);
			appearSpeed(snake.getLevel()-1,win1);
			speedCnt = 1;
		}

		int input = wgetch(win1); 
		char d = snake.getDirection(); 
		switch(input)
		{
			case 'w':
			case KEY_UP: 
				if(d!='u' && d!='d') snake.setDirection(0);
				else if (d=='d') {
          			snake.setEnd(true); 
          			whyDead = "You've moved wrong direction!";
        		}
				break;
			case 's':
			case KEY_DOWN: 
				if(d!='d' && d!='u') snake.setDirection(2);
				else if (d=='u') {
          			snake.setEnd(true);
          			whyDead = "You've moved wrong direction!";
        		}
				break;
			case 'a':
			case KEY_LEFT: 
				if(d!='l' && d!='r') snake.setDirection(3);
				else if (d=='r') {
					snake.setEnd(true);
					whyDead = "You've moved wrong direction!";
				}
				break;
			case 'd':
			case KEY_RIGHT: 
				if(d!='r' && d!='l') snake.setDirection(1);
				else if (d=='l') {
					snake.setEnd(true);
					whyDead = "You've moved wrong direction!";
        		}
				break;
			case 'r' :
			case 'R' :
				snake.setEnd(true);
				snake.removeGate(map[snake.getLevel()]);
				disappearGrowth(snake.getLevel()-1,win1);
				disappearPoison(snake.getLevel()-1,win1);
				disappearSpeed(snake.getLevel()-1,win1);
				game();
		}
		if(snake.getSize() <3) {
			snake.setEnd(TRUE); 
			whyDead = "Snake is too short (length < 3)";
    	}
		snake.moveSnakeBody(); 
		snake.moveSnakeHead(map[snake.getLevel()-1]); 
    	nextLevel(snake, win1); 
		usleep(snake.getSpeed()); 
	}
}

int main(){
	if (startGame(0, 0) == 'y') {
			game();
		}
  	finishWindow(0, 0);
	endwin();
	return 0;
}
